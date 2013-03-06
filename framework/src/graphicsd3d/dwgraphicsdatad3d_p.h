#pragma once
#include "dwgraphicsdata.h"
#include "dwcore/dwglobalstatic.h"
#include "dwcore/dwsize.h"
#include "dwcore/dwstring.h"
#include "dwcore/dwrect.h"
#include "dwcore/dwpoint.h"
#include "dwcore/dwvector.h"
#include "dwcore/dwlinkedlist.h"

// D3D Data
//------------------------------------------------------------------------------
struct ContextD3D 
{
    LPDIRECT3D9 d3d9;                   // d3d
    LPDIRECT3DDEVICE9 device;           // device
    D3DPRESENT_PARAMETERS d3dpp;        // setting
    D3DCAPS9 d3dCaps;                   // caps
    //! What the renderer thinks the max texture size is.
    int maxTextureSize;
    //! whether the hardware supports non-power of two textures
    bool supportNPOTTex;
    //! whether the hardware supports non-square textures.
    bool supportNonSquareTex;
    bool    isBegin;                    // in scene painting
    ContextD3D() : d3d9(NULL), 
        device(NULL),
        isBegin(false),
        maxTextureSize(2048), supportNPOTTex(false),supportNonSquareTex(false) {
        ::ZeroMemory(&d3dpp, sizeof(d3dpp));
        ::ZeroMemory(&d3dCaps, sizeof(d3dCaps));
    }
    void updateCaps(){
        D3DCAPS9 caps;
        device->GetDeviceCaps(&caps);

        maxTextureSize = dwMin<int>(caps.MaxTextureHeight, caps.MaxTextureWidth);

        supportNonSquareTex = !(caps.TextureCaps & D3DPTEXTURECAPS_SQUAREONLY);

        supportNPOTTex = !(caps.TextureCaps & D3DPTEXTURECAPS_POW2) ||
            (caps.TextureCaps & D3DPTEXTURECAPS_NONPOW2CONDITIONAL);
    }
};
DW_GLOBAL_STATIC(ContextD3D, _dw_ContentD3D);
#define _dx (*_dw_ContentD3D())
#define _dx_release(p) if(p) p->Release(); p = NULL;

// Texture Data
//------------------------------------------------------------------------------
class DwTextureData : public DwGraphicsObjectData
{
public:
    LPDIRECT3DTEXTURE9 texture;
    D3DXIMAGE_INFO textureInfo;
    D3DSURFACE_DESC surfaceDesc;
    DwSizeF scales;

    DwTextureData() : texture(NULL), scales(0., 0.) {
        ::ZeroMemory(&textureInfo, sizeof(textureInfo));
        ::ZeroMemory(&surfaceDesc, sizeof(surfaceDesc));
    }
    virtual ~DwTextureData() { _dx_release(texture); }
    void updateScales(){
        if(texture){
            texture->GetLevelDesc(0, &surfaceDesc);
            scales.setWidth(surfaceDesc.Width ? 1.0/surfaceDesc.Width : 0.);
            scales.setHeight(surfaceDesc.Height ? 1.0/surfaceDesc.Height : 0.);
        }
    }
    void clean(){
        _dx_release(texture);
        ::ZeroMemory(&textureInfo, sizeof(textureInfo));
        ::ZeroMemory(&surfaceDesc, sizeof(surfaceDesc));
    }
};

// Effect Data
//------------------------------------------------------------------------------
class DwGraphicsEffectData : public DwGraphicsObjectData
{
public:
    LPD3DXEFFECT    effect;
    D3DXEFFECT_DESC effectDesc;
    // TODO: set the effect var

    DwGraphicsEffectData() : effect(NULL) {
        ::ZeroMemory(&effectDesc, sizeof(effectDesc));
    }
    virtual ~DwGraphicsEffectData() { _dx_release(effect); }

    const DwGraphicsEffectData* clone() const
    {
        return this;
    }
    void updateEffectDesc()
    {
        if (effect){
            effect->GetDesc(&effectDesc);
        }
    }
protected:
private:
};

// Canvas Data
//------------------------------------------------------------------------------
class DwCanvasData : public DwGraphicsObjectData
{
public:
    DwSize size;
    BlendMode blendMode;
    DwCanvasData():size(0, 0) ,
        blendMode(BM_NORMAL) {}
};

// GemetryBuffer Data
//------------------------------------------------------------------------------
struct DwBatch { 
    DwGraphicsObjectDataPtr<DwTextureData> textureData;
    int len; 
    DwBatch() : textureData(0), len(0) {} 
};
struct DCustomVertex 
{
    D3DXVECTOR3 position;
    D3DCOLOR color;    // The color
    FLOAT tu, tv;   // The texture coordinates
};
typedef DwLinkedList<DwBatch> DwBatchs;
typedef DwVector<DCustomVertex> DwVertexs;

class DwGeometryBufferData : public DwGraphicsObjectData
{
public:
    DwBatchs batchs;
    DwVertexs vertexs;
    //! translation vector
    DwVector3 translation;
    //! rotation vector
    DwVector3 rotation;
    //! pivot point for rotation
    DwVector3 pivot;
    //! model matrix cache
    mutable D3DXMATRIX matrix;
    //! true when d_matrix is valid and up to date
    mutable bool matrixValid;
    DwGraphicsObjectDataPtr<DwGraphicsEffectData> effectData;

    DwGeometryBufferData() :translation(0, 0, 0),
        rotation(0, 0, 0),
        pivot(0, 0, 0), 
        matrixValid(false)
    {
        ;
    }
};

// CanvasWindow Data
//------------------------------------------------------------------------------
class DwCanvasWindowData  : public DwCanvasData
{
public:
    HWND hWnd;
    LPDIRECT3DSWAPCHAIN9 swapChain;
    LPDIRECT3DSURFACE9 surface;
    LPDIRECT3DSURFACE9 offscreenSurface;

    DwCanvasWindowData() : hWnd(NULL), 
        swapChain(NULL), 
        surface(NULL),
        offscreenSurface(NULL){}
    virtual ~DwCanvasWindowData() { 
        _dx_release(surface);
        _dx_release(offscreenSurface);
        _dx_release(swapChain); 
    }
};

// 
//------------------------------------------------------------------------------
class DwCanvasTextureData : public DwCanvasData
{
public:
    DwTextureData *textureData;
    LPDIRECT3DSURFACE9 surface;
    LPDIRECT3DSURFACE9 preTargetSurface;

    DwCanvasTextureData() : textureData(new DwTextureData()), 
        surface(NULL), 
        preTargetSurface(NULL) {}
    virtual ~DwCanvasTextureData(){
        _dx_release(surface);
        _dx_release(preTargetSurface);
        _dx_release(textureData);
    }
    void clean(){
        _dx_release(surface);
        textureData->clean();
    }
protected:
private:
};