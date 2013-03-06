#include "stable.h"
#include "dweasingcurve.h"
#include "dweasingcurvefunction_p.h"

//------------------------------------------------------------------------------
/**
*/
DwEasingCurve::DwEasingCurve(Dw::Curve c)
    : m_curve(Dw::Linear)
    , m_customFunc(&easeNone)
    , m_curveFuncObj(NULL)
{
    setCurve(c);
}

//------------------------------------------------------------------------------
/**
*/
DwEasingCurve::DwEasingCurve(const DwEasingCurve &other)
    : m_curve(Dw::Linear)
    , m_customFunc(&easeNone)
    , m_curveFuncObj(NULL)
{
    *this = other;
}

//------------------------------------------------------------------------------
/**
*/
DwEasingCurve& DwEasingCurve::operator=(const DwEasingCurve &other)
{
    if(this == &other) return *this;
    DW_SAFE_DELETE(m_curveFuncObj);

    m_curve = other.m_curve;
    m_customFunc = other.m_customFunc;
    if (other.m_curveFuncObj)
    {
        m_curveFuncObj = other.m_curveFuncObj->copy();
    }
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
DwEasingCurve::~DwEasingCurve()
{
    DW_SAFE_DELETE(m_curveFuncObj);
}

//------------------------------------------------------------------------------
/**
*/
bool DwEasingCurve::operator==(const DwEasingCurve &other) const
{
    bool res = m_curve == other.m_curve && m_customFunc == other.m_customFunc;
    if (res)
    {
        if (m_curveFuncObj && other.m_curveFuncObj)
        {
            res = m_curveFuncObj->operator==(*other.m_curveFuncObj);
        }
        else if (m_curveFuncObj || other.m_curveFuncObj)
        {
            res = false; // explicit express
        }
    }
    return res;
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwEasingCurve::curveOf(DwReal progress)
{
    progress = dwClamp<DwReal>(progress, 0.0, 1.0);
    if (m_customFunc)
    {
        return m_customFunc(progress);
    }
    else if (m_curveFuncObj)
    {
        return m_curveFuncObj->value(progress);
    }
    return progress;
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwEasingCurve::amplitude() const
{
    if(m_curveFuncObj) return m_curveFuncObj->_a;
    return DwReal(1.0);
}

//------------------------------------------------------------------------------
/**
*/
void DwEasingCurve::setAmplitude(DwReal a)
{
    if(m_curveFuncObj) m_curveFuncObj->_a = a;
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwEasingCurve::period() const
{
    if(m_curveFuncObj) return m_curveFuncObj->_p;
    return DwReal(0.3);
}

//------------------------------------------------------------------------------
/**
*/
void DwEasingCurve::setPeriod(DwReal p)
{
    if(m_curveFuncObj) m_curveFuncObj->_p = p;
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwEasingCurve::overshoot() const
{
    if(m_curveFuncObj) return m_curveFuncObj->_o;
    return DwReal(1.70158);
}

//------------------------------------------------------------------------------
/**
*/
void DwEasingCurve::setOvershoot(DwReal o)
{
    if(m_curveFuncObj) m_curveFuncObj->_o = o;
}

//------------------------------------------------------------------------------
/**
*/
Dw::Curve DwEasingCurve::curve() const
{
    return m_curve;
}

//------------------------------------------------------------------------------
/**
*/
void DwEasingCurve::setCurve(Dw::Curve c)
{
    if(m_curve == c) return;

    setCurve_helper(c);
}

//------------------------------------------------------------------------------
/**
*/
void DwEasingCurve::setCustomType(EasingFunction func)
{
    m_customFunc = func;
}

//------------------------------------------------------------------------------
/**
*/
DwEasingCurve::EasingFunction DwEasingCurve::customType() const
{
    return m_customFunc;
}

//------------------------------------------------------------------------------
/**
*/
static bool isConfigFunction(Dw::Curve c)
{
    return c >= Dw::InElastic && c <= Dw::OutInBounce;
}

//------------------------------------------------------------------------------
/**
*/
static DwEasingCurve::EasingFunction curveToFunc(Dw::Curve curve)
{
    switch(curve)
    {
    case Dw::Linear:
        return &easeNone;
    case Dw::InQuad:
        return &easeInQuad;
    case Dw::OutQuad:
        return &easeOutQuad;
    case Dw::InOutQuad:
        return &easeInOutQuad;
    case Dw::OutInQuad:
        return &easeOutInQuad;
    case Dw::InCubic:
        return &easeInCubic;
    case Dw::OutCubic:
        return &easeOutCubic;
    case Dw::InOutCubic:
        return &easeInOutCubic;
    case Dw::OutInCubic:
        return &easeOutInCubic;
    case Dw::InQuart:
        return &easeInQuart;
    case Dw::OutQuart:
        return &easeOutQuart;
    case Dw::InOutQuart:
        return &easeInOutQuart;
    case Dw::OutInQuart:
        return &easeOutInQuart;
    case Dw::InQuint:
        return &easeInQuint;
    case Dw::OutQuint:
        return &easeOutQuint;
    case Dw::InOutQuint:
        return &easeInOutQuint;
    case Dw::OutInQuint:
        return &easeOutInQuint;
    case Dw::InSine:
        return &easeInSine;
    case Dw::OutSine:
        return &easeOutSine;
    case Dw::InOutSine:
        return &easeInOutSine;
    case Dw::OutInSine:
        return &easeOutInSine;
    case Dw::InExpo:
        return &easeInExpo;
    case Dw::OutExpo:
        return &easeOutExpo;
    case Dw::InOutExpo:
        return &easeInOutExpo;
    case Dw::OutInExpo:
        return &easeOutInExpo;
    case Dw::InCirc:
        return &easeInCirc;
    case Dw::OutCirc:
        return &easeOutCirc;
    case Dw::InOutCirc:
        return &easeInOutCirc;
    case Dw::OutInCirc:
        return &easeOutInCirc;
        // Internal for, compatibility with QTimeLine only ??
    case Dw::InCurve:
        return &easeInCurve;
    case Dw::OutCurve:
        return &easeOutCurve;
    case Dw::SineCurve:
        return &easeSineCurve;
    case Dw::CosineCurve:
        return &easeCosineCurve;
    default:
        return 0;
    };
}

static DwEasingCurveFunction *curveToFunctionObject(Dw::Curve type)
{
    DwEasingCurveFunction *curveFunc = 0;
    switch(type)
    {
    case Dw::InElastic:
        curveFunc = new DwElasticEase(DwElasticEase::In);
        break;
    case Dw::OutElastic:
        curveFunc = new DwElasticEase(DwElasticEase::Out);
        break;
    case Dw::InOutElastic:
        curveFunc = new DwElasticEase(DwElasticEase::InOut);
        break;
    case Dw::OutInElastic:
        curveFunc = new DwElasticEase(DwElasticEase::OutIn);
        break;
    case Dw::OutBounce:
        curveFunc = new DwBounceEase(DwBounceEase::Out);
        break;
    case Dw::InBounce:
        curveFunc = new DwBounceEase(DwBounceEase::In);
        break;
    case Dw::OutInBounce:
        curveFunc = new DwBounceEase(DwBounceEase::OutIn);
        break;
    case Dw::InOutBounce:
        curveFunc = new DwBounceEase(DwBounceEase::InOut);
        break;
    case Dw::InBack:
        curveFunc = new DwBackEase(DwBackEase::In);
        break;
    case Dw::OutBack:
        curveFunc = new DwBackEase(DwBackEase::Out);
        break;
    case Dw::InOutBack:
        curveFunc = new DwBackEase(DwBackEase::InOut);
        break;
    case Dw::OutInBack:
        curveFunc = new DwBackEase(DwBackEase::OutIn);
        break;
    default:
        curveFunc = new DwEasingCurveFunction(DwEasingCurveFunction::In, DwReal(0.3), DwReal(1.0), DwReal(1.70158));
    }
    return curveFunc;
}

//------------------------------------------------------------------------------
/**
*/
void DwEasingCurve::setCurve_helper(Dw::Curve c)
{
    DwReal a = -1.0;
    DwReal p = -1.0;
    DwReal o = -1.0;

    if (m_curveFuncObj)
    {
        a = m_curveFuncObj->_a;
        p = m_curveFuncObj->_p;
        o = m_curveFuncObj->_o;
        DW_SAFE_DELETE(m_curveFuncObj);
    }

    if (isConfigFunction(c) || (a != -1.0) || (p != -1.0) || (o != -1.0))
    {
        m_curveFuncObj = curveToFunctionObject(c);
        if (a != -1.0)
            m_curveFuncObj->_a = a;
        if (p != -1.0)
            m_curveFuncObj->_p = p;
        if (o != -1.0)
            m_curveFuncObj->_o = o;
        m_customFunc = 0;
    }
    else if (c != Dw::Custom)
    {
        m_customFunc = curveToFunc(c);
    }
    DW_ASSERT((m_customFunc == 0) == (m_curveFuncObj != 0));
    m_curve = c;
}
