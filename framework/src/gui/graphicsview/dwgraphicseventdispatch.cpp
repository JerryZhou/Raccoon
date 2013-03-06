#include "stable.h"
#include "dwdummys.h"
#include "dwgraphicseventdispatch.h"
#include "dwgraphicseventhandler.h"
#include "dwgraphicsevent.h"
#include "dwgraphicsitem.h"
#include "dwgraphicsscene.h"
#include "dwgraphicsview.h"
#include "dwgraphicseventdispatch_p.h"
#include "dwcore/dwglobalstatic.h"

//------------------------------------------------------------------------------
/**
*/
DwGraphicsEventDispatch::DwGraphicsEventDispatch()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsEventDispatch::~DwGraphicsEventDispatch()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsEventDispatch::dispatchEvent(const DwGraphicsItem* item, DwSceneEvent* evt)const
{
    if (evt->isUp())
    {
        dispatchEventUp(item, evt);
    }
    else
    {
        dispatchEventDown(item, evt);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsEventDispatch::dispatchEventDown(const DwGraphicsItem* item, DwSceneEvent* evt)const
{
    static int dispatchDeepth = 0;
    DwLevelRefCounted deepth(dispatchDeepth);
    DW_ASSERT(item);
    DW_ASSERT(dispatchDeepth <= DW_GUI_MAX_EVENT_LEVEL && "DO NOT SEND ITEM EVENT IN ITEM'S EVENT HANDLER GRETER 32 LEVEL");
    DW_ASSERT(evt && !evt->isAccepted() && !evt->isUp());
    DW_ASSERT(evt->dispatchDownLevel() <= DW_GUI_MAX_EVENT_LEVEL);
    DW_EVENT_TRACE(item, evt);

    DwItemEventHandler *handler = item->eventHandler();
    if (handler && evt->isInterestedBy(item))
    {
        if (evt->dispatchDownLevel() == CountedLevelOne || handler->isAcceptedParentEvents())
        {
            handler->onEventEn(evt);
            if (evt->isAccepted())
            {
                evt->setDoneItem(item);
                // reback to scene
                DwEventAcceptedEvent eventAceeptedEvt(evt);
                if(item->scene()) item->scene()->sendEvent(&eventAceeptedEvt);
                return;
            }
        }
    }

    if (item->children().size() > 0)
    {
        DwLevelRefCounted count(evt->dispatchDownLevel());
        (const_cast<DwGraphicsItem*>(item))->ensureSortedChildren();
        for (int i=item->children().size()-1; i>=0; --i)
        {
            DwGraphicsItem* child = item->children().at(i);
            dispatchEventDown(child, evt);
            if (evt->isAccepted())
            {
                return;
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsEventDispatch::dispatchEventUp(const DwGraphicsItem* item, DwSceneEvent* evt)const
{
    static int dispatchDeepth = 0;
    DwLevelRefCounted deepth(dispatchDeepth);
    DW_ASSERT(item);
    DW_ASSERT(dispatchDeepth <= DW_GUI_MAX_EVENT_LEVEL && "DO NOT SEND ITEM EVENT IN ITEM'S EVENT HANDLER GRETER 32 LEVEL");
    DW_ASSERT(evt && !evt->isAccepted() && evt->isUp());
    DW_ASSERT(evt->dispatchUpLevel() <= DW_GUI_MAX_EVENT_LEVEL);
    DW_EVENT_TRACE(item, evt);

    DwItemEventHandler *handler = item->eventHandler();
    if (handler && evt->isInterestedBy(item))
    {
        if (evt->dispatchUpLevel() == CountedLevelOne || handler->isAcceptedChildEvents())
        {
            handler->onEventEn(evt);
            if (evt->isAccepted())
            {
                evt->setDoneItem(item);
                // reback to scene
                DwEventAcceptedEvent eventAceeptedEvt(evt);
                if(item->scene()) item->scene()->sendEvent(&eventAceeptedEvt);
                return;
            }
        }
    }

    if (item->parentItem())
    {
        DwGraphicsItem* parent = item->parentItem();
        DwLevelRefCounted count(evt->dispatchUpLevel());
        dispatchEventUp(parent, evt);
    }
}

//------------------------------------------------------------------------------
/**
	Accept The SceneEvent In View Coordinate System
	TODO: xx
*/
void DwGraphicsEventDispatch::dispatchEvent(const DwGraphicsView* view, DwSceneEvent* evt)const
{
    DW_EVENT_TRACE(view, evt);

    // keep view
    evt->setView(view);
    // filter in view event handler
    DwViewEventHandler* viewEventHandler = view->eventHandler();
    if (viewEventHandler)
    {
        viewEventHandler->onEventEn(evt);
    }

    if (!evt->isAccepted())
    {
        // just translated to scene
        evt->translatedToScene(view);
        // dispath to scene
        dispatchEvent(view->scene(), evt);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsEventDispatch::dispatchEvent(const DwGraphicsScene* scene, DwSceneEvent* evt)const
{
    DW_CHECK(scene);
    DW_EVENT_TRACE(scene, evt);

    DwSceneEventHandler* sceneEventHandler = scene->eventHandler();
    if (sceneEventHandler)
    {
        sceneEventHandler->onEventEn(evt);
    }

    if (!evt->isAccepted())
    {
        DwGraphicsItem* item = evt->indexInterestItemFromScene(scene);
        if (item)
        {
            // just translated to first item
            evt->translatedToItem(item);
            // dispatch to item
            dispatchEvent(item, evt);
        }
        else
        {
            // TODO: other policy
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsEventDispatch::dispatch(const DwRttiObject *obj, DwEvent *evt)const
{
    DW_CHECK(obj);
    if (obj->isA(DwGraphicsView::RTTI()))
    {
        DW_ASSERT(evt->isA(DwSceneEvent::EventId()));
        dispatchEvent(dwsafe_cast<DwGraphicsView>(obj), dwsafe_cast<DwSceneEvent>(evt));
    }
    else if (obj->isA(DwGraphicsScene::RTTI()))
    {
        DW_ASSERT(evt->isA(DwSceneEvent::EventId()));
        dispatchEvent(dwsafe_cast<DwGraphicsScene>(obj), dwsafe_cast<DwSceneEvent>(evt));
    }
    else if (obj->isA(DwGraphicsItem::RTTI()))
    {
        DW_ASSERT(evt->isA(DwSceneEvent::EventId()));
        dispatchEvent(dwsafe_cast<DwGraphicsItem>(obj), dwsafe_cast<DwSceneEvent>(evt));
    }
    else
    {
        // TODO: xx
    }
}

//------------------------------------------------------------------------------
/**
*/
DW_GLOBAL_STATIC(DwGraphicsEventDispatch, _dw_GraphicsEventDispatch);
DW_GUI_EXPORT DwGraphicsEventDispatch *dwGraphicsEventDispatch()
{
    return _dw_GraphicsEventDispatch();
}