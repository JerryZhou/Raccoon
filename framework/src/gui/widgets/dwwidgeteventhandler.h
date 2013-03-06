#pragma once
#include "dwgui/dwdefaultitemeventhandler.h"

//------------------------------------------------------------------------------
class DwGraphicsBackgroundItem;
class DwEventSignalDispatch;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwWidgetEventHandler : public DwDefaultItemEventHandler
{
    typedef DwDefaultItemEventHandler SuperClass;
    DW_DISABLE_COPY(DwWidgetEventHandler);
    DW_DECLARE_CLASS(DwWidgetEventHandler);
public:
    explicit DwWidgetEventHandler(DwGraphicsItem* item = NULL, DwGraphicsBackgroundItem* back = NULL);

    virtual ~DwWidgetEventHandler();

    inline DwGraphicsBackgroundItem *background() const;

    virtual void onMouseDown(DwMouseDownEvent* evt);

    virtual void onMouseUp(DwMouseUpEvent* evt);

#if DW_ENABLE_EVENT_HOVER
    virtual void onHoverEnter(DwHoverEnterEvent* evt);

    virtual void onHoverLeave(DwHoverLeaveEvent* evt);
#endif

#if DW_ENABLE_EVENT_FOCUS
    virtual void onFocusIn(DwFocusInEvent* evt);

    virtual void onFocusOut(DwFocusOutEvent* evt);
#endif

#if DW_ENABLE_EVENT_DRAGDROP
    virtual void onDragStartEvent(DwDragStartEvent* evt);

    virtual void onDragMoveEvent(DwDragMoveEvent* evt);

    virtual void onDragEndEvent(DwDragEndEvent* evt);
#endif

#if DW_ENABLE_EVENT_TIPS
    virtual void onToolTipEvent(DwToolTipEvent *evt);
#endif

protected:
    DwGraphicsBackgroundItem* m_background;
};// end of DwWidgetEventHandler
DW_REGISTER_CLASS(DwWidgetEventHandler);

//------------------------------------------------------------------------------
/**
*/
inline DwGraphicsBackgroundItem *DwWidgetEventHandler::background() const
{
    return m_background;
}