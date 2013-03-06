#include "stable.h"
#include "dwmatrix.h"

// some defines to inline some code
#define MAPDOUBLE(x, y, nx, ny) \
{ \
    DwReal fx = x; \
    DwReal fy = y; \
    nx = _m11*fx + _m21*fy + _dx; \
    ny = _m12*fx + _m22*fy + _dy; \
}

#define MAPINT(x, y, nx, ny) \
{ \
    DwReal fx = x; \
    DwReal fy = y; \
    nx = dwRound(_m11*fx + _m21*fy + _dx); \
    ny = dwRound(_m12*fx + _m22*fy + _dy); \
}

DwMatrix::DwMatrix()
    : _m11(1.)
    , _m12(0.)
    , _m21(0.)
    , _m22(1.)
    , _dx(0.)
    , _dy(0.)
{
}

DwMatrix::DwMatrix(DwReal m11, DwReal m12, DwReal m21, DwReal m22, DwReal dx, DwReal dy)
    : _m11(m11)
    , _m12(m12)
    , _m21(m21)
    , _m22(m22)
    , _dx(dx)
    , _dy(dy)
{
}


/*!
     Constructs a matrix that is a copy of the given \a matrix.
 */
DwMatrix::DwMatrix(const DwMatrix &matrix)
    : _m11(matrix._m11)
    , _m12(matrix._m12)
    , _m21(matrix._m21)
    , _m22(matrix._m22)
    , _dx(matrix._dx)
    , _dy(matrix._dy)
{
}

/*!
    Sets the matrix elements to the specified values, \a m11, \a m12,
    \a m21, \a m22, \a dx and \a dy.

    Note that this function replaces the previous values. DwMatrix
    provide the translate(), rotate(), scale() and shear() convenience
    functions to manipulate the various matrix elements based on the
    currently defined coordinate system.

    \sa DwMatrix()
*/

void DwMatrix::setMatrix(DwReal m11, DwReal m12, DwReal m21, DwReal m22, DwReal dx, DwReal dy)
{
    _m11 = m11;
    _m12 = m12;
    _m21 = m21;
    _m22 = m22;
    _dx  = dx;
    _dy  = dy;
}

void DwMatrix::map(DwReal x, DwReal y, DwReal *tx, DwReal *ty) const
{
    MAPDOUBLE(x, y, *tx, *ty);
}

void DwMatrix::map(int x, int y, int *tx, int *ty) const
{
    MAPINT(x, y, *tx, *ty);
}

DwRect DwMatrix::mapRect(const DwRect &rect) const
{
    DwRect result;
    if (_m12 == 0.0F && _m21 == 0.0F)
    {
        int x = dwRound(_m11*rect.x() + _dx);
        int y = dwRound(_m22*rect.y() + _dy);
        int w = dwRound(_m11*rect.width());
        int h = dwRound(_m22*rect.height());
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
        result = DwRect(x, y, w, h);
    }
    else
    {
        // see mapToPolygon for explanations of the algorithm.
        DwReal x0, y0;
        DwReal x, y;
        MAPDOUBLE(rect.left(), rect.top(), x0, y0);
        DwReal xmin = x0;
        DwReal ymin = y0;
        DwReal xmax = x0;
        DwReal ymax = y0;
        MAPDOUBLE(rect.right() + 1, rect.top(), x, y);
        xmin = dwMin(xmin, x);
        ymin = dwMin(ymin, y);
        xmax = dwMax(xmax, x);
        ymax = dwMax(ymax, y);
        MAPDOUBLE(rect.right() + 1, rect.bottom() + 1, x, y);
        xmin = dwMin(xmin, x);
        ymin = dwMin(ymin, y);
        xmax = dwMax(xmax, x);
        ymax = dwMax(ymax, y);
        MAPDOUBLE(rect.left(), rect.bottom() + 1, x, y);
        xmin = dwMin(xmin, x);
        ymin = dwMin(ymin, y);
        xmax = dwMax(xmax, x);
        ymax = dwMax(ymax, y);
        result = DwRect(dwRound(xmin), dwRound(ymin), dwRound(xmax)-dwRound(xmin), dwRound(ymax)-dwRound(ymin));
    }
    return result;
}

/*!
    \fn DwRectF DwMatrix::mapRect(const DwRectF &rectangle) const

    Creates and returns a DwRectF object that is a copy of the given \a
    rectangle, mapped into the coordinate system defined by this
    matrix.

    The rectangle's coordinates are transformed using the following
    formulas:

    \snippet doc/src/snippets/code/src_gui_painting_DwMatrix.cpp 2

    If rotation or shearing has been specified, this function returns
    the \e bounding rectangle. To retrieve the exact region the given
    \a rectangle maps to, use the mapToPolygon() function instead.

    \sa mapToPolygon(), {DwMatrix#Basic Matrix Operations}{Basic Matrix
    Operations}
*/
DwRectF DwMatrix::mapRect(const DwRectF &rect) const
{
    DwRectF result;
    if (_m12 == 0.0F && _m21 == 0.0F)
    {
        DwReal x = _m11*rect.x() + _dx;
        DwReal y = _m22*rect.y() + _dy;
        DwReal w = _m11*rect.width();
        DwReal h = _m22*rect.height();
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
        result = DwRectF(x, y, w, h);
    }
    else
    {
        DwReal x0, y0;
        DwReal x, y;
        MAPDOUBLE(rect.x(), rect.y(), x0, y0);
        DwReal xmin = x0;
        DwReal ymin = y0;
        DwReal xmax = x0;
        DwReal ymax = y0;
        MAPDOUBLE(rect.x() + rect.width(), rect.y(), x, y);
        xmin = dwMin(xmin, x);
        ymin = dwMin(ymin, y);
        xmax = dwMax(xmax, x);
        ymax = dwMax(ymax, y);
        MAPDOUBLE(rect.x() + rect.width(), rect.y() + rect.height(), x, y);
        xmin = dwMin(xmin, x);
        ymin = dwMin(ymin, y);
        xmax = dwMax(xmax, x);
        ymax = dwMax(ymax, y);
        MAPDOUBLE(rect.x(), rect.y() + rect.height(), x, y);
        xmin = dwMin(xmin, x);
        ymin = dwMin(ymin, y);
        xmax = dwMax(xmax, x);
        ymax = dwMax(ymax, y);
        result = DwRectF(xmin, ymin, xmax-xmin, ymax - ymin);
    }
    return result;
}

/*!
    \fn DwRect DwMatrix::mapRect(const DwRect &rectangle) const
    \overload

    Creates and returns a DwRect object that is a copy of the given \a
    rectangle, mapped into the coordinate system defined by this
    matrix. Note that the transformed coordinates are rounded to the
    nearest integer.
*/


/*!
    \fn DwPoint operator*(const DwPoint &point, const DwMatrix &matrix)
    \relates DwMatrix

    This is the same as \a{matrix}.map(\a{point}).

    \sa DwMatrix::map()
*/

DwPoint DwMatrix::map(const DwPoint &p) const
{
    DwReal fx = p.x();
    DwReal fy = p.y();
    return DwPoint(dwRound(_m11*fx + _m21*fy + _dx),
                   dwRound(_m12*fx + _m22*fy + _dy));
}

/*!
    \fn DwPointF operator*(const DwPointF &point, const DwMatrix &matrix)
    \relates DwMatrix

    Same as \a{matrix}.map(\a{point}).

    \sa DwMatrix::map()
*/

/*!
    \overload

    Creates and returns a DwPointF object that is a copy of the given
    \a point, mapped into the coordinate system defined by this
    matrix.
*/
DwPointF DwMatrix::map(const DwPointF &point) const
{
    DwReal fx = point.x();
    DwReal fy = point.y();
    return DwPointF(_m11*fx + _m21*fy + _dx, _m12*fx + _m22*fy + _dy);
}

void DwMatrix::reset()
{
    _m11 = _m22 = 1.0;
    _m12 = _m21 = _dx = _dy = 0.0;
}

/*!
    \fn bool DwMatrix::isIdentity() const

    Returns true if the matrix is the identity matrix, otherwise
    returns false.

    \sa reset()
*/

/*!
    Moves the coordinate system \a dx along the x axis and \a dy along
    the y axis, and returns a reference to the matrix.

    \sa setMatrix()
*/

DwMatrix &DwMatrix::translate(DwReal dx, DwReal dy)
{
    _dx += dx*_m11 + dy*_m21;
    _dy += dy*_m22 + dx*_m12;
    return *this;
}

/*!
    \fn DwMatrix &DwMatrix::scale(DwReal sx, DwReal sy)

    Scales the coordinate system by \a sx horizontally and \a sy
    vertically, and returns a reference to the matrix.

    \sa setMatrix()
*/

DwMatrix &DwMatrix::scale(DwReal sx, DwReal sy)
{
    _m11 *= sx;
    _m12 *= sx;
    _m21 *= sy;
    _m22 *= sy;
    return *this;
}

/*!
    Shears the coordinate system by \a sh horizontally and \a sv
    vertically, and returns a reference to the matrix.

    \sa setMatrix()
*/

DwMatrix &DwMatrix::shear(DwReal sh, DwReal sv)
{
    DwReal tm11 = sv*_m21;
    DwReal tm12 = sv*_m22;
    DwReal tm21 = sh*_m11;
    DwReal tm22 = sh*_m12;
    _m11 += tm11;
    _m12 += tm12;
    _m21 += tm21;
    _m22 += tm22;
    return *this;
}

const DwReal deg2rad = DwReal(0.017453292519943295769);        // pi/180

/*!
    \fn DwMatrix &DwMatrix::rotate(DwReal degrees)

    Rotates the coordinate system the given \a degrees
    counterclockwise.

    Note that if you apply a DwMatrix to a point defined in widget
    coordinates, the direction of the rotation will be clockwise
    because the y-axis points downwards.

    Returns a reference to the matrix.

    \sa setMatrix()
*/

DwMatrix &DwMatrix::rotate(DwReal a)
{
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
        DwReal b = deg2rad*a;                        // convert to radians
        sina = dwSin(b);               // fast and convenient
        cosa = dwCos(b);
    }
    DwReal tm11 = cosa*_m11 + sina*_m21;
    DwReal tm12 = cosa*_m12 + sina*_m22;
    DwReal tm21 = -sina*_m11 + cosa*_m21;
    DwReal tm22 = -sina*_m12 + cosa*_m22;
    _m11 = tm11;
    _m12 = tm12;
    _m21 = tm21;
    _m22 = tm22;
    return *this;
}

/*!
    \fn bool DwMatrix::isInvertible() const

    Returns true if the matrix is invertible, otherwise returns false.

    \sa inverted()
*/

/*!
    \obsolete
    \fn DwReal DwMatrix::det() const

    Returns the matrix's determinant.

    \sa determinant()
*/

/*!
    \since 4.6
    \fn DwReal DwMatrix::determinant() const

    Returns the matrix's determinant.
*/

/*!
    \fn DwMatrix DwMatrix::invert(bool *invertible) const

    Returns an inverted copy of this matrix.

    Use the inverted() function instead.
*/

/*!
    Returns an inverted copy of this matrix.

    If the matrix is singular (not invertible), the returned matrix is
    the identity matrix. If \a invertible is valid (i.e. not 0), its
    value is set to true if the matrix is invertible, otherwise it is
    set to false.

    \sa isInvertible()
*/

DwMatrix DwMatrix::inverted(bool *invertible) const
{
    DwReal dtr = determinant();
    if (dtr == 0.0)
    {
        if (invertible)
            *invertible = false;                // singular matrix
        return DwMatrix(true);
    }
    else                                          // invertible matrix
    {
        if (invertible)
            *invertible = true;
        DwReal dinv = 1.0/dtr;
        return DwMatrix((_m22*dinv),        (-_m12*dinv),
                        (-_m21*dinv), (_m11*dinv),
                        ((_m21*_dy - _m22*_dx)*dinv),
                        ((_m12*_dx - _m11*_dy)*dinv),
                        true);
    }
}


/*!
    \fn bool DwMatrix::operator==(const DwMatrix &matrix) const

    Returns true if this matrix is equal to the given \a matrix,
    otherwise returns false.
*/

bool DwMatrix::operator==(const DwMatrix &m) const
{
    return _m11 == m._m11 &&
           _m12 == m._m12 &&
           _m21 == m._m21 &&
           _m22 == m._m22 &&
           _dx == m._dx &&
           _dy == m._dy;
}

/*!
    \fn bool DwMatrix::operator!=(const DwMatrix &matrix) const

    Returns true if this matrix is not equal to the given \a matrix,
    otherwise returns false.
*/

bool DwMatrix::operator!=(const DwMatrix &m) const
{
    return _m11 != m._m11 ||
           _m12 != m._m12 ||
           _m21 != m._m21 ||
           _m22 != m._m22 ||
           _dx != m._dx ||
           _dy != m._dy;
}

/*!
    \fn DwMatrix &DwMatrix::operator *=(const DwMatrix &matrix)
    \overload

    Returns the result of multiplying this matrix by the given \a
    matrix.
*/

DwMatrix &DwMatrix::operator *=(const DwMatrix &m)
{
    DwReal tm11 = _m11*m._m11 + _m12*m._m21;
    DwReal tm12 = _m11*m._m12 + _m12*m._m22;
    DwReal tm21 = _m21*m._m11 + _m22*m._m21;
    DwReal tm22 = _m21*m._m12 + _m22*m._m22;

    DwReal tdx  = _dx*m._m11  + _dy*m._m21 + m._dx;
    DwReal tdy =  _dx*m._m12  + _dy*m._m22 + m._dy;

    _m11 = tm11;
    _m12 = tm12;
    _m21 = tm21;
    _m22 = tm22;
    _dx = tdx;
    _dy = tdy;
    return *this;
}

/*!
    \fn DwMatrix DwMatrix::operator *(const DwMatrix &matrix) const

    Returns the result of multiplying this matrix by the given \a
    matrix.

    Note that matrix multiplication is not commutative, i.e. a*b !=
    b*a.
*/

DwMatrix DwMatrix::operator *(const DwMatrix &m) const
{
    DwReal tm11 = _m11*m._m11 + _m12*m._m21;
    DwReal tm12 = _m11*m._m12 + _m12*m._m22;
    DwReal tm21 = _m21*m._m11 + _m22*m._m21;
    DwReal tm22 = _m21*m._m12 + _m22*m._m22;

    DwReal tdx  = _dx*m._m11  + _dy*m._m21 + m._dx;
    DwReal tdy =  _dx*m._m12  + _dy*m._m22 + m._dy;
    return DwMatrix(tm11, tm12, tm21, tm22, tdx, tdy, true);
}

/*!
    Assigns the given \a matrix's values to this matrix.
*/
DwMatrix &DwMatrix::operator=(const DwMatrix &matrix)
{
    _m11 = matrix._m11;
    _m12 = matrix._m12;
    _m21 = matrix._m21;
    _m22 = matrix._m22;
    _dx  = matrix._dx;
    _dy  = matrix._dy;
    return *this;
}
