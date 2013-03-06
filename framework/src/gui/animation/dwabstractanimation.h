#pragma once
#include "dwnamespace.h"
#include "dwcore/dwobject.h"
#include "dwgui/dwrttiobject.h"
#include "dwgui/dwrtti.h"
#include "dwgui/dwgraphicseventdispatch.h"

//------------------------------------------------------------------------------
class DwAnimationGroup;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwAbstractAnimation : public DwObject, public DwRttiObject
{
    DW_DECLARE_CLASS(DwAbstractAnimation);
public:
    DwAbstractAnimation(DwObject *parent = NULL);
    virtual ~DwAbstractAnimation();

    Dw::State state() const;

    DwAnimationGroup *group() const;

    Dw::Direction direction() const;
    void setDirection(Dw::Direction );

    int currentTime() const;
    int currentLoopTime() const;

    int loopCount() const;
    void setLoopCount(int );
    int currentLoop() const;

    int duration() const ;
    void setDuration(int msecs);
    int totalDuration() const;

    void start(Dw::DeletionPolicy policy = Dw::KeepWhenStopped);
    void pause();
    void resume();
    void setPaused(bool);
    void stop();
    void setCurrentTime(int msecs);

    bool isDeleteWhenStopped() const;

    // sub class must implement this interface
    virtual void updateCurrentTime(int ) = 0;
    virtual void updateState(Dw::State newState, Dw::State oldState);
    virtual void updateDirection(Dw::Direction );

    virtual void debugString(DwDebug &stream)const;

    void attachTo(DwRttiObject *obj);
    void dettach();
    DwRttiObject *attachedObj() const;

    bool isSendEvent() const;
    void setSendEvent(bool sended);

    uint32_t id() const;

    // lib of animations
    static DwAbstractAnimation* findAnimation(uint32_t uid);

protected:
    friend class DwAnimationLibrary;
    friend class DwGraphicsItem;

    virtual void onAttachTo(DwRttiObject *obj);
    virtual void onDettachTo(DwRttiObject *obj);

    void setState(Dw::State );

    Dw::State m_state;
    Dw::Direction m_direction;

    int m_duration;
    int m_totalCurrentTime;
    int m_currentTime;
    int m_loopCount;
    int m_currentLoop;

    bool m_deleteWhenStopped;
    bool m_hasRegisteredTimer;
    bool m_isPause;
    bool m_isGroup;
    bool m_sendEvent;

    DwAnimationGroup *m_group;
    DwRttiObject *m_obj;

    uint32_t m_id;
};// end of DwAbstractAnimation