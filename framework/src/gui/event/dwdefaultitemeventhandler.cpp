#include "stable.h"
#include "graphicsview/dwdummys.h"
#include "graphicsview/dwgraphicsitem.h"
#include "dwdefaultitemeventhandler.h"

DW_IMPLEMENT_CLASS(DwDefaultItemEventHandler, 'DIEH', DwItemEventHandler);

//------------------------------------------------------------------------------
/**
*/
DwDefaultItemEventHandler::DwDefaultItemEventHandler(DwGraphicsItem* item)
    : DwItemEventHandler(item)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwDefaultItemEventHandler::~DwDefaultItemEventHandler()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultItemEventHandler::onEvent(DwEvent* evt)
{
    DW_ASSERT(evt->isA(DwSceneEvent::EventId()));
    // base class procedure
    DwItemEventHandler::onEvent(evt);
    DW_EVENT_CHECK(evt);

    DW_EVENT_BEGIN();
    DW_EVENT_DISPATCH_AS(evt, DwMouseEvent, onMouseEvent);
    DW_EVENT_DISPATCH_AS(evt, DwKeyEvent, onKeyEvent);
    DW_EVENT_DISPATCH_AS(evt, DwCharEvent, onCharEvent);
#if DW_ENABLE_EVENT_HOVER
    DW_EVENT_DISPATCH_AS(evt, DwHoverEvent, onHoverEvent);
#endif
#if DW_ENABLE_EVENT_FOCUS
    DW_EVENT_DISPATCH_AS(evt, DwFocusEvent, onFocusEvent);
#endif
#if DW_ENABLE_EVENT_DRAGDROP
    DW_EVENT_DISPATCH_AS(evt, DwSceneDragEvent, onDragEvent);
#endif
#if DW_ENABLE_EVENT_GRABER
    DW_EVENT_DISPATCH_AS(evt, DwGraberEvent, onGrabEvent);
#endif
#if DW_ENABLE_EVENT_TIPS
    DW_EVENT_DISPATCH_AS(evt, DwTipsEvent, onTipsEvent);
#endif
    DW_EVENT_END();
}

//------------------------------------------------------------------------------
/**
    Mouse Stuffs
*/
void DwDefaultItemEventHandler::onMouseEvent(DwMouseEvent* evt)
{
    DW_EVENT_BEGIN();
    DW_EVENT_DISPATCH(evt, DwMouseDownEvent, onMouseDown);
    DW_EVENT_DISPATCH(evt, DwMouseUpEvent, onMouseUp);
    DW_EVENT_DISPATCH(evt, DwMouseWheelEvent, onMouseWheel);
    DW_EVENT_DISPATCH(evt, DwMouseMoveEvent, onMouseMoved);
    DW_EVENT_DISPATCH(evt, DwMouseDbClickEvent, onMouseDbClicked);
    DW_EVENT_DISPATCH(evt, DwMouseClickedEvent, onMouseClicked);
    DW_EVENT_END();
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultItemEventHandler::onMouseDown(DwMouseDownEvent* evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultItemEventHandler::onMouseUp(DwMouseUpEvent* evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultItemEventHandler::onMouseWheel(DwMouseWheelEvent* evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultItemEventHandler::onMouseMoved(DwMouseMoveEvent* evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultItemEventHandler::onMouseDbClicked(DwMouseDbClickEvent* evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultItemEventHandler::onMouseClicked(DwMouseClickedEvent* evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
    Key Stuffs
*/
void DwDefaultItemEventHandler::onKeyEvent(DwKeyEvent* evt)
{
    DW_EVENT_BEGIN();
    DW_EVENT_DISPATCH(evt, DwKeyDownEvent, onKeyDown);
    DW_EVENT_DISPATCH(evt, DwKeyUpEvent, onKeyUp);
    DW_EVENT_END();
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultItemEventHandler::onKeyDown(DwKeyDownEvent* evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultItemEventHandler::onKeyUp(DwKeyUpEvent* evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
    Char Stuffs
*/
void DwDefaultItemEventHandler::onCharEvent(DwCharEvent* evt)
{
    DW_UNUSED(evt);
}

#if DW_ENABLE_EVENT_HOVER
//------------------------------------------------------------------------------
/**
    Hover Stuffs
*/
void DwDefaultItemEventHandler::onHoverEvent(DwHoverEvent* evt)
{
    DW_EVENT_BEGIN();
    DW_EVENT_DISPATCH(evt, DwHoverEnterEvent, onHoverEnter);
    DW_EVENT_DISPATCH(evt, DwHoverLeaveEvent, onHoverLeave);
    DW_EVENT_END();
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultItemEventHandler::onHoverEnter(DwHoverEnterEvent* evt)
{
    DW_UNUSED(evt);
    m_item->update();
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultItemEventHandler::onHoverLeave(DwHoverLeaveEvent* evt)
{
    DW_UNUSED(evt);
    m_item->update();
}
#endif

#if DW_ENABLE_EVENT_FOCUS
//------------------------------------------------------------------------------
/**
*/
void DwDefaultItemEventHandler::onFocusEvent(DwFocusEvent* evt)
{
    DW_EVENT_BEGIN();
    DW_EVENT_DISPATCH(evt, DwFocusInEvent, onFocusIn);
    DW_EVENT_DISPATCH(evt, DwFocusOutEvent, onFocusOut);
    DW_EVENT_END();
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultItemEventHandler::onFocusIn(DwFocusInEvent* evt)
{
    DW_UNUSED(evt);
    m_item->update();
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultItemEventHandler::onFocusOut(DwFocusOutEvent* evt)
{
    DW_UNUSED(evt);
    m_item->update();
}
#endif

#if DW_ENABLE_EVENT_DRAGDROP
//------------------------------------------------------------------------------
/**
    Drag Stuffs
*/
void DwDefaultItemEventHandler::onDragEvent(DwSceneDragEvent* evt)
{
    DW_EVENT_BEGIN();
    DW_EVENT_DISPATCH(evt, DwDragStartEvent, onDragStartEvent);
    DW_EVENT_DISPATCH(evt, DwDragMoveEvent, onDragMoveEvent);
    DW_EVENT_DISPATCH(evt, DwDragEndEvent, onDragEndEvent);
    DW_EVENT_END();
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultItemEventHandler::onDragStartEvent(DwDragStartEvent* evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultItemEventHandler::onDragMoveEvent(DwDragMoveEvent* evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultItemEventHandler::onDragEndEvent(DwDragEndEvent* evt)
{
    DW_UNUSED(evt);
}
#endif

#if DW_ENABLE_EVENT_GRABER
//------------------------------------------------------------------------------
/**
    Grab Stuffs
*/
void DwDefaultItemEventHandler::onGrabEvent(DwGraberEvent* evt)
{
    DW_EVENT_BEGIN();
    DW_EVENT_DISPATCH(evt, DwGrabMouseEvent, onGainMouseEvent);
    DW_EVENT_DISPATCH(evt, DwUngrabMouseEvent, onLoseMouseEvent);
    DW_EVENT_DISPATCH(evt, DwGrabKeyboardEvent, onKeyboardGainEvent);
    DW_EVENT_DISPATCH(evt, DwUngrabKeyboardEvent, onKeyboardLoseEvent);
    DW_EVENT_END();
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultItemEventHandler::onGainMouseEvent(DwGrabMouseEvent* evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultItemEventHandler::onLoseMouseEvent(DwUngrabMouseEvent* evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultItemEventHandler::onKeyboardGainEvent(DwGrabKeyboardEvent* evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwDefaultItemEventHandler::onKeyboardLoseEvent(DwUngrabKeyboardEvent* evt)
{
    DW_UNUSED(evt);
}
#endif

#if DW_ENABLE_EVENT_TIPS
//------------------------------------------------------------------------------
/**
*/
void DwDefaultItemEventHandler::onTipsEvent(DwTipsEvent* evt)
{
    DW_EVENT_BEGIN();
    DW_EVENT_DISPATCH(evt, DwToolTipEvent, onToolTipEvent);
    DW_EVENT_DISPATCH(evt, DwWhatsThisEvent, onWhatsThisEvent);
    DW_EVENT_DISPATCH(evt, DwStatusTipEvent, onStatusTipEvent);
    DW_EVENT_END();
}

void DwDefaultItemEventHandler::onToolTipEvent(DwToolTipEvent* evt)
{
    DW_UNUSED(evt);
}

void DwDefaultItemEventHandler::onWhatsThisEvent(DwWhatsThisEvent* evt)
{
    DW_UNUSED(evt);
}

void DwDefaultItemEventHandler::onStatusTipEvent(DwStatusTipEvent* evt)
{
    DW_UNUSED(evt);
}
#endif
