#include "stable.h"
#include "dwgraphicsd3d_p.h"
#include "dwgraphicsdatad3d_p.h"

//------------------------------------------------------------------------------
/**
*/
LPDIRECT3D9 DwGraphicsD3D9::d3d9()
{
    return _dx.d3d9;
}

//------------------------------------------------------------------------------
/**
*/
LPDIRECT3DDEVICE9 DwGraphicsD3D9::device()
{
    return _dx.device;
}

//------------------------------------------------------------------------------
/**
*/
const D3DPRESENT_PARAMETERS& DwGraphicsD3D9::d3dpp()
{
    return _dx.d3dpp;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsD3D9::setup(HWND hWnd)
{
    if (!_dx.d3d9)
    {
        if(!(_dx.d3d9 = Direct3DCreate9( D3D_SDK_VERSION )))
        {
            DW_ASSERT("create d3d failed" && false);
        }
    }

    if (!_dx.device)
    {
        bool windowed = true;

        //Get display mode
        D3DDISPLAYMODE d3ddm;
        _dx.d3d9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

        ZeroMemory(&(_dx.d3dpp), 0);
        //need to check is the device is support
        _dx.d3dpp.BackBufferFormat = d3ddm.Format;
        _dx.d3dpp.hDeviceWindow             = hWnd;
        _dx.d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        _dx.d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
        _dx.d3dpp.Windowed = windowed;
        // if not set , will the createdevice return invalid call
        _dx.d3dpp.BackBufferWidth          = 1;
        _dx.d3dpp.BackBufferHeight         = 1;
        _dx.d3dpp.BackBufferCount          = 1;

        if (!windowed)
        {
            _dx.d3dpp.MultiSampleType          = D3DMULTISAMPLE_NONE;
            _dx.d3dpp.FullScreen_RefreshRateInHz       = D3DPRESENT_RATE_DEFAULT;
        }

        //Get device capabilities
        _dx.d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &_dx.d3dCaps);

        //Check if hardware vertex processing is available
        DWORD behaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
        if (_dx.d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
        {
            behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
        }

        //Create device with behaviorFlags
        HRESULT hr = _dx.d3d9->CreateDevice(D3DADAPTER_DEFAULT, // default graphics card (graphics hardware cards)
            D3DDEVTYPE_HAL,                                 // prefer a hardware device over a software device 
            hWnd,                                           // no window set
            behaviorFlags,                                  // hardware vertex processing 
            &_dx.d3dpp,                                         // the right d3d
            &_dx.device);                                   // [out] return the device

        if (FAILED(hr))
        {
            DW_ASSERT("create d3d device failed" && false);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsD3D9::setup(LPDIRECT3D9 d3d, LPDIRECT3DDEVICE9 dev, D3DPRESENT_PARAMETERS pp)
{
    _dx_release(_dx.device);
    _dx_release(_dx.d3d9);

    _dx.d3d9 = d3d;
    _dx.device = dev;
    _dx.d3d9->AddRef();
    _dx.device->AddRef();
    _dx.d3dpp = pp;
    _dx.d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &_dx.d3dCaps);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsD3D9::discard()
{
    _dx_release(_dx.device);
    _dx_release(_dx.d3d9);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsD3D9::beginPaint()
{
    DW_ASSERT(!_dx.isBegin);

    // tell d3d begin render
    _dx.device->BeginScene();
    _dx.device->Clear(0, 0, D3DCLEAR_TARGET,
        D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    _dx.isBegin = true;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsD3D9::endPaint()
{
    DW_ASSERT(_dx.isBegin);
    _dx.isBegin = false;

    _dx.device->EndScene();
    _dx.device->Present(0, 0, 0, 0);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsD3D9::setupStates()
{
    _dx.device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

    // no shaders initially
    _dx.device->SetVertexShader(0);
    _dx.device->SetPixelShader(0);

    // set device states
    _dx.device->SetRenderState(D3DRS_LIGHTING, FALSE);
    _dx.device->SetRenderState(D3DRS_FOGENABLE, FALSE);
    _dx.device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
    _dx.device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    _dx.device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    _dx.device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    _dx.device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
    _dx.device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

    // setup texture addressing settings
    _dx.device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    _dx.device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

    // setup colour calculations
    _dx.device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    _dx.device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    _dx.device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

    // setup alpha calculations
    _dx.device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    _dx.device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    _dx.device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

    // setup filtering
    _dx.device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    _dx.device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

    // disable texture stages we do not need.
    _dx.device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);

    // setup scene alpha blending
    _dx.device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
}

//------------------------------------------------------------------------------
/**
*/
struct D3DSize {int w, h;};
static int getSizeNextPOT(int size) 
{
    // if not power of 2
    if ((size & (size - 1)) || !size)
    {
        int log = 0;

        // get integer log of 'size' to base 2
        while (size >>= 1)
            ++log;

        // use log to calculate value to use as size.
        size = (2 << log);
    }

    return size;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsD3D9::adjustTexSize(DwSize& sz)
{
    D3DSize s = {sz.width(), sz.height()};
    if (!_dx.supportNPOTTex){
        s.w  = getSizeNextPOT(s.w);
        s.h = getSizeNextPOT(s.h);
    }
    if (!_dx.supportNonSquareTex)
        s.w = s.h = dwMax<int>(s.w, s.h);

    sz.setWidth(s.w);
    sz.setHeight(s.h);
}