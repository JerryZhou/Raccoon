#include "stable.h"
#include "simplewidgets.h"
#include "dwgui/dwi18n.h"

//////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE , HINSTANCE , LPSTR lpCmdLine, int )
{
	DwApplication app;
    
    DwString cmds(lpCmdLine);
    DwCommandLineArgs args(cmds);

    DwI18n::instance()->load("bin:i18n.xml");

    DwMainWindow *mutliWindow = new DwMainWindow(NULL);
    mutliWindow->setAttribute(Dw::WA_DeleteOnClose);
    mutliWindow->setRect(DwRect(0, 0, 558, 408));
    animation_test(mutliWindow, args);
    mutliWindow->show();
    return app.exec();
}