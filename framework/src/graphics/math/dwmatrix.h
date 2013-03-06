#pragma once
#include "dwcore/dwpoint.h"
#include "dwcore/dwrect.h"

// dummpy dependency
//////////////////////////////////////////////////////////////////////////
//typedef double DwReal; // moved to dwtypes.h
#define dwCos ::cos
#define dwSin ::sin
#define dwPow ::pow
#define dwSqrt ::sqrt
#define dwFabs ::fabs
#define dwAsin ::asin
//////////////////////////////////////////////////////////////////////////

class DW_GRAPHICS_EXPORT DwMatrix // 2D transform matrix
{
public:
    DwMatrix();
    DwMatrix(DwReal m11, DwReal m12, DwReal m21, DwReal m22,
             DwReal dx, DwReal dy);
    DwMatrix(const DwMatrix &matrix);

    void setMatrix(DwReal m11, DwReal m12, DwReal m21, DwReal m22,
                   DwReal dx, DwReal dy);

    inline DwReal m11() const;
    inline DwReal m12() const;
    inline DwReal m21() const;
    inline DwReal m22() const;
    inline DwReal dx() const;
    inline DwReal dy() const;

    void map(int x, int y, int *tx, int *ty) const;
    void map(DwReal x, DwReal y, DwReal *tx, DwReal *ty) const;
    DwRect mapRect(const DwRect &) const;
    DwRectF mapRect(const DwRectF &) const;

    DwPoint map(const DwPoint &p) const;
    DwPointF map(const DwPointF&p) const;

    void reset();
    inline bool isIdentity() const;

    DwMatrix &translate(DwReal dx, DwReal dy);
    DwMatrix &scale(DwReal sx, DwReal sy);
    DwMatrix &shear(DwReal sh, DwReal sv);
    DwMatrix &rotate(DwReal a);

    inline bool isInvertible() const;
    inline DwReal determinant() const;

    DwMatrix inverted(bool *invertible = 0) const;

    bool operator==(const DwMatrix &) const;
    bool operator!=(const DwMatrix &) const;

    DwMatrix &operator*=(const DwMatrix &);
    DwMatrix operator*(const DwMatrix &o) const;

    DwMatrix &operator=(const DwMatrix &);

private:
    inline DwMatrix(bool)
        : _m11(1.)
        , _m12(0.)
        , _m21(0.)
        , _m22(1.)
        , _dx(0.)
        , _dy(0.) {}
    inline DwMatrix(DwReal am11, DwReal am12, DwReal am21, DwReal am22, DwReal adx, DwReal ady, bool)
        : _m11(am11)
        , _m12(am12)
        , _m21(am21)
        , _m22(am22)
        , _dx(adx)
        , _dy(ady) {}
    friend class DwTransform;
    DwReal _m11, _m12;
    DwReal _m21, _m22;
    DwReal _dx, _dy;
};// end of DwMatrix

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwMatrix::m11() const
{
    return _m11;
}

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwMatrix::m12() const
{
    return _m12;
}

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwMatrix::m21() const
{
    return _m21;
}

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwMatrix::m22() const
{
    return _m22;
}

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwMatrix::dx() const
{
    return _dx;
}

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwMatrix::dy() const
{
    return _dy;
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwMatrix::isIdentity() const
{
    return dwFuzzyIsNull(_m11 - 1) && dwFuzzyIsNull(_m22 - 1) && dwFuzzyIsNull(_m12)
        && dwFuzzyIsNull(_m21) && dwFuzzyIsNull(_dx) && dwFuzzyIsNull(_dy);
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwMatrix::isInvertible() const
{
    return !dwFuzzyIsNull(_m11*_m22 - _m12*_m21);
}
//------------------------------------------------------------------------------
/**
*/
inline DwReal DwMatrix::determinant() const
{
    return _m11*_m22 - _m12*_m21;
}