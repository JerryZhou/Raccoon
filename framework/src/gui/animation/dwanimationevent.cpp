#include "stable.h"
#include "dwanimationevent.h"
#include "dwabstractanimation.h"
#include "dwgui/dwgraphicsitem.h"

#if DW_ENABLE_ANIMATION_EVENT

DW_IMPLEMENT_EVENTID(DwAnimationEvent, DwSceneEvent);

//------------------------------------------------------------------------------
/**
*/
DwAnimationEvent::DwAnimationEvent(DwAbstractAnimation *animation)
    : m_animation(animation)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwAnimationEvent::~DwAnimationEvent()
{
    m_animation = 0;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem* DwAnimationEvent::indexInterestItemFromScene(DwGraphicsScene *scene)
{
    DW_UNUSED(scene);

    if (m_interestItem)
    {
        return m_interestItem;
    }

    if (m_animation)
    {
        DwRttiObject *obj = m_animation->attachedObj();
        m_interestItem = dwsafe_cast<DwGraphicsItem>(obj);
    }

    return m_interestItem;
}

//------------------------------------------------------------------------------
/**
*/
bool DwAnimationEvent::isInterestedBy(DwGraphicsItem* item)
{
    if (item == m_interestItem)
    {
        return true;
    }
    if (m_animation)
    {
        DwRttiObject *obj = m_animation->attachedObj();
        m_interestItem = dwsafe_cast<DwGraphicsItem>(obj);
    }
    return item == m_interestItem;
}

//------------------------------------------------------------------------------
/**
*/
DwAbstractAnimation * DwAnimationEvent::animation() const
{
    return m_animation;
}

//------------------------------------------------------------------------------
DW_IMPLEMENT_EVENTID(DwAnimationStartEvent, DwAnimationEvent);

//------------------------------------------------------------------------------
/**
*/
DwAnimationStartEvent::DwAnimationStartEvent(DwAbstractAnimation * anim/* = NULL*/)
    : DwAnimationEvent(anim)
{

}

//------------------------------------------------------------------------------
/**
*/
DwAnimationStartEvent::~DwAnimationStartEvent()
{

}

//------------------------------------------------------------------------------
DW_IMPLEMENT_EVENTID(DwAnimationEndEvent, DwAnimationEvent);

//------------------------------------------------------------------------------
/**
*/
DwAnimationEndEvent::DwAnimationEndEvent(DwAbstractAnimation * anim/* = NULL*/)
    : DwAnimationEvent(anim)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwAnimationEndEvent::~DwAnimationEndEvent()
{
    ;
}

//------------------------------------------------------------------------------
DW_IMPLEMENT_EVENTID(DwAnimationPausedEvent, DwAnimationEvent);

//------------------------------------------------------------------------------
/**
*/
DwAnimationPausedEvent::DwAnimationPausedEvent(DwAbstractAnimation * anim/* = NULL*/)
    : DwAnimationEvent(anim)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwAnimationPausedEvent::~DwAnimationPausedEvent()
{
    ;
}

//------------------------------------------------------------------------------
DW_IMPLEMENT_EVENTID(DwAnimationAttachEvent, DwAnimationEvent);

//------------------------------------------------------------------------------
/**
*/
DwAnimationAttachEvent::DwAnimationAttachEvent(DwAbstractAnimation * anim/* = NULL*/)
    : DwAnimationEvent(anim)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwAnimationAttachEvent::~DwAnimationAttachEvent()
{
    ;
}

//------------------------------------------------------------------------------
DW_IMPLEMENT_EVENTID(DwAnimationDettachEvent, DwAnimationEvent);

//------------------------------------------------------------------------------
/**
*/
DwAnimationDettachEvent::DwAnimationDettachEvent(DwAbstractAnimation * anim/* = NULL*/)
    : DwAnimationEvent(anim)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwAnimationDettachEvent::~DwAnimationDettachEvent()
{
    ;
}

//------------------------------------------------------------------------------
DW_IMPLEMENT_EVENTID(DwAnimationDirectionChangeEvent, DwAnimationEvent);

//------------------------------------------------------------------------------
/**
*/
DwAnimationDirectionChangeEvent::DwAnimationDirectionChangeEvent(DwAbstractAnimation * anim)
    : DwAnimationEvent(anim)
    , m_oldDir(Dw::Forward)
    , m_newDir(Dw::Forward)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwAnimationDirectionChangeEvent::~DwAnimationDirectionChangeEvent()
{
    ;
}

//------------------------------------------------------------------------------
DW_IMPLEMENT_EVENTID(DwAnimationStateChangeEvent, DwAnimationEvent);

//------------------------------------------------------------------------------
/**
*/
DwAnimationStateChangeEvent::DwAnimationStateChangeEvent(DwAbstractAnimation * anim)
    : DwAnimationEvent(anim)
    , m_oldState(Dw::Stopped)
    , m_newState(Dw::Stopped)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwAnimationStateChangeEvent::~DwAnimationStateChangeEvent()
{
    ;
}

//------------------------------------------------------------------------------
DW_IMPLEMENT_EVENTID(DwAnimationLoopChangeEvent, DwAnimationEvent);

//------------------------------------------------------------------------------
/**
*/
DwAnimationLoopChangeEvent::DwAnimationLoopChangeEvent(DwAbstractAnimation * anim)
    : DwAnimationEvent(anim)
    , m_oldLoop(0)
    , m_newLoop(0)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwAnimationLoopChangeEvent::~DwAnimationLoopChangeEvent()
{
    ;
}

//------------------------------------------------------------------------------
DW_IMPLEMENT_EVENTID(DwAnimationProgressEvent, DwAnimationEvent);

//------------------------------------------------------------------------------
/**
*/
DwAnimationProgressEvent::DwAnimationProgressEvent( DwAbstractAnimation * anim /*= NULL*/ )
: DwAnimationEvent(anim), m_msecs(0)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwAnimationProgressEvent::~DwAnimationProgressEvent()
{
    ;
}
#endif //end of DW_ENABLE_ANIMATION_EVENT