#include "stable.h"
#include "dwcanvaswindow.h"
#include "dwgraphicsdatad3d_p.h"
#include "dwgraphicsd3d_p.h"

//------------------------------------------------------------------------------
/**
*/
DwCanvasWindow::DwCanvasWindow()
: DwCanvas(new DwCanvasWindowData())
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwCanvasWindow::~DwCanvasWindow()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvasWindow::setup(HWND hWnd)
{
    DwCanvasWindowData *d = data();
    DW_ASSERT(!d->hWnd);
    DW_ASSERT(!d->swapChain);
    d->hWnd = hWnd;
    RECT cRect = {0, 0, 0, 0};
    if (hWnd) {
        ::GetWindowRect(hWnd, &cRect);
    }
    setSize(DwSize(cRect.right - cRect.left, cRect.bottom - cRect.top));
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvasWindow::beginPaint( const DwRect &, const DColor& c )
{
    DwCanvasWindowData *d = data();
    DW_ASSERT(d->surface);
    
    // tell d3d the render target 
    DwGraphicsD3D9::device()->SetRenderTarget(0, d->surface);
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
void DwCanvasWindow::endPaint()
{
    DwCanvasWindowData *d = data();
    DW_ASSERT(d->swapChain);
    DwGraphicsD3D9::device()->EndScene();
    // present the content to window
    d->swapChain->Present(NULL, NULL, NULL, NULL, NULL);
    // if the window is translucent, update it
    if (isNeedOffscreenSurface())
    {
        setupOffscreenSurface();
        updateOffscreenSurface();
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvasWindow::resize( const DwSize& s )
{
    DwCanvasWindowData *d = data();
    _dx_release(d->surface);
    _dx_release(d->offscreenSurface);
    _dx_release(d->swapChain);

    // will init with the default d3d valid setting
    D3DPRESENT_PARAMETERS pp = DwGraphicsD3D9::d3dpp();
    pp.hDeviceWindow = d->hWnd;
    pp.BackBufferWidth = s.width();
    pp.BackBufferHeight = s.height();

    // create the render target
    if (FAILED(DwGraphicsD3D9::device()->CreateAdditionalSwapChain(&pp, &d->swapChain))
        || FAILED(d->swapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &d->surface))){
        DW_ASSERT("failed to resize the window" && false);
        _dx_release(d->surface);
        _dx_release(d->swapChain);
    }

    // create the offscreen used to updateLayer window
    D3DSURFACE_DESC desc;
    d->surface->GetDesc(&desc);
    if (FAILED( DwGraphicsD3D9::device()->CreateOffscreenPlainSurface(desc.Width, desc.Height, 
        desc.Format, 
        D3DPOOL_SYSTEMMEM, &d->offscreenSurface, NULL) ))
    {
        DW_ASSERT("failed to resize the window" && false);
        _dx_release(d->surface);
        _dx_release(d->offscreenSurface);
        _dx_release(d->swapChain);
    }

    // resize will recreate the offscreen surface
    if (isNeedOffscreenSurface())
    {
        setupOffscreenSurface();
    }
}

//------------------------------------------------------------------------------
/**
*/
void  DwCanvasWindow::setupOffscreenSurface()
{
    DwCanvasWindowData *d = data();
    if (!d->offscreenSurface)
    {
        // create the offscreen used to updateLayer window
        D3DSURFACE_DESC desc;
        d->surface->GetDesc(&desc);
        if (FAILED( DwGraphicsD3D9::device()->CreateOffscreenPlainSurface(desc.Width, desc.Height, 
            desc.Format, 
            D3DPOOL_SYSTEMMEM, &d->offscreenSurface, NULL) ))
        {
            DW_ASSERT("failed to resize the window" && false);
            _dx_release(d->surface);
            _dx_release(d->offscreenSurface);
            _dx_release(d->swapChain);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
bool DwCanvasWindow::isNeedOffscreenSurface()
{
    DwCanvasWindowData *d = data();
    DWORD exstyle = (DWORD)GetWindowLong(d->hWnd, GWL_EXSTYLE);
    // only the layered window need
    if (exstyle & WS_EX_LAYERED)
    {
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void DwCanvasWindow::updateOffscreenSurface()
{
    DwCanvasWindowData *d = data();

    LPDIRECT3DDEVICE9 device = DwGraphicsD3D9::device();
    device->GetRenderTargetData(d->surface, d->offscreenSurface);

    HDC surfaceDc;
    d->offscreenSurface->GetDC(&surfaceDc);
    HDC hdc = ::GetDC(d->hWnd);
    tagRECT rcWin;
    ::GetWindowRect(d->hWnd, &rcWin);
    tagPOINT ptWin = { rcWin.left, rcWin.top};
    tagSIZE szWin = { rcWin.right - rcWin.left, rcWin.bottom - rcWin.top};
    tagPOINT ptSrc = { 0, 0 };
    BLENDFUNCTION bfPixelFunction = { AC_SRC_OVER, 0, 255,  1};
    // we assume the offscreen surface have the format: D3DFMT_A8R8G8B8
    ::UpdateLayeredWindow(d->hWnd, hdc, &ptWin, &szWin, surfaceDc, &ptSrc, RGB(0, 0, 0), &bfPixelFunction, ULW_ALPHA);
    ::ReleaseDC(d->hWnd, hdc);
    d->offscreenSurface->ReleaseDC(surfaceDc);
}
