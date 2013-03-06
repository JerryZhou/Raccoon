#include "stable.h"
#include "dwbitmap.h"
#include "dwcanvas.h"


class DwCanvasPrivate: public DwSharedData
{
public:
	DwCanvasPrivate();
	~DwCanvasPrivate();
	void resetData();

	FILTERMODE m_filterMode;
	DRAWMODE   m_drawMode;
	HDC m_hdc;
	HGDIOBJ m_hStockObject;
	DwBitmap *m_pBitmap;
};
DwCanvasPrivate::DwCanvasPrivate()
:m_hdc(NULL)
,m_hStockObject(NULL)
,m_pBitmap(NULL)
{


}
DwCanvasPrivate::~DwCanvasPrivate()
{

}

void DwCanvasPrivate::resetData()
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
	destroy();

	if (d->m_pBitmap)
	{
		delete d->m_pBitmap;
		d->m_pBitmap = NULL;
	}
}

bool DwCanvas::create(int nWidth, int nHeight)
{
	DW_ASSERT(nWidth > 0 && nHeight > 0);

	destroy();

	d->m_pBitmap = new DwBitmap();
	DW_ASSERT(d->m_pBitmap);

	DW_ASSERT(d->m_pBitmap->createBitmap(nWidth,nHeight));
	if (!d->m_pBitmap->createBitmap(nWidth,nHeight))
	{
		delete d->m_pBitmap;
		d->m_pBitmap = NULL;
		return false;
	}

	HDC hDCScreen = GetDC(NULL);
	d->m_hdc = CreateCompatibleDC(hDCScreen);
	ReleaseDC(NULL,hDCScreen);

	if (!d->m_hdc)
	{
		d->m_pBitmap->clear();
	}

	d->m_hStockObject = SelectObject(d->m_hdc,(HBITMAP)d->m_pBitmap->handle());

	return true;
}

void DwCanvas::filterMode(LPFILTERMODE pFilter) const
{
	*pFilter = d->m_filterMode;
}

void DwCanvas::setFilterMode(FILTERMODE filter)
{
	d->m_filterMode = filter;
}

void DwCanvas::detachBitmap(DwBitmap *pBitmap)const
{
	pBitmap = d->m_pBitmap;
	d->m_pBitmap = NULL;
}

void* DwCanvas::memHDc()
{
	return (void *)(d->m_hdc);
}

void DwCanvas::resetData()
{
	d->resetData();
}

void DwCanvas::destroy()
{
	if (d->m_hdc || d->m_hStockObject)
	{
		SelectObject(d->m_hdc,d->m_hStockObject);
		DeleteDC(d->m_hdc);
	}

	if (d->m_pBitmap)
	{
		d->m_pBitmap->clear();
	}

	d->resetData();

}