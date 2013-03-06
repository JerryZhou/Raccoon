#include "stable.h"
#include "dwdummys.h"
#include "dwgraphicseventhandler.h"
#include "dwgraphicsevent.h"
#include "dwgraphicsitem.h"
#include "dwgraphicspixmapitem.h"
#include "dwgraphicsrectitem.h"
#include "dwgui/dweventsignaldispatch.h"

//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_CLASS(DwItemEventHandler, 'IEHR', DwEventHandler);
//------------------------------------------------------------------------------
/**
*/
DwItemEventHandler::DwItemEventHandler(DwGraphicsItem* item)
    : DwEventHandler()
    , m_acceptChildEvents(0)
    , m_acceptParentEvents(0)
    , m_enabled(1)
    , m_dispatcher(NULL)
{
    m_item = item;
}

//------------------------------------------------------------------------------
/**
*/
DwItemEventHandler::~DwItemEventHandler()
{
    m_item = NULL;
    m_callBackMap.clear();
    DW_SAFE_DELETE(m_dispatcher);
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem* DwItemEventHandler::item() const
{
    return m_item;
}

//------------------------------------------------------------------------------
/**
*/
void DwItemEventHandler::setItem(DwGraphicsItem* item)
{
    m_item = item;
}

//------------------------------------------------------------------------------
/**
	TODO: test event handler
*/
void DwItemEventHandler::onEvent(DwEvent* evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwItemEventHandler::addEventCallBack(DwEventId *id, pfnOnItemEvent callBack)
{
    m_callBackMap[id].append(callBack);
}

//------------------------------------------------------------------------------
/**
*/
void DwItemEventHandler::removeEventCallBack(DwEventId *id, pfnOnItemEvent callBack)
{
    EventCallBackMap::iterator ite = m_callBackMap.find(id);
    if (ite != m_callBackMap.end())
    {
        ite->second.removeAll(callBack);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwItemEventHandler::removeEventCallBack(DwEventId *id)
{
    m_callBackMap.remove(id);
}

//------------------------------------------------------------------------------
/**
*/
void DwItemEventHandler::onEventEn(DwEvent* evt)
{
    if (m_enabled)
    {
        if(m_dispatcher) m_dispatcher->dispatchEvent(m_item, evt);

        if (!evt->isAccepted())
        {
            // execute call back
            if(onCallBackEvent(evt)) return;

            onEvent(evt);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
DwEventSignalDispatch * DwItemEventHandler::dispatcher()
{
    if (!m_dispatcher)
    {
        m_dispatcher = new DwEventSignalDispatch(m_item);
    }
    return m_dispatcher;
}

//------------------------------------------------------------------------------
/**
*/
bool DwItemEventHandler::onCallBackEvent(DwEvent* evt)
{
    DW_ASSERT(!evt->isAccepted());

    if (m_callBackMap.count() > 0)
    {
        // execute call back find
        EventCallBackMap::iterator ite = m_callBackMap.find(evt->eventId());
        if (ite != m_callBackMap.end())
        {
            const DwVector<pfnOnItemEvent>& callBacks = (*ite).second;
            for (int i=0; i<callBacks.size() && !evt->isAccepted(); ++i)
            {
                (callBacks.at(i))(m_item, evt);
            }
            return evt->isAccepted();
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void DwItemEventHandler::debugString(DwDebug &stream)const
{
    stream  <<" " << this->rtti()->name() << "["
            <<" item name :" << m_item->name()
            << " call back count :" << m_callBackMap.count();
#ifdef _DEBUG
    EventCallBackMap::const_iterator ite = m_callBackMap.begin();
    while(ite != m_callBackMap.end())
    {
        DwEventId* evtId = ite->first;
        stream << " Event: "<< evtId->name();
        ++ite;
    }
#endif
    stream  << "]";
}

//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_CLASS(DwSceneEventHandler, 'SEHR', DwEventHandler);

//------------------------------------------------------------------------------
/**
*/
DwSceneEventHandler::DwSceneEventHandler(DwGraphicsScene* scene)
    : m_scene(scene)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwSceneEventHandler::~DwSceneEventHandler()
{
    m_scene = NULL;
}

//------------------------------------------------------------------------------
/**
*/
void DwSceneEventHandler::onEvent(DwEvent* evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwSceneEventHandler::addEventCallBack(DwEventId *id, pfnOnSceneEvent callBack)
{
    m_callBackMap[id].append(callBack);
}

//------------------------------------------------------------------------------
/**
*/
void DwSceneEventHandler::onEventEn(DwEvent* evt)
{
    // execute call back
    if(onCallBackEvent(evt)) return;

    onEvent(evt);
}

//------------------------------------------------------------------------------
/**
*/
bool DwSceneEventHandler::onCallBackEvent(DwEvent* evt)
{
    DW_ASSERT(!evt->isAccepted());

    if (m_callBackMap.count() > 0)
    {
        // execute call back find
        EventCallBackMap::iterator ite = m_callBackMap.find(evt->eventId());
        if (ite != m_callBackMap.end())
        {
            const DwVector<pfnOnSceneEvent>& callBacks = (*ite).second;
            for (int i=0; i<callBacks.size() && !evt->isAccepted(); ++i)
            {
                (callBacks.at(i))(m_scene, evt);
            }
            return evt->isAccepted();
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void DwSceneEventHandler::debugString(DwDebug &stream)const
{
    stream  <<" " << this->rtti()->name() << "["
            <<" scene :" << (int)(m_scene)
            << " call back count :" << m_callBackMap.count();
#ifdef _DEBUG
    EventCallBackMap::const_iterator ite = m_callBackMap.begin();
    while(ite != m_callBackMap.end())
    {
        DwEventId* evtId = ite->first;
        stream << " Event: "<< evtId->name();
        ++ite;
    }
#endif
    stream  << "]";
}

//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_CLASS(DwViewEventHandler, 'VEHR', DwEventHandler);
//------------------------------------------------------------------------------
/**
*/
DwViewEventHandler::DwViewEventHandler(DwGraphicsView* view)
    : m_view(view)
{

}

//------------------------------------------------------------------------------
/**
*/
DwViewEventHandler::~DwViewEventHandler()
{
    m_view = NULL;
}

//------------------------------------------------------------------------------
/**
*/
void DwViewEventHandler::onEvent(DwEvent* evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwViewEventHandler::addEventCallBack(DwEventId *id, pfnOnViewEvent callBack)
{
    m_callBackMap[id].append(callBack);
}

//------------------------------------------------------------------------------
/**
*/
void DwViewEventHandler::onEventEn(DwEvent* evt)
{
    // execute call back
    if(onCallBackEvent(evt)) return;

    onEvent(evt);
}

//------------------------------------------------------------------------------
/**
*/
bool DwViewEventHandler::onCallBackEvent(DwEvent* evt)
{
    DW_ASSERT(!evt->isAccepted());

    if (m_callBackMap.count() > 0)
    {
        // execute call back find
        EventCallBackMap::iterator ite = m_callBackMap.find(evt->eventId());
        if (ite != m_callBackMap.end())
        {
            const DwVector<pfnOnViewEvent>& callBacks = (*ite).second;
            for (int i=0; i<callBacks.size() && !evt->isAccepted(); ++i)
            {
                (callBacks.at(i))(m_view, evt);
            }
            return evt->isAccepted();
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void DwViewEventHandler::debugString(DwDebug &stream)const
{
    stream  <<" " << this->rtti()->name() << "["
            <<" view :" << (int)(m_view)
            << " call back count :" << m_callBackMap.count();
#ifdef _DEBUG
    EventCallBackMap::const_iterator ite = m_callBackMap.begin();
    while(ite != m_callBackMap.end())
    {
        DwEventId* evtId = ite->first;
        stream << " Event: "<< evtId->name();
        ++ite;
    }
#endif
    stream  << "]";
}