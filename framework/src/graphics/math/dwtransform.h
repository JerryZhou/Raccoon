#pragma once
#include "dwmatrix.h"
#include "dwcore/dwpoint.h"
#include "dwcore/dwrect.h"

//------------------------------------------------------------------------------
class DW_GRAPHICS_EXPORT DwTransform
{
public:
    enum TransformationType
    {
        TxNone      = 0x00,
        TxTranslate = 0x01,
        TxScale     = 0x02,
        TxRotate    = 0x04,
        TxShear     = 0x08,
        TxProject   = 0x10
    };

    inline explicit DwTransform(Dw::Initialization) : affine(Dw::Uninitialized) {}
    DwTransform();
    DwTransform(DwReal h11, DwReal h12, DwReal h13,
                DwReal h21, DwReal h22, DwReal h23,
                DwReal h31, DwReal h32, DwReal h33 = 1.0);
    DwTransform(DwReal h11, DwReal h12, DwReal h21,
                DwReal h22, DwReal dx, DwReal dy);
    explicit DwTransform(const DwMatrix &mtx);

    bool isAffine() const;
    bool isIdentity() const;
    bool isInvertible() const;
    bool isScaling() const;
    bool isRotating() const;
    bool isTranslating() const;

    TransformationType type() const;

    inline DwReal determinant() const;
    DwReal det() const;

    DwReal m11() const;
    DwReal m12() const;
    DwReal m13() const;
    DwReal m21() const;
    DwReal m22() const;
    DwReal m23() const;
    DwReal m31() const;
    DwReal m32() const;
    DwReal m33() const;
    DwReal dx() const;
    DwReal dy() const;

    void setMatrix(DwReal m11, DwReal m12, DwReal m13,
                   DwReal m21, DwReal m22, DwReal m23,
                   DwReal m31, DwReal m32, DwReal m33);

    DwTransform inverted(bool *invertible = 0) const;
    DwTransform adjoint() const;
    DwTransform transposed() const;

    DwTransform &translate(DwReal dx, DwReal dy);
    DwTransform &scale(DwReal sx, DwReal sy);
    DwTransform &shear(DwReal sh, DwReal sv);
    DwTransform &rotate(DwReal a, Dw::Axis axis = Dw::ZAxis);
    DwTransform &rotateRadians(DwReal a, Dw::Axis axis = Dw::ZAxis);

    void setTranslated(DwReal dx, DwReal dy);
    DwPointF translated() const;
    void setScaled(DwReal sx, DwReal sy);
    DwPointF scaled() const;
    void setSheard(DwReal sh, DwReal sv);
    DwPointF shearded() const;
    // TODO: supported
    //void setRotated(DwReal a, Dw::Axis axis = Dw::ZAxis);
    //DwPointF rotated() const;

    bool operator==(const DwTransform &) const;
    bool operator!=(const DwTransform &) const;

    DwTransform &operator*=(const DwTransform &);
    DwTransform operator*(const DwTransform &o) const;

    DwTransform &operator=(const DwTransform &);

    void reset();
    DwPoint       map(const DwPoint &p) const;
    DwPointF      map(const DwPointF &p) const;
    DwRect mapRect(const DwRect &) const;
    DwRectF mapRect(const DwRectF &) const;
    void map(int x, int y, int *tx, int *ty) const;
    void map(DwReal x, DwReal y, DwReal *tx, DwReal *ty) const;

    const DwMatrix &toAffine() const;

    DwTransform &operator*=(DwReal div);
    DwTransform &operator/=(DwReal div);
    DwTransform &operator+=(DwReal div);
    DwTransform &operator-=(DwReal div);

    static DwTransform fromTranslate(DwReal dx, DwReal dy);
    static DwTransform fromScale(DwReal dx, DwReal dy);

private:
    friend inline bool dwFuzzyCompare(const DwTransform& t1, const DwTransform& t2);
    inline DwTransform(DwReal h11, DwReal h12, DwReal h13,
                       DwReal h21, DwReal h22, DwReal h23,
                       DwReal h31, DwReal h32, DwReal h33, bool)
        : affine(h11, h12, h21, h22, h31, h32, true)
        , m_13(h13), m_23(h23), m_33(h33)
        , m_type(TxNone)
        , m_dirty(TxProject) {}
    inline DwTransform(bool)
        : affine(true)
        , m_13(0), m_23(0), m_33(1)
        , m_type(TxNone)
        , m_dirty(TxNone) {}
    inline TransformationType inline_type() const;
    DwMatrix affine;
    DwReal   m_13;
    DwReal   m_23;
    DwReal   m_33;

    mutable uint32_t m_type : 5;
    mutable uint32_t m_dirty : 5;
};// end of DwTransform

/******* inlines *****/
inline DwTransform::TransformationType DwTransform::inline_type() const
{
    if (m_dirty == TxNone)
        return static_cast<TransformationType>(m_type);
    return type();
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwTransform::isAffine() const
{
    return inline_type() < TxProject;
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwTransform::isIdentity() const
{
    return inline_type() == TxNone;
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwTransform::isInvertible() const
{
    return !dwFuzzyIsNull(determinant());
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwTransform::isScaling() const
{
    return type() >= TxScale;
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwTransform::isRotating() const
{
    return inline_type() >= TxRotate;
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwTransform::isTranslating() const
{
    return inline_type() >= TxTranslate;
}

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwTransform::determinant() const
{
    return affine._m11*(m_33*affine._m22-affine._dy*m_23) -
           affine._m21*(m_33*affine._m12-affine._dy*m_13)+affine._dx*(m_23*affine._m12-affine._m22*m_13);
}

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwTransform::det() const
{
    return determinant();
}

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwTransform::m11() const
{
    return affine._m11;
}

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwTransform::m12() const
{
    return affine._m12;
}

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwTransform::m13() const
{
    return m_13;
}

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwTransform::m21() const
{
    return affine._m21;
}

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwTransform::m22() const
{
    return affine._m22;
}

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwTransform::m23() const
{
    return m_23;
}

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwTransform::m31() const
{
    return affine._dx;
}

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwTransform::m32() const
{
    return affine._dy;
}

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwTransform::m33() const
{
    return m_33;
}

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwTransform::dx() const
{
    return affine._dx;
}

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwTransform::dy() const
{
    return affine._dy;
}

//------------------------------------------------------------------------------
/**
*/
inline DwTransform &DwTransform::operator*=(DwReal num)
{
    if (num == 1.)
        return *this;
    affine._m11 *= num;
    affine._m12 *= num;
    m_13        *= num;
    affine._m21 *= num;
    affine._m22 *= num;
    m_23        *= num;
    affine._dx  *= num;
    affine._dy  *= num;
    m_33        *= num;
    if (m_dirty < TxScale)
        m_dirty = TxScale;
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
inline DwTransform &DwTransform::operator/=(DwReal div)
{
    if (div == 0)
        return *this;
    div = 1/div;
    return operator*=(div);
}

//------------------------------------------------------------------------------
/**
*/
inline DwTransform &DwTransform::operator+=(DwReal num)
{
    if (num == 0)
        return *this;
    affine._m11 += num;
    affine._m12 += num;
    m_13        += num;
    affine._m21 += num;
    affine._m22 += num;
    m_23        += num;
    affine._dx  += num;
    affine._dy  += num;
    m_33        += num;
    m_dirty     = TxProject;
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
inline DwTransform &DwTransform::operator-=(DwReal num)
{
    if (num == 0)
        return *this;
    affine._m11 -= num;
    affine._m12 -= num;
    m_13        -= num;
    affine._m21 -= num;
    affine._m22 -= num;
    m_23        -= num;
    affine._dx  -= num;
    affine._dy  -= num;
    m_33        -= num;
    m_dirty     = TxProject;
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
inline bool dwFuzzyCompare(const DwTransform& t1, const DwTransform& t2)
{
    return dwFuzzyCompare(t1.m11(), t2.m11())
           && dwFuzzyCompare(t1.m12(), t2.m12())
           && dwFuzzyCompare(t1.m13(), t2.m13())
           && dwFuzzyCompare(t1.m21(), t2.m21())
           && dwFuzzyCompare(t1.m22(), t2.m22())
           && dwFuzzyCompare(t1.m23(), t2.m23())
           && dwFuzzyCompare(t1.m31(), t2.m31())
           && dwFuzzyCompare(t1.m32(), t2.m32())
           && dwFuzzyCompare(t1.m33(), t2.m33());
}