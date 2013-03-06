#include "stable.h"
#include "dwtrayicon.h"
#include "dwgui/dwresourceinterface.h"
#include "dwgraphics/dwninepatch.h"
#include "dwgui/dwpropertyanimation.h"
#include "dwgui/dwgraphicsevent.h"
#include "dwgui/dwappeventdispatch.h"
#include "dwgui/dwcursor.h"

DW_IMPLEMENT_CLASS(DwTrayIcon, 'TION', DwGraphicsItem);

//------------------------------------------------------------------------------
#ifndef DW_SAFE_DESTROY_ICON
#define DW_SAFE_DESTROY_ICON(h) { if(h) DestroyIcon(h); h = NULL; }
#endif

const int   KTrayIconId = 1;
const int   KTrayIconMsg = (WM_APP + 10);
//------------------------------------------------------------------------------
/**
*/
LRESULT _trayicon_WindowProc(HWND hWnd, UINT uMsg, LPARAM lParam, WPARAM wParam)
{
    DwTrayIcon * d = (DwTrayIcon *)::GetWindowLongPtr(hWnd, GWLP_USERDATA);

    HRESULT result = 0;
    if (d && d->onWinEvent(uMsg, wParam, lParam, &result))
    {
        return result;
    }

    return ::DefWindowProc(hWnd, uMsg, lParam, wParam);
}

//------------------------------------------------------------------------------
/**
*/
bool _trayicon_Shell_NotifyIcon(DWORD dwMessage, PNOTIFYICONDATAW lpData)
{
    return !!(::Shell_NotifyIconW(dwMessage, lpData));
}

//------------------------------------------------------------------------------
/**
*/
DwTrayIcon::DwTrayIcon(DwGraphicsItem *parent/* = NULL*/)
: DwGraphicsItem(parent),
m_winId(0), 
m_iconIdx(0), 
m_visible(0), 
m_contents(0),
m_anis(NULL)
{
    setAcceptedMouseButtons(Dw::LeftButton|Dw::RightButton|Dw::MiddleButton);
}

//------------------------------------------------------------------------------
/**
*/
DwTrayIcon::~DwTrayIcon()
{
    DW_SAFE_DELETE(m_anis);
    hide();
    if (m_winId)
    {
        DestroyWindow(m_winId);
        m_winId = 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwTrayIcon::setTips( const DwString& s )
{
    if (m_tips != s)
    {
        m_tips = s;
        m_contents |= ETips;

        update();
    }
}

//------------------------------------------------------------------------------
/**
*/
DwString DwTrayIcon::tips() const
{
    return m_tips;
}

//------------------------------------------------------------------------------
/**
*/
static HICON _dw_icon_of(const DwUrl &url, int idx)
{
    DwImage img = DW_PIXMAP(url);
    DwNinePatch::NinePatchInfo info = DwNinePatch::ninePatchInfo(img, idx);
    return img.toWinHICON(info.srcRect);
}
#define _dw_tray_icon _dw_icon_of(m_icon, m_iconIdx)

//------------------------------------------------------------------------------
/**
*/
void DwTrayIcon::setIcon( const DwUrl& s, int idx /*= 0*/ )
{
    if (m_icon != s || m_iconIdx != idx)
    {
        m_icon = s;
        m_iconIdx = idx;
        m_contents |= EIcon;

        update();
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwTrayIcon::setIconIdx(int idx)
{
    if (m_iconIdx != idx)
    {
        m_iconIdx = idx;
        m_contents |= EIcon;
    }

    update();
}

//------------------------------------------------------------------------------
/**
*/
const DwUrl& DwTrayIcon::Icon() const
{
    return m_icon;
}

//------------------------------------------------------------------------------
/**
*/
int DwTrayIcon::IconIdx() const
{
    return m_iconIdx;
}

//------------------------------------------------------------------------------
/**
*/
WinId DwTrayIcon::winId() const
{
    if (m_winId == 0)
    {
        const_cast<DwTrayIcon*>(this)->m_winId = ::CreateWindowEx(0,_T("STATIC"),NULL,0, 0,0,0,0, NULL,NULL,NULL,NULL);
        ::SetWindowLongPtr(const_cast<DwTrayIcon*>(this)->m_winId, GWLP_WNDPROC, (LONG_PTR) _trayicon_WindowProc);
        ::SetWindowLongPtr(const_cast<DwTrayIcon*>(this)->m_winId, GWLP_USERDATA, (LONG_PTR) this);
    }
    return m_winId;
}

//------------------------------------------------------------------------------
/**
*/
void DwTrayIcon::show()
{
    if (!m_visible)
    {
        NOTIFYICONDATAW ncd = {0};
        ncd.cbSize = sizeof(NOTIFYICONDATAW);
        ncd.hWnd = winId();
        ncd.uID = KTrayIconId;

        ncd.uFlags = NIF_ICON | NIF_MESSAGE;
        ncd.hIcon = _dw_tray_icon;
        ncd.uCallbackMessage = KTrayIconMsg;
        ncd.uVersion = NOTIFYICON_VERSION_4;

        m_visible = _trayicon_Shell_NotifyIcon(NIM_ADD, &ncd);
        DW_SAFE_DESTROY_ICON(ncd.hIcon);
    }

    update();
}

//------------------------------------------------------------------------------
/**
*/
void DwTrayIcon::hide()
{
    if (m_visible)
    {
        NOTIFYICONDATAW ncd = {0};
        ncd.cbSize = sizeof(NOTIFYICONDATAW);
        ncd.hWnd = winId();
        ncd.uID = KTrayIconId;
        m_visible = !_trayicon_Shell_NotifyIcon(NIM_DELETE, &ncd);
    }
}

//------------------------------------------------------------------------------
/**
*/
int DwTrayIcon::modify(TrayIconContent econtent)
{
    int content = econtent;
    if (content && m_visible)
    {
        NOTIFYICONDATAW ncd = {0};
        ncd.cbSize = sizeof(NOTIFYICONDATAW);
        ncd.hWnd = winId();
        ncd.uID = KTrayIconId;

        if (content & ETips)
        {
            ncd.uFlags |= NIF_TIP;
            int size = sizeof(ncd.szTip) / sizeof(TCHAR);
            wcsncpy_s(ncd.szTip, size, m_tips.constString(), size - 1);
            content &= (~ETips);
        }

        if (content & EIcon)
        {
            ncd.uFlags |= NIF_ICON;
            ncd.hIcon = _dw_tray_icon;
            content &= (~EIcon);
        }
        _trayicon_Shell_NotifyIcon(NIM_MODIFY, &ncd);
        DW_SAFE_DESTROY_ICON(ncd.hIcon);
    }
    return econtent;
}

//------------------------------------------------------------------------------
/**
*/
void DwTrayIcon::startFlash(int duration, bool loop)
{
    startFlashAnis(duration, loop, 0, DwNinePatch::numOfSubs(DW_PIXMAP(m_icon))-1);
}

//------------------------------------------------------------------------------
/**
*/
void DwTrayIcon::startFlashAnis(int duration, bool loop, int start, int end)
{
    if (!m_anis)
    {
        m_anis = new DwPropertyAnimation();
        m_anis->setProperty("IconIdx");
    }

    m_anis->setTargetItem(this);
    m_anis->setStartValue(start);
    m_anis->setEndValue(end);
    m_anis->setDuration(duration);
    if (loop)
    {
        m_anis->setLoopCount(-1);
    }
    m_anis->start();
}

//------------------------------------------------------------------------------
/**
*/
void DwTrayIcon::stopFlash(int idx/* = 0*/)
{
    if(m_anis)
    {
        m_anis->stop();

        if (idx >= 0)
        {
            setIconIdx(idx);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
DwItemEventHandler* DwTrayIcon::eventHandler() const
{
    return DwGraphicsItem::eventHandler();
}

//------------------------------------------------------------------------------
/**
*/
static void _dw_set_mouseEvt(const DwGraphicsItem* item, DwMouseEvent& evt, int x, int y)
{
    // screen coordinate
    evt.m_screenPoint = DwCursor::pos();
    // device coordinate , NB! MUST BE CLIENTRECT
    evt.m_devicePoint = DwCursor::pos();
    // mapping to view coordinate
    evt.m_viewPoint = DwCursor::pos();
    // item point
    evt.m_itemPoint = DwPoint(x, y);
    // interest item
    evt.setInterestItem(item);
}

//------------------------------------------------------------------------------
/**
*/
bool DwTrayIcon::onWinEvent(UINT uMsg, LPARAM lParam, WPARAM wParam, HRESULT *hr)
{
    bool handlered = false;

    DW_UNUSED((uMsg && lParam && wParam && hr));
    if (KTrayIconMsg == uMsg)
    {
        // ms-help://MS.MSDNQTR.v90.chs/shellcc/platform/shell/reference/structures/notifyicondata.htm
        WORD message = LOWORD(lParam);
        WORD iconId = HIWORD(lParam);
        int x = GET_X_LPARAM(wParam);
        int y = GET_Y_LPARAM(wParam);
        DW_UNUSED((message && iconId && x && y));

        switch (message)
        {
        case WM_CONTEXTMENU:
            {
                DwContexMenuEvent evt;
                // interest item
                evt.setInterestItem(this);
                // event dispatch
                dwAppEventDispatch()->dispatchEvent(&evt);
                // handlerd
                handlered = true;
            }
        	break;
        case WM_MOUSEMOVE:
            {
                DwMouseMoveEvent evt;
                // config evt
                _dw_set_mouseEvt(this, evt, x, y);
                // event dispatch
                dwAppEventDispatch()->dispatchEvent(&evt);
                // handlerd
                handlered = true;
            }
            break;
        case WM_LBUTTONUP:
            {
                if (m_reserved & 0x01)
                {
                    DwMouseClickedEvent evt;
                    // button
                    evt.m_button = Dw::LeftButton;
                    // target item
                    evt.m_clickedItem = this;
                    // config evt
                    _dw_set_mouseEvt(this, evt, x, y);
                    // dispath to view
                    dwAppEventDispatch()->dispatchEvent(&evt);
                }

                DwMouseUpEvent evt;
                // button
                evt.m_button = Dw::LeftButton;
                // config evt
                _dw_set_mouseEvt(this, evt, x, y);
                // dispath to view
                dwAppEventDispatch()->dispatchEvent(&evt);
                // handlerd
                handlered = true;

                m_reserved = 0x00;
            }
            break;
        case WM_LBUTTONDOWN:
            {
                DwMouseDownEvent evt;
                // button
                evt.m_button = Dw::LeftButton;
                // config evt
                _dw_set_mouseEvt(this, evt, x, y);
                // dispath to view
                dwAppEventDispatch()->dispatchEvent(&evt);
                // handlerd
                handlered = true;

                m_reserved = 0x01;
            }
            break;
        case WM_RBUTTONUP:
            {
                if (m_reserved & 0x02)
                {
                    DwMouseClickedEvent evt;
                    // button
                    evt.m_button = Dw::RightButton;
                    // target item
                    evt.m_clickedItem = this;
                    // config evt
                    _dw_set_mouseEvt(this, evt, x, y);
                    // dispath to view
                    dwAppEventDispatch()->dispatchEvent(&evt);
                }

                DwMouseUpEvent evt;
                // button
                evt.m_button = Dw::RightButton;
                // config evt
                _dw_set_mouseEvt(this, evt, x, y);
                // dispath to view
                dwAppEventDispatch()->dispatchEvent(&evt);
                // handlerd
                handlered = true;

                m_reserved = 0x00;
            }
        case WM_RBUTTONDOWN:
            {
                DwMouseDownEvent evt;
                // button
                evt.m_button = Dw::RightButton;
                // config evt
                _dw_set_mouseEvt(this, evt, x, y);
                // dispath to view
                dwAppEventDispatch()->dispatchEvent(&evt);
                // handlerd
                handlered = true;

                m_reserved = 0x02;
            }
            break;
        }

        *hr = 0;
    }
    return handlered;
}

//------------------------------------------------------------------------------
/**
*/
DwRect DwTrayIcon::trayIconRect() const
{
    // WIN7 API获得任务栏位置
    /*static PtrShell_NotifyIconGetRect Shell_NotifyIconGetRect =
        (PtrShell_NotifyIconGetRect)QLibrary::resolve(QLatin1String("shell32"), "Shell_NotifyIconGetRect");
    if (Shell_NotifyIconGetRect) 
    {
        NOTIFYICONIDENTIFIER nid = {0};
        nid.cbSize = sizeof(nid);
        nid.hWnd = winId();
        nid.uID = KTrayIconId;

        RECT rect;
        HRESULT hr = Shell_NotifyIconGetRect(&nid, &rect);
        if (SUCCEEDED(hr)) 
        {
            if(hr != S_FALSE)
            {
                return QRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
            }
        }
    }*/

    // WIN7以前的版本获取任务栏位置
    DwRect ret;

    TBBUTTON buttonData;
    DWORD processID = 0;
    HWND trayHandle = ::FindWindow(L"Shell_TrayWnd", NULL);

    //find the toolbar used in the notification area
    if (trayHandle) {
        trayHandle = ::FindWindowEx(trayHandle, NULL, L"TrayNotifyWnd", NULL);
        if (trayHandle) {
            HWND hwnd = ::FindWindowEx(trayHandle, NULL, L"SysPager", NULL);
            if (hwnd) {
                hwnd = ::FindWindowEx(hwnd, NULL, L"ToolbarWindow32", NULL);
                if (hwnd)
                    trayHandle = hwnd;
            }
        }
    }

    if (!trayHandle)
        return ret;

    ::GetWindowThreadProcessId(trayHandle, &processID);
    if (processID <= 0)
        return ret;

    HANDLE trayProcess = ::OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ, 0, processID);
    if (!trayProcess)
        return ret;

    int buttonCount = ::SendMessage(trayHandle, TB_BUTTONCOUNT, 0, 0);
    LPVOID data = ::VirtualAllocEx(trayProcess, NULL, sizeof(TBBUTTON), MEM_COMMIT, PAGE_READWRITE);

    if ( buttonCount < 1 || !data ) {
        ::CloseHandle(trayProcess);
        return ret;
    }

    //search for our icon among all toolbar buttons
    for (int toolbarButton = 0; toolbarButton  < buttonCount; ++toolbarButton ) {
        SIZE_T numBytes = 0;
        DWORD appData[2] = { 0, 0 };
        ::SendMessage(trayHandle, TB_GETBUTTON, toolbarButton , (LPARAM)data);

        if (!ReadProcessMemory(trayProcess, data, &buttonData, sizeof(TBBUTTON), &numBytes))
            continue;

        if (!ReadProcessMemory(trayProcess, (LPVOID) buttonData.dwData, appData, sizeof(appData), &numBytes))
            continue;

        int currentIconId = appData[1];
        HWND currentIconHandle = (HWND) appData[0];
        bool isHidden = !!(buttonData.fsState & TBSTATE_HIDDEN);

        if (currentIconHandle == winId() &&
            currentIconId == KTrayIconId && !isHidden) {
                ::SendMessage(trayHandle, TB_GETITEMRECT, toolbarButton , (LPARAM)data);
                RECT iconRect = {0, 0};
                if(::ReadProcessMemory(trayProcess, data, &iconRect, sizeof(RECT), &numBytes)) {
                    ::MapWindowPoints(trayHandle, NULL, (LPPOINT)&iconRect, 2);
                    DwRect geometry(iconRect.left + 1, iconRect.top + 1,
                        iconRect.right - iconRect.left - 2,
                        iconRect.bottom - iconRect.top - 2);
                    if (geometry.isValid())
                        ret = geometry;
                    break;
                }
        }
    }
    ::VirtualFreeEx(trayProcess, data, 0, MEM_RELEASE);
    ::CloseHandle(trayProcess);
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
void DwTrayIcon::update()
{
    m_contents = modify((TrayIconContent)m_contents);
}
DW_IMPLEMENT_PROPERTY(DwTrayIcon);
