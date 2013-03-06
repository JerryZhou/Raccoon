#include "stable.h"
#include "graphicsview/dwdummys.h"
#include "graphicsview/dwgraphicsscene.h"
#include "graphicsview/dwgraphicsitem.h"
#include "graphicsview/dwgraphicsutil.h"
#include "graphicsview/dwgraphicsview.h"
#include "kernel/dwwindow.h"
#include "dwdefaultsceneeventhandler.h"

//redefine
//------------------------------------------------------------------------------
#define	LastDragItem Dw::UserIndicator0
#define LastPressedItem Dw::UserIndicator1
#define LastMouseGraberItem Dw::UserIndicator2
#define LastDragMoveItem Dw::UserIndicator3

DW_IMPLEMENT_CLASS(DwDefaultSceneEventHandler, 'DSEH', DwSceneEventHandler);

//------------------------------------------------------------------------------
/**
*/
DwDefaultSceneEventHandler::DwDefaultSceneEventHandler(DwGraphicsScene* scene)
    : DwSceneEventHandler(scene)
    , m_pressedMode(ReqNormal)
    , m_pressedGrabCnt(0)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwDefaultSceneEventHandler::~DwDefaultSceneEventHandler()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultSceneEventHandler::onEvent(DwEvent* evt)
{
    DW_ASSERT(evt->isA(DwSceneEvent::EventId()));
    // base class procedure
    DwSceneEventHandler::onEvent(evt);
    DW_EVENT_CHECK(evt);

    DW_EVENT_BEGIN();
    // mouse event handler
    DW_EVENT_DISPATCH_AS(evt, DwMouseEvent, onMouseEvent);
    // keyborard event handler
    DW_EVENT_DISPATCH_AS(evt, DwKeyEvent, onKeyEvent);
    // char event handler
    DW_EVENT_DISPATCH_AS(evt, DwCharEvent, onCharEvent);
    // hover event handler
#if DW_ENABLE_EVENT_HOVER
    DW_EVENT_DISPATCH_AS(evt, DwHoverEvent, onHoverEvent);
#endif
    // command event handler
#if DW_ENABLE_EVENT_COMMAND
    DW_EVENT_DISPATCH_AS(evt, DwCommandEvent, onCommandEvent);
#endif
    // timer event handler
    DW_EVENT_DISPATCH(evt, DwTimerEvent, onTimerEvent);
    // event accepted handler
    DW_EVENT_DISPATCH(evt, DwEventAcceptedEvent, onEventAccpted);
    // resize event handler
    DW_EVENT_DISPATCH(evt, DwResizeEvent, onResizeEvent);
    DW_EVENT_END();
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
void DwDefaultSceneEventHandler::onMouseEvent(DwMouseEvent* evt)
{
    // dispatch
    DW_EVENT_BEGIN();
    DW_EVENT_DISPATCH(evt, DwMouseMoveEvent, onMouseMoved);
    DW_EVENT_DISPATCH(evt, DwMouseDownEvent, onMouseDown);
    DW_EVENT_DISPATCH(evt, DwMouseUpEvent, onMouseUp);
    DW_EVENT_DISPATCH(evt, DwMouseWheelEvent, onMouseWheel);
    DW_EVENT_END();
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultSceneEventHandler::onMouseDown(DwMouseDownEvent* evt)
{
    // reback grab Mouse
    DW_ASSERT(m_pressedGrabCnt <= 1);
    while(m_pressedGrabCnt)
    {
        m_scene->clearImplicitMouseGrabber();
        --m_pressedGrabCnt;
    }

    // find the target
    DwGraphicsItem* targetItem = evt->indexInterestItemFromScene(m_scene);

    // pressed it
    m_scene->setIndicator(LastPressedItem, targetItem);

    // drag it
    // start drag
#if DW_ENABLE_EVENT_DRAGDROP
    DwDragStartEvent drag(targetItem);
    DwGraphicsItem* dragItem = targetItem; //DwGraphicsUtil::ancestorOfAccept(targetItem, Dw::ItemIsMovable);
    if (drag.isInterestedBy(dragItem))
    {
        // hijack the evt
        evt->setInterestItem(dragItem);

        // start drag
        DwDragStartEvent drag(dragItem);
        drag.constructFromMouseEvent(evt);
        m_scene->sendEvent(dragItem, &drag);

        // set drag item
        m_scene->setIndicator(LastDragItem, drag.doneItem());
    }
    else
#endif
    {
        // grab it
        if (targetItem
                && evt->isInterestedBy(targetItem)
                && targetItem->focusPolicy() & Dw::ClickFocus)
        {
            ++m_pressedGrabCnt;
            m_scene->grabMouse(targetItem, true);
        }
    }

    // what's this
#if DW_ENABLE_EVENT_TIPS
    if (m_pressedMode == ReqWhatThis && targetItem)
    {
        DwWhatsThisEvent evt;
        m_scene->sendEvent(targetItem, &evt);
    }
#endif
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
void DwDefaultSceneEventHandler::onMouseUp(DwMouseUpEvent* evt)
{
    // find the target
    // DwGraphicsItem* targetItem = evt->indexInterestItem(m_scene);

    DwGraphicsItem* lastPressedItem = m_scene->itemOf(LastPressedItem);
    if (lastPressedItem)
    {
        DwGraphicsItem* targetItem = evt->indexInterestItemFromScene(m_scene);
        if (targetItem != lastPressedItem)
        {
            // dispatch a copy to the last pressed item
            DwMouseUpEvent releaseEvent(*evt);
            m_scene->sendEvent(lastPressedItem, &releaseEvent);

            // cut off the new release
            evt->accept();
        }
        else
        {
            evt->setInterestItem(lastPressedItem);
            evt->translatedToItem(targetItem);
            if (targetItem->boundingRect().contains(evt->m_itemPoint))
            {
                DwGraphicsItem* dragItem = m_scene->itemOf(LastDragMoveItem);
                if (!dragItem)
                {
                    // dispatch a click event to last pressed item
                    DwMouseClickedEvent clickedEvt(lastPressedItem);
                    clickedEvt.m_isNC = evt->m_isNC;
                    clickedEvt.m_nc = evt->m_nc;
                    clickedEvt.m_screenPoint = evt->m_screenPoint;
                    clickedEvt.m_devicePoint = evt->m_devicePoint;
                    clickedEvt.m_scenePoint = evt->m_scenePoint;
                    clickedEvt.m_button = evt->m_button;
                    m_scene->sendEvent(&clickedEvt);
                }
            }
        }
    }

    DwGraphicsItem* dragItem = m_scene->itemOf(LastDragItem);
    if (dragItem)
    {
        // hijack the evt
        evt->setInterestItem(dragItem);

        // set drag item null
        m_scene->setIndicator(LastDragItem, NULL);

        // end drag
#if DW_ENABLE_EVENT_DRAGDROP
        DwDragEndEvent drag(dragItem);
        drag.constructFromMouseEvent(evt);
        m_scene->sendEvent(dragItem, &drag);

        // send a drop event
        DwDragDropEvent drop;
        drop.constructFromDragEvent(&drag);
        drop.m_excludeItem = dragItem;
        drop.m_mimeData.setFormat("objects", "item");
        drop.m_mimeData.appendObject(dragItem);
        m_scene->sendEvent(&drop);
#endif
    }

    // reback grab Mouse
    bool needUpdateHover = false;
    DW_ASSERT(m_pressedGrabCnt <= 1);
    while(m_pressedGrabCnt)
    {
        m_scene->clearImplicitMouseGrabber();
        --m_pressedGrabCnt;
        needUpdateHover = true;
    }

    // update the hover after the clear the grab
    if (needUpdateHover)
    {
        DwUpdateHoverEvent hoverUpdateEvt;
        hoverUpdateEvt.m_mousEvt = evt;
        if (evt->view())
        {
            evt->view()->sendEvent(&hoverUpdateEvt);
        }
    }
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
void DwDefaultSceneEventHandler::onMouseMoved(DwMouseMoveEvent* evt)
{
    // drag it
    DwGraphicsItem* dragItem = m_scene->itemOf(LastDragItem);
    if (dragItem)
    {
        // hijack the evt
        evt->setInterestItem(dragItem);
        m_scene->setIndicator(LastDragMoveItem, dragItem);

#if DW_ENABLE_EVENT_DRAGDROP
        DwMouseMoveEvent* mouseMoveEvt = DW_EVENT_CAST(evt, DwMouseMoveEvent);
        DwDragMoveEvent drag(dragItem, mouseMoveEvt->m_scenePoint, mouseMoveEvt->m_delta);
        drag.constructFromMouseEvent(evt);
        m_scene->sendEvent(dragItem, &drag);
#endif
    }
    else
    {
        m_scene->setIndicator(LastDragMoveItem, NULL);
    }
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
void DwDefaultSceneEventHandler::onMouseWheel(DwMouseWheelEvent* evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultSceneEventHandler::onKeyEvent(DwKeyEvent* evt)
{
    // dispatch
    DW_EVENT_BEGIN();
    DW_EVENT_DISPATCH(evt, DwKeyDownEvent, onKeyDown);
    DW_EVENT_DISPATCH(evt, DwKeyUpEvent, onKeyUp);
    DW_EVENT_END();
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
void DwDefaultSceneEventHandler::onKeyDown(DwKeyDownEvent* evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
	TODO: xx
*/
void DwDefaultSceneEventHandler::onKeyUp(DwKeyUpEvent* evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
	TODO: Dispath Char Input Event
*/
void DwDefaultSceneEventHandler::onCharEvent(DwCharEvent* evt)
{
    DW_UNUSED(evt);
}

#if DW_ENABLE_EVENT_HOVER
//------------------------------------------------------------------------------
/**
*/
void DwDefaultSceneEventHandler::onHoverEvent(DwHoverEvent* evt)
{
    DW_EVENT_BEGIN();
    DW_EVENT_DISPATCH(evt, DwHoverEnterEvent, onHoverEnter);
    DW_EVENT_DISPATCH(evt, DwHoverGainEvent, onHoverGain);
    DW_EVENT_DISPATCH(evt, DwHoverLeaveEvent, onHoverLeave);
    DW_EVENT_END();
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultSceneEventHandler::onHoverEnter(DwHoverEnterEvent* evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultSceneEventHandler::onHoverGain(DwHoverGainEvent* evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultSceneEventHandler::onHoverLeave(DwHoverLeaveEvent* evt)
{
    DW_UNUSED(evt);
}
#endif

#if DW_ENABLE_EVENT_COMMAND
//------------------------------------------------------------------------------
/**
*/
void DwDefaultSceneEventHandler::onCommandEvent(DwCommandEvent* evt)
{
    DW_EVENT_BEGIN();
    DW_EVENT_DISPATCH(evt, DwWindowCreateEvent, onWindowCreate);
    DW_EVENT_DISPATCH(evt, DwWindowDestroyEvent, onWindowDestroy);
    DW_EVENT_DISPATCH(evt, DwWindowClosedEvent, onWindowClosed);
    DW_EVENT_DISPATCH(evt, DwCursorChangeEvent, onCursorChange);
    DW_EVENT_DISPATCH(evt, DwContexMenuEvent, onContexMenu);
    DW_EVENT_DISPATCH(evt, DwQuitEvent, onQuit);
    DW_EVENT_END();
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultSceneEventHandler::onWindowCreate(DwWindowCreateEvent* evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultSceneEventHandler::onWindowClosed(DwWindowClosedEvent *evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultSceneEventHandler::onWindowDestroy(DwWindowDestroyEvent* evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultSceneEventHandler::onCursorChange(DwCursorChangeEvent* evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultSceneEventHandler::onContexMenu(DwContexMenuEvent* evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultSceneEventHandler::onQuit(DwQuitEvent* evt)
{
    DW_UNUSED(evt);
}
#endif

//------------------------------------------------------------------------------
/**
*/
void DwDefaultSceneEventHandler::onTimerEvent(DwTimerEvent* evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
*/
void _dw_onMousePressedEventAccpted(DwMouseDownEvent* evt)
{
    DW_UNUSED(evt);
    DwGraphicsItem* item = evt->doneItem();
    item->scene()->setIndicator(LastPressedItem, item);
}

//------------------------------------------------------------------------------
/**
*/
void _dw_onMouseReleasedEventAccpted(DwMouseUpEvent* evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
    NB! THIS EVENT IS THE SPECIAL EVENT, IN THE EVENT META SPACE,
    FORBID USED TO DILIDER ANY OTHER MESSAGE, FORBID USER TO INHERT IT
*/
void DwDefaultSceneEventHandler::onEventAccpted(DwEventAcceptedEvent* evt)
{
    DwSceneEvent* acptEvt = evt->m_acceptedEvent;
    DW_ASSERT(acptEvt->isAccepted());
    DW_EVENT_BEGIN();
    DW_EVENT_DISPATCH(acptEvt, DwMouseDownEvent, _dw_onMousePressedEventAccpted);
    DW_EVENT_DISPATCH(acptEvt, DwMouseUpEvent, _dw_onMouseReleasedEventAccpted);
    DW_EVENT_END();

    evt->accept();
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultSceneEventHandler::onResizeEvent(DwResizeEvent *evt)
{
    DW_UNUSED(evt);
    // now we just want see the item in view with scale 1:1, so set the scene size to view size
    // if you want to show all of the scene ,just set the scene rect to itemsBoundingRect()
    m_scene->setSceneRect(DwRectF(DwPointF(), evt->m_newSize));
}