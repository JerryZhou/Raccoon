#include "stable.h"
#include "dwdesktop.h"
#include "dwcore/dwvector.h"
#include "graphicsview/dwdummys.h"

DW_IMPLEMENT_CLASS(DwDesktop, 'DTOP', DwRttiObject);

//------------------------------------------------------------------------------
/**
*/
DwRect DwDesktop::desktopRect()
{
    return DwRect(0, 0,
                  ::GetSystemMetrics(SM_CXSCREEN),
                  ::GetSystemMetrics(SM_CYSCREEN));
}

//------------------------------------------------------------------------------
/**
*/
DwRect DwDesktop::desktopWorkArea()
{
    CRect rect;
    ::SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
    return DwRect(0, 0, rect.Width(), rect.Height());
}

//------------------------------------------------------------------------------
/**
*/
class DwSystemLibrary
{
public:
    explicit DwSystemLibrary(const DwString &libraryName)
    {
        m_libraryName = libraryName;
        m_handle = 0;
        m_didLoad = false;
    }

    explicit DwSystemLibrary(const wchar_t *libraryName)
    {
        m_libraryName = DwString::fromWCharArray(libraryName);
        m_handle = 0;
        m_didLoad = false;
    }

    bool load(bool onlySystemDirectory = true)
    {
        m_handle = load((const wchar_t *)m_libraryName.constString(), onlySystemDirectory);
        m_didLoad = true;
        return (m_handle != 0);
    }

    bool isLoaded()
    {
        return (m_handle != 0);
    }

    void *resolve(const char *symbol)
    {
        if (!m_didLoad)
            load();
        if (!m_handle)
            return 0;
#if defined(WINCE) || defined(_WIN32_WCE)
        return (void*)GetProcAddress(m_handle, (const wchar_t*)DwString(symbol).constString());
#else
        return (void*)GetProcAddress(m_handle, symbol);
#endif
    }

    static void *resolve(const DwString &libraryName, const char *symbol)
    {
        return DwSystemLibrary(libraryName).resolve(symbol);
    }

    static DW_GUI_EXPORT HINSTANCE load(const wchar_t *lpFileName, bool onlySystemDirectory = true);

private:
    HINSTANCE m_handle;
    DwString m_libraryName;
    bool m_didLoad;
};

//------------------------------------------------------------------------------
/**
*/
static DwString dwSystemDirectory()
{
    DwVector<wchar_t> fullPath;
    fullPath.resize(MAX_PATH);

    UINT retLen = ::GetSystemDirectory((wchar_t*)&fullPath.front(), MAX_PATH);
    if (retLen > MAX_PATH) {
        fullPath.resize(MAX_PATH);
        retLen = ::GetSystemDirectory((wchar_t*)&fullPath.front(), retLen);
    }
    // in some rare cases retLen might be 0
    return DwString::fromWCharArray((wchar_t*)&fullPath.front(), int(retLen));
}

//------------------------------------------------------------------------------
/**
*/
HINSTANCE DwSystemLibrary::load(const wchar_t *libraryName, bool onlySystemDirectory /* = true */)
{
    DwVector<DwString> searchOrder;
    searchOrder.append(dwSystemDirectory());

    if (!onlySystemDirectory) {
        const DwString PATH(DwString(getenv("PATH")));
        DwVector<DwString> paths = dwTokenize(";", " ");
        for (int i=0; i<paths.size(); ++i){
            searchOrder.append(paths.at(i));
        }
    }
    DwString fileName = DwString::fromWCharArray(libraryName);
    fileName.append(DwString(".dll"));

    // Start looking in the order specified
    for (int i = 0; i < searchOrder.count(); ++i) {
        DwString fullPathAttempt = searchOrder.at(i);
        if (!fullPathAttempt.endsWith(('\\'))) {
            fullPathAttempt.append(('\\'));
        }
        fullPathAttempt.append(fileName);
        HINSTANCE inst = ::LoadLibrary((const wchar_t *)fullPathAttempt.constString());
        if (inst != 0)
            return inst;
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
BOOL WINAPI enumCallback(HMONITOR hMonitor, HDC, LPRECT, LPARAM);
struct  DesktopData
{
    struct MONITORINFO
    {
        DWORD   cbSize;
        RECT    rcMonitor;
        RECT    rcWork;
        DWORD   dwFlags;
    };

    typedef BOOL (WINAPI *InfoFunc)(HMONITOR, MONITORINFO*);
    typedef BOOL (WINAPI *EnumProc)(HMONITOR, HDC, LPRECT, LPARAM);
    typedef BOOL (WINAPI *EnumFunc)(HDC, LPCRECT, EnumProc, LPARAM);

    static EnumFunc enumDisplayMonitors;
    static InfoFunc getMonitorInfo;
    static DesktopData* pThis;

    int screenCount;
    int primaryScreen;
    DwVector<DwRect> rects;
    DwVector<DwRect> workrects;

    DesktopData()
    {
        screenCount = 0;
        primaryScreen = 0;
        pThis = this;

        DwSystemLibrary user32Lib("user32");
        if (user32Lib.load()) {
            enumDisplayMonitors = (EnumFunc)user32Lib.resolve("EnumDisplayMonitors");
            getMonitorInfo = (InfoFunc)user32Lib.resolve("GetMonitorInfoW");
        }

        if (!enumDisplayMonitors || !getMonitorInfo) {
            screenCount = ::GetSystemMetrics(80);  // SM_CMONITORS
            rects.resize(screenCount);
            return;
        }
        // Calls enumCallback
        enumDisplayMonitors(0, 0, enumCallback, 0);
        enumDisplayMonitors = 0;
        getMonitorInfo = 0;
    }
};
DesktopData* DesktopData::pThis = NULL;
DesktopData::InfoFunc DesktopData::getMonitorInfo = NULL;
DesktopData::EnumFunc DesktopData::enumDisplayMonitors = NULL;
DW_GLOBAL_STATIC(DesktopData, _dw_DesktopData);
#define _D_DesktopPrivate (*DesktopData::pThis)
#define _D_Desktop (*_dw_DesktopData())
static int screen_number = 0;

//------------------------------------------------------------------------------
/**
*/
BOOL WINAPI enumCallback(HMONITOR hMonitor, HDC, LPRECT, LPARAM)
{
    _D_DesktopPrivate.screenCount++;
    _D_DesktopPrivate.rects.resize(_D_DesktopPrivate.screenCount);
    _D_DesktopPrivate.workrects.resize(_D_DesktopPrivate.screenCount);
    // Get the MONITORINFO block
    DesktopData::MONITORINFO info;
    memset(&info, 0, sizeof(DesktopData::MONITORINFO));
    info.cbSize = sizeof(DesktopData::MONITORINFO);
    BOOL res = DesktopData::getMonitorInfo(hMonitor, &info);
    if (!res) {
        (_D_DesktopPrivate.rects)[screen_number] = DwRect();
        (_D_DesktopPrivate.workrects)[screen_number] = DwRect();
        return true;
    }

    // Fill list of rects
    RECT r = info.rcMonitor;
    DwRect qr(DwPoint(r.left, r.top), DwPoint(r.right - 1, r.bottom - 1));
    (_D_DesktopPrivate.rects)[screen_number] = qr;

    r = info.rcWork;
    qr = DwRect(DwPoint(r.left, r.top), DwPoint(r.right - 1, r.bottom - 1));
    (_D_DesktopPrivate.workrects)[screen_number] = qr;

    if (info.dwFlags & 0x00000001) //MONITORINFOF_PRIMARY
        _D_DesktopPrivate.primaryScreen = screen_number;

    ++screen_number;
    // Stop the enumeration if we have them all
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwDesktop::isVirtualDesktop() 
{
    return true;
}

//------------------------------------------------------------------------------
/**
*/
int DwDesktop::screenNumber( const DwPoint &p ) 
{
    for (int i=0; i<_D_Desktop.screenCount; ++i)
    {
        if(_D_Desktop.rects.at(i).contains(p))
        {
            return i;
        }
    }
    return -1;
}

//------------------------------------------------------------------------------
/**
*/
int DwDesktop::numScreens() 
{
    return _D_Desktop.screenCount;
}

//------------------------------------------------------------------------------
/**
*/
int DwDesktop::screenCount() 
{
    return _D_Desktop.screenCount;
}

//------------------------------------------------------------------------------
/**
*/
int DwDesktop::primaryScreen() 
{
    return _D_Desktop.primaryScreen;
}

//------------------------------------------------------------------------------
/**
*/
DwRect DwDesktop::screenGeometry( int  idx/*= -1*/ ) 
{
    if(idx == -1)
    {
        idx = _D_Desktop.primaryScreen;
    }
    if (idx >= 0 && idx < _D_Desktop.screenCount)
    {
        return _D_Desktop.rects.at(idx);
    }
    return DwRect();
}

//------------------------------------------------------------------------------
/**
*/
DwRect DwDesktop::availableGeometry( int  idx/*= -1*/ ) 
{
    if(idx == -1)
    {
        idx = _D_Desktop.primaryScreen;
    }
    if (idx >= 0 && idx < _D_Desktop.screenCount)
    {
        return _D_Desktop.workrects.at(idx);
    }
    return DwRect();
}

//------------------------------------------------------------------------------
/**
    require size of t little than avaliable
*/
bool DwDesktop::adjustedAvailableRect(DwRect &t)
{
    bool adjusted = false;

    DwRect avaliable = availableGeometry(t.topLeft());
    if (t.left()<avaliable.left())
    {
        adjusted = true;
        t.moveLeft(avaliable.left());
    }
    else if (t.right() > avaliable.right())
    {
        adjusted = true;
        t.moveRight(avaliable.right());
    }

    if (t.top() < avaliable.top())
    {
        adjusted = true;
        t.moveTop(avaliable.top());
    }
    else if (t.bottom() > avaliable.bottom())
    {
        adjusted = true;
        t.moveBottom(avaliable.bottom());
    }
    return adjusted;
}

//------------------------------------------------------------------------------
/**
*/
DwRect DwDesktop::adjustedAvailableRect(DwPoint& p, const DwSize &t, Direction dir/*=Down*/)
{
    DwRect org((dir==Down ? p : DwPoint(p.x(), p.y()-t.height())), t);

    adjustedAvailableRect(org);

    p = org.topLeft();
    return org;
}

//------------------------------------------------------------------------------
/**
*/
DwRect DwDesktop::availableRect(DwPoint& p, const DwSize &t, Direction dir/*=Down*/)
{
    DwRect org((dir==Down ? p : DwPoint(p.x(), p.y()-t.height())), t);
    
    bool b = adjustedAvailableRect(org);
    if (b)
    {
        DwRect anthor((dir!=Down ? p : DwPoint(p.x(), p.y()-t.height())), t);
        b = adjustedAvailableRect(anthor);
        if (!b)
        {
            p = anthor.topLeft();
            return anthor;
        }
    }
    p = org.topLeft();
    return org;
}