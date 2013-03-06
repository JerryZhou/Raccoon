#pragma once
#include "dwgui/dwgraphicsevent.h"
#include "dwgui/dwrttiobjectptr.h"

#if DW_ENABLE_ANIMATION_EVENT
//------------------------------------------------------------------------------
class DwGraphicsItem;
class DwAbstractAnimation;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwAnimationEvent : public DwSceneEvent
{
    DW_DECLARE_EVENTID(DwAnimationEvent);
public:
    explicit DwAnimationEvent(DwAbstractAnimation * anim = NULL);
    virtual ~DwAnimationEvent();

    // event must tell the target item in scene
    virtual DwGraphicsItem* indexInterestItemFromScene(DwGraphicsScene *scene);

    // if the item is reached the require
    virtual bool isInterestedBy(DwGraphicsItem* item);

    // index the animation this event
    DwAbstractAnimation * animation() const;

protected:
    DwRttiObjectPtr<DwAbstractAnimation> m_animation;
};
DW_REGISTER_CLASS(DwAnimationEvent);

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwAnimationStartEvent : public DwAnimationEvent
{
    DW_DECLARE_EVENTID(DwAnimationStartEvent);
public:
    explicit DwAnimationStartEvent(DwAbstractAnimation * anim = NULL);
    virtual ~DwAnimationStartEvent();
protected:
private:
};
DW_REGISTER_CLASS(DwAnimationStartEvent);

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwAnimationEndEvent : public DwAnimationEvent
{
    DW_DECLARE_EVENTID(DwAnimationEndEvent);
public:
    explicit DwAnimationEndEvent(DwAbstractAnimation * anim = NULL);
    virtual ~DwAnimationEndEvent();
protected:
};
DW_REGISTER_CLASS(DwAnimationEndEvent);

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwAnimationPausedEvent : public DwAnimationEvent
{
    DW_DECLARE_EVENTID(DwAnimationPausedEvent);
public:
    explicit DwAnimationPausedEvent(DwAbstractAnimation * anim = NULL);
    virtual ~DwAnimationPausedEvent();
protected:
};
DW_REGISTER_CLASS(DwAnimationPausedEvent);

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwAnimationAttachEvent : public DwAnimationEvent
{
    DW_DECLARE_EVENTID(DwAnimationAttachEvent);
public:
    explicit DwAnimationAttachEvent(DwAbstractAnimation * anim = NULL);
    virtual ~DwAnimationAttachEvent();
protected:
private:
};
DW_REGISTER_CLASS(DwAnimationAttachEvent);

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwAnimationDettachEvent : public DwAnimationEvent
{
    DW_DECLARE_EVENTID(DwAnimationDettachEvent);
public:
    explicit DwAnimationDettachEvent(DwAbstractAnimation * anim = NULL);
    virtual ~DwAnimationDettachEvent();
protected:
private:
};
DW_REGISTER_CLASS(DwAnimationDettachEvent);

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwAnimationDirectionChangeEvent : public DwAnimationEvent
{
    DW_DECLARE_EVENTID(DwAnimationDirectionChangeEvent);
public:
    explicit DwAnimationDirectionChangeEvent(DwAbstractAnimation * anim = NULL);
    virtual ~DwAnimationDirectionChangeEvent();

    Dw::Direction m_oldDir;
    Dw::Direction m_newDir;
protected:
private:
};
DW_REGISTER_CLASS(DwAnimationDirectionChangeEvent);

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwAnimationStateChangeEvent : public DwAnimationEvent
{
    DW_DECLARE_EVENTID(DwAnimationStateChangeEvent);
public:
    explicit DwAnimationStateChangeEvent(DwAbstractAnimation * anim = NULL);
    virtual ~DwAnimationStateChangeEvent();

    Dw::State m_oldState;
    Dw::State m_newState;
protected:
private:
};
DW_REGISTER_CLASS(DwAnimationStateChangeEvent);

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwAnimationLoopChangeEvent : public DwAnimationEvent
{
    DW_DECLARE_EVENTID(DwAnimationLoopChangeEvent);
public:
    explicit DwAnimationLoopChangeEvent(DwAbstractAnimation * anim = NULL);
    virtual ~DwAnimationLoopChangeEvent();

    int m_oldLoop;
    int m_newLoop;
protected:
private:
};
DW_REGISTER_CLASS(DwAnimationLoopChangeEvent);

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwAnimationProgressEvent : public DwAnimationEvent
{
    DW_DECLARE_EVENTID(DwAnimationProgressEvent);
public:
    explicit DwAnimationProgressEvent(DwAbstractAnimation * anim = NULL);
    virtual ~DwAnimationProgressEvent();

    int m_msecs;
protected:
private:
};
DW_REGISTER_CLASS(DwAnimationProgressEvent);

#endif // end of DW_ENABLE_ANIMATION_EVENT