#include "stable.h"
#include "dwgui/dwapplication.h"
#include "dwgui/dwwindow.h"
#include "dwgui/dwtimer.h"
#include <Box2D/Box2D.h>

#include "mainFrameWindow.h"

int WINAPI WinMain(HINSTANCE , HINSTANCE , LPSTR , int )
{
	DwApplication app;
	DwTimer showTimer;


	MainFrameWindow *myWindow = new MainFrameWindow(NULL);
	myWindow->setSize(1024,768);
	myWindow->setAttribute(Dw::WA_DeleteOnClose);
	myWindow->show();
	showTimer.sigTimeout.connect(myWindow,&MainFrameWindow::timeStep);

// 	AllocConsole();
// 	freopen( "CONOUT$","w",stdout);

	showTimer.start(15);
	app.exec();
	showTimer.stop();

	DwRttiObject::dumpObjects();
	

/*	FreeConsole();*/

	return 0;
}