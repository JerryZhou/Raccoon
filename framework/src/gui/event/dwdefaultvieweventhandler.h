#pragma once
#include "dwgui/dwgraphicseventhandler.h"
#include "dwgui/dwgraphicsevent.h"
#include "dwgui/dwtimer.h"
#include "dwgui/dwgraphicsscenelistener.h"

//------------------------------------------------------------------------------
class DwGraphicsView;
class DwGraphicsItem;
class DwGraphicsScene;

// NB! IN THIS IMPLEMENT THE VIEW CANNOT LISTENER TO FOCUS EVENT
//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwDefaultViewEventHandler :
    public DwViewEventHandler,
    public sigslot::has_slots ,
    public DwGraphicsSceneListener
{
    DW_DECLARE_CLASS(DwDefaultViewEventHandler);

public:
    explicit DwDefaultViewEventHandler(DwGraphicsView* view = NULL);
    virtual ~DwDefaultViewEventHandler();

    // Implement DwViewEventHandler
    virtual void onEvent(DwEvent* evt);

    // Implement DwGraphicsSceneListener
    virtual void onAdded(DwGraphicsScene *scene)
    {
        DW_UNUSED(scene);
    }
    virtual void onRemoved(DwGraphicsScene *scene)
    {
        DW_UNUSED(scene);
    }
    virtual void onNotify(Dw::GraphicsSceneNotify notify, DwGraphicsScene *scene, DwGraphicsItem* item);

    void setShowDirtyRect(bool show);
    bool showDirtyRect();

protected:
    // Mouse Stuffs
    void onMouseEvent(DwMouseEvent* evt);
    virtual void onMouseDown(DwMouseDownEvent* evt);
    virtual void onMouseUp(DwMouseUpEvent* evt);
    virtual void onMouseWheel(DwMouseWheelEvent* evt);
    virtual void onMouseMoved(DwMouseMoveEvent* evt);

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
    virtual void onHoverGain(DwHoverGainEvent* evt);
    virtual void onHoverLeave(DwHoverLeaveEvent* evt);
    void dispatchHoverEvent(DwMouseEvent *mouseEvent);
    bool itemAcceptsHoverEvents_helper(DwGraphicsItem *item) const;
#endif
    void clearHovers();

    // Focus Stuffs (ONLY DISPATCH)
#if DW_ENABLE_EVENT_FOCUS
    void dispatchFocusEvent(DwMouseDownEvent* evt);	// reason mouse
    void dispatchFocusEvent(DwKeyDownEvent* evt);		// reason tab
    bool itemAcceptsFocusEvents_helper(DwGraphicsItem *item) const;
    DwGraphicsItem * focusItem() const;
    DwGraphicsItem * lastFocusItem() const;
    void setFocusItem(DwGraphicsItem *item, Dw::FocusReason reason);
    void setFocus(Dw::FocusReason focusReason = Dw::OtherFocusReason);
    bool focusNextPrevChild(bool next);
#endif
    void clearFocus();

    // Timer Stuffs
    virtual void onTimerEvent(DwTimerEvent* evt);

    // Tips Stuffs
#if DW_ENABLE_EVENT_TIPS
    void onTipsEvent(DwTipsEvent* evt);
    virtual void onShowToolTip(DwShowToolTipEvent* evt);
    virtual void onHideToolTip(DwHideToolTipEvent* evt);
#endif
    void clearTips();

    // Timer Event
    void onTipsTimer();

    // Resize Event
    virtual void onResizeEvent(DwResizeEvent *evt);

    // GUI Command Evednt
#if DW_ENABLE_EVENT_COMMAND
    void onCommandEvent(DwCommandEvent *evt);
    virtual void onWindowCreate(DwWindowCreateEvent *evt);
    virtual void onWindowClosed(DwWindowClosedEvent *evt);
    virtual void onWindowDestroy(DwWindowDestroyEvent *evt);
#endif

    // Paint Event
    virtual void onPaintEvent(DwPaintEvent *evt);

    // Erase Bk Event
    virtual void onEraseBkgndEvent(DwEraseBkgndEvent *evt);

    // Update the Hover
    virtual void onUpdateHoverEvent(DwUpdateHoverEvent *evt);

    // Tips Timing
    bool m_startTipsTiming;
    uint32_t m_tipsCount;
    DwTimer m_tipsTimer;

    // Hover Stuffs
    DwVector<DwGraphicsItem *> m_cachedItemsUnderMouse;
    DwVector<DwGraphicsItem *> m_hoverItems;

    bool m_showDirtyRect;
private:
};// end of DwDefaultViewEventHandler
DW_REGISTER_CLASS(DwDefaultViewEventHandler);