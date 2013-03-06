#include "stable.h"
#include "dwanimationlibrary.h"
#include "dwabstractanimation.h"
#include "dwcore/dwglobalstatic.h"

// NB! REFRE TO QT 4.7.3 CORELIB
#define DEFAULT_TIMER_INTERVAL 16
#define STARTSTOP_TIMER_DELAY 0
//------------------------------------------------------------------------------
/**
*/
DwAnimationLibrary::DwAnimationLibrary(DwObject *parent)
    : DwObject(parent)
    , m_lastTick(0)
    , m_timingInterval(DEFAULT_TIMER_INTERVAL)
    , m_currentAnimationIdx(0)
    , m_consistentTiming(false)
    , m_slowMode(false)
    , m_slowdownFactor(5.0f)
    , m_isPauseTimerActive(false)
    , m_runningLeafAnimations(0)
{
    m_time.invalidate();
    m_animationTimer.sigTimeout.connect(this, &DwAnimationLibrary::onAnimationTimerEvent);
    m_startStopAnimationTimer.sigTimeout.connect(this, &DwAnimationLibrary::onStartStopAnimationTimerEvent);

    m_animationTimer.start(0);
    m_startStopAnimationTimer.start(0);
}

//------------------------------------------------------------------------------
/**
*/
DwAnimationLibrary::~DwAnimationLibrary()
{
    stopAll();

    DW_ASSERT(m_animations.size() == 0);
    DW_ASSERT(m_animationsToStart.size() == 0);
}

//------------------------------------------------------------------------------
/**
*/
DW_GLOBAL_STATIC(DwAnimationLibrary, _dw_animationLibrary);
DwAnimationLibrary *DwAnimationLibrary::instance()
{
    return _dw_animationLibrary();
}

//------------------------------------------------------------------------------
/**
*/
void DwAnimationLibrary::registerAnimation(DwAbstractAnimation *animation, bool isTopLevel)
{
    DwAnimationLibrary *inst = DwAnimationLibrary::instance(); //we create the instance if needed
    inst->registerRunningAnimation(animation);
    if (isTopLevel)
    {
        DW_ASSERT(!animation->m_hasRegisteredTimer);
        animation->m_hasRegisteredTimer = true;
        inst->m_animationsToStart.append(animation);
        if (!inst->m_startStopAnimationTimer.isActive())
        {
            inst->m_startStopAnimationTimer.start(DEFAULT_TIMER_INTERVAL);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwAnimationLibrary::unregisterAnimation(DwAbstractAnimation *animation)
{
    DwAnimationLibrary *inst = DwAnimationLibrary::instance();
    if (inst)
    {
        //at this point the unified timer should have been created
        //but it might also have been already destroyed in case the application is shutting down

        inst->unregisterRunningAnimation(animation);

        if (!animation->m_hasRegisteredTimer)
            return;

        int idx = inst->m_animations.indexOf(animation);
        if (idx != -1)
        {
            inst->m_animations.remove(idx);
            // this is needed if we unregister an animation while its running
            if (idx <= inst->m_currentAnimationIdx)
                --inst->m_currentAnimationIdx;

            if (inst->m_animations.isEmpty() && !inst->m_startStopAnimationTimer.isActive())
                inst->m_startStopAnimationTimer.start(DEFAULT_TIMER_INTERVAL);
        }
        else
        {
            inst->m_animationsToStart.removeOne(animation);
        }
    }
    animation->m_hasRegisteredTimer = false;
}

//------------------------------------------------------------------------------
/**
	defines the timing interval. Default is DEFAULT_TIMER_INTERVAL
*/
void DwAnimationLibrary::setTimingInterval(int interval)
{
    m_timingInterval = interval;
    if (m_animationTimer.isActive() && !m_isPauseTimerActive)
    {
        //we changed the timing interval
        m_animationTimer.start(m_timingInterval);
    }
}

//------------------------------------------------------------------------------
/**
	this is used for updating the currentTime of all animations in case the pause
	timer is active or, otherwise, only of the animation passed as parameter.
*/
void DwAnimationLibrary::ensureTimerUpdate()
{
    DwAnimationLibrary *inst = DwAnimationLibrary::instance();
    if (inst && inst->m_isPauseTimerActive)
        inst->updateAnimationsTime();
}

//------------------------------------------------------------------------------
/**
	this will evaluate the need of restarting the pause timer in case there is still
	some pause animations running.
*/
void DwAnimationLibrary::updateAnimationTimer()
{
    DwAnimationLibrary *inst = DwAnimationLibrary::instance();
    if (inst)
        inst->restartAnimationTimer();
}

void DwAnimationLibrary::stopAll()
{
    DwAnimationLibrary *inst = DwAnimationLibrary::instance();
    if (!inst) return;

    inst->m_animationTimer.sigTimeout.disconnect_all();
    inst->m_startStopAnimationTimer.sigTimeout.disconnect_all();

    if (inst->m_animations.size() > 0)
    {
        DwVector<DwAbstractAnimation*>  anis = inst->m_animations;
        for (int i=0; i<anis.size(); ++i)
        {
            anis.at(i)->stop();
        }
    }

    if (inst->m_animationsToStart.size() > 0)
    {
        DwVector<DwAbstractAnimation*>  anis = inst->m_animationsToStart;
        for (int i=0; i<anis.size(); ++i)
        {
            anis.at(i)->stop();
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwAnimationLibrary::onAnimationTimerEvent()
{
    onTimerEvent(m_animationTimer.timerId());
}

//------------------------------------------------------------------------------
/**
*/
void DwAnimationLibrary::onStartStopAnimationTimerEvent()
{
    onTimerEvent(m_startStopAnimationTimer.timerId());
}

//------------------------------------------------------------------------------
/**
*/
void DwAnimationLibrary::onTimerEvent(int id)
{
    //in the case of consistent timing we make sure the orders in which events come is always the same
    //for that purpose we do as if the startstoptimer would always fire before the animation timer
    if ((m_consistentTiming && m_startStopAnimationTimer.isActive()) ||
            id == m_startStopAnimationTimer.timerId())
    {
        m_startStopAnimationTimer.stop();

        //we transfer the waiting animations into the "really running" state
        for (int i=0; i<m_animationsToStart.size(); ++i)
        {
            m_animations.append(m_animationsToStart.at(i));
        }
        m_animationsToStart.clear();
        if (m_animations.isEmpty())
        {
            m_animationTimer.stop();
            m_isPauseTimerActive = false;
            // invalidate the start reference time
            m_time.invalidate();
        }
        else
        {
            restartAnimationTimer();
            if (!m_time.isValid())
            {
                m_lastTick = 0;
                m_time.start();
            }
        }
    }

    if (id == m_animationTimer.timerId())
    {
        // update current time on all top level animations
        updateAnimationsTime();
        restartAnimationTimer();
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwAnimationLibrary::registerRunningAnimation(DwAbstractAnimation *animation)
{
    if (animation->m_isGroup)
        return;

    if (animation->m_isPause)
    {
        m_runningPauseAnimations.append(animation);
    }
    else
    {
        m_runningLeafAnimations++;
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwAnimationLibrary::unregisterRunningAnimation(DwAbstractAnimation *animation)
{
    if (animation->m_isGroup)
        return;

    if (animation->m_isPause)
        m_runningPauseAnimations.removeOne(animation);
    else
        m_runningLeafAnimations--;
    DW_ASSERT(m_runningLeafAnimations >= 0);
}

//------------------------------------------------------------------------------
/**
*/
void DwAnimationLibrary::restartAnimationTimer()
{
    if (m_runningLeafAnimations == 0 && !m_runningPauseAnimations.isEmpty())
    {
        int closestTimeToFinish = closestPauseAnimationTimeToFinish();
        if (closestTimeToFinish < 0)
        {
            //qDebug() << runningPauseAnimations;
            //qDebug() << closestPauseAnimationTimeToFinish();
        }
        m_animationTimer.start(closestTimeToFinish);
        m_isPauseTimerActive = true;
    }
    else if (!m_animationTimer.isActive() || m_isPauseTimerActive)
    {
        m_animationTimer.start(m_timingInterval);
        m_isPauseTimerActive = false;
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwAnimationLibrary::updateAnimationsTime()
{
    int64_t totalElapsed = m_time.elapsed();
    // ignore consistentTiming in case the pause timer is active
    int delta = (m_consistentTiming && !m_isPauseTimerActive) ? m_timingInterval : (int)(totalElapsed - m_lastTick);
    if (m_slowMode)
    {
        if (m_slowdownFactor > 0)
            delta = dwRound(delta / m_slowdownFactor);
        else
            delta = 0;
    }

    m_lastTick = totalElapsed;

    //we make sure we only call update time if the time has actually changed
    //it might happen in some cases that the time doesn't change because events are delayed
    //when the CPU load is high
    if (delta)
    {
        for (m_currentAnimationIdx = 0; m_currentAnimationIdx < m_animations.count(); ++m_currentAnimationIdx)
        {
            DwAbstractAnimation *animation = m_animations.at(m_currentAnimationIdx);
            int elapsed = animation->m_totalCurrentTime
                          + (animation->direction() == Dw::Forward ? delta : -delta);
            animation->setCurrentTime(elapsed);
        }
        m_currentAnimationIdx = 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
int DwAnimationLibrary::closestPauseAnimationTimeToFinish()
{
    int closestTimeToFinish = INT_MAX;
    for (int i = 0; i < m_runningPauseAnimations.size(); ++i)
    {
        DwAbstractAnimation *animation = m_runningPauseAnimations.at(i);
        int timeToFinish;

        if (animation->direction() == Dw::Forward)
            timeToFinish = animation->duration() - animation->currentLoopTime();
        else
            timeToFinish = animation->currentLoopTime();

        if (timeToFinish < closestTimeToFinish)
            closestTimeToFinish = timeToFinish;
    }
    return closestTimeToFinish;
}