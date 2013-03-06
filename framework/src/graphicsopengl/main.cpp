//#include "stable.h"
//#include "dwgraphicsd3d_p.h"
//#include "dwcanvaswindow.h"
//#include "dwcanvastexture.h"
//#include "dwtexture.h"
//#include "dwgeometrybuffer.h"
//#include "dwcore/dwmargins.h"
//
//DwCanvasWindow canvas;
//DwCanvasWindow canvasWin;
//DwTexture texture;
//DwGeometryBuffer geometry;
//DwTexture ninePatchTexture;
//
//DwGeometryBuffer geometryTexture;
//DwCanvasTexture canvasTexture;
//DwTexture textureFromCanvas = canvasTexture.texture();
//
////------------------------------------------------------------------------------
///**
//*/
//LRESULT CALLBACK	DWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    switch (message)
//    {
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        break;
//    case WM_NCHITTEST:
//        return HTCAPTION;
//    default:
//        return DefWindowProc(hWnd, message, wParam, lParam);
//    }
//    return 0;
//}
//
//void setupCanvas(DwCanvasWindow& canvas)
//{
//    LPCWSTR szWindowClass = L"szWindowClass";
//
//    WNDCLASSEX wcex;
//    ZeroMemory(&wcex, sizeof(wcex));
//    wcex.cbSize = sizeof(WNDCLASSEX);
//    wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
//    wcex.lpfnWndProc	= DWndProc;
//    wcex.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
//    wcex.cbClsExtra		= 0;
//    wcex.cbWndExtra		= 0;
//    wcex.hInstance		= NULL;
//    wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
//    wcex.hbrBackground	= (HBRUSH)(NULL_BRUSH);
//    wcex.lpszMenuName  = NULL;
//    wcex.lpszClassName	= szWindowClass;
//    wcex.hIconSm       = NULL;
//    RegisterClassEx(&wcex);
//
//    // get size of window to use so that client area is desired size
//    RECT wnd_size = {0,0,800,600};
//    AdjustWindowRect(&wnd_size, WS_POPUP | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX, FALSE);
//
//    HWND hWnd = CreateWindowEx(
//        WS_EX_APPWINDOW,//WS_EX_LAYERED | WS_EX_APPWINDOW, 
//        szWindowClass, L"Raccoon", 
//        WS_POPUP | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX ,
//        CW_USEDEFAULT, 0, wnd_size.right - wnd_size.left, wnd_size.bottom - wnd_size.top,
//        NULL, NULL, NULL, NULL);
//
//    ::ShowWindow(hWnd, SW_SHOWNORMAL);
//    ::UpdateWindow(hWnd);
//
//    static bool d3d_init = false;
//    if (!d3d_init)
//    {
//        DwGraphicsD3D9::setup(hWnd);
//        d3d_init = true;
//    }
//    
//    canvas.setup(hWnd);
//}
//
//struct D3DRectF 
//{
//    FLOAT left, right, top, bottom;
//    D3DRectF(FLOAT l, FLOAT t, FLOAT r, FLOAT b)
//        :left(l), right(r), top(t), bottom(b){
//    }
//};
//
//struct D3DRect
//{
//    DWORD left, right, top, bottom;
//    D3DRect(DWORD l, DWORD t, DWORD r, DWORD b)
//        :left(l), right(r), top(t), bottom(b){
//    }
//};
//
//D3DRectF dstRect(const DwRect &dstR)
//{
//#define _D3DFLOAT (FLOAT)
//    return D3DRectF(_D3DFLOAT dstR.left(), _D3DFLOAT dstR.top(), _D3DFLOAT dstR.right()+1, _D3DFLOAT dstR.bottom()+1);
//}
//
//D3DRectF srcRect(const DwRect &srcR, const DwTexture& texture)
//{
//    FLOAT dx = (FLOAT)texture.scale().width();
//    FLOAT dy = (FLOAT)texture.scale().height();
//    return D3DRectF(srcR.left()*dx, srcR.top()*dy, (srcR.right()+1)*dx, (srcR.bottom()+1)*dy);
//}
//
//void drawTexture(DwGeometryBuffer & geometry, const DwTexture& texture, const DwRect &dstR, const DwRect &srcR, DColor c)
//{
//    D3DRectF  dst = dstRect(dstR);
//    D3DRectF src = srcRect(srcR, texture);
//
//    DwVertex vertex[6];
//    vertex[0].x = dst.left;
//    vertex[0].y = dst.top;
//    vertex[0].z = 0;
//    vertex[0].tu = src.left;
//    vertex[0].tv = src.top;
//    vertex[0].diffuse = c;
//
//    vertex[1].x = dst.left;
//    vertex[1].y = dst.bottom;
//    vertex[1].z = 0;
//    vertex[1].tu = src.left;
//    vertex[1].tv = src.bottom;
//    vertex[1].diffuse = c;
//
//    vertex[2].x = dst.right;
//    vertex[2].y = dst.bottom;
//    vertex[2].z = 0;
//    vertex[2].tu = src.right;
//    vertex[2].tv = src.bottom;
//    vertex[2].diffuse = c;
//
//    vertex[3].x = dst.right;
//    vertex[3].y = dst.top;
//    vertex[3].z = 0;
//    vertex[3].tu = src.right;
//    vertex[3].tv = src.top;
//    vertex[3].diffuse = c;
//
//    vertex[4].x = dst.left;
//    vertex[4].y = dst.top;
//    vertex[4].z = 0;
//    vertex[4].tu = src.left;
//    vertex[4].tv = src.top;
//    vertex[4].diffuse = c;
//
//    vertex[5].x = dst.right;
//    vertex[5].y = dst.bottom;
//    vertex[5].z = 0;
//    vertex[5].tu = src.right;
//    vertex[5].tv = src.bottom;
//    vertex[5].diffuse = c;
//
//    geometry.setTexture(&texture);
//    geometry.addGeometry(vertex, 6);
//}
//int cRed = D3DCOLOR_XRGB(255, 0, 0);
//int cGreen = D3DCOLOR_XRGB(0, 255, 0);
//int cBlue = D3DCOLOR_XRGB(0, 0, 255);
//void drawTextureNinePatch(DwGeometryBuffer & geometry, const DwTexture& texture, 
//                          const DwRect &dst, const DwRect &src, const DwMargins& margins, DColor c)
//{
//
//    int cx1 = margins.left();//texture.width() / 3 + 5;
//    int cx3 = margins.right();//texture.width() / 3 - 5;
//    int cx2 = src.width() - cx1 - cx3;
//    int cy1 = margins.top();//texture.height() / 3 - 5;
//    int cy3 = margins.bottom();//texture.height() / 3 + 5;
//    int cy2 = src.height() - cy1 - cy3;
//
//    int sleft = src.left();
//    int stop = src.top();
//
//    //corners
//    if (cx1)
//    {
//        if (cy1) drawTexture(geometry, texture,
//            DwRect(dst.left(), dst.top(), cx1, cy1), 
//            DwRect(sleft, stop, cx1, cy1), c);
//        if (cy3) drawTexture(geometry, texture,
//            DwRect(dst.left(), dst.top() + dst.height() - cy3, cx1, cy3), 
//            DwRect(sleft, stop + cy1 + cy2, cx1, cy3), c);
//    }
//
//    if (cx3)
//    {
//        if(cy1) drawTexture(geometry, texture, 
//            DwRect(dst.left() + dst.width() - cx3, dst.top(), cx3, cy1), 
//            DwRect(sleft + cx1 + cx2, stop, cx3, cy1), c);
//        if(cy3) drawTexture(geometry, texture, 
//            DwRect(dst.left() + dst.width() - cx3, dst.top() + dst.height() - cy3, cx3, cy3), 
//            DwRect(sleft + cx1 + cx2, stop + cy1 + cy2, cx3, cy3), c);
//    }
//
//    //////////////////////////////////////////////////////////////////////////
//    #define _dw_v_m(m) ((m) ? 1 : 0)
//    int ws = _dw_v_m(cx1) + _dw_v_m(cx3);
//    int hs = _dw_v_m(cy1) + _dw_v_m(cy3);
//
//    //top bottom border
//    if (cx2 > ws)
//    {
//        if (cy1) drawTexture(geometry, texture, 
//            DwRect(dst.left() + cx1 , dst.top(), dst.width() - cx1 - cx3, cy1), 
//            DwRect(sleft + cx1 + _dw_v_m(cx1), stop, cx2-ws, cy1), c);
//        if (cy3) drawTexture(geometry, texture, 
//            DwRect(dst.left() + cx1, dst.top() + dst.height() - cy3, dst.width() - cx1 - cx3, cy3), 
//            DwRect(sleft + cx1 + _dw_v_m(cx1), stop + cy1 + cy2, cx2-ws, cy3), c);
//    }
//
//    //left right border
//    if(cy2 > hs)
//    {
//        if(cx1) drawTexture(geometry, texture, 
//            DwRect(dst.left(), dst.top() + cy1, cx1, dst.height() - cy1 - cy3), 
//            DwRect(sleft, stop + cy1 + _dw_v_m(cy1), cx1, cy2 - hs), c);
//        if(cx3) drawTexture(geometry, texture, 
//            DwRect(dst.left() + dst.width() - cx3, dst.top() + cy1, cx3, dst.height() - cy1 - cy3), 
//            DwRect(sleft + cx1 + cx2, stop + cy1 + _dw_v_m(cy1), cx3, cy2 - hs), c);
//    }
//
//    {
//        int mleft = margins.left();
//        int mtop = margins.top();
//        int mright = margins.right();
//        int mbottom = margins.bottom();
//#define _v_len(len) ((len) ? ((len) + 1) : (len) )
//
//        DwRect cdstRect = dst;
//        cdstRect.adjust(mleft, mtop, -mright, -mbottom);
//        DwRect csrcRect = src;
//        csrcRect.adjust(_v_len(mleft), _v_len(mtop), -_v_len(mright), -_v_len(mbottom));
//        drawTexture(geometry, texture, cdstRect, csrcRect, c);
//    }
//}
//
//void setupApp()
//{
//    setupCanvas(canvas);
//    setupCanvas(canvasWin);
//    canvasTexture.setSize(800, 600);
//
//    int c = D3DCOLOR_XRGB(255, 255, 255);
//
//    texture.loadFromFile("H:\\Project\\raccoon\\bin\\testdata\\mu\\eg.bmp");
//    drawTexture(geometry, texture, 
//        DwRect(100, 100, texture.width(), texture.height()) , 
//        DwRect(0, 0, texture.width(), texture.height()), c);
//
//    ninePatchTexture.loadFromFile("H:\\Project\\raccoon\\bin\\ninepatchs\\shadow.png");
//    drawTextureNinePatch(geometry, ninePatchTexture, 
//        DwRect(150, 150, 158, 158), 
//        DwRect(0, 0, 30, 30), 
//        DwMargins(14, 13, 13,14), c);
//
//    drawTexture(geometry, textureFromCanvas, DwRect(200, 200, 100, 100), DwRect(0, 0, 800, 600), c);
//
//    drawTexture(geometryTexture, texture, 
//        DwRect(100, 100, texture.width(), texture.height()) , 
//        DwRect(0, 0, texture.width(), texture.height()), c);
//    canvasTexture.beginPaint(DwRect(), D3DCOLOR_XRGB(128, 0, 128));
//    //geometryTexture.draw();
//    geometry.draw();
//    canvasTexture.endPaint();
//}
//
//void discardApp()
//{
//    DwGraphicsD3D9::discard();
//}
//
//void render_ui()
//{
//    canvas.beginPaint(DwRect(), D3DCOLOR_XRGB(0, 128, 128));
//    geometry.draw();
//    canvas.endPaint();
//
//    canvasWin.beginPaint(DwRect(), D3DCOLOR_XRGB(128, 128, 0));
//    geometryTexture.draw();
//    canvasWin.endPaint();
//}
//
//void eventLoop()
//{
//    bool quit = false;
//    while (!quit)
//    {
//        MSG msg = {0};
//        //if(
//        while (
//        PeekMessage( &msg, NULL, 0, 0, PM_REMOVE )
//        )
//        {
//            TranslateMessage( &msg );
//            DispatchMessage( &msg );
//
//            if (msg.message == WM_QUIT)
//            {
//                quit = true;
//                //break;
//            }
//        }
//
//        render_ui();
//    }
//}

int WINAPI WinMain( __in HINSTANCE , __in_opt HINSTANCE , __in_opt LPSTR , __in int  )
{
   /* setupApp();
    eventLoop();
    discardApp();*/
    return 0;
}