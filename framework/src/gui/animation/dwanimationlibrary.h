#pragma once
#include "dwcore/dwobject.h"
#include "dwcore/dwobjectptr.h"
#include "dwcore/dwlinkedlist.h"
#include "dwcore/sigslot.h"
#include "dwgui/dwtimer.h"
#include "dwgui/dwelapsedtimer.h"

//------------------------------------------------------------------------------
class DwAbstractAnimation;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwAnimationLibrary : public DwObject, public sigslot::has_slots
{
public:
    DwAnimationLibrary(DwObject *parent = NULL);
    virtual ~DwAnimationLibrary();

    static DwAnimationLibrary *instance();

    static void registerAnimation(DwAbstractAnimation *animation, bool isTopLevel);
    static void unregisterAnimation(DwAbstractAnimation *animation);

    //defines the timing interval. Default is DEFAULT_TIMER_INTERVAL
    void setTimingInterval(int interval);

    inline void setConsistentTiming(bool consistent);

    //these facilitate fine-tuning of complex animations
    inline void setSlowModeEnabled(bool enabled);
    inline void setSlowdownFactor(DwReal factor);

    static void ensureTimerUpdate();
    static void updateAnimationTimer();
    static void stopAll();

protected:
    friend class DwAbstractAnimation;

    void onAnimationTimerEvent();
    void onStartStopAnimationTimerEvent();
    void onTimerEvent(int id);
    void onDeleteLaterEvent();

    void registerRunningAnimation(DwAbstractAnimation *animation);
    void unregisterRunningAnimation(DwAbstractAnimation *animation);

    void restartAnimationTimer();

    void updateAnimationsTime();
    int closestPauseAnimationTimeToFinish();

    // timer used for all active (running) animations
    DwTimer m_animationTimer;
    // timer used to delay the check if we should start/stop the animation timer
    DwTimer m_startStopAnimationTimer;

    DwElapsedTimer m_time;

    uint64_t m_lastTick;
    int m_timingInterval;
    int m_currentAnimationIdx;
    bool m_consistentTiming;
    bool m_slowMode;

    // This factor will be used to divide the DEFAULT_TIMER_INTERVAL at each tick
    // when slowMode is enabled. Setting it to 0 or higher than DEFAULT_TIMER_INTERVAL (16)
    // stops all animations.
    DwReal m_slowdownFactor;

    // bool to indicate that only pause animations are active
    bool m_isPauseTimerActive;

    DwVector<DwAbstractAnimation*> m_animations, m_animationsToStart;

    // this is the count of running animations that are not a group neither a pause animation
    int m_runningLeafAnimations;
    DwVector<DwAbstractAnimation*> m_runningPauseAnimations;
private:
};// end of DwAnimationLibrary

//------------------------------------------------------------------------------
/**
*/
inline void DwAnimationLibrary::setConsistentTiming(bool consistent)
{
    m_consistentTiming = consistent;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwAnimationLibrary::setSlowModeEnabled(bool enabled)
{
    m_slowMode = enabled;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwAnimationLibrary::setSlowdownFactor(DwReal factor)
{
    m_slowdownFactor = factor;
}