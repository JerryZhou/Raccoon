#include "stable.h"
#include "dwtexture.h"
#include "dwgraphicsdatad3d_p.h"
#include "dwgraphicsd3d_p.h"


//------------------------------------------------------------------------------
/**
*/
DwTexture::DwTexture()
: DwGraphicsObject(new DwTextureData())
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwTexture::DwTexture(DwTextureData* dat)
: DwGraphicsObject(dat)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwTexture::~DwTexture()
{
}

//------------------------------------------------------------------------------
/**
*/
bool DwTexture::loadFromFile( const DwString & u)
{
    DwTextureData *d = data();
    _dx_release(d->texture);

    HRESULT hr = D3DXCreateTextureFromFileEx(DwGraphicsD3D9::device(), u.constString(), 
        0, 0, 1, 0,
        D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 0, 
        &d->textureInfo, NULL,  &d->texture);

    if (SUCCEEDED(hr))
    {
        updateScales();
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwTexture::loadFromMemory( const DwByteArray& dat )
{
    DwTextureData *d = data();
    _dx_release(d->texture);

    D3DXCreateTextureFromFileInMemoryEx(DwGraphicsD3D9::device(), 
        dat.data(), dat.size(), 0, 0, 1, 0,
        D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 0,
        &d->textureInfo, NULL, &d->texture);

    updateScales();
    return true;
}

//------------------------------------------------------------------------------
/**
*/
DwSize DwTexture::size() const
{
    const DwTextureData *d = data();
    return DwSize(d->textureInfo.Width, d->textureInfo.Height);
}

//------------------------------------------------------------------------------
/**
*/
DwSizeF DwTexture::scale() const
{
    const DwTextureData *d = data();
    return d->scales;
}

//------------------------------------------------------------------------------
/**
*/
void DwTexture::updateScales()
{
    DwTextureData *d = data();
    d->updateScales();
}

//------------------------------------------------------------------------------
/**
*/
int DwTexture::width() const
{
    const DwTextureData *d = data();
    return d->textureInfo.Width;
}

//------------------------------------------------------------------------------
/**
*/
int DwTexture::height() const
{
    const DwTextureData *d = data();
    return d->textureInfo.Height;
}
