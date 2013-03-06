#include "dwstable.h"
#include "dwcanvastexture.h"
#include "dwgraphicsdatad3d_p.h"
#include "dwgraphicsd3d_p.h"

//------------------------------------------------------------------------------
/**
*/
DwCanvasTexture::DwCanvasTexture()
: DwCanvas(new DwCanvasTextureData())
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwCanvasTexture::DwCanvasTexture(const DwSize& s)
: DwCanvas(new DwCanvasTextureData())
{
    setSize(s);
}

//------------------------------------------------------------------------------
/**
*/
DwCanvasTexture::~DwCanvasTexture()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvasTexture::beginPaint( const DwRect &, const DColor& c )
{
    DwCanvasTextureData *d = data();
    DW_ASSERT(d->surface);
    LPDIRECT3DSURFACE9 oldSurface = NULL;

    DwGraphicsD3D9::device()->GetRenderTarget(0, &oldSurface);
    if (oldSurface != d->surface){
        // save it
        d->preTargetSurface = oldSurface;
        // tell d3d the render target 
        DwGraphicsD3D9::device()->SetRenderTarget(0, d->surface);
    }
    else{
        if(oldSurface){
            oldSurface->Release();
        }
    }

    // clear the backbuffer
    DwGraphicsD3D9::device()->Clear( 0, 0, D3DCLEAR_TARGET, c, 1.0f, 0 );
    DwGraphicsD3D9::device()->BeginScene();

    // setup render states
    DwGraphicsD3D9::setupStates();
    // setup the d3d states
    setupBlend();
    // setup matrix
    setupMatrix();
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvasTexture::endPaint()
{
    DwGraphicsD3D9::device()->EndScene();
    // present the content to window
    DwGraphicsD3D9::device()->Present(NULL, NULL, NULL, NULL);

    DwCanvasTextureData *d = data();
    DwGraphicsD3D9::device()->SetRenderTarget(0, d->preTargetSurface);
    _dx_release(d->preTargetSurface);
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvasTexture::resize( const DwSize& s )
{
    DwCanvasTextureData *d = data();
    d->clean();

    d->textureData->textureInfo.Width = s.width();
    d->textureData->textureInfo.Height = s.height();
    d->textureData->textureInfo.Format = D3DFMT_A8R8G8B8;
    d->textureData->textureInfo.MipLevels = 1;
    d->textureData->textureInfo.ResourceType = D3DRTYPE_TEXTURE;
    d->textureData->textureInfo.ImageFileFormat = D3DXIFF_PNG;

    DwSize sz(s);
    DwGraphicsD3D9::adjustTexSize(sz);
    HRESULT hr = DwGraphicsD3D9::device()->CreateTexture(sz.width(), sz.height(),
        1, D3DUSAGE_RENDERTARGET,
        D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT,
        &d->textureData->texture, NULL);
    if (FAILED(hr))
    {
        DW_ASSERT("failed to create canvas texture" && false);
    }
    d->textureData->texture->GetSurfaceLevel(0, &d->surface);
    d->textureData->updateScales();
}

//------------------------------------------------------------------------------
/**
*/
DwTexture DwCanvasTexture::texture() const
{
    const DwCanvasTextureData *d = data();
    return DwTexture(d->textureData);
}
