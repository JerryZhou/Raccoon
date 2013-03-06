#include "stable.h"
#include "dwcachedcanvas.h"

//------------------------------------------------------------------------------
/**
    TODO: xx
*/
DwCachedCanvas::DwCachedCanvas(DwSize size)
    : m_isBitmapCache(false)
    , m_enabled(true)
    , m_opacity(1.0)
    , m_bitmapGdi(NULL)
{
    DwCachedCanvas::resize(size);
}

//------------------------------------------------------------------------------
/**
    TODO: xx
*/
DwCachedCanvas::~DwCachedCanvas()
{
    DW_SAFE_DELETE(m_graphics);
    DW_SAFE_DELETE(m_bitmapGdi);
}

//------------------------------------------------------------------------------
/**
    TODO: xx
*/
void DwCachedCanvas::resize(const DwSize &size)
{
    DW_ASSERT(!m_bitmapGdi);
    m_canvasSize = size;
    DW_SAFE_DELETE(m_bitmapGdi);
    m_bitmapGdi = new Gdiplus::Bitmap(size.width(), size.height(), PixelFormat32bppARGB);

    DW_SAFE_DELETE(m_graphics);
    m_graphics = Gdiplus::Graphics::FromImage(m_bitmapGdi);
}

//------------------------------------------------------------------------------
/**
    TODO: xx
*/
void DwCachedCanvas::beginePaint(const DwRegion& region)
{
    DW_ASSERT(m_bitmapGdi);
    m_savedStates.clear();
    if (!m_graphics)
    {
        m_graphics = Gdiplus::Graphics::FromImage(m_bitmapGdi);
    }
    if (m_graphics)
    {
        m_graphics->SetCompositingQuality(Gdiplus::CompositingQuality(defaultCompositingQuality()));
        m_graphics->SetInterpolationMode(Gdiplus::InterpolationMode(defaultInterpolationMode()));
        m_graphics->SetTextRenderingHint(Gdiplus::TextRenderingHint(defaultTextRenderingHint()));
    }

    clear(region, DwColor(0, 0, 0, 0));
}

//------------------------------------------------------------------------------
/**
    TODO: xx
*/
void DwCachedCanvas::endPaint()
{
    //DW_SAFE_DELETE(m_graphics);
}

//------------------------------------------------------------------------------
/**
*/
Gdiplus::Bitmap* DwCachedCanvas::toBitmap()const
{
    if (m_bitmapGdi)
    {
        if (!m_isBitmapCache)
        {
            const_cast<DwCachedCanvas*>(this)->m_isBitmapCache = true;
        }
        return m_bitmapGdi->Clone(
                   Gdiplus::Rect(0, 0, m_bitmapGdi->GetWidth(), m_bitmapGdi->GetHeight()),
                   PixelFormatDontCare);
    }
    else
    {
        return NULL;
    }
}

//------------------------------------------------------------------------------
/**
*/
HDC DwCachedCanvas::GetHDC()
{
    return m_graphics->GetHDC();
}

//------------------------------------------------------------------------------
/**
*/
void DwCachedCanvas::ReleaseHDC(HDC hdc)
{
    m_graphics->ReleaseHDC(hdc);
}

//------------------------------------------------------------------------------
/**
    TODO: xx
*/
DwCachedCanvas * DwCachedCanvas::cacheOf(const DwString& key)
{
    DW_UNUSED(key);
    return NULL;
}

//------------------------------------------------------------------------------
/**
    TODO: xx
*/
DwCachedCanvas * DwCachedCanvas::releaseCache(const DwString& key)
{
    DW_UNUSED(key);
    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
void DwCachedCanvas::paintCache(DwPainter* painter, DwCachedCanvas* cache)
{
    DwCanvas *paintCanvas = painter->canvas();

    // Initialize the color matrix.
    // Notice the value cache->opacity() in row 4, column 4.
    Gdiplus::ColorMatrix colorMatrix;

    if (!cache->enabled())
    {
        Gdiplus::ColorMatrix c =
        {
            0.3f, 0.3f, 0.3f, 0.0f, 0.0f,
            0.59f, 0.59f, 0.59f, 0.0f, 0.0f,
            0.11f, 0.11f, 0.11f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, Gdiplus::REAL(cache->opacity()), 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 1.0f
        };
        colorMatrix = c;
    }
    else
    {
        Gdiplus::ColorMatrix c =
        {
            1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, Gdiplus::REAL(cache->opacity()), 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 1.0f
        };
        colorMatrix = c;
    }

    // Create an ImageAttributes object and set its color matrix.
    Gdiplus::ImageAttributes imageAtt;
    imageAtt.SetColorMatrix(&colorMatrix, Gdiplus::ColorMatrixFlagsDefault,
                            Gdiplus::ColorAdjustTypeBitmap);

    // Draw Bitmap By Image Attributes
    INT iWidth = cache->m_bitmapGdi->GetWidth();
    INT iHeight = cache->m_bitmapGdi->GetHeight();
    paintCanvas->graphics()->DrawImage(
        cache->m_bitmapGdi,
        Gdiplus::Rect(0, 0, iWidth, iHeight),  // Destination rectangle
        0,                             // Source rectangle X
        0,                             // Source rectangle Y
        iWidth,                        // Source rectangle width
        iHeight,                       // Source rectangle height
        Gdiplus::UnitPixel,
        &imageAtt);
}