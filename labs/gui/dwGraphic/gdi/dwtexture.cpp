#include "stable.h"
#include "dwbitmap.h"
#include "dwtexture.h"

class DwTexturePrivate: public DwSharedData
{
public:
	DwTexturePrivate();
	~DwTexturePrivate();

	FILTERMODE m_filterMode;
	DRAWMODE   m_drawMode;
	DwBitmap   m_bitmap;

};
DwTexturePrivate::DwTexturePrivate()
{
	ZeroMemory(&m_filterMode,sizeof(FILTERMODE));
	ZeroMemory(&m_drawMode,sizeof(DRAWMODE));
    m_bitmap.clear();
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

}
HGTEXTURE DwTexture::createBitmap(const DwString &file)
{
	bool bSuccess = d->m_bitmap.createBitmap(file);
	DW_ASSERT(bSuccess);

	HGTEXTURE hTexture;
	hTexture.lpvoid = this;
	return hTexture;

}

bool DwTexture::createBitmap(int nWidth, int nHeight)
{
	return d->m_bitmap.createBitmap(nWidth,nHeight);
}

unsigned int DwTexture::handle() const
{
	return d->m_bitmap.handle();
}
const  uint8_t *DwTexture::bits() const
{
	return d->m_bitmap.bits();
}

int DwTexture::height() const
{
	return d->m_bitmap.height();
}

int DwTexture::width() const
{
	return d->m_bitmap.width();
}

void DwTexture::setFilterMode(FILTERMODE filter)
{
	d->m_filterMode = filter;
}

void DwTexture::filterMode(LPFILTERMODE lpFilter) const
{
	*lpFilter = d->m_filterMode;
}

void DwTexture::setDrawMode(DRAWMODE drawMode)
{
	d->m_drawMode = drawMode;
}

void DwTexture::drawMode(LPDRAWMODE lpDrawMode) const
{
	*lpDrawMode = d->m_drawMode;
}


