#include "stable.h"
#include "bitmap/dwbitmap.h"
#include "dwcanvas.h"

class DwCanvasPrivate: public DwSharedData
{
public:
	DwCanvasPrivate();
	~DwCanvasPrivate();
	void reset();

	HDC m_hdc;
	HGDIOBJ m_hStockObject;
	DwBitmap *m_pbitmap;
};
DwCanvasPrivate::DwCanvasPrivate()
:m_hdc(NULL)
,m_hStockObject(NULL)
,m_pbitmap(NULL)
{

}
DwCanvasPrivate::~DwCanvasPrivate()
{

}

void DwCanvasPrivate::reset()
{
	m_hdc = NULL;
	m_hStockObject = NULL;
}

DwCanvas::DwCanvas()
{
	d= new DwCanvasPrivate;
}

DwCanvas::~DwCanvas()
{
	clear();
}

HDC DwCanvas::getDC() const 
{
	return d->m_hdc;
}

void DwCanvas::releaseDC() 
{
	clear();
}

HDC DwCanvas::createDC(int width, int height)
{
	clear();
	d->m_pbitmap = new DwBitmap();
	if (!d->m_pbitmap->createBitmapFromSize(width,height))
	{
		DW_ASSERT(0);
		return NULL;
	}

	d->m_hdc = CreateCompatibleDC( NULL);	
	if (!d->m_hdc)
	{
		d->m_pbitmap->reset();
	}

	d->m_hStockObject = SelectObject(d->m_hdc,(HBITMAP)d->m_pbitmap->handle());
	return d->m_hdc;
}

const DwBitmap* DwCanvas::bitmap() const
{
	return d->m_pbitmap;
}

void DwCanvas::clear()
{
	if (d->m_hdc || d->m_hStockObject)
	{
		SelectObject(d->m_hdc,d->m_hStockObject);
		DeleteDC(d->m_hdc);
	}

	if (d->m_pbitmap)
	{
		delete d->m_pbitmap;
	}
	d->m_pbitmap = NULL;

	d->reset();
}

