#pragma once
#include "dwgui/dweventhandler.h"
#include "dwcore/dwmap.h"

//------------------------------------------------------------------------------
class DwGraphicsItem;
class DwGraphicsScene;
class DwGraphicsView;
class DwEventSignalDispatch;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwItemEventHandler : public DwEventHandler
{
    DW_DECLARE_CLASS(DwItemEventHandler);
public:
    typedef void (*pfnOnItemEvent)(DwGraphicsItem* item, DwEvent* evt);

    explicit DwItemEventHandler(DwGraphicsItem* item = NULL);
    virtual ~DwItemEventHandler();

    DwGraphicsItem* item() const;
    void setItem(DwGraphicsItem* item);

    bool isEnabled() const;
    void setEnabled(bool enabled);

    virtual void onEvent(DwEvent* evt);

    void addEventCallBack(DwEventId *id, pfnOnItemEvent callBack);
    void removeEventCallBack(DwEventId *id, pfnOnItemEvent callBack);
    void removeEventCallBack(DwEventId *id);

    bool isAcceptedChildEvents() const;
    void setAcceptedChildEvents(bool accept);

    bool isAcceptedParentEvents() const;
    void setAcceptedParentEvents(bool accept);

    void onEventEn(DwEvent* evt);

    DwEventSignalDispatch * dispatcher() ;

    virtual void debugString(DwDebug &stream)const;

protected:
    bool onCallBackEvent(DwEvent* evt);

    typedef DwMap<DwEventId*, DwVector<pfnOnItemEvent> > EventCallBackMap;

    DwGraphicsItem* m_item;
    EventCallBackMap m_callBackMap;
    DwEventSignalDispatch* m_dispatcher;

    uint16_t m_acceptChildEvents : 1;
    uint16_t m_acceptParentEvents : 1;
    uint16_t m_enabled : 1;
};// end of DwItemEventHandler
DW_REGISTER_CLASS(DwItemEventHandler);

//------------------------------------------------------------------------------
/**
*/
inline bool DwItemEventHandler::isEnabled() const
{
    return m_enabled;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwItemEventHandler::setEnabled(bool enabled)
{
    if(m_enabled == enabled) return;
    m_enabled = enabled;
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwItemEventHandler::isAcceptedChildEvents() const
{
    return m_acceptChildEvents;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwItemEventHandler::setAcceptedChildEvents(bool accept)
{
    m_acceptChildEvents = accept;
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwItemEventHandler::isAcceptedParentEvents() const
{
    return m_acceptParentEvents;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwItemEventHandler::setAcceptedParentEvents(bool accept)
{
    m_acceptParentEvents = accept;
}

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwSceneEventHandler : public DwEventHandler
{
    DW_DECLARE_CLASS(DwSceneEventHandler);
public:
    typedef void (*pfnOnSceneEvent)(DwGraphicsScene* scene, DwEvent* evt);

    explicit DwSceneEventHandler(DwGraphicsScene* scene = NULL);
    virtual ~DwSceneEventHandler();

    virtual void onEvent(DwEvent* evt);

    void addEventCallBack(DwEventId *id, pfnOnSceneEvent callBack);

    void onEventEn(DwEvent* evt);

    virtual void debugString(DwDebug &stream)const;

protected:
    bool onCallBackEvent(DwEvent* evt);

    typedef DwMap<DwEventId*, DwVector<pfnOnSceneEvent> > EventCallBackMap;

    DwGraphicsScene* m_scene;
    EventCallBackMap m_callBackMap;
private:
};// end of DwSceneEventHandler
DW_REGISTER_CLASS(DwSceneEventHandler);

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwViewEventHandler : public DwEventHandler
{
    DW_DECLARE_CLASS(DwViewEventHandler);
public:
    typedef void (*pfnOnViewEvent)(DwGraphicsView* view, DwEvent* evt);

    explicit DwViewEventHandler(DwGraphicsView* view = NULL);
    virtual ~DwViewEventHandler();

    virtual void onEvent(DwEvent* evt);

    void addEventCallBack(DwEventId *id, pfnOnViewEvent callBack);

    void onEventEn(DwEvent* evt);

    virtual void debugString(DwDebug &stream)const;

protected:
    bool onCallBackEvent(DwEvent* evt);

    typedef DwMap<DwEventId*, DwVector<pfnOnViewEvent> > EventCallBackMap;

    DwGraphicsView* m_view;
    EventCallBackMap m_callBackMap;
private:
};// end of DwViewEventHandler
DW_REGISTER_CLASS(DwViewEventHandler);