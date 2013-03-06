#include "stable.h"
#include "dwtexture.h"
#include "dwcanvas.h"
#include "dwrender.h"
#include "dwinrender.h"


class DwRenderPrivate:public DwSharedData
{
public:
	DwRenderPrivate();
	~DwRenderPrivate();

	DwCanvas  *m_pCanvas;
	DwInRender m_inRender;
	HDC       m_mHDC;
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

bool DwRender::drawCanvas(int nwidth,int height)
{
	DW_ASSERT(nwidth > 0 && height > 0);

	d->m_pCanvas = new DwCanvas();
	DW_ASSERT(d->m_pCanvas);
	return d->m_pCanvas->create(nwidth,height);
} 

BOOL DwRender::drawTextureToMemDC(HGTEXTURE htexture,POINT pt)
{
	BOOL bRet = FALSE;
	d->m_mHDC = (HDC)d->m_pCanvas->mHDC(); 
	return d->m_inRender.drawTexture(htexture,d->m_mHDC,pt);
}

BOOL DwRender::canvasToWindow(uint32_t hdc,int width,int height)
{
	return::BitBlt((HDC)hdc, 0, 0, 400, 400, d->m_mHDC,0,0,SRCCOPY);
}
