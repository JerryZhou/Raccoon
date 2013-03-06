#include "stable.h"
#include "dwabstractanimation.h"
#include "dwanimationgroup.h"
#include "dwanimationlibrary.h"
#include "dwanimationevent.h"
#include "dwcore/dwobjectptr.h"
#include "dwgui/dwgraphicsitem.h"

//------------------------------------------------------------------------------
DW_IMPLEMENT_ABSTRACT_CLASS(DwAbstractAnimation, 'AANN', DwRttiObject);


//------------------------------------------------------------------------------
/**
*/
static uint32_t _dw_animation_id()
{
    static uint32_t _dwId = 1;
    return _dwId++;
}

//------------------------------------------------------------------------------
/**
*/
struct AnimationLib
{
    DwMap<uint32_t, DwAbstractAnimation*> allAnimations;

    ~AnimationLib()
    {
        DW_ASSERT(allAnimations.count() == 0 && "error animation leak");
    }

    void insertAnimation(DwAbstractAnimation *ani)
    {
        DW_ASSERT(ani);
        allAnimations[ani->id()] = ani;
    }

    void removeAnimation(DwAbstractAnimation *ani)
    {
        allAnimations.remove(ani->id());
    }

    DwAbstractAnimation *findAnimation(uint32_t uid) const
    {
        DwMap<uint32_t, DwAbstractAnimation*>::const_iterator ite = allAnimations.find(uid);
        if (ite != allAnimations.end())
        {
            return ite->second;
        }
        return NULL;
    }
};
DW_GLOBAL_STATIC(AnimationLib, _dw_AnimationLib);

//------------------------------------------------------------------------------
/**
*/
DwAbstractAnimation::DwAbstractAnimation(DwObject *parent)
    : DwObject(parent)
    , m_state(Dw::Stopped)
    , m_direction(Dw::Forward)
    , m_totalCurrentTime(0)
    , m_currentTime(0)
    , m_loopCount(1)
    , m_currentLoop(0)
    , m_deleteWhenStopped(false)
    , m_hasRegisteredTimer(false)
    , m_isPause(false)
    , m_isGroup(false)
    , m_group(0)
    , m_obj(0)
    , m_sendEvent(0)
    , m_id(_dw_animation_id())
{
    _dw_AnimationLib()->insertAnimation(this);
}

//------------------------------------------------------------------------------
/**
*/
DwAbstractAnimation::~DwAbstractAnimation()
{
    if(m_obj) dettach();

    if (state() != Dw::Stopped)
    {
        Dw::State oldState = state();
        m_state = Dw::Stopped;

        if (m_sendEvent)
        {
#if DW_ENABLE_ANIMATION_EVENT
            DwAnimationStateChangeEvent evt(this);
            evt.m_newState = m_state;
            evt.m_oldState = oldState;
            dwGraphicsEventDispatch()->dispatch(m_obj, &evt);
#endif
        }

        if (oldState == Dw::Running)
        {
            DwAnimationLibrary::unregisterAnimation(this);
        }
    }

    _dw_AnimationLib()->removeAnimation(this);
}

//------------------------------------------------------------------------------
/**
*/
Dw::State DwAbstractAnimation::state() const
{
    return m_state;
}

//------------------------------------------------------------------------------
/**
*/
DwAnimationGroup *DwAbstractAnimation::group() const
{
    return m_group;
}

//------------------------------------------------------------------------------
/**
*/
Dw::Direction DwAbstractAnimation::direction() const
{
    return m_direction;
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
void DwAbstractAnimation::setDirection(Dw::Direction dir)
{
    if (m_direction == dir)
        return;

    if (state() == Dw::Stopped)
    {
        if (dir == Dw::Backward)
        {
            m_currentTime = duration();
            m_currentLoop = m_loopCount - 1;
        }
        else
        {
            m_currentTime = 0;
            m_currentLoop = 0;
        }
    }

    // the commands order below is important: first we need to setCurrentTime with the old direction,
    // then update the direction on this and all children and finally restart the pauseTimer if needed
    if (m_hasRegisteredTimer)
        DwAnimationLibrary::ensureTimerUpdate();

    Dw::Direction old = m_direction;
    m_direction = dir;
    updateDirection(dir);

    if (m_hasRegisteredTimer)
        // needed to update the timer interval in case of a pause animation
        DwAnimationLibrary::updateAnimationTimer();

    if (m_sendEvent)
    {
        DW_UNUSED(old);
#if DW_ENABLE_ANIMATION_EVENT
        DwAnimationDirectionChangeEvent evt(this);
        evt.m_newDir = m_direction;
        evt.m_oldDir = old;
        dwGraphicsEventDispatch()->dispatch(m_obj, &evt);
#endif
    }
}

//------------------------------------------------------------------------------
/**
*/
int DwAbstractAnimation::currentTime() const
{
    return m_currentTime;
}

//------------------------------------------------------------------------------
/**
*/
int DwAbstractAnimation::currentLoopTime() const
{
    return m_totalCurrentTime;
}

//------------------------------------------------------------------------------
/**
*/
int DwAbstractAnimation::loopCount() const
{
    return m_loopCount;
}

//------------------------------------------------------------------------------
/**
*/
void DwAbstractAnimation::setLoopCount(int cnt)
{
    m_loopCount = cnt;
}

//------------------------------------------------------------------------------
/**
*/
int DwAbstractAnimation::currentLoop() const
{
    return m_currentLoop;
}

//------------------------------------------------------------------------------
/**
*/
int DwAbstractAnimation::duration() const
{
    return m_duration;
}

//------------------------------------------------------------------------------
/**
*/
void DwAbstractAnimation::setDuration(int msecs)
{
    m_duration = msecs;
}

//------------------------------------------------------------------------------
/**
*/
int DwAbstractAnimation::totalDuration() const
{
    int dura = duration();
    if (dura <= 0)
        return dura;
    int loopcount = loopCount();
    if (loopcount < 0)
        return -1;
    return dura * loopcount;
}

//------------------------------------------------------------------------------
/**
*/
void DwAbstractAnimation::start(Dw::DeletionPolicy policy/* = Dw::KeepWhenStopped*/)
{
    if (state() == Dw::Running)
        return;
    m_deleteWhenStopped = (policy == Dw::DeleteWhenStopped);
    setState(Dw::Running);
}

//------------------------------------------------------------------------------
/**
*/
void DwAbstractAnimation::pause()
{
    if (state() == Dw::Stopped)
    {
        DW_WARNING("DwAbstractAnimation::pause: Cannot pause a stopped animation");
        return;
    }
    setState(Dw::Paused);
}

//------------------------------------------------------------------------------
/**
*/
void DwAbstractAnimation::resume()
{
    if (state() != Dw::Paused)
    {
        DW_WARNING("DwAbstractAnimation::resume: Cannot resume an animation that is not paused");
        return;
    }

    setState(Dw::Running);
}

//------------------------------------------------------------------------------
/**
*/
void DwAbstractAnimation::setPaused(bool b)
{
    if (b)
    {
        pause();
    }
    else
    {
        resume();
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwAbstractAnimation::stop()
{
    setState(Dw::Stopped);
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
void DwAbstractAnimation::setCurrentTime(int msecs)
{
    msecs = dwMax(msecs, 0);

    // Calculate new time and loop.
    int dura = duration();
    int totalDura = dura <= 0 ? dura : ((m_loopCount < 0) ? -1 : dura * m_loopCount);
    if (totalDura != -1)
        msecs = dwMin(totalDura, msecs);
    m_totalCurrentTime = msecs;

    // Update new values.
    int oldLoop = m_currentLoop;
    m_currentLoop = ((dura <= 0) ? 0 : (msecs / dura));
    if (m_currentLoop == m_loopCount)
    {
        //we're at the end
        m_currentTime = dwMax(0, dura);
        m_currentLoop = dwMax(0, m_loopCount - 1);
    }
    else
    {
        if (m_direction == Dw::Forward)
        {
            m_currentTime = (dura <= 0) ? msecs : (msecs % dura);
        }
        else
        {
            m_currentTime = (dura <= 0) ? msecs : ((msecs - 1) % dura) + 1;
            if (m_currentTime == dura)
                --m_currentLoop;
        }
    }

    updateCurrentTime(m_currentTime);
    if (m_sendEvent)
    {
#if DW_ENABLE_ANIMATION_EVENT
        if (m_currentLoop != oldLoop)
        {
            DwAnimationLoopChangeEvent evt(this);
            evt.m_oldLoop = oldLoop;
            evt.m_newLoop = m_currentLoop;
            dwGraphicsEventDispatch()->dispatch(m_obj, &evt);
        }
        DwAnimationProgressEvent evt(this);
        evt.m_msecs = m_currentTime;
        dwGraphicsEventDispatch()->dispatch(m_obj, &evt);
#endif
    }

    // All animations are responsible for stopping the animation when their
    // own end state is reached; in this case the animation is time driven,
    // and has reached the end.
    if ((m_direction == Dw::Forward && m_totalCurrentTime == totalDura)
            || (m_direction == Dw::Backward && m_totalCurrentTime == 0))
    {
        stop();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool DwAbstractAnimation::isDeleteWhenStopped() const
{
    return m_deleteWhenStopped;
}

//------------------------------------------------------------------------------
/**
*/
void DwAbstractAnimation::updateState(Dw::State newState, Dw::State oldState)
{
    DW_UNUSED(newState);
    DW_UNUSED(oldState);
}

//------------------------------------------------------------------------------
/**
*/
void DwAbstractAnimation::updateDirection(Dw::Direction dir)
{
    DW_UNUSED(dir);
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
void DwAbstractAnimation::setState(Dw::State newState)
{
    if (m_state == newState)
        return;

    if (m_loopCount == 0)
        return;

    Dw::State oldState = m_state;
    int oldCurrentTime = m_currentTime;
    int oldCurrentLoop = m_currentLoop;
    Dw::Direction oldDirection = m_direction;

    // check if we should Rewind
    if ((newState == Dw::Paused || newState == Dw::Running)
            && oldState == Dw::Stopped)
    {
        //here we reset the time if needed
        //we don't call setCurrentTime because this might change the way the animation
        //behaves: changing the state or changing the current value
        m_totalCurrentTime = m_currentTime = (m_direction == Dw::Forward) ?
                                             0 : (m_loopCount == -1 ? duration() : totalDuration());
    }

    m_state = newState;
    DwObjectPtr<DwAbstractAnimation> guard(this);

    //(un)registration of the animation must always happen before calls to
    //virtual function (updateState) to ensure a correct state of the timer
    bool isTopLevel = !m_group || m_group->state() == Dw::Stopped;
    if (oldState == Dw::Running)
    {
        if (newState == Dw::Paused && m_hasRegisteredTimer)
            DwAnimationLibrary::ensureTimerUpdate();
        //the animation, is not running any more
        DwAnimationLibrary::unregisterAnimation(this);
    }
    else if (newState == Dw::Running)
    {
        DwAnimationLibrary::registerAnimation(this, isTopLevel);
    }

    updateState(newState, oldState);
    if (!guard || newState != m_state) //this is to be safe if updateState changes the state
        return;

    // Notify state change
    if (m_sendEvent)
    {
#if DW_ENABLE_ANIMATION_EVENT
        DwAnimationStateChangeEvent evt(this);
        evt.m_newState = newState;
        evt.m_oldState = oldState;
        dwGraphicsEventDispatch()->dispatch(m_obj, &evt);
#endif
    }

    if (!guard || newState != m_state) //this is to be safe if updateState changes the state
        return;

    switch (m_state)
    {
    case Dw::Paused:
    {
        if (m_sendEvent)
        {
#if DW_ENABLE_ANIMATION_EVENT
            DwAnimationPausedEvent evt(this);
            dwGraphicsEventDispatch()->dispatch(m_obj, &evt);
#endif
        }
    }
    break;
    case Dw::Running:
    {
        // this ensures that the value is updated now that the animation is running
        if (oldState == Dw::Stopped)
        {
            if (isTopLevel)
            {
                // currentTime needs to be updated if pauseTimer is active
                DwAnimationLibrary::ensureTimerUpdate();
                setCurrentTime(m_totalCurrentTime);
            }
        }
        if (m_sendEvent)
        {
#if DW_ENABLE_ANIMATION_EVENT
            DwAnimationStartEvent evt(this);
            dwGraphicsEventDispatch()->dispatch(m_obj, &evt);
#endif
        }
    }
    break;
    case Dw::Stopped:
        // Leave running state.
        int dura = duration();

        if (m_deleteWhenStopped)
        {
            deleteLater();
        }

        if (dura == -1 || m_loopCount < 0
                || (oldDirection == Dw::Forward && (oldCurrentTime * (oldCurrentLoop + 1)) == (dura * m_loopCount))
                || (oldDirection == Dw::Backward && oldCurrentTime == 0))
        {
            if (m_sendEvent)
            {
#if DW_ENABLE_ANIMATION_EVENT
                DwAnimationEndEvent evt(this);
                dwGraphicsEventDispatch()->dispatch(m_obj, &evt);
#endif
            }
        }
        break;
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwAbstractAnimation::attachTo(DwRttiObject *obj)
{
    if(m_obj == obj) return;

    if (m_obj)
    {
        dettach();
    }
    m_obj = obj;
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(m_obj);
    if (item)
    {
        item->attachAnimation(this);
    }

    if (m_sendEvent)
    {
#if DW_ENABLE_ANIMATION_EVENT
        DwAnimationAttachEvent evt(this);
        dwGraphicsEventDispatch()->dispatch(m_obj, &evt);
#endif
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwAbstractAnimation::dettach()
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(m_obj);
    if (item)
    {
        item->dettachAnimation(this);
    }
    if (m_sendEvent)
    {
#if DW_ENABLE_ANIMATION_EVENT
        DwAnimationDettachEvent evt(this);
        dwGraphicsEventDispatch()->dispatch(m_obj, &evt);
#endif
    }
    m_obj = NULL;
}

//------------------------------------------------------------------------------
/**
*/
DwRttiObject *DwAbstractAnimation::attachedObj() const
{
    return m_obj;
}

//------------------------------------------------------------------------------
/**
*/
void DwAbstractAnimation::onAttachTo(DwRttiObject *obj)
{
    DW_ASSERT(m_obj == obj);
    m_obj = obj;
}

//------------------------------------------------------------------------------
/**
*/
void DwAbstractAnimation::onDettachTo(DwRttiObject *obj)
{
    DW_ASSERT(m_obj == obj);
    DW_UNUSED(obj);
}

//------------------------------------------------------------------------------
/**
*/
bool DwAbstractAnimation::isSendEvent() const
{
    return m_sendEvent;
}

//------------------------------------------------------------------------------
/**
*/
void DwAbstractAnimation::setSendEvent(bool sended)
{
    m_sendEvent = sended;
}

//------------------------------------------------------------------------------
/**
*/
uint32_t DwAbstractAnimation::id() const
{
    return m_id;
}

//------------------------------------------------------------------------------
/**
*/
DwAbstractAnimation* DwAbstractAnimation::findAnimation(uint32_t uid)
{
    return _dw_AnimationLib()->findAnimation(uid);
}

//------------------------------------------------------------------------------
/**
*/
static DwString state2String(Dw::State s)
{
    switch(s)
    {
    case Dw::Stopped :
        return L"Stopped";
    case Dw::Paused :
        return L"Paused";
    case Dw::Running :
        return L"Running";
    }

    return L"ERROR";
}

//------------------------------------------------------------------------------
/**
*/
static DwString direction2String(Dw::Direction s)
{
    switch(s)
    {
    case Dw::Forward :
        return L"Forward";
    case Dw::Backward :
        return L"Backward";
    }
    return L"ERROR";
}

//------------------------------------------------------------------------------
/**
*/
void DwAbstractAnimation::debugString(DwDebug &stream)const
{
    stream  <<" " << this->rtti()->name() << "["
            <<" state :" << state2String(m_state)
            << " direction :" << direction2String(m_direction)
            << " duration:" << m_duration
            << " totalCurrentTime: " << m_totalCurrentTime
            << " currentTime: " << m_currentTime
            << " loopCount: " << m_loopCount
            << " currentLoop: " << m_currentLoop
            << " isGroup: " << m_isGroup
            << " isPause: " << m_isPause
            << "]";
}
