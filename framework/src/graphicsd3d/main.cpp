#include "stable.h"
#include "dwgraphicsd3d_p.h"
#include "dwgraphicsdatad3d_p.h"
#include "dwcanvaswindow.h"
#include "dwcanvastexture.h"
#include "dwtexture.h"
#include "dwgeometrybuffer.h"
#include "dwgraphicseffect.h"

DwCanvasWindow canvas;
DwCanvasWindow canvasWin;
DwTexture texture;
DwGeometryBuffer geometry;
DwTexture ninePatchTexture;
DwGraphicsEffect effect;

DwGeometryBuffer geometryTexture;
DwCanvasTexture canvasTexture;
DwTexture textureFromCanvas = canvasTexture.texture();

//------------------------------------------------------------------------------
/**
*/
LRESULT CALLBACK	DWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_NCHITTEST:
        return HTCAPTION;
    case WM_SIZE:
        {
            if (canvas.size().width())
            {
                RECT cRect = {0, 0, 0, 0};
                if (hWnd) {
                    ::GetWindowRect(hWnd, &cRect);
                }
                canvas.setSize(DwSize(cRect.right - cRect.left, cRect.bottom - cRect.top));
            }
        }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void setupCanvas(DwCanvasWindow& canvas)
{
    LPCWSTR szWindowClass = L"szWindowClass";

    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(wcex));
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc	= DWndProc;
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

    ::ShowWindow(hWnd, SW_SHOWNORMAL);
    ::UpdateWindow(hWnd);

    static bool d3d_init = false;
    if (!d3d_init)
    {
        DwGraphicsD3D9::setup(hWnd);
        d3d_init = true;
    }
    
    canvas.setup(hWnd);
}

struct D3DRectF 
{
    FLOAT left, right, top, bottom;
    D3DRectF(FLOAT l, FLOAT t, FLOAT r, FLOAT b)
        :left(l), right(r), top(t), bottom(b){
    }
};

struct D3DRect
{
    DWORD left, right, top, bottom;
    D3DRect(DWORD l, DWORD t, DWORD r, DWORD b)
        :left(l), right(r), top(t), bottom(b){
    }
};

D3DRectF dstRect(const DwRect &dstR)
{
#define _D3DFLOAT (FLOAT)
    return D3DRectF(_D3DFLOAT dstR.left(), _D3DFLOAT dstR.top(), _D3DFLOAT dstR.right()+1, _D3DFLOAT dstR.bottom()+1);
}

D3DRectF srcRect(const DwRect &srcR, const DwTexture& texture)
{
    FLOAT dx = (FLOAT)texture.scale().width();
    FLOAT dy = (FLOAT)texture.scale().height();
    return D3DRectF(srcR.left()*dx, srcR.top()*dy, (srcR.right()+1)*dx, (srcR.bottom()+1)*dy);
}

void drawTexture(DwGeometryBuffer & geometry, const DwTexture& texture, const DwRect &dstR, const DwRect &srcR, DColor c)
{
    D3DRectF  dst = dstRect(dstR);
    D3DRectF src = srcRect(srcR, texture);

    DwVertex vertex[6];
    vertex[0].x = dst.left;
    vertex[0].y = dst.top;
    vertex[0].z = 0;
    vertex[0].tu = src.left;
    vertex[0].tv = src.top;
    vertex[0].diffuse = c;

    vertex[1].x = dst.left;
    vertex[1].y = dst.bottom;
    vertex[1].z = 0;
    vertex[1].tu = src.left;
    vertex[1].tv = src.bottom;
    vertex[1].diffuse = c;

    vertex[2].x = dst.right;
    vertex[2].y = dst.bottom;
    vertex[2].z = 0;
    vertex[2].tu = src.right;
    vertex[2].tv = src.bottom;
    vertex[2].diffuse = c;

    vertex[3].x = dst.right;
    vertex[3].y = dst.top;
    vertex[3].z = 0;
    vertex[3].tu = src.right;
    vertex[3].tv = src.top;
    vertex[3].diffuse = c;

    vertex[4].x = dst.left;
    vertex[4].y = dst.top;
    vertex[4].z = 0;
    vertex[4].tu = src.left;
    vertex[4].tv = src.top;
    vertex[4].diffuse = c;

    vertex[5].x = dst.right;
    vertex[5].y = dst.bottom;
    vertex[5].z = 0;
    vertex[5].tu = src.right;
    vertex[5].tv = src.bottom;
    vertex[5].diffuse = c;

    geometry.setTexture(&texture);
    geometry.addGeometry(vertex, 6);
}
int cRed = D3DCOLOR_XRGB(255, 0, 0);
int cGreen = D3DCOLOR_XRGB(0, 255, 0);
int cBlue = D3DCOLOR_XRGB(0, 0, 255);
void drawTextureNinePatch(DwGeometryBuffer & geometry, const DwTexture& texture, 
                          const DwRect &dst, const DwRect &src, const DwMargins& margins, DColor c)
{

    int cx1 = margins.left();//texture.width() / 3 + 5;
    int cx3 = margins.right();//texture.width() / 3 - 5;
    int cx2 = src.width() - cx1 - cx3;
    int cy1 = margins.top();//texture.height() / 3 - 5;
    int cy3 = margins.bottom();//texture.height() / 3 + 5;
    int cy2 = src.height() - cy1 - cy3;

    int sleft = src.left();
    int stop = src.top();

    //corners
    if (cx1)
    {
        if (cy1) drawTexture(geometry, texture,
            DwRect(dst.left(), dst.top(), cx1, cy1), 
            DwRect(sleft, stop, cx1, cy1), c);
        if (cy3) drawTexture(geometry, texture,
            DwRect(dst.left(), dst.top() + dst.height() - cy3, cx1, cy3), 
            DwRect(sleft, stop + cy1 + cy2, cx1, cy3), c);
    }

    if (cx3)
    {
        if(cy1) drawTexture(geometry, texture, 
            DwRect(dst.left() + dst.width() - cx3, dst.top(), cx3, cy1), 
            DwRect(sleft + cx1 + cx2, stop, cx3, cy1), c);
        if(cy3) drawTexture(geometry, texture, 
            DwRect(dst.left() + dst.width() - cx3, dst.top() + dst.height() - cy3, cx3, cy3), 
            DwRect(sleft + cx1 + cx2, stop + cy1 + cy2, cx3, cy3), c);
    }

    //////////////////////////////////////////////////////////////////////////
    #define _dw_v_m(m) ((m) ? 1 : 0)
    int ws = _dw_v_m(cx1) + _dw_v_m(cx3);
    int hs = _dw_v_m(cy1) + _dw_v_m(cy3);

    //top bottom border
    if (cx2 > ws)
    {
        if (cy1) drawTexture(geometry, texture, 
            DwRect(dst.left() + cx1 , dst.top(), dst.width() - cx1 - cx3, cy1), 
            DwRect(sleft + cx1 + _dw_v_m(cx1), stop, cx2-ws, cy1), c);
        if (cy3) drawTexture(geometry, texture, 
            DwRect(dst.left() + cx1, dst.top() + dst.height() - cy3, dst.width() - cx1 - cx3, cy3), 
            DwRect(sleft + cx1 + _dw_v_m(cx1), stop + cy1 + cy2, cx2-ws, cy3), c);
    }

    //left right border
    if(cy2 > hs)
    {
        if(cx1) drawTexture(geometry, texture, 
            DwRect(dst.left(), dst.top() + cy1, cx1, dst.height() - cy1 - cy3), 
            DwRect(sleft, stop + cy1 + _dw_v_m(cy1), cx1, cy2 - hs), c);
        if(cx3) drawTexture(geometry, texture, 
            DwRect(dst.left() + dst.width() - cx3, dst.top() + cy1, cx3, dst.height() - cy1 - cy3), 
            DwRect(sleft + cx1 + cx2, stop + cy1 + _dw_v_m(cy1), cx3, cy2 - hs), c);
    }

    {
        int mleft = margins.left();
        int mtop = margins.top();
        int mright = margins.right();
        int mbottom = margins.bottom();
#define _v_len(len) ((len) ? ((len) + 1) : (len) )

        DwRect cdstRect = dst;
        cdstRect.adjust(mleft, mtop, -mright, -mbottom);
        DwRect csrcRect = src;
        csrcRect.adjust(_v_len(mleft), _v_len(mtop), -_v_len(mright), -_v_len(mbottom));
        drawTexture(geometry, texture, cdstRect, csrcRect, c);
    }
}

struct DPos
{
    float x, y, z;
    DPos(float _x, float _y, float _z):x(_x), y(_y), z(_z) {}
    DPos(){}
};

struct DRect
{
    DPos lt;
    DPos lb;
    DPos rb;
    DPos rt;
};

// v0. lt, v1. lb, v2. rb, v3. rt
DwVertex *plans(const DPos& v0, 
                const DPos& v1, 
                const DPos& v2, 
                const DPos& v3, 
                const DwRect &srcR, 
                const DwTexture& texture, 
                DColor c)
{
    static DwVertex vbox[6];
    D3DRectF src = srcRect(srcR, texture);

    vbox[0].x = v0.x;
    vbox[0].y = v0.y;
    vbox[0].z = v0.z;
    vbox[0].tu = src.left;
    vbox[0].tv = src.top;
    vbox[0].diffuse = c;

    vbox[1].x = v1.x;
    vbox[1].y = v1.y;
    vbox[1].z = v1.z;
    vbox[1].tu = src.left;
    vbox[1].tv = src.bottom;
    vbox[1].diffuse = c;

    vbox[2].x = v2.x;
    vbox[2].y = v2.y;
    vbox[2].z = v2.z;
    vbox[2].tu = src.right;
    vbox[2].tv = src.bottom;
    vbox[2].diffuse = c;

    vbox[3].x = v3.x;
    vbox[3].y = v3.y;
    vbox[3].z = v3.z;
    vbox[3].tu = src.right;
    vbox[3].tv = src.top;
    vbox[3].diffuse = c;

    vbox[4].x = v0.x;
    vbox[4].y = v0.y;
    vbox[4].z = v0.z;
    vbox[4].tu = src.left;
    vbox[4].tv = src.top;
    vbox[4].diffuse = c;

    vbox[5].x = v2.x;
    vbox[5].y = v2.y;
    vbox[5].z = v2.z;
    vbox[5].tu = src.right;
    vbox[5].tv = src.bottom;
    vbox[5].diffuse = c;

    return vbox;
}


void drawTexture(DwGeometryBuffer & geometry,  const DwTexture& tex, const DRect dst, const DwRect &srcR, DColor c)
{
    DwVertex *v = plans(dst.lt, dst.lb, dst.rb, dst.rt, srcR, tex, c);

    geometry.setTexture(&tex);
    geometry.addGeometry(v, 6);
}

void setupApp()
{
    setupCanvas(canvas);
    //setupCanvas(canvasWin);
    canvasTexture.setSize(800, 600);
    effect.loadFromFile("H:\\Project\\raccoon\\bin\\SimpleEffect.fxo");
    //geometry.setEffect(&effect);

    int c = D3DCOLOR_XRGB(255, 255, 255);

    texture.loadFromFile("H:\\Project\\raccoon\\bin\\testdata\\mu\\eg.bmp");
    drawTexture(geometry, texture, 
        DwRect(100, 100, texture.width(), texture.height()) , 
        DwRect(0, 0, texture.width(), texture.height()), c);

    DwGraphicsD3D9::device()->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

    //- box
    /*DRect dst;
    dst.lt = DPos(100, 200, 0);
    dst.lb = DPos(200, 200, 0);
    dst.rb = DPos(200, 200, 100);
    dst.rt = DPos(100, 200, 100);
    drawTexture(geometry, texture, dst, DwRect(0, 0, texture.width(), texture.height()), c);

    dst.lt = DPos(100, 100, 0);
    dst.lb = DPos(100, 200, 0);
    dst.rb = DPos(200, 200, 0);
    dst.rt = DPos(200, 100, 0);
    drawTexture(geometry, texture, dst, DwRect(0, 0, texture.width(), texture.height()), c);

    dst.lt = DPos(200, 100, 0);
    dst.lb = DPos(200, 200, 0);
    dst.rb = DPos(200, 200, 100);
    dst.rt = DPos(200, 100, 100);
    drawTexture(geometry, texture, dst, DwRect(0, 0, texture.width(), texture.height()), c);

    dst.lt = DPos(100, 100, 0);
    dst.lb = DPos(200, 100, 0);
    dst.rb = DPos(200, 100, 100);
    dst.rt = DPos(100, 100, 100);
    drawTexture(geometry, texture, dst, DwRect(0, 0, texture.width(), texture.height()), c);

    dst.lt = DPos(100, 100, 0);
    dst.lb = DPos(100, 200, 0);
    dst.rb = DPos(100, 200, 100);
    dst.rt = DPos(100, 100, 100);
    drawTexture(geometry, texture, dst, DwRect(0, 0, texture.width(), texture.height()), c);

    dst.lt = DPos(100, 100, 100);
    dst.lb = DPos(100, 200, 100);
    dst.rb = DPos(200, 200, 100);
    dst.rt = DPos(200, 100, 100);
    drawTexture(geometry, texture, dst, DwRect(0, 0, texture.width(), texture.height()), c);*/

    

    /*ninePatchTexture.loadFromFile("H:\\Project\\raccoon\\bin\\ninepatchs\\shadow.png");
    drawTextureNinePatch(geometry, ninePatchTexture, 
        DwRect(150, 150, 158, 158), 
        DwRect(0, 0, 30, 30), 
        DwMargins(14, 13, 13,14), c);*/

    /*drawTexture(geometry, textureFromCanvas, DwRect(200, 200, 100, 100), DwRect(0, 0, 800, 600), c);*/

    /*drawTexture(geometryTexture, texture, 
        DwRect(100, 100, texture.width(), texture.height()) , 
        DwRect(0, 0, texture.width(), texture.height()), c);*/
    /*canvasTexture.beginPaint(DwRect(), D3DCOLOR_XRGB(128, 0, 128));
    geometry.draw();
    canvasTexture.endPaint();*/
}

void discardApp()
{
    DwGraphicsD3D9::discard();
}

void render_ui()
{
    //D3DXMATRIXA16 mView, mProj, mWorld;
    //DwGraphicsD3D9::device()->GetTransform(D3DTS_VIEW, &mView);
    //DwGraphicsD3D9::device()->GetTransform(D3DTS_PROJECTION, &mProj);
    //DwGraphicsD3D9::device()->GetTransform(D3DTS_WORLD, &mWorld);

    //// Set effect variables as needed
    //D3DXCOLOR colorMtrlDiffuse( 1.0f, 1.0f, 1.0f, 1.0f );
    //D3DXCOLOR colorMtrlAmbient( 0.35f, 0.35f, 0.35f, 0 );
    //effect.data()->effect->SetVector( "g_MaterialAmbientColor", ( D3DXVECTOR4* )&colorMtrlAmbient );
    //effect.data()->effect->SetVector( "g_MaterialDiffuseColor", ( D3DXVECTOR4* )&colorMtrlDiffuse );

    //D3DXMATRIXA16 mWorldViewProjection  = mWorld * mView * mProj;
    //D3DXHANDLE g_hWorldViewProjection = effect.data()->effect->GetParameterByName( NULL, "g_mWorldViewProjection" );
    //D3DXHANDLE g_hWorld = effect.data()->effect->GetParameterByName( NULL, "g_mWorld" );
    //D3DXHANDLE g_hMeshTexture = effect.data()->effect->GetParameterByName( NULL, "g_MeshTexture" );

    //HRESULT hr = effect.data()->effect->SetMatrix( g_hWorldViewProjection, &mWorldViewProjection );
    //hr = effect.data()->effect->SetMatrix( g_hWorld, &mWorld );
    //hr = effect.data()->effect->SetTexture(g_hMeshTexture, texture.data()->texture);

    canvas.beginPaint(DwRect(), D3DCOLOR_XRGB(0, 128, 128));
    geometry.draw();
    canvas.endPaint();

    /*canvasWin.beginPaint(DwRect(), D3DCOLOR_XRGB(128, 128, 0));
    geometryTexture.draw();
    canvasWin.endPaint();*/
}

void eventLoop()
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

        render_ui();
    }
}

int WINAPI WinMain( __in HINSTANCE , __in_opt HINSTANCE , __in_opt LPSTR , __in int  )
{
    setupApp();
    eventLoop();
    //discardApp();
    return 0;
}