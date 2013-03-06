#pragma once
#include "dwgui/dwgraphicseventhandler.h"
#include "dwgui/dwgraphicsevent.h"

//------------------------------------------------------------------------------
class DwGraphicsView;
class DwGraphicsItem;
class DwGraphicsScene;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwDefaultSceneEventHandler : public DwSceneEventHandler
{
    DW_DECLARE_CLASS(DwDefaultSceneEventHandler);

public:
    explicit DwDefaultSceneEventHandler(DwGraphicsScene* scene = NULL);
    virtual ~DwDefaultSceneEventHandler();

    virtual void onEvent(DwEvent* evt);

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
#endif

    // Command Stuffs
#if DW_ENABLE_EVENT_COMMAND
    void onCommandEvent(DwCommandEvent* evt);
    virtual void onWindowCreate(DwWindowCreateEvent* evt);
    virtual void onWindowClosed(DwWindowClosedEvent *evt);
    virtual void onWindowDestroy(DwWindowDestroyEvent* evt);
    virtual void onCursorChange(DwCursorChangeEvent* evt);
    virtual void onContexMenu(DwContexMenuEvent* evt);
    virtual void onQuit(DwQuitEvent* evt);
#endif

    // Timer Stuffs
    virtual void onTimerEvent(DwTimerEvent* evt);

    // Event Result Stuffs
    virtual void onEventAccpted(DwEventAcceptedEvent* evt);

    // Resize Event Stuffs
    virtual void onResizeEvent(DwResizeEvent *evt);

    enum PressedMode
    {
        ReqNormal,
        ReqWhatThis,
        ReqStatusTip,
    };
    PressedMode m_pressedMode;
    int m_pressedGrabCnt;
private:
};// end of DwDefaultSceneEventHandler
DW_REGISTER_CLASS(DwDefaultSceneEventHandler);