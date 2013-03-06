#include "stable.h"
#include "dwgraphicsd3d_p.h"
#include "dwcanvaswindow.h"
#include "dwtexture.h"
#include "dwgeometrybuffer.h"
#include "dwcore/dwmargins.h"
#include "dwgraphicsd3d_p.h"

//------------------------------------------------------------------------------
/**
*/
LRESULT CALLBACK	DWndProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_NCHITTEST:
        return HTCAPTION;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

HWND window()
{
    LPCWSTR szWindowClass = L"szWindowClass";

    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(wcex));
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc	= DWndProc2;
    wcex.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= NULL;
    wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= (HBRUSH)(NULL_BRUSH);
    wcex.lpszMenuName  = NULL;
    wcex.lpszClassName	= szWindowClass;
    wcex.hIconSm       = NULL;
    RegisterClassEx(&wcex);

    // get size of window to use so that client area is desired size
    RECT wnd_size = {0,0,800,600};
    AdjustWindowRect(&wnd_size, WS_POPUP | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX, FALSE);

    HWND hWnd = CreateWindowEx(
        WS_EX_APPWINDOW,//WS_EX_LAYERED | WS_EX_APPWINDOW, 
        szWindowClass, L"Raccoon", 
        WS_POPUP | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX ,
        CW_USEDEFAULT, 0, wnd_size.right - wnd_size.left, wnd_size.bottom - wnd_size.top,
        NULL, NULL, NULL, NULL);
    return hWnd;
}

//----------------------------------------------------------------------------//
static const D3DMATRIX s_identityMatrix =
{
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0
};
struct D3DContext 
{
    LPDIRECT3D9 d3d9;                   // d3d
    LPDIRECT3DDEVICE9 device;           // device
    D3DPRESENT_PARAMETERS d3dpp;        // setting
    D3DCAPS9 d3dCaps;
};
D3DContext context;

struct D3DRectF 
{
    FLOAT left, right, top, bottom;
    D3DRectF(FLOAT l, FLOAT t, FLOAT r, FLOAT b)
        :left(l), right(r), top(t), bottom(b){
    }
};

D3DRectF dstRect2(const DwRect &dstR)
{
#define _D3DFLOAT (FLOAT)
    return D3DRectF(_D3DFLOAT dstR.left() - 0.5f, _D3DFLOAT dstR.top() - 0.5f, _D3DFLOAT dstR.right()+1 - 0.5f, _D3DFLOAT dstR.bottom()+1 - 0.5f);
}

D3DRectF srcRect2(const DwRect &srcR, const DwTexture& texture)
{
    FLOAT dx = (FLOAT)texture.scale().width();
    FLOAT dy = (FLOAT)texture.scale().height();
    return D3DRectF(srcR.left()*dx, srcR.top()*dy, (srcR.right()+1)*dx, (srcR.bottom()+1)*dy);
}
struct DCustomVertex2
{
    FLOAT x, y, z;
    float rhw;
    D3DCOLOR diffuse;    // The color
    FLOAT tu, tv;   // The texture coordinates
    DCustomVertex2()
    {
        rhw = 1.0f;
    }
};

struct TLVERTEX
{
    float x;
    float y;
    float z;
    float rhw;
    D3DCOLOR colour;
    float u;
    float v;
};

LPDIRECT3DTEXTURE9 drawTexture = NULL;
D3DXIMAGE_INFO textureInfo = {0};
D3DSURFACE_DESC surfaceDesc;
D3DVIEWPORT9 viewPort = {0, 0, 800, 600, 0.0f, 1.0f};
DCustomVertex2 vertex[6];
//Vertex buffer for drawing quads
IDirect3DVertexBuffer9* vertexBuffer;
void setupData()
{
    D3DXCreateTextureFromFileEx (context.device, 
        L"K:\\Project\\raccoon\\bin\\testdata\\mu\\eg.bmp", 0, 0, 1, 0,
        D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT,
        0, &textureInfo, NULL, &drawTexture);

    drawTexture->GetLevelDesc(0, &surfaceDesc);

    //TLVERTEX* vertices;

    ////Lock the vertex buffer
    //vertexBuffer->Lock(0, 0, (void **)&vertices, NULL);

    //D3DRectF  dst = dstRect2(DwRect(0, 0, textureInfo.Width, textureInfo.Height));
    //D3DRectF src = D3DRectF(0.0f, 0.0f, 1.0f, 1.0f);
    //D3DCOLOR c = D3DCOLOR_XRGB(0, 255, 255);

    //vertex[0].x = dst.left;
    //vertex[0].y = dst.top;
    //vertex[0].z = 0;
    //vertex[0].tu = src.left;
    //vertex[0].tv = src.top;
    //vertex[0].diffuse = c;

    ///*vertex[1].x = dst.left;
    //vertex[1].y = dst.bottom;
    //vertex[1].z = 0;
    //vertex[1].tu = src.left;
    //vertex[1].tv = src.bottom;
    //vertex[1].diffuse = c;

    //vertex[2].x = dst.right;
    //vertex[2].y = dst.bottom;
    //vertex[2].z = 0;
    //vertex[2].tu = src.right;
    //vertex[2].tv = src.bottom;
    //vertex[2].diffuse = c;*/

    ///*vertex[3].x = dst.right;
    //vertex[3].y = dst.top;
    //vertex[3].z = 0;
    //vertex[3].tu = src.right;
    //vertex[3].tv = src.top;
    //vertex[3].diffuse = c;*/

    //vertex[1].x = dst.right;
    //vertex[1].y = dst.top;
    //vertex[1].z = 0;
    //vertex[1].tu = src.right;
    //vertex[1].tv = src.top;
    //vertex[1].diffuse = c;

    ///*vertex[4].x = dst.left;
    //vertex[4].y = dst.top;
    //vertex[4].z = 0;
    //vertex[4].tu = src.left;
    //vertex[4].tv = src.top;
    //vertex[4].diffuse = c;*/

    ///*vertex[5].x = dst.right;
    //vertex[5].y = dst.bottom;
    //vertex[5].z = 0;
    //vertex[5].tu = src.right;
    //vertex[5].tv = src.bottom;
    //vertex[5].diffuse = c;*/

    //vertex[2].x = dst.right;
    //vertex[2].y = dst.bottom;
    //vertex[2].z = 0;
    //vertex[2].tu = src.right;
    //vertex[2].tv = src.bottom;
    //vertex[2].diffuse = c;

    //vertex[3].x = dst.left;
    //vertex[3].y = dst.bottom;
    //vertex[3].z = 0;
    //vertex[3].tu = src.left;
    //vertex[3].tv = src.bottom;
    //vertex[3].diffuse = c;

    ////Unlock the vertex buffer
    //vertexBuffer->Unlock();
}

const DWORD D3DFVF_TLVERTEX = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

void initD3D(HWND hWnd)
{
    context.d3d9 = Direct3DCreate9( D3D_SDK_VERSION );
    D3DDISPLAYMODE d3ddm;
    context.d3d9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
    ZeroMemory(&(context.d3dpp), 0);
    //need to check is the device is support
    context.d3dpp.BackBufferFormat = d3ddm.Format;
    context.d3dpp.hDeviceWindow = NULL;
    context.d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    context.d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    context.d3dpp.Windowed = TRUE;
    // if not set , will the createdevice return invalid call
    context.d3dpp.BackBufferWidth          = 800;
    context.d3dpp.BackBufferHeight         = 600;
    context.d3dpp.BackBufferCount          = 1;
    context.d3dpp.hDeviceWindow             = hWnd;

    context.d3d9->CreateDevice(D3DADAPTER_DEFAULT, // default graphics card (graphics hardware cards)
        D3DDEVTYPE_HAL,                                 // prefer a hardware device over a software device 
        hWnd,                                           // no window set
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,            // hardware vertex processing 
        &context.d3dpp,                                         // the right d3d
        &context.device);
}

//Initialize Direct3D
int InitD3D (int resWidth, int resHeight, D3DFORMAT resFormat, HWND hWnd, BOOL bWindowedMode = false)
{
    HRESULT hr;

    //Make Direct3D object
    context.d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

    //Make sure NULL pointer was not returned
    if (!context.d3d9)
        return FALSE;

    //Get device capabilities
    ZeroMemory (&context.d3dCaps, sizeof(context.d3dCaps));
    if (FAILED(context.d3d9->GetDeviceCaps (D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &context.d3dCaps)))
        return FALSE;

    //Setup present parameters
    ZeroMemory(&context.d3dpp,sizeof(context.d3dpp));
    context.d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
    context.d3dpp.hDeviceWindow = hWnd;
    context.d3dpp.BackBufferCount = 1;

    //Check if windowed
    if (bWindowedMode)
    {
        D3DDISPLAYMODE d3ddm;
        RECT rWindow;

        //Get display mode
        context.d3d9->GetAdapterDisplayMode (D3DADAPTER_DEFAULT, &d3ddm);

        //Get window bounds
        GetClientRect (hWnd, &rWindow);

        //Setup screen dimensions
        resWidth = rWindow.right - rWindow.left;
        resHeight = rWindow.bottom - rWindow.top;

        //Setup backbuffer
        context.d3dpp.Windowed = true;
        context.d3dpp.BackBufferFormat = d3ddm.Format;
        context.d3dpp.BackBufferWidth = rWindow.right - rWindow.left;
        context.d3dpp.BackBufferHeight = rWindow.bottom - rWindow.top;
    }
    else
    {        
        context.d3dpp.Windowed = false;
        context.d3dpp.BackBufferWidth = resWidth;
        context.d3dpp.BackBufferHeight = resHeight;
        context.d3dpp.BackBufferFormat = resFormat;
    }

    //Check if hardware vertex processing is available
    if (context.d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
    {    
        //Create device with hardware vertex processing
        hr = context.d3d9->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL, hWnd,
            D3DCREATE_HARDWARE_VERTEXPROCESSING, &context.d3dpp, &context.device);        
    }
    else
    {
        //Create device with software vertex processing
        hr = context.d3d9->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL, hWnd,
            D3DCREATE_SOFTWARE_VERTEXPROCESSING, &context.d3dpp, &context.device);
    }

    //Make sure device was created
    if (FAILED(hr))
        return FALSE;

    //Set vertex shader
    context.device->SetVertexShader(NULL);
    context.device->SetFVF (D3DFVF_TLVERTEX);

    //Create vertex buffer and set as stream source
    context.device->CreateVertexBuffer(sizeof(TLVERTEX) * 4, NULL, D3DFVF_TLVERTEX, D3DPOOL_MANAGED,
        &vertexBuffer, NULL);
    context.device->SetStreamSource (0, vertexBuffer, 0, sizeof(TLVERTEX));

    //Setup rendering states
    context.device->SetRenderState(D3DRS_LIGHTING, FALSE);
    context.device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    context.device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    context.device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    context.device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

    //Successfully initalized Direct3D
    return TRUE;
}

//Close Direct3D
int CloseD3D ()
{
    //Release vertex buffer
    if (vertexBuffer)
        vertexBuffer->Release ();

    //Release device
    if (context.device)
        context.device->Release();

    //Release d3d object
    if (context.d3d9)
        context.d3d9->Release();

    //Successfully closed Direct3D
    return TRUE;
}

void setupBlend()
{
    LPDIRECT3DDEVICE9 device = context.device;

    HRESULT hr = S_OK;
    hr = device->SetFVF(D3DFVF_TLVERTEX);

    // no shaders initially
    hr = device->SetVertexShader(0);
    //hr = device->SetPixelShader(0);

    // set device states
    hr = device->SetRenderState(D3DRS_LIGHTING, FALSE);
    hr = device->SetRenderState(D3DRS_FOGENABLE, FALSE);
    hr = device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
    hr = device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    hr = device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    hr = device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    hr = device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
    hr = device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

    // setup texture addressing settings
    hr = device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    hr = device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

    // setup colour calculations
    hr = device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    hr = device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    hr = device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

    // setup alpha calculations
    hr = device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    hr = device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    hr = device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

    // setup filtering
    hr = device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    hr = device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

    // disable texture stages we do not need.
    hr = device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);

    // setup scene alpha blending
    hr = device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

    /*if (BM_RTT_PREMULTIPLIED)
    {
        hr = device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, FALSE);
        hr = device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
        hr = device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    }
    else*/
    {
        hr = device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
        hr = device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        hr = device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        hr = device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA);
        hr = device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);
    }
}

void setupMatrix()
{
    const float fov = 0.523598776f;
    const float w = (float)800;
    const float h = (float)600;
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

    context.device->SetViewport(&viewPort);
    context.device->SetTransform(D3DTS_WORLD, &s_identityMatrix);
    context.device->SetTransform(D3DTS_VIEW, &s_identityMatrix);
    context.device->SetTransform(D3DTS_PROJECTION, &d_matrix);
}

void drawUI(int X, int Y)
{
    //context.device->SetTransform(D3DTS_WORLD, &s_identityMatrix);
    /*context.device->SetTexture(0, drawTexture);
    context.device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2,
        vertex, sizeof(DCustomVertex2));*/

    D3DCOLOR vertexColour = 0xFFFFFFFF;
    RECT rcDest;

    //Setup destination rectangle
    rcDest.left = X;
    rcDest.right = X + surfaceDesc.Width;
    rcDest.top = Y;
    rcDest.bottom = Y + surfaceDesc.Height;

    RECT *rDest=&rcDest;

    TLVERTEX* vertices;

    //Lock the vertex buffer
    vertexBuffer->Lock(0, 0, (void **)&vertices, NULL);

    //Setup vertices
    //A -0.5f modifier is applied to vertex coordinates to match texture and screen coords
    //Some drivers may compensate for this automatically, but on others texture alignment errors are introduced
    //More information on this can be found in the Direct3D 9 documentation
    vertices[0].colour = vertexColour;
    vertices[0].x = (float) rDest->left - 0.5f;
    vertices[0].y = (float) rDest->top - 0.5f;
    vertices[0].z = 0.0f;
    vertices[0].rhw = 1.0f;
    vertices[0].u = 0.0f;
    vertices[0].v = 0.0f;

    vertices[1].colour = vertexColour;
    vertices[1].x = (float) rDest->right - 0.5f;
    vertices[1].y = (float) rDest->top - 0.5f;
    vertices[1].z = 0.0f;
    vertices[1].rhw = 1.0f;
    vertices[1].u = 1.0f;
    vertices[1].v = 0.0f;

    vertices[2].colour = vertexColour;
    vertices[2].x = (float) rDest->right - 0.5f;
    vertices[2].y = (float) rDest->bottom - 0.5f;
    vertices[2].z = 0.0f;
    vertices[2].rhw = 1.0f;
    vertices[2].u = 1.0f;
    vertices[2].v = 1.0f;

    vertices[3].colour = vertexColour;
    vertices[3].x = (float) rDest->left - 0.5f;
    vertices[3].y = (float) rDest->bottom - 0.5f;
    vertices[3].z = 0.0f;
    vertices[3].rhw = 1.0f;
    vertices[3].u = 0.0f;
    vertices[3].v = 1.0f;

    //Unlock the vertex buffer
    vertexBuffer->Unlock();

    //Set texture
    context.device->SetTexture (0, drawTexture);

    //Draw image
    context.device->DrawPrimitive (D3DPT_TRIANGLEFAN, 0, 2);
}

void render_ui2()
{
    context.device->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 0, 0), 1.0f, 0);
    context.device->BeginScene();
    //setupMatrix();
    //setupBlend();
    drawUI(0, 0);
    context.device->EndScene();
    context.device->Present(0, 0, 0, 0);
}

void eventLoop2()
{
    bool quit = false;
    while (!quit)
    {
        MSG msg = {0};
        //if(
        while (
            PeekMessage( &msg, NULL, 0, 0, PM_REMOVE )
            )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );

            if (msg.message == WM_QUIT)
            {
                quit = true;
                //break;
            }
        }

        render_ui2();
    }
}

int WINAPI WinMain( __in HINSTANCE , __in_opt HINSTANCE , __in_opt LPSTR , __in int  )
{
    HWND hWnd = window();
    InitD3D(0, 0, D3DFMT_A8R8G8B8, hWnd, true);
    ::ShowWindow(hWnd, SW_SHOWNORMAL);
    ::UpdateWindow(hWnd);
    setupData();

    eventLoop2();
    CloseD3D();
    return 0;
}