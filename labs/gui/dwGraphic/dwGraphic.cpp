// dwGraphic.cpp : Defines the entry point for the console application.
//

#include "stable.h"
#include "testwnd/dwtestwin.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nShow)
{
	DwWindow wnd;
	wnd.CreateEx(NULL,L"TESTWND",L"TESEGDI",WS_POPUP,400,100,400,400,NULL,NULL,hInst);
	wnd.ShowWindow(nShow);
	wnd.UpdateWindow();
	wnd.MessageLoop();
	return 0;
}

