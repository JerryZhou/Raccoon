#pragma once
#include "dwgui/dwgraphicseventhandler.h"
#include "dwgui/dwgraphicsevent.h"

//------------------------------------------------------------------------------
class DwGraphicsItem;
class DwGraphicsScene;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwDefaultItemEventHandler : public DwItemEventHandler
{
    DW_DECLARE_CLASS(DwDefaultItemEventHandler);

public:
    explicit DwDefaultItemEventHandler(DwGraphicsItem* item = NULL);
    virtual ~DwDefaultItemEventHandler();

    virtual void onEvent(DwEvent* evt);

protected:
    // Mouse Stuffs
    void onMouseEvent(DwMouseEvent* evt);
    virtual void onMouseDown(DwMouseDownEvent* evt);
    virtual void onMouseUp(DwMouseUpEvent* evt);
    virtual void onMouseWheel(DwMouseWheelEvent* evt);
    virtual void onMouseMoved(DwMouseMoveEvent* evt);
    virtual void onMouseDbClicked(DwMouseDbClickEvent* evt);
    virtual void onMouseClicked(DwMouseClickedEvent* evt);

    // Key Stuffs
    void onKeyEvent(DwKeyEvent* evt);
    virtual void onKeyDown(DwKeyDownEvent* evt);
    virtual void onKeyUp(DwKeyUpEvent* evt);

    // Char Stuffs
    virtual void onCharEvent(DwCharEvent* evt);

    // Hover Stuffs
#if DW_ENABLE_EVENT_HOVER
    void onHoverEvent(DwHoverEvent* evt);
    virtual void onHoverEnter(DwHoverEnterEvent* evt);
    virtual void onHoverLeave(DwHoverLeaveEvent* evt);
#endif

    // Focus Stuffs
#if DW_ENABLE_EVENT_FOCUS
    void onFocusEvent(DwFocusEvent* evt);
    virtual void onFocusIn(DwFocusInEvent* evt);
    virtual void onFocusOut(DwFocusOutEvent* evt);
#endif

    // Drag Stuffs
#if DW_ENABLE_EVENT_DRAGDROP
    void onDragEvent(DwSceneDragEvent* evt);
    virtual void onDragStartEvent(DwDragStartEvent* evt);
    virtual void onDragMoveEvent(DwDragMoveEvent* evt);
    virtual void onDragEndEvent(DwDragEndEvent* evt);
#endif

    // Grab Stuffs
#if DW_ENABLE_EVENT_GRABER
    void onGrabEvent(DwGraberEvent* evt);
    virtual void onGainMouseEvent(DwGrabMouseEvent* evt);
    virtual void onLoseMouseEvent(DwUngrabMouseEvent* evt);
    virtual void onKeyboardGainEvent(DwGrabKeyboardEvent* evt);
    virtual void onKeyboardLoseEvent(DwUngrabKeyboardEvent* evt);
#endif

    // Tips Stuffs
#if DW_ENABLE_EVENT_TIPS
    void onTipsEvent(DwTipsEvent* evt);
    virtual void onToolTipEvent(DwToolTipEvent* evt);
    virtual void onWhatsThisEvent(DwWhatsThisEvent* evt);
    virtual void onStatusTipEvent(DwStatusTipEvent* evt);
#endif
private:
};// end of DwDefaultItemEventHandler
DW_REGISTER_CLASS(DwDefaultItemEventHandler);