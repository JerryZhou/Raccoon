#include "stable.h"
#include "dwtransform.h"


#define DW_NEAR_CLIP (sizeof(DwReal) == sizeof(double) ? 0.000001 : 0.0001)

#ifdef MAP
#  undef MAP
#endif
#define MAP(x, y, nx, ny) \
    do { \
        DwReal FX_ = x; \
        DwReal FY_ = y; \
        switch(t) {   \
        case TxNone:  \
            nx = FX_;   \
            ny = FY_;   \
            break;    \
        case TxTranslate:    \
            nx = FX_ + affine._dx;                \
            ny = FY_ + affine._dy;                \
            break;                              \
        case TxScale:                           \
            nx = affine._m11 * FX_ + affine._dx;  \
            ny = affine._m22 * FY_ + affine._dy;  \
            break;                              \
        case TxRotate:                          \
        case TxShear:                           \
        case TxProject:                                      \
            nx = affine._m11 * FX_ + affine._m21 * FY_ + affine._dx;        \
            ny = affine._m12 * FX_ + affine._m22 * FY_ + affine._dy;        \
            if (t == TxProject) {                                       \
                DwReal w = (m_13 * FX_ + m_23 * FY_ + m_33);              \
                if (w < DwReal(DW_NEAR_CLIP)) w = DwReal(DW_NEAR_CLIP);     \
                w = 1./w;                                               \
                nx *= w;                                                \
                ny *= w;                                                \
            }                                                           \
        }                                                               \
    } while (0)

DwTransform::DwTransform()
    : affine(true)
    , m_13(0), m_23(0), m_33(1)
    , m_type(TxNone)
    , m_dirty(TxNone)
{
}

DwTransform::DwTransform(DwReal h11, DwReal h12, DwReal h13,
                         DwReal h21, DwReal h22, DwReal h23,
                         DwReal h31, DwReal h32, DwReal h33)
    : affine(h11, h12, h21, h22, h31, h32, true)
    , m_13(h13), m_23(h23), m_33(h33)
    , m_type(TxNone)
    , m_dirty(TxProject)
{
}

DwTransform::DwTransform(DwReal h11, DwReal h12, DwReal h21,
                         DwReal h22, DwReal dx, DwReal dy)
    : affine(h11, h12, h21, h22, dx, dy, true)
    , m_13(0), m_23(0), m_33(1)
    , m_type(TxNone)
    , m_dirty(TxShear)
{
}

/*!
    \fn DwTransform::DwTransform(const DwMatrix &matrix)

    Constructs a matrix that is a copy of the given \a matrix.
    Note that the \c m13, \c m23, and \c m33 elements are set to 0, 0,
    and 1 respectively.
 */
DwTransform::DwTransform(const DwMatrix &mtx)
    : affine(mtx._m11, mtx._m12, mtx._m21, mtx._m22, mtx._dx, mtx._dy, true),
      m_13(0), m_23(0), m_33(1)
      , m_type(TxNone)
      , m_dirty(TxShear)
{
}

/*!
    Returns the adjoint of this matrix.
*/
DwTransform DwTransform::adjoint() const
{
    DwReal h11, h12, h13,
           h21, h22, h23,
           h31, h32, h33;
    h11 = affine._m22*m_33 - m_23*affine._dy;
    h21 = m_23*affine._dx - affine._m21*m_33;
    h31 = affine._m21*affine._dy - affine._m22*affine._dx;
    h12 = m_13*affine._dy - affine._m12*m_33;
    h22 = affine._m11*m_33 - m_13*affine._dx;
    h32 = affine._m12*affine._dx - affine._m11*affine._dy;
    h13 = affine._m12*m_23 - m_13*affine._m22;
    h23 = m_13*affine._m21 - affine._m11*m_23;
    h33 = affine._m11*affine._m22 - affine._m12*affine._m21;

    return DwTransform(h11, h12, h13,
                       h21, h22, h23,
                       h31, h32, h33, true);
}

/*!
    Returns the transpose of this matrix.
*/
DwTransform DwTransform::transposed() const
{
    DwTransform t(affine._m11, affine._m21, affine._dx,
                  affine._m12, affine._m22, affine._dy,
                  m_13, m_23, m_33, true);
    t.m_type = m_type;
    t.m_dirty = m_dirty;
    return t;
}

/*!
    Returns an inverted copy of this matrix.

    If the matrix is singular (not invertible), the returned matrix is
    the identity matrix. If \a invertible is valid (i.e. not 0), its
    value is set to true if the matrix is invertible, otherwise it is
    set to false.

    \sa isInvertible()
*/
DwTransform DwTransform::inverted(bool *invertible) const
{
    DwTransform invert(true);
    bool inv = true;

    switch(inline_type())
    {
    case TxNone:
        break;
    case TxTranslate:
        invert.affine._dx = -affine._dx;
        invert.affine._dy = -affine._dy;
        break;
    case TxScale:
        inv = !dwFuzzyIsNull(affine._m11);
        inv &= !dwFuzzyIsNull(affine._m22);
        if (inv)
        {
            invert.affine._m11 = 1. / affine._m11;
            invert.affine._m22 = 1. / affine._m22;
            invert.affine._dx = -affine._dx * invert.affine._m11;
            invert.affine._dy = -affine._dy * invert.affine._m22;
        }
        break;
    case TxRotate:
    case TxShear:
        invert.affine = affine.inverted(&inv);
        break;
    default:
        // general case
        DwReal det = determinant();
        inv = !dwFuzzyIsNull(det);
        if (inv)
            invert = (adjoint() /= det);
        break;
    }

    if (invertible)
        *invertible = inv;

    if (inv)
    {
        // inverting doesn't change the type
        invert.m_type = m_type;
        invert.m_dirty = m_dirty;
    }

    return invert;
}

/*!
    Moves the coordinate system \a dx along the x axis and \a dy along
    the y axis, and returns a reference to the matrix.

    \sa setMatrix()
*/
DwTransform &DwTransform::translate(DwReal dx, DwReal dy)
{
    if (dx == 0 && dy == 0)
        return *this;

    switch(inline_type())
    {
    case TxNone:
        affine._dx = dx;
        affine._dy = dy;
        break;
    case TxTranslate:
        affine._dx += dx;
        affine._dy += dy;
        break;
    case TxScale:
        affine._dx += dx*affine._m11;
        affine._dy += dy*affine._m22;
        break;
    case TxProject:
        m_33 += dx*m_13 + dy*m_23;
        // Fall through
    case TxShear:
    case TxRotate:
        affine._dx += dx*affine._m11 + dy*affine._m21;
        affine._dy += dy*affine._m22 + dx*affine._m12;
        break;
    }
    if (m_dirty < TxTranslate)
        m_dirty = TxTranslate;
    return *this;
}

/*!
    Creates a matrix which corresponds to a translation of \a dx along
    the x axis and \a dy along the y axis. This is the same as
    DwTransform().translate(dx, dy) but slightly faster.

    \since 4.5
*/
DwTransform DwTransform::fromTranslate(DwReal dx, DwReal dy)
{
    DwTransform transform(1, 0, 0, 0, 1, 0, dx, dy, 1, true);
    if (dx == 0 && dy == 0)
        transform.m_type = TxNone;
    else
        transform.m_type = TxTranslate;
    transform.m_dirty = TxNone;
    return transform;
}

void DwTransform::setTranslated(DwReal dx, DwReal dy)
{
    if (dx == affine._dx && dy == affine._dy)
        return ;

    affine._dx = dx;
    affine._dy = dy;

    if (m_dirty < TxTranslate)
        m_dirty = TxTranslate;
}

DwPointF DwTransform::translated() const
{
    if (inline_type() >= TxTranslate)
    {
        return DwPointF(affine._dx, affine._dy);
    }
    return DwPointF(0., 0.);
}

/*!
    Scales the coordinate system by \a sx horizontally and \a sy
    vertically, and returns a reference to the matrix.

    \sa setMatrix()
*/
DwTransform & DwTransform::scale(DwReal sx, DwReal sy)
{
    if (sx == 1 && sy == 1)
        return *this;

    switch(inline_type())
    {
    case TxNone:
    case TxTranslate:
        affine._m11 = sx;
        affine._m22 = sy;
        break;
    case TxProject:
        m_13 *= sx;
        m_23 *= sy;
        // fall through
    case TxRotate:
    case TxShear:
        affine._m12 *= sx;
        affine._m21 *= sy;
        // fall through
    case TxScale:
        affine._m11 *= sx;
        affine._m22 *= sy;
        break;
    }
    if (m_dirty < TxScale)
        m_dirty = TxScale;
    return *this;
}

/*!
    Creates a matrix which corresponds to a scaling of
    \a sx horizontally and \a sy vertically.
    This is the same as DwTransform().scale(sx, sy) but slightly faster.

    \since 4.5
*/
DwTransform DwTransform::fromScale(DwReal sx, DwReal sy)
{
    DwTransform transform(sx, 0, 0, 0, sy, 0, 0, 0, 1, true);
    if (sx == 1. && sy == 1.)
        transform.m_type = TxNone;
    else
        transform.m_type = TxScale;
    transform.m_dirty = TxNone;
    return transform;
}

void DwTransform::setScaled(DwReal sx, DwReal sy)
{
    if (sx == affine._m11 && sy == affine._m22)
        return ;

    affine._m11 = sx;
    affine._m22 = sy;

    if (m_dirty < TxScale)
        m_dirty = TxScale;
}

DwPointF DwTransform::scaled() const
{
    if (inline_type() >= TxScale)
    {
        return DwPointF(affine._m11, affine._m22);
    }
    return DwPointF(1., 1.);
}

/*!
    Shears the coordinate system by \a sh horizontally and \a sv
    vertically, and returns a reference to the matrix.

    \sa setMatrix()
*/
DwTransform & DwTransform::shear(DwReal sh, DwReal sv)
{
    if (sh == 0 && sv == 0)
        return *this;

    switch(inline_type())
    {
    case TxNone:
    case TxTranslate:
        affine._m12 = sv;
        affine._m21 = sh;
        break;
    case TxScale:
        affine._m12 = sv*affine._m22;
        affine._m21 = sh*affine._m11;
        break;
    case TxProject:
    {
        DwReal tm13 = sv*m_23;
        DwReal tm23 = sh*m_13;
        m_13 += tm13;
        m_23 += tm23;
    }
    // fall through
    case TxRotate:
    case TxShear:
    {
        DwReal tm11 = sv*affine._m21;
        DwReal tm22 = sh*affine._m12;
        DwReal tm12 = sv*affine._m22;
        DwReal tm21 = sh*affine._m11;
        affine._m11 += tm11;
        affine._m12 += tm12;
        affine._m21 += tm21;
        affine._m22 += tm22;
        break;
    }
    }
    if (m_dirty < TxShear)
        m_dirty = TxShear;
    return *this;
}

void DwTransform::setSheard(DwReal sh, DwReal sv)
{
    if (sh == affine._m12 && sv == affine._m21)
        return ;

    affine._m12 = sv;
    affine._m21 = sh;

    if (m_dirty < TxShear)
        m_dirty = TxShear;
}

DwPointF DwTransform::shearded() const
{
    if (inline_type() >= TxShear)
    {
        return DwPointF(affine._m12, affine._m21);
    }
    return DwPointF(0., 0.);
}

const DwReal deg2rad = DwReal(0.017453292519943295769);        // pi/180
const DwReal inv_dist_to_plane = 1. / 1024.;

/*!
    \fn DwTransform &DwTransform::rotate(DwReal angle, Dw::Axis axis)

    Rotates the coordinate system counterclockwise by the given \a angle
    about the specified \a axis and returns a reference to the matrix.

    Note that if you apply a DwTransform to a point defined in widget
    coordinates, the direction of the rotation will be clockwise
    because the y-axis points downwards.

    The angle is specified in degrees.

    \sa setMatrix()
*/
DwTransform & DwTransform::rotate(DwReal a, Dw::Axis axis)
{
    if (a == 0)
        return *this;

    DwReal sina = 0;
    DwReal cosa = 0;
    if (a == 90. || a == -270.)
        sina = 1.;
    else if (a == 270. || a == -90.)
        sina = -1.;
    else if (a == 180.)
        cosa = -1.;
    else
    {
        DwReal b = deg2rad*a;          // convert to radians
        sina = dwSin(b);               // fast and convenient
        cosa = dwCos(b);
    }

    if (axis == Dw::ZAxis)
    {
        switch(inline_type())
        {
        case TxNone:
        case TxTranslate:
            affine._m11 = cosa;
            affine._m12 = sina;
            affine._m21 = -sina;
            affine._m22 = cosa;
            break;
        case TxScale:
        {
            DwReal tm11 = cosa*affine._m11;
            DwReal tm12 = sina*affine._m22;
            DwReal tm21 = -sina*affine._m11;
            DwReal tm22 = cosa*affine._m22;
            affine._m11 = tm11;
            affine._m12 = tm12;
            affine._m21 = tm21;
            affine._m22 = tm22;
            break;
        }
        case TxProject:
        {
            DwReal tm13 = cosa*m_13 + sina*m_23;
            DwReal tm23 = -sina*m_13 + cosa*m_23;
            m_13 = tm13;
            m_23 = tm23;
            // fall through
        }
        case TxRotate:
        case TxShear:
        {
            DwReal tm11 = cosa*affine._m11 + sina*affine._m21;
            DwReal tm12 = cosa*affine._m12 + sina*affine._m22;
            DwReal tm21 = -sina*affine._m11 + cosa*affine._m21;
            DwReal tm22 = -sina*affine._m12 + cosa*affine._m22;
            affine._m11 = tm11;
            affine._m12 = tm12;
            affine._m21 = tm21;
            affine._m22 = tm22;
            break;
        }
        }
        if (m_dirty < TxRotate)
            m_dirty = TxRotate;
    }
    else
    {
        DwTransform result;
        if (axis == Dw::YAxis)
        {
            result.affine._m11 = cosa;
            result.m_13 = -sina * inv_dist_to_plane;
        }
        else
        {
            result.affine._m22 = cosa;
            result.m_23 = -sina * inv_dist_to_plane;
        }
        result.m_type = TxProject;
        *this = result * *this;
    }

    return *this;
}

/*!
    \fn DwTransform & DwTransform::rotateRadians(DwReal angle, Dw::Axis axis)

    Rotates the coordinate system counterclockwise by the given \a angle
    about the specified \a axis and returns a reference to the matrix.

    Note that if you apply a DwTransform to a point defined in widget
    coordinates, the direction of the rotation will be clockwise
    because the y-axis points downwards.

    The angle is specified in radians.

    \sa setMatrix()
*/
DwTransform & DwTransform::rotateRadians(DwReal a, Dw::Axis axis)
{
    DwReal sina = dwSin(a);
    DwReal cosa = dwCos(a);

    if (axis == Dw::ZAxis)
    {
        switch(inline_type())
        {
        case TxNone:
        case TxTranslate:
            affine._m11 = cosa;
            affine._m12 = sina;
            affine._m21 = -sina;
            affine._m22 = cosa;
            break;
        case TxScale:
        {
            DwReal tm11 = cosa*affine._m11;
            DwReal tm12 = sina*affine._m22;
            DwReal tm21 = -sina*affine._m11;
            DwReal tm22 = cosa*affine._m22;
            affine._m11 = tm11;
            affine._m12 = tm12;
            affine._m21 = tm21;
            affine._m22 = tm22;
            break;
        }
        case TxProject:
        {
            DwReal tm13 = cosa*m_13 + sina*m_23;
            DwReal tm23 = -sina*m_13 + cosa*m_23;
            m_13 = tm13;
            m_23 = tm23;
            // fall through
        }
        case TxRotate:
        case TxShear:
        {
            DwReal tm11 = cosa*affine._m11 + sina*affine._m21;
            DwReal tm12 = cosa*affine._m12 + sina*affine._m22;
            DwReal tm21 = -sina*affine._m11 + cosa*affine._m21;
            DwReal tm22 = -sina*affine._m12 + cosa*affine._m22;
            affine._m11 = tm11;
            affine._m12 = tm12;
            affine._m21 = tm21;
            affine._m22 = tm22;
            break;
        }
        }
        if (m_dirty < TxRotate)
            m_dirty = TxRotate;
    }
    else
    {
        DwTransform result;
        if (axis == Dw::YAxis)
        {
            result.affine._m11 = cosa;
            result.m_13 = -sina * inv_dist_to_plane;
        }
        else
        {
            result.affine._m22 = cosa;
            result.m_23 = -sina * inv_dist_to_plane;
        }
        result.m_type = TxProject;
        *this = result * *this;
    }
    return *this;
}

/*!
    \fn bool DwTransform::operator==(const DwTransform &matrix) const
    Returns true if this matrix is equal to the given \a matrix,
    otherwise returns false.
*/
bool DwTransform::operator==(const DwTransform &o) const
{
    return affine._m11 == o.affine._m11 &&
           affine._m12 == o.affine._m12 &&
           affine._m21 == o.affine._m21 &&
           affine._m22 == o.affine._m22 &&
           affine._dx == o.affine._dx &&
           affine._dy == o.affine._dy &&
           m_13 == o.m_13 &&
           m_23 == o.m_23 &&
           m_33 == o.m_33;
}

/*!
    \fn bool DwTransform::operator!=(const DwTransform &matrix) const
    Returns true if this matrix is not equal to the given \a matrix,
    otherwise returns false.
*/
bool DwTransform::operator!=(const DwTransform &o) const
{
    return !operator==(o);
}

/*!
    \fn DwTransform & DwTransform::operator*=(const DwTransform &matrix)
    \overload

    Returns the result of multiplying this matrix by the given \a
    matrix.
*/
DwTransform & DwTransform::operator*=(const DwTransform &o)
{
    const TransformationType otherType = o.inline_type();
    if (otherType == TxNone)
        return *this;

    const TransformationType thisType = inline_type();
    if (thisType == TxNone)
        return operator=(o);

    TransformationType t = dwMax(thisType, otherType);
    switch(t)
    {
    case TxNone:
        break;
    case TxTranslate:
        affine._dx += o.affine._dx;
        affine._dy += o.affine._dy;
        break;
    case TxScale:
    {
        DwReal m11 = affine._m11*o.affine._m11;
        DwReal m22 = affine._m22*o.affine._m22;

        DwReal m31 = affine._dx*o.affine._m11 + o.affine._dx;
        DwReal m32 = affine._dy*o.affine._m22 + o.affine._dy;

        affine._m11 = m11;
        affine._m22 = m22;
        affine._dx = m31;
        affine._dy = m32;
        break;
    }
    case TxRotate:
    case TxShear:
    {
        DwReal m11 = affine._m11*o.affine._m11 + affine._m12*o.affine._m21;
        DwReal m12 = affine._m11*o.affine._m12 + affine._m12*o.affine._m22;

        DwReal m21 = affine._m21*o.affine._m11 + affine._m22*o.affine._m21;
        DwReal m22 = affine._m21*o.affine._m12 + affine._m22*o.affine._m22;

        DwReal m31 = affine._dx*o.affine._m11 + affine._dy*o.affine._m21 + o.affine._dx;
        DwReal m32 = affine._dx*o.affine._m12 + affine._dy*o.affine._m22 + o.affine._dy;

        affine._m11 = m11;
        affine._m12 = m12;
        affine._m21 = m21;
        affine._m22 = m22;
        affine._dx = m31;
        affine._dy = m32;
        break;
    }
    case TxProject:
    {
        DwReal m11 = affine._m11*o.affine._m11 + affine._m12*o.affine._m21 + m_13*o.affine._dx;
        DwReal m12 = affine._m11*o.affine._m12 + affine._m12*o.affine._m22 + m_13*o.affine._dy;
        DwReal m13 = affine._m11*o.m_13 + affine._m12*o.m_23 + m_13*o.m_33;

        DwReal m21 = affine._m21*o.affine._m11 + affine._m22*o.affine._m21 + m_23*o.affine._dx;
        DwReal m22 = affine._m21*o.affine._m12 + affine._m22*o.affine._m22 + m_23*o.affine._dy;
        DwReal m23 = affine._m21*o.m_13 + affine._m22*o.m_23 + m_23*o.m_33;

        DwReal m31 = affine._dx*o.affine._m11 + affine._dy*o.affine._m21 + m_33*o.affine._dx;
        DwReal m32 = affine._dx*o.affine._m12 + affine._dy*o.affine._m22 + m_33*o.affine._dy;
        DwReal m33 = affine._dx*o.m_13 + affine._dy*o.m_23 + m_33*o.m_33;

        affine._m11 = m11;
        affine._m12 = m12;
        m_13 = m13;
        affine._m21 = m21;
        affine._m22 = m22;
        m_23 = m23;
        affine._dx = m31;
        affine._dy = m32;
        m_33 = m33;
    }
    }

    m_dirty = t;
    m_type = t;

    return *this;
}

/*!
    \fn DwTransform DwTransform::operator*(const DwTransform &matrix) const
    Returns the result of multiplying this matrix by the given \a
    matrix.

    Note that matrix multiplication is not commutative, i.e. a*b !=
    b*a.
*/
DwTransform DwTransform::operator*(const DwTransform &m) const
{
    const TransformationType otherType = m.inline_type();
    if (otherType == TxNone)
        return *this;

    const TransformationType thisType = inline_type();
    if (thisType == TxNone)
        return m;

    DwTransform t(true);
    TransformationType type = dwMax(thisType, otherType);
    switch(type)
    {
    case TxNone:
        break;
    case TxTranslate:
        t.affine._dx = affine._dx + m.affine._dx;
        t.affine._dy += affine._dy + m.affine._dy;
        break;
    case TxScale:
    {
        DwReal m11 = affine._m11*m.affine._m11;
        DwReal m22 = affine._m22*m.affine._m22;

        DwReal m31 = affine._dx*m.affine._m11 + m.affine._dx;
        DwReal m32 = affine._dy*m.affine._m22 + m.affine._dy;

        t.affine._m11 = m11;
        t.affine._m22 = m22;
        t.affine._dx = m31;
        t.affine._dy = m32;
        break;
    }
    case TxRotate:
    case TxShear:
    {
        DwReal m11 = affine._m11*m.affine._m11 + affine._m12*m.affine._m21;
        DwReal m12 = affine._m11*m.affine._m12 + affine._m12*m.affine._m22;

        DwReal m21 = affine._m21*m.affine._m11 + affine._m22*m.affine._m21;
        DwReal m22 = affine._m21*m.affine._m12 + affine._m22*m.affine._m22;

        DwReal m31 = affine._dx*m.affine._m11 + affine._dy*m.affine._m21 + m.affine._dx;
        DwReal m32 = affine._dx*m.affine._m12 + affine._dy*m.affine._m22 + m.affine._dy;

        t.affine._m11 = m11;
        t.affine._m12 = m12;
        t.affine._m21 = m21;
        t.affine._m22 = m22;
        t.affine._dx = m31;
        t.affine._dy = m32;
        break;
    }
    case TxProject:
    {
        DwReal m11 = affine._m11*m.affine._m11 + affine._m12*m.affine._m21 + m_13*m.affine._dx;
        DwReal m12 = affine._m11*m.affine._m12 + affine._m12*m.affine._m22 + m_13*m.affine._dy;
        DwReal m13 = affine._m11*m.m_13 + affine._m12*m.m_23 + m_13*m.m_33;

        DwReal m21 = affine._m21*m.affine._m11 + affine._m22*m.affine._m21 + m_23*m.affine._dx;
        DwReal m22 = affine._m21*m.affine._m12 + affine._m22*m.affine._m22 + m_23*m.affine._dy;
        DwReal m23 = affine._m21*m.m_13 + affine._m22*m.m_23 + m_23*m.m_33;

        DwReal m31 = affine._dx*m.affine._m11 + affine._dy*m.affine._m21 + m_33*m.affine._dx;
        DwReal m32 = affine._dx*m.affine._m12 + affine._dy*m.affine._m22 + m_33*m.affine._dy;
        DwReal m33 = affine._dx*m.m_13 + affine._dy*m.m_23 + m_33*m.m_33;

        t.affine._m11 = m11;
        t.affine._m12 = m12;
        t.m_13 = m13;
        t.affine._m21 = m21;
        t.affine._m22 = m22;
        t.m_23 = m23;
        t.affine._dx = m31;
        t.affine._dy = m32;
        t.m_33 = m33;
    }
    }

    t.m_dirty = type;
    t.m_type = type;

    return t;
}

/*!
    \fn DwTransform & DwTransform::operator*=(DwReal scalar)
    \overload

    Returns the result of performing an element-wise multiplication of this
    matrix with the given \a scalar.
*/

/*!
    \fn DwTransform & DwTransform::operator/=(DwReal scalar)
    \overload

    Returns the result of performing an element-wise division of this
    matrix by the given \a scalar.
*/

/*!
    \fn DwTransform & DwTransform::operator+=(DwReal scalar)
    \overload

    Returns the matrix obtained by adding the given \a scalar to each
    element of this matrix.
*/

/*!
    \fn DwTransform & DwTransform::operator-=(DwReal scalar)
    \overload

    Returns the matrix obtained by subtracting the given \a scalar from each
    element of this matrix.
*/

/*!
    Assigns the given \a matrix's values to this matrix.
*/
DwTransform & DwTransform::operator=(const DwTransform &matrix)
{
    affine._m11 = matrix.affine._m11;
    affine._m12 = matrix.affine._m12;
    affine._m21 = matrix.affine._m21;
    affine._m22 = matrix.affine._m22;
    affine._dx = matrix.affine._dx;
    affine._dy = matrix.affine._dy;
    m_13 = matrix.m_13;
    m_23 = matrix.m_23;
    m_33 = matrix.m_33;
    m_type = matrix.m_type;
    m_dirty = matrix.m_dirty;

    return *this;
}

/*!
    Resets the matrix to an identity matrix, i.e. all elements are set
    to zero, except \c m11 and \c m22 (specifying the scale) and \c m33
    which are set to 1.

    \sa DwTransform(), isIdentity(), {DwTransform#Basic Matrix
    Operations}{Basic Matrix Operations}
*/
void DwTransform::reset()
{
    affine._m11 = affine._m22 = m_33 = 1.0;
    affine._m12 = m_13 = affine._m21 = m_23 = affine._dx = affine._dy = 0;
    m_type = TxNone;
    m_dirty = TxNone;
}

/*!
    \fn DwPoint operator*(const DwPoint &point, const DwTransform &matrix)
    \relates DwTransform

    This is the same as \a{matrix}.map(\a{point}).

    \sa DwTransform::map()
*/
DwPoint DwTransform::map(const DwPoint &p) const
{
    DwReal fx = p.x();
    DwReal fy = p.y();

    DwReal x = 0, y = 0;

    TransformationType t = inline_type();
    switch(t)
    {
    case TxNone:
        x = fx;
        y = fy;
        break;
    case TxTranslate:
        x = fx + affine._dx;
        y = fy + affine._dy;
        break;
    case TxScale:
        x = affine._m11 * fx + affine._dx;
        y = affine._m22 * fy + affine._dy;
        break;
    case TxRotate:
    case TxShear:
    case TxProject:
        x = affine._m11 * fx + affine._m21 * fy + affine._dx;
        y = affine._m12 * fx + affine._m22 * fy + affine._dy;
        if (t == TxProject)
        {
            DwReal w = 1./(m_13 * fx + m_23 * fy + m_33);
            x *= w;
            y *= w;
        }
    }
    return DwPoint(dwRound(x), dwRound(y));
}


/*!
    \fn DwPointF operator*(const DwPointF &point, const DwTransform &matrix)
    \relates DwTransform

    Same as \a{matrix}.map(\a{point}).

    \sa DwTransform::map()
*/

/*!
    \overload

    Creates and returns a DwPointF object that is a copy of the given point,
    \a p, mapped into the coordinate system defined by this matrix.
*/
DwPointF DwTransform::map(const DwPointF &p) const
{
    DwReal fx = p.x();
    DwReal fy = p.y();

    DwReal x = 0, y = 0;

    TransformationType t = inline_type();
    switch(t)
    {
    case TxNone:
        x = fx;
        y = fy;
        break;
    case TxTranslate:
        x = fx + affine._dx;
        y = fy + affine._dy;
        break;
    case TxScale:
        x = affine._m11 * fx + affine._dx;
        y = affine._m22 * fy + affine._dy;
        break;
    case TxRotate:
    case TxShear:
    case TxProject:
        x = affine._m11 * fx + affine._m21 * fy + affine._dx;
        y = affine._m12 * fx + affine._m22 * fy + affine._dy;
        if (t == TxProject)
        {
            DwReal w = 1./(m_13 * fx + m_23 * fy + m_33);
            x *= w;
            y *= w;
        }
    }
    return DwPointF(x, y);
}

void DwTransform::setMatrix(DwReal m11, DwReal m12, DwReal m13,
                            DwReal m21, DwReal m22, DwReal m23,
                            DwReal m31, DwReal m32, DwReal m33)
{
    affine._m11 = m11;
    affine._m12 = m12;
    m_13 = m13;
    affine._m21 = m21;
    affine._m22 = m22;
    m_23 = m23;
    affine._dx = m31;
    affine._dy = m32;
    m_33 = m33;
    m_type = TxNone;
    m_dirty = TxProject;
}

static inline bool needsPerspectiveClipping(const DwRectF &rect, const DwTransform &transform)
{
    const DwReal wx = dwMin(transform.m13() * rect.left(), transform.m13() * rect.right());
    const DwReal wy = dwMin(transform.m23() * rect.top(), transform.m23() * rect.bottom());

    return wx + wy + transform.m33() < DW_NEAR_CLIP;
}

DwRect DwTransform::mapRect(const DwRect &rect) const
{
    TransformationType t = inline_type();
    if (t <= TxTranslate)
        return rect.translated(dwRound(affine._dx), dwRound(affine._dy));

    if (t <= TxScale)
    {
        int x = dwRound(affine._m11*rect.x() + affine._dx);
        int y = dwRound(affine._m22*rect.y() + affine._dy);
        int w = dwRound(affine._m11*rect.width());
        int h = dwRound(affine._m22*rect.height());
        if (w < 0)
        {
            w = -w;
            x -= w;
        }
        if (h < 0)
        {
            h = -h;
            y -= h;
        }
        return DwRect(x, y, w, h);
    }
    else if (t < TxProject || !needsPerspectiveClipping(rect, *this))
    {
        // see mapToPolygon for explanations of the algorithm.
        DwReal x = 0, y = 0;
        MAP(rect.left(), rect.top(), x, y);
        DwReal xmin = x;
        DwReal ymin = y;
        DwReal xmax = x;
        DwReal ymax = y;
        MAP(rect.right() + 1, rect.top(), x, y);
        xmin = dwMin(xmin, x);
        ymin = dwMin(ymin, y);
        xmax = dwMax(xmax, x);
        ymax = dwMax(ymax, y);
        MAP(rect.right() + 1, rect.bottom() + 1, x, y);
        xmin = dwMin(xmin, x);
        ymin = dwMin(ymin, y);
        xmax = dwMax(xmax, x);
        ymax = dwMax(ymax, y);
        MAP(rect.left(), rect.bottom() + 1, x, y);
        xmin = dwMin(xmin, x);
        ymin = dwMin(ymin, y);
        xmax = dwMax(xmax, x);
        ymax = dwMax(ymax, y);
        return DwRect(dwRound(xmin), dwRound(ymin), dwRound(xmax)-dwRound(xmin), dwRound(ymax)-dwRound(ymin));
    }
    else
    {
        DW_ASSERT(false);
        return DwRect();
    }
}

/*!
    \fn DwRectF DwTransform::mapRect(const DwRectF &rectangle) const

    Creates and returns a DwRectF object that is a copy of the given \a
    rectangle, mapped into the coordinate system defined by this
    matrix.

    The rectangle's coordinates are transformed using the following
    formulas:

    \snippet doc/src/snippets/code/src_gui_painting_DwTransform.cpp 2

    If rotation or shearing has been specified, this function returns
    the \e bounding rectangle. To retrieve the exact region the given
    \a rectangle maps to, use the mapToPolygon() function instead.

    \sa mapToPolygon(), {DwTransform#Basic Matrix Operations}{Basic Matrix
    Operations}
*/
DwRectF DwTransform::mapRect(const DwRectF &rect) const
{
    TransformationType t = inline_type();
    if (t <= TxTranslate)
        return rect.translated(affine._dx, affine._dy);

    if (t <= TxScale)
    {
        DwReal x = affine._m11*rect.x() + affine._dx;
        DwReal y = affine._m22*rect.y() + affine._dy;
        DwReal w = affine._m11*rect.width();
        DwReal h = affine._m22*rect.height();
        if (w < 0)
        {
            w = -w;
            x -= w;
        }
        if (h < 0)
        {
            h = -h;
            y -= h;
        }
        return DwRectF(x, y, w, h);
    }
    else if (t < TxProject || !needsPerspectiveClipping(rect, *this))
    {
        DwReal x = 0, y = 0;
        MAP(rect.x(), rect.y(), x, y);
        DwReal xmin = x;
        DwReal ymin = y;
        DwReal xmax = x;
        DwReal ymax = y;
        MAP(rect.x() + rect.width(), rect.y(), x, y);
        xmin = dwMin(xmin, x);
        ymin = dwMin(ymin, y);
        xmax = dwMax(xmax, x);
        ymax = dwMax(ymax, y);
        MAP(rect.x() + rect.width(), rect.y() + rect.height(), x, y);
        xmin = dwMin(xmin, x);
        ymin = dwMin(ymin, y);
        xmax = dwMax(xmax, x);
        ymax = dwMax(ymax, y);
        MAP(rect.x(), rect.y() + rect.height(), x, y);
        xmin = dwMin(xmin, x);
        ymin = dwMin(ymin, y);
        xmax = dwMax(xmax, x);
        ymax = dwMax(ymax, y);
        return DwRectF(xmin, ymin, xmax-xmin, ymax - ymin);
    }
    else
    {
        DW_ASSERT(false);
        return DwRect();
    }
}

/*!
    \fn DwRect DwTransform::mapRect(const DwRect &rectangle) const
    \overload

    Creates and returns a DwRect object that is a copy of the given \a
    rectangle, mapped into the coordinate system defined by this
    matrix. Note that the transformed coordinates are rounded to the
    nearest integer.
*/

/*!
    Maps the given coordinates \a x and \a y into the coordinate
    system defined by this matrix. The resulting values are put in *\a
    tx and *\a ty, respectively.

    The coordinates are transformed using the following formulas:

    \snippet doc/src/snippets/code/src_gui_painting_DwTransform.cpp 3

    The point (x, y) is the original point, and (x', y') is the
    transformed point.

    \sa {DwTransform#Basic Matrix Operations}{Basic Matrix Operations}
*/
void DwTransform::map(DwReal x, DwReal y, DwReal *tx, DwReal *ty) const
{
    TransformationType t = inline_type();
    MAP(x, y, *tx, *ty);
}

/*!
    \overload

    Maps the given coordinates \a x and \a y into the coordinate
    system defined by this matrix. The resulting values are put in *\a
    tx and *\a ty, respectively. Note that the transformed coordinates
    are rounded to the nearest integer.
*/
void DwTransform::map(int x, int y, int *tx, int *ty) const
{
    TransformationType t = inline_type();
    DwReal fx = 0, fy = 0;
    MAP(x, y, fx, fy);
    *tx = dwRound(fx);
    *ty = dwRound(fy);
}

/*!
  Returns the DwTransform as an affine matrix.

  \warning If a perspective transformation has been specified,
  then the conversion will cause loss of data.
*/
const DwMatrix &DwTransform::toAffine() const
{
    return affine;
}

/*!
  Returns the transformation type of this matrix.

  The transformation type is the highest enumeration value
  capturing all of the matrix's transformations. For example,
  if the matrix both scales and shears, the type would be \c TxShear,
  because \c TxShear has a higher enumeration value than \c TxScale.

  Knowing the transformation type of a matrix is useful for optimization:
  you can often handle specific types more optimally than handling
  the generic case.
  */
DwTransform::TransformationType DwTransform::type() const
{
    if(m_dirty == TxNone || m_dirty < m_type)
        return static_cast<TransformationType>(m_type);

    switch (static_cast<TransformationType>(m_dirty))
    {
    case TxProject:
        if (!dwFuzzyIsNull(m_13) || !dwFuzzyIsNull(m_23) || !dwFuzzyIsNull(m_33 - 1))
        {
            m_type = TxProject;
            break;
        }
    case TxShear:
    case TxRotate:
        if (!dwFuzzyIsNull(affine._m12) || !dwFuzzyIsNull(affine._m21))
        {
            const DwReal dot = affine._m11 * affine._m12 + affine._m21 * affine._m22;
            if (dwFuzzyIsNull(dot))
                m_type = TxRotate;
            else
                m_type = TxShear;
            break;
        }
    case TxScale:
        if (!dwFuzzyIsNull(affine._m11 - 1) || !dwFuzzyIsNull(affine._m22 - 1))
        {
            m_type = TxScale;
            break;
        }
    case TxTranslate:
        if (!dwFuzzyIsNull(affine._dx) || !dwFuzzyIsNull(affine._dy))
        {
            m_type = TxTranslate;
            break;
        }
    case TxNone:
        m_type = TxNone;
        break;
    }

    m_dirty = TxNone;
    return static_cast<TransformationType>(m_type);
}
