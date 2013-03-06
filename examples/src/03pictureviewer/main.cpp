#include "stable.h"
#include "picViewer.h"

//////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE , HINSTANCE , LPSTR lpCmdLine, int )
{
	DwApplication app;

	DwMainWindow *mainWindow = new DwMainWindow(NULL);
    mainWindow->setAttribute(Dw::WA_DeleteOnClose);
    showImage(mainWindow, DwCommandLineArgs(DwString(lpCmdLine)));

    return app.exec();
}