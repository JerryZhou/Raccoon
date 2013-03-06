#pragma once
#include "dwgui/dwrttiobject.h"
#include "dwgraphics/dwnativecanvas.h"
#include "dwgraphics/dwregion.h"

//------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////
#ifdef DW_MESSAGE_HANDLER
#else
#define DW_MESSAGE_HANDLER(func) \
    if(true) \
    { \
    bHandled = TRUE; \
    lResult = func(uMsg, wParam, lParam, bHandled); \
    if(bHandled) \
    return TRUE; \
    }
#endif

//------------------------------------------------------------------------------
class DwGraphicsView;
class DwMouseEvent;
class DwWindow;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwWindowAdapter : public DwRttiObject
{
    DW_DISABLE_COPY(DwWindowAdapter);
    DW_DECLARE_CLASS(DwWindowAdapter);

    BEGIN_MSG_MAP(DwWindowAdapter)
    DW_MESSAGE_HANDLER(OnWinMsg)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    MESSAGE_HANDLER(WM_NCDESTROY, OnNcDestroy)
    MESSAGE_HANDLER(WM_SIZE, OnSize)
    MESSAGE_HANDLER(WM_MOVE, OnMove)
    MESSAGE_HANDLER(WM_NCCALCSIZE, OnNcCalcSize)
    MESSAGE_HANDLER(WM_NCHITTEST, OnNcHitTest)
    MESSAGE_HANDLER(WM_TIMER, OnTimer)
    // Mouse Move
    MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
    MESSAGE_HANDLER(WM_NCMOUSEMOVE, OnNcMouseMove)
    // Mouse Left Button
    MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
    MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDBClick)
    MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
    MESSAGE_HANDLER(WM_NCLBUTTONDOWN, OnNcLButtonDown)
    MESSAGE_HANDLER(WM_NCLBUTTONDBLCLK, OnNcLButtonDBClick)
    MESSAGE_HANDLER(WM_NCLBUTTONUP, OnNcLButtonUp)
    // Mouse Right Button
    MESSAGE_HANDLER(WM_RBUTTONDOWN, OnRButtonDown)
    MESSAGE_HANDLER(WM_RBUTTONDBLCLK, OnRButtonDown)
    MESSAGE_HANDLER(WM_RBUTTONUP, OnRButtonUp)
    MESSAGE_HANDLER(WM_NCRBUTTONDOWN, OnNcRButtonDown)
    MESSAGE_HANDLER(WM_NCRBUTTONDBLCLK, OnNcRButtonDBClick)
    MESSAGE_HANDLER(WM_NCRBUTTONUP, OnNcRButtonUp)
    // Mouse Middle Button
    MESSAGE_HANDLER(WM_MBUTTONDOWN, OnMButtonDown)
    MESSAGE_HANDLER(WM_MBUTTONDBLCLK, OnMButtonDBClick)
    MESSAGE_HANDLER(WM_MBUTTONUP, OnMButtonUp)
    // Mouse Wheel
    MESSAGE_HANDLER(WM_MOUSEWHEEL, OnMouseWheel)
    // Mouse Hover
    MESSAGE_HANDLER(WM_MOUSEHOVER, OnMouseHover);
    // Mouse Leave
    MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave);
    MESSAGE_HANDLER(WM_CAPTURECHANGED, OnCaptureChanged)
    MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMiniMaxInfo)
    MESSAGE_HANDLER(WM_PAINT, OnPaint)
    MESSAGE_HANDLER(WM_NCPAINT, OnNcPaint)
    MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
    // Key
    MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
    MESSAGE_HANDLER(WM_KEYUP, OnKeyUp)
    MESSAGE_HANDLER(WM_SYSKEYDOWN, OnKeyDown)
    MESSAGE_HANDLER(WM_SYSKEYUP, OnKeyUp)
    // Char
    MESSAGE_HANDLER(WM_CHAR, OnChar)
    // Drop
    MESSAGE_HANDLER(WM_DROPFILES, OnDropFiles)
    // SHOW
    MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
    // Active
    MESSAGE_HANDLER(WM_ACTIVATE, OnActive)
    // Command
    MESSAGE_HANDLER(WM_COMMAND, OnCommand)
    // Sys Command
    MESSAGE_HANDLER(WM_SYSCOMMAND, OnSysCommand)
    // Accessible Get Object
    MESSAGE_HANDLER(WM_GETOBJECT, OnAccGetObject)
    // Accessbile Get Text
    MESSAGE_HANDLER(WM_GETTEXT, OnAccGetText)
    END_MSG_MAP()
public:
    DwWindowAdapter();
    virtual ~DwWindowAdapter();

    void open(HWND hwnd, DwGraphicsView *view);
    void open(DwWindow *window, DwGraphicsView *view);
    void close();

    // Entery
    HRESULT dispatchWinMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    // Will Dispatch All The Window Msg
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnWinMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    // Window Create
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

    // Window Closed
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

    // Window Destroy
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnNcDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);

    // Resize Event
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    // Mouse Left Button
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);

    // Hit Test
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnNcHitTest(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled);

    // Calc Size
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnNcCalcSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);

    // Mouse Left Button
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);

    // Mouse Left Button
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);

    // Db Clicked
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnLButtonDBClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    // Mouse Left Button Up
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);

    //
    //////////////////////////////////////////////////////////////////////////
    // Mouse Left Button
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnNcLButtonDown(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    // Db Clicked
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnNcLButtonDBClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    // Mouse Left Button Up
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnNcLButtonUp(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    // Mouse Right Button
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnRButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);

    // Mouse Right Button Double Click
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnRButtonDBClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    // Mouse Right Button Up
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnRButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);

    // Mouse Right Button
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnNcRButtonDown(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    // Mouse Right Button Double Click
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnNcRButtonDBClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    // Mouse Right Button Up
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnNcRButtonUp(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    // Mouse Middle Button
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnMButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);

    // Mouse Middle Button Double Clicked
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnMButtonDBClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    // Mouse Middle Button Up
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnMButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);

    // Mouse Move
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);

    // On Nc Mouse Move
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnNcMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    // Mouse Wheel
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnMouseWheel(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);

    // OnMouseHover
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnMouseHover(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

    // OnMouseLeave
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnMouseLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

    // Drop Files
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnDropFiles(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);

    LRESULT OnCaptureChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

    LRESULT OnGetMiniMaxInfo(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);

    LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);

    // TODO: use paint event
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);

    // TODO: use paint event
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnNcPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);

    // Key Down
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    // Key Up
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnKeyUp(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    // Char
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnChar(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    // Show Window
    LRESULT OnShowWindow(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    // Active
    LRESULT OnActive(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    // Command
    LRESULT OnCommand(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    // SysCommand
    LRESULT OnSysCommand(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


    // GetObject
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnAccGetObject(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    // GetText
    LRESULT OnAccGetText(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    // SHow
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnShow(int command);

    //////////////////////////////////////////////////////////////////////////
    void update(PAINTSTRUCT *ps = NULL);

    //////////////////////////////////////////////////////////////////////////
    template<typename T>
    void reserveLastMouseEvent(const T& evt);

    //////////////////////////////////////////////////////////////////////////
    DwCanvas *canvas();

protected:
    HWND m_hWnd;

    bool m_open;
    DwNativeCanvas m_canvas;

    DwWindow *m_window;
    DwGraphicsView *m_view;
    DwMouseEvent* m_lastMouseEvent;

    bool m_translucentBackground;
    bool m_ncLeftMouseDown;
    bool m_ncRightMouseDown;
    bool m_ncMouseMoved;
};// end of DwWindowAdapter

//------------------------------------------------------------------------------
/**
*/
template<typename T>
inline void DwWindowAdapter::reserveLastMouseEvent(const T& evt)
{
    if(m_lastMouseEvent && m_lastMouseEvent->EventId() == T::EventId())
    {
        *(static_cast<T*>(m_lastMouseEvent)) = evt;
    }
    else
    {
        if(m_lastMouseEvent) delete m_lastMouseEvent;
        m_lastMouseEvent = new T(evt);
    }
}