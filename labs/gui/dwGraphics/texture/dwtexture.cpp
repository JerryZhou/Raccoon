#include "stable.h"
#include "bitmap/dwbitmap.h"
#include "mode/dwdrawmode.h"
#include "render/dwalpha.h"
#include "render/dwrasterize.h"

class DwTexturePrivate: public DwSharedData
{
public:
	DwTexturePrivate();
	~DwTexturePrivate();

	DwBitmap   m_bitmap;
	DwDrawMode m_drawMode;
	DwAlpha    m_alpha;
	DwRasterize m_rasterize;
};
DwTexturePrivate::DwTexturePrivate()
{
	m_bitmap.reset();
}

DwTexturePrivate::~DwTexturePrivate()
{

}

DwTexture::DwTexture()
{
	d= new DwTexturePrivate;
}

DwTexture::~DwTexture()
{
	if(d)
	{
		delete d;
	}
	d = NULL;
}
bool DwTexture::createBitmapFromFile(const DwString &filePath)
{
	return d->m_bitmap.createBitmapFromFile(filePath);
}

bool DwTexture::createBitmapFromSize(int width, int height)
{
	return d->m_bitmap.createBitmapFromSize(width,height);
}

uint32_t DwTexture::handle() const
{
	return (uint32_t)d->m_bitmap.handle();
}
const uint8_t *DwTexture::bits() const
{
	return d->m_bitmap.bits();
}

const BITMAPINFO DwTexture::bitmapInfo()const
{
	return d->m_bitmap.bitmapinfo();
}

int DwTexture::height() const
{
	return d->m_bitmap.height();
}

int DwTexture::width() const
{
	return d->m_bitmap.width();
}

const CompositionMode& DwTexture::compositionMode() const
{
	return d->m_rasterize.compositionMode();
}

void DwTexture::setCompositionMode(const CompositionMode& CompositionMode)
{
	d->m_rasterize.setCompositionMode(CompositionMode);
}

void DwTexture::setDrawMode(DrawMode drawMode)
{
	d->m_drawMode.setDrawMode(drawMode);
}

DrawMode DwTexture::drawMode() const
{
	return d->m_drawMode.drawMode();
}


