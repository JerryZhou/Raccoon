#include "stable.h"
#include "dwwindowadapter.h"
#include <dwcore/dwarraylist.h>
#include "dwgui/dwgraphicsview.h"
#include "dwgui/dwgraphicsevent.h"
#include "dwgui/dwgraphicsscene.h"
#include "dwgui/dwgraphicsitem.h"
#include "dwgui/dwgraphicseventdispatch.h"
#include "dwgui/dwaccessible.h"
#include "dwgui/dwapplication.h"
#include "dwgui/dwcursor.h"
#include "dwgui/dwringlogcase.h"
#include "dwgui/dwwindow.h"
#include "dwgui/dwgraphicsutil.h"
#include "dwgraphics/dwpainter.h"
#include "dwgraphics/dwimage.h"
#include "graphicsview/dwdummys.h"

DW_IMPLEMENT_CLASS(DwWindowAdapter, 'WAER', DwRttiObject);

//------------------------------------------------------------------------------
/**
*/
DwWindowAdapter::DwWindowAdapter()
    : m_hWnd(0)
    , m_open(false)
    , m_view(NULL)
    , m_lastMouseEvent(NULL)
    , m_window(NULL)
    , m_translucentBackground(false)
    , m_ncLeftMouseDown(false)
    , m_ncRightMouseDown(false)
    , m_ncMouseMoved(false)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwWindowAdapter::~DwWindowAdapter()
{
    DW_SAFE_DELETE(m_lastMouseEvent);
}

//------------------------------------------------------------------------------
/**
*/
void DwWindowAdapter::open(HWND hWnd, DwGraphicsView *view)
{
    DW_ASSERT(!m_open);
    DW_ASSERT(hWnd && view);
    m_window = NULL;
    m_hWnd = hWnd;
    m_view = view;
    m_open = true;
    m_translucentBackground = ((DWORD)::GetWindowLong(m_hWnd, GWL_EXSTYLE) & WS_EX_LAYERED) ? true : false;
    m_canvas.setWinHwnd(m_hWnd);
    m_canvas.setTranslucent(m_translucentBackground);
}

//------------------------------------------------------------------------------
/**
*/
void DwWindowAdapter::open(DwWindow *window, DwGraphicsView *view)
{
    DW_ASSERT(!m_open);
    m_window = window;
    m_hWnd = (HWND)(m_window->winId());
    m_view = view;
    m_open = true;
    m_translucentBackground = ((DWORD)::GetWindowLong(m_hWnd, GWL_EXSTYLE) & WS_EX_LAYERED) ? true : false;
    m_canvas.setWinHwnd(m_hWnd);
    m_canvas.setTranslucent(m_translucentBackground);
}

//------------------------------------------------------------------------------
/**
*/
void DwWindowAdapter::close()
{
    m_open = false;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT DwWindowAdapter::dispatchWinMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    HRESULT lResult = 0;
    bHandled = ProcessWindowMessage(m_hWnd, uMsg, wParam, lParam, lResult, 0);
    return lResult;
}

// Will Dispatch All The Window Msg
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnWinMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = false;
    if (m_view)
    {
        DwWindowEvent evt;
        // assign windows standard message params
        evt.assignWin((int)m_hWnd, uMsg, wParam, lParam);

        if (m_view->requireRawMessage() || uMsg >= WM_USER)
        {
            // dispath to window
            dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);
        }

        if (!evt.isAccepted() && m_view->scene())
        {
            DwGraphicsItem * window = m_view->indicator(Dw::WindowItem);
            
            DwGraphicsItem * acceptItem = NULL;
            acceptItem = !acceptItem ? DwGraphicsUtil::ancestorOfAccept(
                m_view->scene()->lastHoverItem(), Dw::ItemRequireRawMessageWhenHover) : acceptItem;
            acceptItem = !acceptItem ? DwGraphicsUtil::ancestorOfAccept(
                m_view->scene()->focusItem(), Dw::ItemRequireRawMessageWhenFocus) : acceptItem;

            if (acceptItem && window != acceptItem) // may be we only send to edit, or we can ask a flag from focus
            {
                // dispatch to focusitem
                evt.setInterestItem(acceptItem);
                // dispath to view
                dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);
            }
        }
        if (evt.isAccepted())
        {
            return evt.m_result;
        }
    }
    return 0;
}

// Window Create
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    if (m_view)
    {
#if DW_ENABLE_EVENT_COMMAND
        DwWindowCreateEvent evt(m_window);
        // fill the windows handle
        evt.m_winId = (int)m_hWnd;
        // dispath to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);
#endif
    }

    m_translucentBackground = ((DWORD)::GetWindowLong(m_hWnd, GWL_EXSTYLE) & WS_EX_LAYERED) ? true : false;

    return 0;
}

// Window Closed
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    if (m_view)
    {
#if DW_ENABLE_EVENT_COMMAND
        DwWindowClosedEvent evt(m_window);
        // fill the windows handle
        evt.m_winId = (int)m_hWnd;
        // dispath to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);
#endif
    }

    return 1;
}

// Window Destroy
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnNcDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    bHandled = FALSE;

    if (m_view)
    {
#if DW_ENABLE_EVENT_COMMAND
        DwWindowDestroyEvent evt(m_window);
        // fill the windows handle
        evt.m_winId = (int)m_hWnd;
        // dispath to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);
#endif
    }

    return 0;
}

// Resize Event
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = FALSE;

    if (!m_hWnd)
    {
        return 0;
    }
    CRect rcClient;
    ::GetClientRect(m_hWnd, &rcClient);
    m_canvas.resize(DwSize(rcClient.Width(), rcClient.Height()));

    if (m_view)
    {
        CRect rcClient;
        ::GetWindowRect(m_hWnd, &rcClient);

        DwResizeEvent evt((DwResizeEvent::ResizePattern)wParam, DwSizeF(), DwSizeF());
        switch (wParam)
        {
        case SIZE_RESTORED:
            evt.m_pattern = DwResizeEvent::Restored;
            break;
        case SIZE_MINIMIZED:
            evt.m_pattern = DwResizeEvent::Minimized;
            break;
        case SIZE_MAXIMIZED:
            evt.m_pattern = DwResizeEvent::Maximized;
            break;
        case SIZE_MAXSHOW:
            evt.m_pattern = DwResizeEvent::MaxShow;
            break;
        case SIZE_MAXHIDE:
            evt.m_pattern = DwResizeEvent::MaxHide;
            break;
        }
        // win handle
        evt.m_hwnd = (int)(m_hWnd);
        // old size
        evt.m_oldSize = DwSizeF(rcClient.Width(), rcClient.Height());
        // new size
        evt.m_newSize = DwSizeF(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        // new rect
        evt.m_clientRect.setRect(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height());
        // dispath to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);
    }

    update(NULL);

    return 0;
}

// Mouse Left Button
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    if (m_view)
    {
        DwMoveEvent evt;

        CRect rcClient;
        ::GetWindowRect(m_hWnd, &rcClient);
        // new rect
        evt.m_clientRect.setRect(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), rcClient.Width(), rcClient.Height());
        // dispath to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);
    }
    return 0;
}

// Hit Test
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnNcHitTest(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
    bHandled = TRUE;
    CRect rcClient;
    ::GetWindowRect(m_hWnd, &rcClient);

    // in screen coordinate
    int x = GET_X_LPARAM(lParam);
    int y = GET_Y_LPARAM(lParam);

    // to client coordinate
    tagPOINT p;
    p.x = x;
    p.y = y;
    BOOL ok = ::ScreenToClient(m_hWnd, &p);
    if (m_view && ok)
    {
        DwNcHitTestEvent evt;
        // default
        evt.m_result = HTCAPTION;
        // a win id
        evt.m_winId = (long)m_hWnd;
        // client rect
        evt.m_clientRect.setRect(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height());
        // screen coordinate
        evt.m_screenPoint = DwPoint(x, y);
        // device coordinate, NB! MUST BE CLIENTRECT
        evt.m_devicePoint = DwPoint(p.x, p.y);
        // mapping to view coordinate
        evt.m_viewPoint = m_view->mapFromDevice(evt.m_devicePoint);
        // dispath to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);

        //if (evt.isAccepted())
        {
            return (LRESULT)evt.m_result;
        }
    }

    return HTCAPTION;
}

// Calc Size
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnNcCalcSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    bHandled = TRUE;
    return 0;
}

// Mouse Left Button
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
    if (m_view)
    {
        DwTimerEvent evt(wParam, m_window);
        // dispath to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);
    }
    bHandled = TRUE;
    return 0;
}

// Mouse Left Button
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    ::SetCapture(m_hWnd);
    if (m_view)
    {
        tagPOINT p = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        ::ClientToScreen(m_hWnd, &p);

        DwMouseDownEvent evt;
        // button
        evt.m_button = Dw::LeftButton;
        // screen coordinate
        evt.m_screenPoint = DwPoint(p.x, p.y);
        // device coordinate, NB! MUST BE CLIENTRECT
        evt.m_devicePoint = DwPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        // mapping to view coordinate
        evt.m_viewPoint = m_view->mapFromDevice(evt.m_devicePoint);
        // reserver last mouse event
        reserveLastMouseEvent(evt);
        // dispath to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);
    }
    return 0;
}

// Db Clicked
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnLButtonDBClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    DW_UNUSED(uMsg);
    DW_UNUSED(bHandled);
    DW_UNUSED(wParam);

    if (m_view)
    {
        tagPOINT p = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        ::ClientToScreen(m_hWnd, &p);

        DwMouseDbClickEvent evt;
        // button
        evt.m_button = Dw::LeftButton;
        // screen coordinate
        evt.m_screenPoint = DwPoint(p.x, p.y);
        // device coordinate, NB! MUST BE CLIENTRECT
        evt.m_devicePoint = DwPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        // mapping to view coordinate
        evt.m_viewPoint = m_view->mapFromDevice(evt.m_devicePoint);
        // reserver last mouse event
        reserveLastMouseEvent(evt);
        // dispath to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);

        // produce a pressed event
        OnLButtonDown(uMsg, wParam, lParam, bHandled);
    }
    return 0;
}

// Mouse Left Button Up
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    if (m_view)
    {
        tagPOINT p = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        ::ClientToScreen(m_hWnd, &p);

        DwMouseUpEvent evt;
        // button
        evt.m_button = Dw::LeftButton;
        // screen coordinate
        evt.m_screenPoint = DwPoint(p.x, p.y);
        // device coordinate, NB! MUST BE CLIENTRECT
        evt.m_devicePoint = DwPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        // mapping to view coordinate
        evt.m_viewPoint = m_view->mapFromDevice(evt.m_devicePoint);
        // reserver last mouse event
        reserveLastMouseEvent(evt);
        // dispath to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);
    }
    ::ReleaseCapture();
    return 0;
}

//
//////////////////////////////////////////////////////////////////////////
// Mouse Left Button
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnNcLButtonDown(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    m_ncLeftMouseDown = true;
    if (m_view)
    {
        CPoint p(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        ::ScreenToClient(m_hWnd, &p);

        DwMouseDownEvent evt;
        // nc
        evt.m_isNC = true;
        // nc param
        evt.m_nc = wParam;
        // button
        evt.m_button = Dw::LeftButton;
        // screen coordinate
        evt.m_screenPoint = DwPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        // device coordinate, NB! MUST BE CLIENTRECT
        evt.m_devicePoint = DwPoint(p.x, p.y);
        // mapping to view coordinate
        evt.m_viewPoint = m_view->mapFromDevice(evt.m_devicePoint);
        // reserver last mouse event
        reserveLastMouseEvent(evt);
        // dispath to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);
    }
    bHandled = False;
    return 0;
}

// Db Clicked
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnNcLButtonDBClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    DW_UNUSED(uMsg);
    DW_UNUSED(bHandled);
    if (m_view)
    {
        CPoint p(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        ::ScreenToClient(m_hWnd, &p);

        DwMouseDbClickEvent evt;
        // nc
        evt.m_isNC = true;
        // nc param
        evt.m_nc = wParam;
        // button
        evt.m_button = Dw::LeftButton;
        // screen coordinate
        evt.m_screenPoint = DwPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        // device coordinate, NB! MUST BE CLIENTRECT
        evt.m_devicePoint = DwPoint(p.x, p.y);
        // mapping to view coordinate
        evt.m_viewPoint = m_view->mapFromDevice(evt.m_devicePoint);
        // reserver last mouse event
        reserveLastMouseEvent(evt);
        // dispath to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);

        // produce a pressed event
        OnNcLButtonDown(uMsg, wParam, lParam, bHandled);
    }
    bHandled = False;
    return 1;
}

// Mouse Left Button Up
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnNcLButtonUp(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if (m_view)
    {
        CPoint p(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        ::ScreenToClient(m_hWnd, &p);

        DwMouseUpEvent evt;
        // nc
        evt.m_isNC = true;
        // nc param
        evt.m_nc = wParam;
        // button
        evt.m_button = Dw::LeftButton;
        // screen coordinate
        evt.m_screenPoint = DwPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        // device coordinate, NB! MUST BE CLIENTRECT
        evt.m_devicePoint = DwPoint(p.x, p.y);
        // mapping to view coordinate
        evt.m_viewPoint = m_view->mapFromDevice(evt.m_devicePoint);
        // reserver last mouse event
        reserveLastMouseEvent(evt);
        // dispath to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);
    }
    bHandled = False;
    return 1;
}

// Mouse Right Button
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnRButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    ::SetCapture(m_hWnd);
    if (m_view)
    {
        tagPOINT p = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        ::ClientToScreen(m_hWnd, &p);

        DwMouseDownEvent evt;
        // button
        evt.m_button = Dw::RightButton;
        // screen coordinate
        evt.m_screenPoint = DwPoint(p.x, p.y);
        // device coordinate, NB! MUST BE CLIENTRECT
        evt.m_devicePoint = DwPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        // mapping to view coordinate
        evt.m_viewPoint = m_view->mapFromDevice(evt.m_devicePoint);
        // reserver last mouse event
        reserveLastMouseEvent(evt);
        // dispath to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);
    }
    return 0;
}

// Mouse Right Button Double Click
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnRButtonDBClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    DW_UNUSED(uMsg);
    DW_UNUSED(bHandled);
    DW_UNUSED(wParam);
    if (m_view)
    {
        tagPOINT p = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        ::ClientToScreen(m_hWnd, &p);

        DwMouseDbClickEvent evt;
        // button
        evt.m_button = Dw::RightButton;
        // screen coordinate
        evt.m_screenPoint = DwPoint(p.x, p.y);
        // device coordinate, NB! MUST BE CLIENTRECT
        evt.m_devicePoint = DwPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        // mapping to view coordinate
        evt.m_viewPoint = m_view->mapFromDevice(evt.m_devicePoint);
        // reserver last mouse event
        reserveLastMouseEvent(evt);
        // dispath to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);

        // produce a pressed event
        OnRButtonDown(uMsg, wParam, lParam, bHandled);
    }
    return 0;
}

// Mouse Right Button Up
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnRButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    if (m_view)
    {
        tagPOINT p = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        ::ClientToScreen(m_hWnd, &p);

        DwMouseUpEvent evt;
        // button
        evt.m_button = Dw::RightButton;
        // screen coordinate
        evt.m_screenPoint = DwPoint(p.x, p.y);
        // device coordinate, NB! MUST BE CLIENTRECT
        evt.m_devicePoint = DwPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        // mapping to view coordinate
        evt.m_viewPoint = m_view->mapFromDevice(evt.m_devicePoint);
        // reserver last mouse event
        reserveLastMouseEvent(evt);
        // dispath to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);
    }
    ::ReleaseCapture();
    return 0;
}

// Mouse Right Button
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnNcRButtonDown(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    m_ncRightMouseDown = true;

    if (m_view)
    {
        CPoint p(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        ::ScreenToClient(m_hWnd, &p);

        DwMouseDownEvent evt;
        // is nc
        evt.m_isNC = true;
        // nc param
        evt.m_nc = wParam;
        // button
        evt.m_button = Dw::RightButton;
        // screen coordinate
        evt.m_screenPoint = DwPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        // device coordinate, NB! MUST BE CLIENTRECT
        evt.m_devicePoint = DwPoint(p.x, p.y);
        // mapping to view coordinate
        evt.m_viewPoint = m_view->mapFromDevice(evt.m_devicePoint);
        // reserver last mouse event
        reserveLastMouseEvent(evt);
        // dispath to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);
    }
    bHandled = False;
    return 0;
}

// Mouse Right Button Double Click
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnNcRButtonDBClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    DW_UNUSED(uMsg);
    DW_UNUSED(bHandled);
    if (m_view)
    {
        CPoint p(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        ::ScreenToClient(m_hWnd, &p);

        DwMouseDbClickEvent evt;
        // is nc
        evt.m_isNC = true;
        // nc param
        evt.m_nc = wParam;
        // button
        evt.m_button = Dw::RightButton;
        // screen coordinate
        evt.m_screenPoint = DwPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        // device coordinate, NB! MUST BE CLIENTRECT
        evt.m_devicePoint = DwPoint(p.x, p.y);
        // mapping to view coordinate
        evt.m_viewPoint = m_view->mapFromDevice(evt.m_devicePoint);
        // reserver last mouse event
        reserveLastMouseEvent(evt);
        // dispath to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);

        // produce a pressed event
        OnNcRButtonDown(uMsg, wParam, lParam, bHandled);
    }
    bHandled = False;
    return 1;
}

// Mouse Right Button Up
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnNcRButtonUp(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if (m_view)
    {
        CPoint p(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        ::ScreenToClient(m_hWnd, &p);

        DwMouseUpEvent evt;
        // is nc
        evt.m_isNC = true;
        // nc param
        evt.m_nc = wParam;
        // button
        evt.m_button = Dw::RightButton;
        // screen coordinate
        evt.m_screenPoint = DwPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        // device coordinate, NB! MUST BE CLIENTRECT
        evt.m_devicePoint = DwPoint(p.x, p.y);
        // mapping to view coordinate
        evt.m_viewPoint = m_view->mapFromDevice(evt.m_devicePoint);
        // reserver last mouse event
        reserveLastMouseEvent(evt);
        // dispath to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);
    }
    bHandled = False;
    return 0;
}

// Mouse Middle Button
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnMButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    ::SetCapture(m_hWnd);
    if (m_view)
    {
        tagPOINT p = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        ::ClientToScreen(m_hWnd, &p);

        DwMouseDownEvent evt;
        // button
        evt.m_button = Dw::MiddleButton;
        // screen coordinate
        evt.m_screenPoint = DwPoint(p.x, p.y);
        // device coordinate, NB! MUST BE CLIENTRECT
        evt.m_devicePoint = DwPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        // mapping to view coordinate
        evt.m_viewPoint = m_view->mapFromDevice(evt.m_devicePoint);
        // reserver last mouse event
        reserveLastMouseEvent(evt);
        // dispath to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);
    }
    return 0;
}

// Mouse Middle Button Double Clicked
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnMButtonDBClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    DW_UNUSED(uMsg);
    DW_UNUSED(wParam);
    DW_UNUSED(bHandled);
    if (m_view)
    {
        tagPOINT p = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        ::ClientToScreen(m_hWnd, &p);

        DwMouseDbClickEvent evt;
        // button
        evt.m_button = Dw::MiddleButton;
        // screen coordinate
        evt.m_screenPoint = DwPoint(p.x, p.y);
        // device coordinate, NB! MUST BE CLIENTRECT
        evt.m_devicePoint = DwPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        // mapping to view coordinate
        evt.m_viewPoint = m_view->mapFromDevice(evt.m_devicePoint);
        // reserver last mouse event
        reserveLastMouseEvent(evt);
        // dispath to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);

        // produce a pressed event
        OnMButtonDown(uMsg, wParam, lParam, bHandled);
    }
    return 0;
}

// Mouse Middle Button Up
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnMButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    if (m_view)
    {
        tagPOINT p = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        ::ClientToScreen(m_hWnd, &p);

        DwMouseUpEvent evt;
        // button
        evt.m_button = Dw::MiddleButton;
        // screen coordinate
        evt.m_screenPoint = DwPoint(p.x, p.y);
        // device coordinate, NB! MUST BE CLIENTRECT
        evt.m_devicePoint = DwPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        // mapping to view coordinate
        evt.m_viewPoint = m_view->mapFromDevice(evt.m_devicePoint);
        // reserver last mouse event
        reserveLastMouseEvent(evt);
        // dispath to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);
    }
    ::ReleaseCapture();
    return 0;
}

// Mouse Move
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    // dispatch a nc to client event
    if (m_ncMouseMoved)
    {
        m_ncMouseMoved = false;
        if (m_view)
        {
            DwMouseNcToClientEvent evt;
            // dispath to view
            dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);
        }
    }

    if (m_view)
    {
        tagPOINT p = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        ::ClientToScreen(m_hWnd, &p);

        DwMouseMoveEvent evt;
        // screen coordinate
        evt.m_screenPoint = DwPoint(p.x, p.y);
        // device coordinate , NB! MUST BE CLIENTRECT
        evt.m_devicePoint = DwPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        // mapping to view coordinate
        evt.m_viewPoint = m_view->mapFromDevice(evt.m_devicePoint);
        // fetch delta
        if(m_lastMouseEvent) evt.m_delta = evt.m_viewPoint - m_lastMouseEvent->m_viewPoint;
        // reserver last mouse event
        reserveLastMouseEvent(evt);
        // dispath to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);
    }

    return 0;
}

// On Nc Mouse Move
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnNcMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    m_ncMouseMoved = true;

    if (m_ncLeftMouseDown)
    {
        m_ncLeftMouseDown = false;
        OnNcLButtonUp(uMsg, wParam, lParam, bHandled);
        return 0;
    }

    if (m_ncRightMouseDown)
    {
        m_ncRightMouseDown = false;
        OnNcRButtonUp(uMsg, wParam, lParam, bHandled);
        return 0;
    }

    if (m_view)
    {
        CPoint p(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        ::ScreenToClient(m_hWnd, &p);

        DwMouseMoveEvent evt;
        // is the nc msg
        evt.m_isNC = true;
        // nc param
        evt.m_nc = wParam;
        // screen coordinate
        evt.m_screenPoint = DwPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        // device coordinate , NB! MUST BE CLIENTRECT
        evt.m_devicePoint = DwPoint(p.x, p.y);
        // mapping to view coordinate
        evt.m_viewPoint = m_view->mapFromDevice(evt.m_devicePoint);
        // fetch delta
        if(m_lastMouseEvent) evt.m_delta = evt.m_viewPoint - m_lastMouseEvent->m_viewPoint;
        // reserver last mouse event
        reserveLastMouseEvent(evt);
        // dispath to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);
    }
    bHandled = False;
    return 0;
}

// Mouse Wheel
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnMouseWheel(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    if (m_view)
    {
        tagPOINT p = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        ::ClientToScreen(m_hWnd, &p);

        DwMouseWheelEvent evt;
        // virtual key state
        evt.m_fwKeys = (Dw::MouseVirtualKeyState)(GET_KEYSTATE_WPARAM(wParam));
        // screen coordinate
        evt.m_screenPoint = DwPoint(p.x, p.y);
        // device coordinate, NB! MUST BE CLIENTRECT
        evt.m_devicePoint = DwPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        // mapping to view coordinate
        evt.m_viewPoint = m_view->mapFromDevice(evt.m_devicePoint);
        // fetch delta
        evt.m_delta = GET_WHEEL_DELTA_WPARAM(wParam);
        // reserver last mouse event
        reserveLastMouseEvent(evt);
        // dispath to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);
    }
    return 0;
}

// OnMouseHover
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnMouseHover(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    //TODO: xx
    return 0;
}

// OnMouseLeave
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnMouseLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    //TODO: xx
    return 0;
}

// Drop Files
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnDropFiles(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    HDROP hDrop = (HDROP)(wParam);
#if DW_ENABLE_EVENT_DRAGDROP
    if (m_view)
    {
        UINT dragNum = ::DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
        tagPOINT point;
        ::DragQueryPoint(hDrop, &point);

        tagPOINT p = point;
        ::ClientToScreen(m_hWnd, &p);

        DwDragDropEvent dropEvent;
        // screen coordinate
        dropEvent.m_screenPoint = DwPoint(p.x, p.y);
        dropEvent.m_devicePoint = DwPoint(point.x, point.y);
        dropEvent.m_viewPoint = m_view->mapFromDevice(dropEvent.m_devicePoint);
        dropEvent.m_mimeData.setFormat("urls", "files");

        for (UINT i = 0; i < dragNum; ++i)
        {
            WCHAR fileName[DW_GUI_MAX_PATH] = { 0 };
            ::DragQueryFile(hDrop, i, fileName, DW_GUI_MAX_PATH);
            dropEvent.m_mimeData.appendUrl(DwUrl(DwString(fileName)));
        }
        // dispath to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &dropEvent);
    }
#endif
    DragFinish(hDrop);
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnCaptureChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnGetMiniMaxInfo(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    RECT rtWorkArea = {0};
    ::SystemParametersInfo(SPI_GETWORKAREA, 0, &rtWorkArea, 0);
    MINMAXINFO* pInfo = (MINMAXINFO*)lParam;
    pInfo->ptMaxSize.x = rtWorkArea.right - rtWorkArea.left + 4;
    pInfo->ptMaxSize.y = rtWorkArea.bottom - rtWorkArea.top + 4;
    pInfo->ptMaxPosition.x = rtWorkArea.left - 2;
    pInfo->ptMaxPosition.y = rtWorkArea.top - 2;
    pInfo->ptMaxTrackSize = pInfo->ptMaxSize;

    return 0;
}

//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    bHandled = TRUE;
    if (m_view)
    {
        DwEraseBkgndEvent evt;
        evt.m_window = m_window;
        // dispath to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);
    }
    return 1;
}

// TODO: use paint event
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    CRect r;
    BOOL ok = ::GetUpdateRect(m_hWnd, &r, TRUE);
    PAINTSTRUCT ps;
    ::BeginPaint(m_hWnd, &ps);
    if (ok)
    {
        ps.rcPaint.left = r.left;
        ps.rcPaint.top = r.top;
        ps.rcPaint.right = r.right;
        ps.rcPaint.bottom = r.bottom;
    }
    update(&ps);
    ::EndPaint(m_hWnd, &ps);
    bHandled = TRUE;
    return 0;
}

// TODO: use paint event
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnNcPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
    return 0;
    CRect r;
    HRGN h = (HRGN)wParam;
    HRESULT hr = ::GetRgnBox(h, &r);
    PAINTSTRUCT ps;
    BeginPaint(m_hWnd, &ps);
    if (SUCCEEDED(hr))
    {
        ps.rcPaint.left = r.left;
        ps.rcPaint.top = r.top;
        ps.rcPaint.right = r.right;
        ps.rcPaint.bottom = r.bottom;
    }
    update(&ps);
    ::EndPaint(m_hWnd, &ps);
    bHandled = TRUE;
    return 0;
}

// Key Down
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if (m_view)
    {
        DwKeyDownEvent evt;
        // virtual key
        evt.m_virtualKey = wParam;
        // all kinds of code
        evt.m_param = lParam;
        // dispath to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);
    }
    bHandled = TRUE;
    return 0;
}

// Key Up
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnKeyUp(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if (m_view)
    {
        DwKeyUpEvent evt;
        // virtual key
        evt.m_virtualKey = wParam;
        // all kinds of code
        evt.m_param = lParam;
        // dispath to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);
    }
    bHandled = TRUE;
    return 0;
}

// Char
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnChar(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if (m_view)
    {
        DwCharEvent evt;
        // character code
        evt.m_characterCode = wParam;
        // all kinds of special code
        evt.m_param = lParam;
        // dispath to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);
    }
    bHandled = TRUE;
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnShowWindow(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    DW_UNUSED(lParam);
    DW_UNUSED(wParam);
    bHandled = FALSE;
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnActive(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    DW_UNUSED(lParam);
    DW_UNUSED(wParam);
    bHandled = FALSE;
    if (m_view)
    {
        DwWindowActiveEvent evt;
        // assign windows standard message params
        evt.assignWin((int)m_hWnd, uMsg, wParam, lParam);
        // if the actived
        evt.m_actived = (LOWORD(wParam) != WA_INACTIVE) ? true : false;
        // dispath to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);

        // may be can use the mark to do with handled
        if (evt.isAccepted())
        {
            bHandled = TRUE;
            return evt.m_result;
        }
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnCommand(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    DW_UNUSED(lParam);
    DW_UNUSED(wParam);
    bHandled = FALSE;
    if (m_view)
    {
#if DW_ENABLE_EVENT_WIN_COMMAND
        DwWinCommandEvent evt;
        evt.m_hWnd = m_hWnd;
        evt.m_wparam = wParam;
        evt.m_lparam = lParam;

        // send event to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);

        if (evt.isAccepted())
        {
            bHandled = TRUE;
        }
        else
        {
            bHandled = FALSE;
        }

        return evt.m_lResult;
#endif
    }

    return 0;
}

//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnSysCommand(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    DW_UNUSED(lParam);
    DW_UNUSED(wParam);
    bHandled = FALSE;
    if (m_view)
    {
#if DW_ENABLE_EVENT_WIN_COMMAND
        DwWinSysCommandEvent evt;
        evt.m_hWnd = m_hWnd;
        evt.m_wparam = wParam;
        evt.m_lparam = lParam;

        // send event to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);

        if (evt.isAccepted())
        {
            bHandled = TRUE;
        }
        else
        {
            bHandled = FALSE;
        }

        return evt.m_lResult;
#endif
    }

    return 0;
}

extern IAccessible *dw_createWindowsAccessible(DwAccessibleInterface *object);
//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnAccGetObject(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = FALSE;

    // Ignoring all requests while starting up
    if ((LONG)lParam != OBJID_CLIENT || !m_view)
    {
        return 0;
    }

    DwGraphicsItem *widget = m_view->windowItem();

    typedef LRESULT (WINAPI *PtrLresultFromObject)(REFIID, WPARAM, LPUNKNOWN);
    static PtrLresultFromObject ptrLresultFromObject = 0;
    static bool oleaccChecked = false;

    LRESULT res = 0;
    if (!oleaccChecked) {
        oleaccChecked = true;

        HMODULE hdll = ::LoadLibraryA("oleacc.dll");
        if (hdll)
        {
            ptrLresultFromObject = (PtrLresultFromObject)GetProcAddress(hdll, "LresultFromObject");
        }
    }
    if (ptrLresultFromObject) {
        DwAccessibleInterface *acc = DwAccessible::queryAccessibleInterface(widget);
        if (!acc) {
            return 0;
        }

        // and get an instance of the IAccessibile implementation
        IAccessible *iface = dw_createWindowsAccessible(acc);
        res = ptrLresultFromObject(IID_IAccessible, wParam, iface);  // ref == 2
        iface->Release(); // the client will release the object again, and then it will destroy itself

        if (res > 0)
        {
            bHandled = TRUE;
            return (res);
        }
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnAccGetText(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = FALSE;
    if (!m_view)
    {
        return 0;
    }

    DwGraphicsItem *widget = m_view->windowItem();

    if (widget) {
        int ret = 0;
        DwAccessibleInterface *acc = DwAccessible::queryAccessibleInterface(widget);
        if (acc) {
            DwString text = acc->text(DwAccessible::Name, 0);
            if (text.isEmpty())
                text = widget->name();
            ret = dwMin<int>(wParam - 1, text.size());
            text.resize(ret);
            memcpy((void *)lParam, text.constString(), (text.size() + 1) * sizeof(wchar_t));
            delete acc;
        }
        if (!ret) {
            bHandled = TRUE;
            return (ret);
        }
    }

    return 0;
}

//------------------------------------------------------------------------------
/**
*/
LRESULT DwWindowAdapter::OnShow(int command)
{
    if (m_view)
    {
        DwWinOnShowEvent evt;
        // show command
        evt.m_showCommand = (Dw::ShowCommand)command;
        // fill the window pointer
        evt.m_window = m_window;
        // fill the windows handle
        evt.m_winId = (int)m_hWnd;
        // send event to view
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);
    }

    // update the propertys
    m_translucentBackground = ((DWORD)::GetWindowLong(m_hWnd, GWL_EXSTYLE) & WS_EX_LAYERED) ? true : false;
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
void DwWindowAdapter::update(PAINTSTRUCT *ps/* = NULL*/)
{
    DwRect rect;
    if (ps)
    {
        rect.setRect(ps->rcPaint.left, ps->rcPaint.top,
                     ps->rcPaint.right - ps->rcPaint.left,
                     ps->rcPaint.bottom - ps->rcPaint.top);
    }
    else
    {
        rect.setRect(0, 0, m_canvas.size().width(), m_canvas.size().height());
    }

    // send event, and draw
    if (m_view)
    {
        DwPaintEvent evt;
        evt.m_deviceRect = rect;
        evt.m_window = m_window;
        dwGraphicsEventDispatch()->dispatchEvent(m_view, &evt);
    }

    // debug
    /*if (m_window )
    {
        DwPainter painter(&m_canvas);
        m_canvas.beginePaint(rect);
        m_window->paintEvent(painter, rect);
        m_canvas.endPaint();
    }*/
}

DwCanvas *DwWindowAdapter::canvas()
{
    return &m_canvas;
}