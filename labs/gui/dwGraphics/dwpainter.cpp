#include "stable.h"
#include "texture/dwcanvascach.h"

class DwPainterPrivate:public DwSharedData
{
public:
	DwPainterPrivate();
	~DwPainterPrivate(){}

	HWND m_hwnd;
	HDC  m_hWndDC;
	DwPoint   m_position;
	DwSize    m_size;
	PaintType m_paintType;
};
DwPainterPrivate::DwPainterPrivate()
:m_hwnd(NULL)
,m_hWndDC(NULL)
,m_position(0,0)
,m_size(0,0)
{

}

DwPainter::DwPainter()
{
	if (!d)
	{
		d = new DwPainterPrivate;
	}
}

DwPainter::DwPainter(HWND hwnd, PaintType paintType)
{
	if (!d)
	{
		d = new DwPainterPrivate;
	}
	begin(hwnd,paintType);
}

DwPainter::~DwPainter()
{
	end(d->m_paintType);
}

void DwPainter::begin(HWND hwnd, PaintType paintType)
{
	d->m_hwnd = hwnd;
	d->m_paintType = paintType;

	PAINTSTRUCT ps ;
	switch(paintType)
	{
	case Paint_Message:
		d->m_hWndDC = BeginPaint(d->m_hwnd,&ps);
		break;
	case NoPaint_Message:
		d->m_hWndDC = GetDC(d->m_hwnd);
		break;
	}
}

void DwPainter::end(PaintType paintType)
{
	canvasToWindow();
	PAINTSTRUCT ps;
	switch(paintType)
	{
	case Paint_Message:
		EndPaint(d->m_hwnd, &ps);
		break;
	case NoPaint_Message:
		ReleaseDC(d->m_hwnd, d->m_hWndDC);
		break;
	}
}

bool DwPainter::drawTexture(const DwPoint& point, const DwTexture& texture)
{
	BOOL bRet = FALSE;
	d->m_position = point;
	d->m_size.setHeight(texture.height());
	d->m_size.setWidth(texture.width());

	HDC  dc = CreateCompatibleDC(NULL);
	void * pbyte =NULL;
	HBITMAP hbmp = ::CreateDIBSection(NULL, &texture.bitmapInfo(), DIB_RGB_COLORS, (void **)&pbyte,NULL,0);
	if (hbmp)
	{
		HGDIOBJ hSrcOld = NULL;
		hSrcOld = SelectObject(dc,hbmp);
		int nReCode = 0;
		nReCode = StretchDIBits(dc, 0, 0, texture.width(), texture.height(), 0, 0, texture.width(),
			texture.height(),(void*)texture.bits(), &texture.bitmapInfo(), DIB_RGB_COLORS, SRCCOPY);
		if (nReCode == GDI_ERROR)
		{
			DW_ASSERT(0);
			return FALSE;
		}

		HDC canvasDC = DwCanvasCach::getInstance().hwndTocanvasDC(d->m_hwnd);
		if (canvasDC)
		{
			bRet = BitBlt(canvasDC, point.x(), point.y(), texture.width(), texture.height(), dc, 0, 0, SRCCOPY);
			DW_ASSERT(bRet);
		}

		SelectObject(dc,hSrcOld);
	}
	DeleteDC(dc);
	return bRet;
}

bool DwPainter::drawTexture(const DwRect& rectangle, const DwTexture& texture)
{
	BOOL bRet = FALSE;
	d->m_position.setX(rectangle.x());
    d->m_position.setY(rectangle.y());
    d->m_size = rectangle.size();

	HDC  dc = CreateCompatibleDC(NULL);
	void * pbyte =NULL;
	HBITMAP hbmp = ::CreateDIBSection(NULL, &texture.bitmapInfo(), DIB_RGB_COLORS, (void **)&pbyte,NULL,0);
	if (hbmp)
	{
		HGDIOBJ hSrcOld = NULL;
		hSrcOld = SelectObject(dc,hbmp);
		int nReCode = 0;
		nReCode = StretchDIBits(dc, 0, 0, texture.width(), texture.height(), 0, 0, texture.width(),
			texture.height(),(void*)texture.bits(), &texture.bitmapInfo(), DIB_RGB_COLORS, SRCCOPY);
		if (nReCode == GDI_ERROR)
		{
			DW_ASSERT(0);
			return FALSE;
		}

		HDC canvasDC = DwCanvasCach::getInstance().hwndTocanvasDC(d->m_hwnd);
		if (canvasDC)
		{
		//	bRet = BitBlt(canvasDC, d->m_position.x(), d->m_position.y(), d->m_size.width(), d->m_size.height(), dc, 0, 0, SRCCOPY);
			bRet = StretchBlt(canvasDC, d->m_position.x(), d->m_position.y(), d->m_size.width(), d->m_size.height(), dc, 0, 0,texture.width(), texture.height(), SRCCOPY);
			DW_ASSERT(bRet);
		}

		SelectObject(dc,hSrcOld);
	}
	DeleteDC(dc);
	return bRet;

}

void DwPainter::canvasToWindow()
{
	WINDOWINFO wndInfo;
	GetWindowInfo(d->m_hwnd,&wndInfo);
	int width = wndInfo.rcWindow.right - wndInfo.rcWindow.left;
	int hight = wndInfo.rcWindow.bottom - wndInfo.rcWindow.top;
	HDC canvasDC = DwCanvasCach::getInstance().hwndTocanvasDC(d->m_hwnd);

	bool bRet = BitBlt(d->m_hWndDC, 0, 0, width, hight, canvasDC, 0, 0, SRCCOPY);
	DW_ASSERT(bRet);
}

