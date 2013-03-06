#include "stable.h"
#include <map>
#include "bitmap/dwbitmap.h"
#include "dwcanvas.h"
#include "dwcanvascach.h"

class DwCanvasCachPrivate: public DwSharedData
{
public:
	DwCanvasCachPrivate();
	~DwCanvasCachPrivate();

	DwCanvas m_cannvas;
	std::map<HWND,HDC> m_memoryDCContainter;
};

DwCanvasCachPrivate::DwCanvasCachPrivate()
{

}

DwCanvasCachPrivate::~DwCanvasCachPrivate()
{

}

DwCanvasCach::DwCanvasCach()
{
	d= new DwCanvasCachPrivate;	
}

DwCanvasCach::~DwCanvasCach()
{

}

DwCanvasCach& DwCanvasCach::getInstance()
{
	static DwCanvasCach instance;
	return instance;
}

HDC DwCanvasCach::createCanvasDC(HWND hwnd) const
{
	DW_ASSERT(hwnd);

	WINDOWINFO wndInfo = {0};
	GetWindowInfo(hwnd,&wndInfo);
	int wndWidth = wndInfo.rcWindow.right - wndInfo.rcWindow.left;
	int wndHeight= wndInfo.rcWindow.bottom - wndInfo.rcWindow.top;
	HDC memDC = d->m_cannvas.createDC(wndWidth,wndHeight);

	DW_ASSERT(memDC);
	if(memDC)
	{
		d->m_memoryDCContainter[hwnd] = memDC;
		return memDC;
	}
	return NULL;
}

HDC DwCanvasCach::hwndTocanvasDC(HWND hwnd) const
{
	std::map<HWND,HDC>::iterator iter;
	iter = d->m_memoryDCContainter.find(hwnd);
	if(iter != d->m_memoryDCContainter.end())
	{
		return iter->second;
	}
	else
	{
		return createCanvasDC(hwnd);
	}
	return NULL;
}
