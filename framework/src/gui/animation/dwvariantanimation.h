#pragma once
#include "dwcore/dwvariant.h"
#include "dwgui/dwabstractanimation.h"
#include "dwgui/dweasingcurve.h"

//NB! VARIANT ANIMATION: ONLY SUPPORT THE TYPES THAT DEALED IN CORE
//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwVariantAnimation : public DwAbstractAnimation
{
    DW_DECLARE_CLASS(DwVariantAnimation);
public:
    typedef eastl::pair<DwReal, DwVariant> KeyValue;
    typedef DwVector<KeyValue> KeyValues;
    typedef DwVariant (*Interpolator)(const DwVariant &from, const DwVariant &to, DwReal progress);

    DwVariantAnimation(DwObject *parent = 0);
    virtual ~DwVariantAnimation();

    DwVariant startValue() const;
    void setStartValue(const DwVariant &value);

    DwVariant endValue() const;
    void setEndValue(const DwVariant &value);

    DwVariant keyValueAt(DwReal step) const;
    void setKeyValueAt(DwReal step, const DwVariant &v);

    KeyValues keyValues() const;
    void setKeyValues(const KeyValues &values);

    DwVariant currentValue() const;

    DwEasingCurve easingCurve() const;
    void setEasingCurve(const DwEasingCurve &easing);

    static Interpolator interpolatorOf(int interpolationType);

protected:
    // ovride the base class virtual interface
    virtual void updateCurrentTime(int);
    virtual void updateState(Dw::State newState, Dw::State oldState);

    // variant animation virtual interface
    virtual void updateCurrentValue(const DwVariant &v) = 0;
    virtual DwVariant interpolated(const DwVariant &from, const DwVariant &to, DwReal progress) const;

    void setCurrentValueForProgress(const DwReal progress);
    void recalculateCurrentInterval(bool force=false);
    void setValueAt(DwReal, const DwVariant &);
    DwVariant valueAt(DwReal step) const;
    void convertValues(int t);
    void updateInterpolator();

    DwVariant m_currentValue;
    DwVariant m_defaultStartEndValue;
    //this is used to keep track of the KeyValue interval in which we currently are
    struct
    {
        DwVariantAnimation::KeyValue start, end;
    } m_currentInterval;

    DwEasingCurve m_easing;
    DwVariantAnimation::KeyValues m_keyValues;
    Interpolator m_interpolator;
};// end of DwVariantAnimation