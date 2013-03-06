#include "stable.h"

#include "dwgraphics.h"
#include "image/dwimage.h"
#include "painting/dwcanvas.h"

Gdiplus::GdiplusStartupInput g_gdiplusStartupInput;
ULONG_PTR g_gdiplusToken = 0;

void dwInitGraphics()
{
    Gdiplus::GdiplusStartup(&g_gdiplusToken, &g_gdiplusStartupInput, NULL);
    DwImage::initImageLib();
    DwCanvas::initCanvas();
}

void dwUninitGraphics()
{
    DwCanvas::unInitCanvas();
    DwImage::unInitImageLib();
    Gdiplus::GdiplusShutdown(g_gdiplusToken);
}

void DwGraphics::initGraphics()
{
    dwInitGraphics();
}

void DwGraphics::uninitGraphics()
{
    dwUninitGraphics();
}
