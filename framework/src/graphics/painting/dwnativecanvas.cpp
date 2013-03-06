#include "stable.h"
#include "dwnativecanvas.h"

DwNativeCanvas::DwNativeCanvas() : 
    m_hWnd(0),
    m_translucent(false)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwNativeCanvas::beginePaint(const DwRegion& region)
{
    DwCanvas::beginePaint(region);
    m_dirtyRegion = region;
}

//------------------------------------------------------------------------------
/**
*/
void DwNativeCanvas::endPaint()
{
    // update the content to window
    if (m_hWnd)
    {
        HDC hdc = ::GetDC(m_hWnd);
        if(m_translucent)
        {
            tagRECT rcWin;
            ::GetWindowRect(m_hWnd, &rcWin);

            tagPOINT ptWin = { rcWin.left, rcWin.top};
            tagSIZE szWin = { rcWin.right - rcWin.left, rcWin.bottom - rcWin.top};
            tagPOINT ptSrc = { 0, 0 };
            BLENDFUNCTION bfPixelFunction = { AC_SRC_OVER, 0, 255,  1};
            ::UpdateLayeredWindow(m_hWnd, hdc, &ptWin, &szWin, memDC(), &ptSrc, RGB(0, 0, 0), &bfPixelFunction, ULW_ALPHA);
        }
        else
        {
            ::BitBlt(hdc, 0, 0, size().width(), size().height(), memDC(), 0, 0, SRCCOPY);
        }
        ::ReleaseDC(m_hWnd, hdc);
    }
    m_dirtyRegion.clear();
    DwCanvas::endPaint();
}

//------------------------------------------------------------------------------
/**
*/
bool DwNativeCanvas::translucent() const
{
    return m_translucent;
}

//------------------------------------------------------------------------------
/**
*/
void DwNativeCanvas::setTranslucent(bool b)
{
    m_translucent = b;
}

//------------------------------------------------------------------------------
/**
*/
void DwNativeCanvas::setWinHwnd(HWND hWnd)
{
    m_hWnd = hWnd;
}

//------------------------------------------------------------------------------
/**
*/
HWND DwNativeCanvas::winHwnd()const
{
    return m_hWnd;
}