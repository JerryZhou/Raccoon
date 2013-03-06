#include "stable.h"

#include "dwcore/dwmempool.h"

#include "dwwindow.h"
#include "dwapplication.h"
#include "dwrttiobject.h"
#include "dwguiconfig.h"
#include "dwgui/dwassignlib.h"
#include "debug/dwminidump.h"
#include "dwgui/dwringlogcase.h"
#include "dwgui/dwaccessiblewidget.h"

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwApplication, 'DAPP', DwRttiObject);

//------------------------------------------------------------------------------
CAppModule _Module;
DwApplication *g_dwApp = NULL;

//------------------------------------------------------------------------------
/**
*/
DwApplication *dwApp()
{
    return g_dwApp;
}

//------------------------------------------------------------------------------
/**
*/

DwApplication::DwApplication()
    : m_fulshLog(false)
{
    processCommandArguments();

    g_dwApp = this;
    m_appName = DW_DEFAULT_APP;
    m_companyName = DW_DEFAULT_COMPANY;

    _Module.Init(NULL, GetModuleHandle(NULL));

    DwMiniDump::setup();
    DwAssignLib::initAssignLib();
    DwGraphics::initGraphics();
    DwRttiObject::initRttiObjects();

    // accessible
    setupAccessibleWidget();
    DW_UI_LOG("***************BEGIN RACCON***********\n");
}

//------------------------------------------------------------------------------
/**
*/
DwApplication::~DwApplication()
{
    _Module.Term();

    while(!m_topLevelWindows.isEmpty())
    {
        DwSet<DwWindow*>::iterator it = m_topLevelWindows.begin();
        if(*it)
        {
            (*it)->close();
        }
    }

    DwRttiObject::unInitRttiObjects();
    DwGraphics::uninitGraphics();
    if (m_fulshLog)
    {
        DW_UI_LOG_FLUSH(DwUrl("bin:lg.txt"));
    }
    DwAssignLib::unInitAssignLib();
    g_dwApp = NULL;
}

//------------------------------------------------------------------------------
/**
*/
int DwApplication::exec()
{
    if (!m_topLevelWindows.isEmpty())
    {
        CMessageLoop loop;
        _Module.AddMessageLoop(&loop);

        int nRet = loop.Run();

        _Module.RemoveMessageLoop();

        return nRet;
    }

    return 0;
}

//------------------------------------------------------------------------------
/**
*/
void DwApplication::quit()
{
    ::PostQuitMessage(0);
}

//------------------------------------------------------------------------------
/**
*/
void DwApplication::addTopLevelWindow( DwWindow *win )
{
    m_topLevelWindows.insert(win);
}

//------------------------------------------------------------------------------
/**
*/
void DwApplication::removeTopLevelWindow( DwWindow *win )
{
    m_topLevelWindows.remove(win);

    if(m_topLevelWindows.isEmpty())
    {
        quit();
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwApplication::setFlushLog(bool b)
{
    m_fulshLog = b;
}

//------------------------------------------------------------------------------
/**
*/
void DwApplication::setAppName(DwString n)
{
    m_appName = n;
}

//------------------------------------------------------------------------------
/**
*/
DwString DwApplication::appName() const
{
    return m_appName;
}

//------------------------------------------------------------------------------
/**
*/
void DwApplication::setCompanyName(DwString n)
{
    m_companyName = n;
}

//------------------------------------------------------------------------------
/**
*/
DwString DwApplication::companyName() const
{
    return m_companyName;
}

//------------------------------------------------------------------------------
/**
*/
DwSet<DwWindow*> DwApplication::topLevelWindows()
{
    return m_topLevelWindows;
}

//------------------------------------------------------------------------------
/**
*/
void DwApplication::processCommandArguments()
{
    TCHAR szCmdLine[MAX_PATH] = {0};
    LPTSTR lpszCmd = GetCommandLine();
    _tcsncpy_s(szCmdLine, _countof(szCmdLine), lpszCmd, _TRUNCATE);
    _tcslwr_s(szCmdLine, _countof(szCmdLine));
    if (_tcsstr(szCmdLine, _T("/disablemempool")) != 0)
    {
        mp_enable(false);
    }
}
