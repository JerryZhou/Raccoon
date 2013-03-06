#include "stable.h"

#include <dwcore/dwarraylist.h>
#include <dwcore/dwwin32api.h>

#include <dwgraphics/dwimage.h>

#include "graphicsview/dwgraphicsview.h"
#include "graphicsview/dwgraphicsevent.h"
#include "graphicsview/dwgraphicseventdispatch.h"
#include "graphicsview/dwdummys.h"
#include "dwapplication.h"
#include "dwwindow.h"
#include "dwcursor.h"
#include "dwgui/dwringlogcase.h"
#include "dwwindowadapter.h"
//------------------------------------------------------------------------------
struct WindowContainer 
{
    DwMap<WinId, DwWindow*> winIdToWindow;

    DwWindow* windowFromWinId(WinId id)
    {
        return winIdToWindow.value(id, NULL);
    }

    void addWindow(DwWindow *window)
    {
        WinId id = (WinId)window->winId();
        winIdToWindow.insert(id, window);
    }

    void removeWindow(DwWindow *window)
    {
        WinId id = (WinId)window->winId();
        winIdToWindow.remove(id);
    }
};
DW_GLOBAL_STATIC(WindowContainer, _dw_WindowContainer);


//------------------------------------------------------------------------------
class DwWindowImpl;

//------------------------------------------------------------------------------
typedef CWinTraits< WS_POPUP | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX, WS_EX_LAYERED | WS_EX_APPWINDOW >	CMainWindowTraits;
class DwWindowImpl :
    public CFrameWindowImpl<DwWindowImpl, CWindow, CMainWindowTraits>
{
public:
    DECLARE_WND_CLASS_EX(_T("DUI_HOST_WINDOW"), CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, NULL_BRUSH);

    BEGIN_MSG_MAP(DwWindowImpl)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	MESSAGE_HANDLER(WM_NCDESTROY, OnNcDestroy)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
    DW_MESSAGE_HANDLER(OnWinMsg)
    END_MSG_MAP()

    DwWindowImpl(DwWindow *window)
        : m_window(window)
        , m_view(NULL)
    {
        ;
    }

    ~DwWindowImpl()
    {
        ;
    }

    // Will Dispatch All The Window Msg
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnWinMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        return m_adapter.dispatchWinMsg(uMsg, wParam, lParam, bHandled);
    }

    // Window Create
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        sigCreated.emit();
        return 0;
    }

    // Window Closed
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        sigClosed.emit();
        return 1;
    }

    // Window Destroy
    //////////////////////////////////////////////////////////////////////////
    LRESULT OnNcDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
    {
        bHandled = FALSE;

        sigDestroyed.emit();

        return 0;
    }

	// Window resize
	//////////////////////////////////////////////////////////////////////////
	LRESULT OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		CRect rc;
		GetClientRect(rc);

		if(wParam != SIZE_MAXIMIZED)
		{
			rc.DeflateRect(6, 6, 6, 6);						
		}

		HRGN hrgn = CreateRectRgn(rc.left, rc.top, rc.right, rc.bottom);
		DwWinAPI::dwDwmEnableBlurBehindWindow(m_hWnd, hrgn);
        DeleteObject(hrgn);

		bHandled = FALSE;
		return 0;
	}

    // Final Delete The Window
    //////////////////////////////////////////////////////////////////////////
    virtual void OnFinalMessage(HWND /*hWnd*/)
    {
        delete this;
    }

    void openAdater(DwWindow *win, DwGraphicsView *view)
    {
        m_adapter.open(win, view);
    }

    DwWindowAdapter &adapter()
    {
        return m_adapter;
    }

public:
    sigslot::signal0<> sigCreated;
    sigslot::signal0<> sigClosed;
    sigslot::signal0<> sigDestroyed;

    DwWindow *m_window;
    DwGraphicsView* m_view;
    DwString m_caption;
    DwWindowAdapter m_adapter;
};

DW_IMPLEMENT_CLASS(DwWindow, 'WINW', DwRttiObject);
//------------------------------------------------------------------------------
/**
*/
DwWindow::DwWindow()
{
    m_window = new DwWindowImpl(this);
    m_window->sigCreated.connect(this, &DwWindow::on_window_created);
    m_window->sigClosed.connect(this, &DwWindow::on_window_closed);
    m_window->sigDestroyed.connect(this, &DwWindow::on_window_destroyed);
}

//------------------------------------------------------------------------------
/**
*/
DwWindow::~DwWindow()
{
    if (m_window)
    {
        if (m_window->IsWindow())
        {
            DwWindowImpl *window = m_window;
            m_window = NULL;
            window->DestroyWindow();
        }
    }
}

//------------------------------------------------------------------------------
/**
    NEED KNOW :
    DwRect is [left, right] [top, bottom]
    CRect is [left, right) [ top, bottom)
*/
void DwWindow::show(int nCmdShow/* = Dw::SW_ShowNormal*/, const DwRect& r/* = DwRect(0, 0, 640, 480)*/)
{
    // create window
    if (!m_window->IsWindow())
    {
        m_window->Create(NULL);
        // If your application must set the cursor while it is in a window,
        // make sure the class cursor for the specified window's class is set to NULL.
        ::SetClassLong(m_window->m_hWnd,    // window handle
                       GCL_HCURSOR,      // change cursor
                       NULL);   // new cursor
        DwCursor::setCursor(Dw::ArrowCursor);
        // Accept Drop Files
        m_window->DragAcceptFiles(TRUE);
        // Open the adapter
        m_window->openAdater(this, m_window->m_view);
        // Set The Rect
        setRect(r);
        // add to container
        _dw_WindowContainer()->addWindow(this);
    }

    // send show event
    m_window->adapter().OnShow(nCmdShow);

    // show window
    DW_ASSERT(m_window && m_window->IsWindow());
    m_window->ShowWindow(nCmdShow);
}

//------------------------------------------------------------------------------
/**
*/
DwSize DwWindow::size() const
{
    CRect rc(0, 0, 0, 0);
    if(m_window) m_window->GetClientRect(&rc);
    return DwSize(rc.Width(), rc.Height());
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsView* DwWindow::view() const
{
    if(m_window) return m_window->m_view;
    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
void DwWindow::setView(DwGraphicsView* v)
{
    if(m_window) m_window->m_view = v;
}

//------------------------------------------------------------------------------
/**
*/
int DwWindow::width() const
{
    return size().width();
}

//------------------------------------------------------------------------------
/**
*/
int DwWindow::height() const
{
    return size().height();
}

//------------------------------------------------------------------------------
/**
*/
DwString DwWindow::winid() const
{
    return "w" + DwString::number((int)m_window, 16);
}

//------------------------------------------------------------------------------
/**
*/
void DwWindow::on_window_created()
{
    dwApp()->addTopLevelWindow(this);

    sigCreated.emit();
}

//------------------------------------------------------------------------------
/**
*/
void DwWindow::on_window_closed()
{
    sigClosed.emit();

    dwApp()->removeTopLevelWindow(this);

    _dw_WindowContainer()->removeWindow(this);
}

//------------------------------------------------------------------------------
/**
*/
void DwWindow::on_window_destroyed()
{
    sigDestroyed.emit();

    if (m_window)
    {
        m_window->m_view = NULL;

        // will delete m_window in onFinalMessage()
        m_window = NULL;
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwWindow::update()
{
    if(m_window) m_window->RedrawWindow();
}

//------------------------------------------------------------------------------
/**
*/
void DwWindow::close()
{
    if(m_window) m_window->DestroyWindow();
}

//------------------------------------------------------------------------------
/**
*/
void DwWindow::move(int x, int y)
{
    if (x || y)
    {
        tagRECT rect;
        if(m_window) m_window->GetWindowRect(&rect);
        rect.left += x;
        rect.right += x;
        rect.top += y;
        rect.bottom += y;
        if(m_window) m_window->SetWindowPos(NULL, &rect, SWP_NOSIZE);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwWindow::setCaption(const DwString& s)
{
    if (m_window)
    {
        m_window->m_caption = s;
        m_window->SetWindowText(s.constString());
    }
}

//------------------------------------------------------------------------------
/**
*/
DwString DwWindow::caption() const
{
    if (m_window)
    {
        return m_window->m_caption;
    }
    return DwString();
}

//------------------------------------------------------------------------------
/**
*/
void DwWindow::setIcon(int icon)
{
    if (m_window)
    {
        HINSTANCE hInstance = ::GetModuleHandle(0);
        HICON hicon = ::LoadIcon(hInstance, MAKEINTRESOURCE(icon));
        m_window->SetIcon(hicon);
    }
}

//------------------------------------------------------------------------------
/**
    NEED KNOW :
    DwRect is [left, right] [top, bottom]
    CRect is [left, right) [ top, bottom)
*/
void DwWindow::setSize(const DwSize &s)
{
    if (m_window)
    {
        tagRECT rect;
        m_window->GetWindowRect(&rect);
        tagRECT newRect;
        newRect.left = (rect.left + rect.right) / 2 - s.width() / 2;
        newRect.right = (rect.left + rect.right) / 2 + s.width() / 2 + 1;
        newRect.top = (rect.top + rect.bottom) / 2 - s.height() / 2;
        newRect.bottom = (rect.top + rect.bottom) / 2 + s.height() / 2 + 1;
        m_window->SetWindowPos(NULL, &newRect, 0);
    }
}

//------------------------------------------------------------------------------
/**
    NEED KNOW :
    DwRect is [left, right] [top, bottom]
    CRect is [left, right) [ top, bottom)
*/
void DwWindow::setRect(const DwRect &r)
{
    if (m_window)
    {
        tagRECT rect;
        m_window->GetWindowRect(&rect);

        tagRECT newRect = { r.left(), r.top(), r.right() + 1, r.bottom() + 1};

        if (rect.left != newRect.left || rect.top != newRect.top
                || rect.right != newRect.right || rect.bottom != newRect.bottom)
        {
            m_window->SetWindowPos(NULL, &newRect, 0);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
WinId DwWindow::winId() const
{
    if (m_window)
    {
        return (WinId)m_window->m_hWnd;
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
void DwWindow::centerWindow(WinId windId/* = 0*/)
{
    if (m_window && m_window->IsWindow())
    {
        m_window->CenterWindow(HWND(windId));
    }
}

//------------------------------------------------------------------------------
/**
*/
bool DwWindow::isWindowEnabled() const
{
    if (m_window && m_window->IsWindow())
    {
        return !!(m_window->IsWindowEnabled());
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void DwWindow::enableWindow(bool b)
{
    if (m_window && m_window->IsWindow())
    {
        m_window->EnableWindow(b);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwWindow::setParentWindow(DwWindow *win)
{
    if (m_window && m_window->IsWindow())
    {
        if (win)
        {
            BOOL b = m_window->ModifyStyle(WS_POPUP, WS_CHILD);
            if (b)
            {
                HWND pre = m_window->SetParent((HWND)(win->winId()));
                if (!pre)
                {
                    DWORD err = GetLastError();
                    err;
                }
                m_window->SetActiveWindow();
                m_window->ShowWindow(SW_SHOW);
            }
        }
        else
        {
            m_window->ModifyStyle(WS_CHILD, WS_POPUP);
            m_window->SetParent(0);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwWindow::setActive()
{
    if (m_window && m_window->IsWindow())
    {
        m_window->SetActiveWindow();
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwWindow::setFocus()
{
    if (m_window && m_window->IsWindow())
    {
        m_window->SetFocus();
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwWindow::paintEvent(DwPainter &, const DwRectF &)
{
    ;
};

//------------------------------------------------------------------------------
/**
*/
DwWindowAdapter * DwWindow::adapter()const
{
    if (m_window)
    {
        return &m_window->adapter();
    }
    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
DwWindow* DwWindow::windowFromPoint(const DwPoint &p)
{
    return windowFromPoint(p.x(), p.y());
}

//------------------------------------------------------------------------------
/**
*/
DwWindow* DwWindow::windowFromPoint(int x, int y)
{
    POINT p={x, y};
    HWND id = WindowFromPoint(p);
    return windowFromWinId(id);
}

//------------------------------------------------------------------------------
/**
*/
DwWindow* DwWindow::windowFromWinId(WinId id)
{
    WinId desktop = GetDesktopWindow();
    DwWindow* window = NULL;
    while (!window && id && id != desktop)
    {
        window = _dw_WindowContainer()->windowFromWinId(id);
        if (!window)
        {
            id = GetParent(id);
        }
    }
    return window;
}