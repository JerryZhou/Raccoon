#pragma once
#include "dwnamespace.h"
#include "dwgraphics/dwmatrix.h" // dependence for dwreal

//------------------------------------------------------------------------------
class DwEasingCurveFunction;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwEasingCurve
{
public:
    typedef DwReal (*EasingFunction)(DwReal progress);

    DwEasingCurve(Dw::Curve );
    DwEasingCurve(const DwEasingCurve &other);
    virtual ~DwEasingCurve();

    DwEasingCurve &operator=(const DwEasingCurve &other);
    bool operator==(const DwEasingCurve &other) const;
    inline bool operator!=(const DwEasingCurve &other) const;

    virtual DwReal curveOf(DwReal progress);

    DwReal amplitude() const;
    void setAmplitude(DwReal amplitude);

    DwReal period() const;
    void setPeriod(DwReal period);

    DwReal overshoot() const;
    void setOvershoot(DwReal overshoot);

    Dw::Curve curve() const;
    void setCurve(Dw::Curve );

    void setCustomType(EasingFunction func);
    EasingFunction customType() const;

protected:
    void setCurve_helper(Dw::Curve );

    Dw::Curve m_curve;
    EasingFunction m_customFunc;
    DwEasingCurveFunction *m_curveFuncObj;
};// end of DwEasingCurve

//------------------------------------------------------------------------------
/**
*/
inline bool DwEasingCurve::operator!=(const DwEasingCurve &other) const
{
    return !(this->operator==(other));
}