#pragma once
#include "dwnamespace.h"
#include "dwgraphics/dwmatrix.h" // dependence for dwreal
#include "dweasing_p.h"

//------------------------------------------------------------------------------
class DwEasingCurveFunction
{
public:
    enum Type { In, Out, InOut, OutIn };

    DwEasingCurveFunction(DwEasingCurveFunction::Type type = In, DwReal period = 0.3, DwReal amplitude = 1.0,
                          DwReal overshoot = 1.70158)
        : _t(type), _p(period), _a(amplitude), _o(overshoot)
    { }
    virtual ~DwEasingCurveFunction() {}
    virtual DwReal value(DwReal t);
    virtual DwEasingCurveFunction *copy() const;
    bool operator==(const DwEasingCurveFunction& other);

    Type _t;
    DwReal _p;
    DwReal _a;
    DwReal _o;
};

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwEasingCurveFunction::value(DwReal t)
{
    return t;
}
//------------------------------------------------------------------------------
/**
*/
inline DwEasingCurveFunction *DwEasingCurveFunction::copy() const
{
    return new DwEasingCurveFunction(_t, _p, _a, _o);
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwEasingCurveFunction::operator==(const DwEasingCurveFunction& other)
{
    return _t == other._t &&
           dwFuzzyCompare(_p, other._p) &&
           dwFuzzyCompare(_a, other._a) &&
           dwFuzzyCompare(_o, other._o);
}

//------------------------------------------------------------------------------
/**
*/
struct DwElasticEase : public DwEasingCurveFunction
{
    DwElasticEase(Type type)
        : DwEasingCurveFunction(type, DwReal(0.3), DwReal(1.0))
    { }

    DwEasingCurveFunction *copy() const
    {
        DwElasticEase *rv = new DwElasticEase(_t);
        rv->_p = _p;
        rv->_a = _a;
        return rv;
    }

    DwReal value(DwReal t)
    {
        DwReal p = (_p < 0) ? DwReal(0.3) : _p;
        DwReal a = (_a < 0) ? DwReal(1.0) : _a;
        switch(_t)
        {
        case In:
            return easeInElastic(t, a, p);
        case Out:
            return easeOutElastic(t, a, p);
        case InOut:
            return easeInOutElastic(t, a, p);
        case OutIn:
            return easeOutInElastic(t, a, p);
        default:
            return t;
        }
    }
};

//------------------------------------------------------------------------------
/**
*/
struct DwBounceEase : public DwEasingCurveFunction
{
    DwBounceEase(Type type)
        : DwEasingCurveFunction(type, DwReal(0.3), DwReal(1.0))
    { }

    DwEasingCurveFunction *copy() const
    {
        DwBounceEase *rv = new DwBounceEase(_t);
        rv->_a = _a;
        return rv;
    }

    DwReal value(DwReal t)
    {
        DwReal a = (_a < 0) ? DwReal(1.0) : _a;
        switch(_t)
        {
        case In:
            return easeInBounce(t, a);
        case Out:
            return easeOutBounce(t, a);
        case InOut:
            return easeInOutBounce(t, a);
        case OutIn:
            return easeOutInBounce(t, a);
        default:
            return t;
        }
    }
};

//------------------------------------------------------------------------------
/**
*/
struct DwBackEase : public DwEasingCurveFunction
{
    DwBackEase(Type type)
        : DwEasingCurveFunction(type, DwReal(0.3), DwReal(1.0), DwReal(1.70158))
    { }

    DwEasingCurveFunction *copy() const
    {
        DwBackEase *rv = new DwBackEase(_t);
        rv->_o = _o;
        return rv;
    }

    DwReal value(DwReal t)
    {
        DwReal o = (_o < 0) ? DwReal(1.70158) : _o;
        switch(_t)
        {
        case In:
            return easeInBack(t, o);
        case Out:
            return easeOutBack(t, o);
        case InOut:
            return easeInOutBack(t, o);
        case OutIn:
            return easeOutInBack(t, o);
        default:
            return t;
        }
    }
};