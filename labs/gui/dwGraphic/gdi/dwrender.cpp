#include "stable.h"
#include "dwtexture.h"
#include "dwcanvas.h"
#include "dwrender.h"


class DwRenderPrivate:public DwSharedData
{
public:
	DwRenderPrivate();
	~DwRenderPrivate();

	DwCanvas  *m_pCanvas;
	HDC       m_memHDc;
	
	//test
	HDC      m_dc;
};
DwRenderPrivate::DwRenderPrivate()
{

}
DwRenderPrivate::~DwRenderPrivate()
{

}

DwRender::DwRender()
{
	d = new DwRenderPrivate;
}

DwRender::~DwRender()
{
	if (d->m_pCanvas)
	{
		delete d->m_pCanvas;
		d->m_pCanvas = NULL;
	}
}

bool DwRender::drawCanvas(int nwidth,int nHeight)
{
	DW_ASSERT(nwidth > 0 && nHeight > 0);

	d->m_pCanvas = new DwCanvas();
	DW_ASSERT(d->m_pCanvas);
	return d->m_pCanvas->create(nwidth,nHeight);
} 

BOOL DwRender::drawTexture(HGTEXTURE htexture,int x, int y, SIZE sz)
{
	DwTexture *pTexture = static_cast<DwTexture *>(htexture.lpvoid);
	d->m_memHDc = (HDC)d->m_pCanvas->memHDc();
	HDC hDC = ::CreateCompatibleDC(d->m_memHDc);
	HGDIOBJ hBmpOld = (HGDIOBJ)::SelectObject(hDC, (HBITMAP)pTexture->handle());

	BOOL bRet = ::BitBlt(d->m_memHDc,x,y,sz.cx,sz.cy,hDC,x,y,SRCCOPY);
	::SelectObject(hDC,hBmpOld);
	d->m_dc = hDC;


	return bRet;
}

void DwRender::canvasToWindow(unsigned int hdc)
{
	//HDC hwnddc = (HDC) hdc;
  // BOOL ret = ::BitBlt(hwnddc,0,0,100,100,d->m_dc,0,0,SRCCOPY);

	::BitBlt((HDC)hdc,0,0,400,400,d->m_memHDc,0,0,SRCCOPY);
}
