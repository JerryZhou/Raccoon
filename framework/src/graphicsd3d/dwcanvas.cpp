#include "stable.h"
#include "dwcanvas.h"
#include "dwgraphicsdatad3d_p.h"
#include "dwgraphicsd3d_p.h"

//----------------------------------------------------------------------------//
static const D3DMATRIX s_identityMatrix =
{
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0
};

//------------------------------------------------------------------------------
/**
*/
DwCanvas::DwCanvas()
: DwGraphicsObject(new DwCanvasData())
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwCanvas::~DwCanvas()
{
}


//------------------------------------------------------------------------------
/**
*/
void DwCanvas::setBlendMode(BlendMode mode)
{
    DwCanvasData *d = data();
    d->blendMode = mode;
}

//------------------------------------------------------------------------------
/**
*/
BlendMode DwCanvas::blendMode()const
{
    const DwCanvasData *d = data();
    return d->blendMode;
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvas::setupMatrix()
{
    const float fov = 0.523598776f;
    const float w = (float)width();
    const float h = (float)height();
    const float aspect = w / h;
    const float midx = w * 0.5f;
    const float midy = h * 0.5f;
    const float d_viewDistance = midx / (aspect * 0.267949192431123f);

    D3DXVECTOR3 eye(midx, midy, -d_viewDistance);
    D3DXVECTOR3 at(midx, midy, 1);
    D3DXVECTOR3 up(0, -1, 0);

    D3DXMATRIX d_matrix;
    D3DXMATRIX tmp;
    D3DXMatrixMultiply(&d_matrix,
        D3DXMatrixLookAtRH(&d_matrix, &eye, &at, &up),
        D3DXMatrixPerspectiveFovRH(&tmp, fov, aspect,
        d_viewDistance * 0.5f,
        d_viewDistance * 2.0f));

    D3DVIEWPORT9 vp;
    vp.X = 0;
    vp.Y = 0;
    vp.Width = width();
    vp.Height = height();
    vp.MinZ = 0.0f;
    vp.MaxZ = 1.0f;
    DwGraphicsD3D9::device()->SetViewport(&vp);
    DwGraphicsD3D9::device()->SetTransform(D3DTS_PROJECTION, &d_matrix);
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvas::setupBlend()
{
    DwCanvasData *d = data();
    LPDIRECT3DDEVICE9 device = DwGraphicsD3D9::device();

    if (d->blendMode == BM_RTT_PREMULTIPLIED)
    {
        device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, FALSE);
        device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
        device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    }
    else
    {
        device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
        device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA);
        device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);
    }

    DwGraphicsD3D9::device()->SetTransform(D3DTS_VIEW, &s_identityMatrix);
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvas::setSize(const DwSize& s)
{
    DwCanvasData *d = data();

    if (d->size == s)
    {
        return;
    }
    d->size = s;
    resize(s);
}

//------------------------------------------------------------------------------
/**
*/
DwSize DwCanvas::size() const
{
    const DwCanvasData *d = data();
    return d->size;
}

//------------------------------------------------------------------------------
/**
*/
int DwCanvas::width() const
{
    const DwCanvasData *d = data();
    return d->size.width();
}

//------------------------------------------------------------------------------
/**
*/
int DwCanvas::height() const
{
    const DwCanvasData *d = data();
    return d->size.height();
}

//------------------------------------------------------------------------------
/**
*/
DwCanvas::DwCanvas(DwCanvasData *d)
: DwGraphicsObject(d)
{
    ;
}