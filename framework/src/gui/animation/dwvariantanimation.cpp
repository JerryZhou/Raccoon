#include "stable.h"
#include "dwvariantanimation.h"
#include "dwvariantanimation_p.h"

//------------------------------------------------------------------------------
DW_IMPLEMENT_ABSTRACT_CLASS(DwVariantAnimation, 'VANN', DwAbstractAnimation);

//------------------------------------------------------------------------------
/**
*/
DwVariantAnimation::DwVariantAnimation(DwObject *parent/* = 0*/)
    : DwAbstractAnimation(parent)
    , m_interpolator(&defaultInterpolator)
    , m_easing(Dw::Linear)
{
    m_duration = 250;
}

//------------------------------------------------------------------------------
/**
*/
DwVariantAnimation::~DwVariantAnimation()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwVariant DwVariantAnimation::startValue() const
{
    return valueAt(0.0);
}

//------------------------------------------------------------------------------
/**
*/
void DwVariantAnimation::setStartValue(const DwVariant &v)
{
    setValueAt(0.0, v);
}

//------------------------------------------------------------------------------
/**
*/
DwVariant DwVariantAnimation::endValue() const
{
    return valueAt(1.0);
}

//------------------------------------------------------------------------------
/**
*/
void DwVariantAnimation::setEndValue(const DwVariant &v)
{
    setValueAt(1.0, v);
}

//------------------------------------------------------------------------------
/**
*/
DwVariant DwVariantAnimation::keyValueAt(DwReal step) const
{
    return valueAt(step);
}

//------------------------------------------------------------------------------
/**
*/
void DwVariantAnimation::setKeyValueAt(DwReal step, const DwVariant &v)
{
    setValueAt(step, v);
}

//------------------------------------------------------------------------------
/**
*/
DwVariantAnimation::KeyValues DwVariantAnimation::keyValues() const
{
    return m_keyValues;
}

//------------------------------------------------------------------------------
/**
*/
void DwVariantAnimation::setKeyValues(const KeyValues &values)
{
    m_keyValues = values;
}

//------------------------------------------------------------------------------
/**
*/
DwVariant DwVariantAnimation::currentValue() const
{
    return m_currentValue;
}

//------------------------------------------------------------------------------
/**
*/
DwEasingCurve DwVariantAnimation::easingCurve() const
{
    return m_easing;
}

//------------------------------------------------------------------------------
/**
*/
void DwVariantAnimation::setEasingCurve(const DwEasingCurve &easing)
{
    m_easing = easing;
}

//------------------------------------------------------------------------------
/**
*/
DwVariantAnimation::Interpolator DwVariantAnimation::interpolatorOf(int interpolationType)
{
    switch (interpolationType)
    {
    case DwVariant::Int:
        return _dw_v_interpolator<DwVariant::Int>;
        break;
    case DwVariant::UInt:
        return _dw_v_interpolator<DwVariant::UInt>;
        break;
    case DwVariant::Double:
        return _dw_v_interpolator<DwVariant::Double>;
        break;
    case DwVariant::Float:
        return _dw_v_interpolator<DwVariant::Float>;
        break;
    case DwVariant::Int64:
        return _dw_v_interpolator<DwVariant::Int64>;
        break;
    case DwVariant::UInt64:
        return _dw_v_interpolator<DwVariant::UInt64>;
        break;
    case DwVariant::RectF:
        return _dw_v_interpolator<DwVariant::RectF>;
        break;
    case DwVariant::Rect:
        return _dw_v_interpolator<DwVariant::Rect>;
        break;
    case DwVariant::PointF:
        return _dw_v_interpolator<DwVariant::PointF>;
        break;
    case DwVariant::Point:
        return _dw_v_interpolator<DwVariant::Point>;
        break;
    case DwVariant::SizeF:
        return _dw_v_interpolator<DwVariant::SizeF>;
        break;
    case DwVariant::Size:
        return _dw_v_interpolator<DwVariant::Size>;
        break;
    }
    return _dw_v_interpolator<DwVariant::Invalid>;
}

//------------------------------------------------------------------------------
/**
*/
void DwVariantAnimation::updateCurrentTime(int)
{
    recalculateCurrentInterval();
}

//------------------------------------------------------------------------------
/**
*/
void DwVariantAnimation::updateState(Dw::State newState, Dw::State oldState)
{
    DW_UNUSED(oldState);
    DW_UNUSED(newState);
}

//------------------------------------------------------------------------------
/**
*/
DwVariant DwVariantAnimation::interpolated(const DwVariant &from, const DwVariant &to, DwReal progress) const
{
    if(m_interpolator)
    {
        return m_interpolator(from, to, progress);
    }
    return defaultInterpolator(from, to, progress);
}

//------------------------------------------------------------------------------
/**
*/
void DwVariantAnimation::setCurrentValueForProgress(const DwReal progress)
{
    const DwReal startProgress = m_currentInterval.start.first;
    const DwReal endProgress = m_currentInterval.end.first;
    const DwReal localProgress = (progress - startProgress) / (endProgress - startProgress);

    m_currentValue = interpolated(m_currentInterval.start.second,
                                  m_currentInterval.end.second,
                                  localProgress);
    updateCurrentValue(m_currentValue);
}

//------------------------------------------------------------------------------
/**
*/
void DwVariantAnimation::recalculateCurrentInterval(bool force/*=false*/)
{
    // can't interpolate if we don't have at least 2 values
    if ((m_keyValues.count() + (m_defaultStartEndValue.isValid() ? 1 : 0)) < 2)
        return;

    const DwReal progress = m_easing.curveOf(((m_duration == 0) ? DwReal(1) : DwReal(m_currentTime) / DwReal(m_duration)));

    //0 and 1 are still the boundaries
    if (force || (m_currentInterval.start.first > 0 && progress < m_currentInterval.start.first)
            || (m_currentInterval.end.first < 1 && progress > m_currentInterval.end.first))
    {
        //let's update currentInterval
        DwVariantAnimation::KeyValues::const_iterator it = dwLowerBound(m_keyValues.begin(),
                m_keyValues.end(),
                dwMakePair(progress, DwVariant()),
                animationValueLessThan);
        if (it == m_keyValues.begin())
        {
            //the item pointed to by it is the start element in the range
            if (it->first == 0 && m_keyValues.count() > 1)
            {
                m_currentInterval.start = *it;
                m_currentInterval.end = *(it+1);
            }
            else
            {
                m_currentInterval.start = dwMakePair(DwReal(0), m_defaultStartEndValue);
                m_currentInterval.end = *it;
            }
        }
        else if (it == m_keyValues.end())
        {
            --it; //position the iterator on the last item
            if (it->first == 1 && m_keyValues.count() > 1)
            {
                //we have an end value (item with progress = 1)
                m_currentInterval.start = *(it-1);
                m_currentInterval.end = *it;
            }
            else
            {
                //we use the default end value here
                m_currentInterval.start = *it;
                m_currentInterval.end = dwMakePair(DwReal(1), m_defaultStartEndValue);
            }
        }
        else
        {
            m_currentInterval.start = *(it-1);
            m_currentInterval.end = *it;
        }

        // update all the values of the currentInterval
        updateInterpolator();
    }
    setCurrentValueForProgress(progress);
}

//------------------------------------------------------------------------------
/**
*/
void DwVariantAnimation::setValueAt(DwReal step, const DwVariant &value)
{
    if (step < DwReal(0.0) || step > DwReal(1.0))
    {
        DW_WARNING("DwVariantAnimation::setValueAt: invalid step");
        return;
    }

    DwVariantAnimation::KeyValue pair(step, value);

    DwVariantAnimation::KeyValues::iterator result = dwLowerBound(m_keyValues.begin(), m_keyValues.end(), pair, animationValueLessThan);
    if (result == m_keyValues.end() || result->first != step)
    {
        m_keyValues.insert(result, pair);
    }
    else
    {
        if (value.isValid())
            result->second = value; // replaces the previous value
        else
            m_keyValues.erase(result); // removes the previous value
    }

    recalculateCurrentInterval(/*force=*/true);
}

//------------------------------------------------------------------------------
/**
*/
DwVariant DwVariantAnimation::valueAt(DwReal step) const
{
    DwVariantAnimation::KeyValues::const_iterator result =
        dwFind(m_keyValues.begin(), m_keyValues.end(),  dwBind2nd(animationValueLessThanFunction(), KeyValue(step, DwVariant())));
    if (result != m_keyValues.end())
        return result->second;

    return DwVariant();
}

//------------------------------------------------------------------------------
/**
	TODO:
*/
void DwVariantAnimation::convertValues(int t)
{
    DW_UNUSED(t);
    ////this ensures that all the keyValues are of type t
    //for (int i = 0; i < m_keyValues.count(); ++i)
    //{
    //	DwVariantAnimation::KeyValue &pair = m_keyValues[i];
    //	pair.second.convert(static_cast<DwVariant::Type>(t));
    //}
    ////we also need update to the current interval if needed
    //currentInterval.start.second.convert(static_cast<DwVariant::Type>(t));
    //currentInterval.end.second.convert(static_cast<DwVariant::Type>(t));

    ////... and the interpolator
    //updateInterpolator();
}

//------------------------------------------------------------------------------
/**
*/
void DwVariantAnimation::updateInterpolator()
{
    m_interpolator = interpolatorOf(m_currentInterval.start.second.vtype());

    if(m_currentInterval.start.second.vtype() != m_currentInterval.end.second.vtype())
    {
        // ### May be can be set according to the start type
        m_interpolator = interpolatorOf(DwVariant::Invalid);// as the default
    }
}