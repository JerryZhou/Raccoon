#pragma once

#include <dwcore/dwobject.h>
#include <dwcore/dwset.h>
#include "dwgui/dwrttiobject.h"

//------------------------------------------------------------------------------
class DwWindow;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwApplication : public DwObject, public DwRttiObject
{
    DW_DECLARE_CLASS(DwApplication);
public:
    /// constructor
    DwApplication();
    /// destructor
    virtual ~DwApplication();

    /// start the message loop
    int exec();
    /// quit loop
    void quit();

    /// configure app name
    void setAppName(DwString n);
    /// return app name
    DwString appName() const;

    /// configure company name
    void setCompanyName(DwString n); 
    /// return company name
    DwString companyName() const;

    /// will get all the top level window
    DwSet<DwWindow*> topLevelWindows();

    /// set if we need to dump log when exit
    void setFlushLog(bool b);

private:
    /// deal with the command
    void processCommandArguments();

    friend class DwWindow;
    /// deal register with the top level window
    void addTopLevelWindow(DwWindow *win);
    /// deal register  with the top level window
    void removeTopLevelWindow(DwWindow *win);

    /// datas
    bool m_fulshLog;
    DwString m_appName;
    DwString m_companyName;
    DwSet<DwWindow*> m_topLevelWindows;
};// end of DwApplication
//------------------------------------------------------------------------------
DW_GUI_EXPORT DwApplication * dwApp();