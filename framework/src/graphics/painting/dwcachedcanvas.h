#pragma once

#include "dwgraphics/dwcanvas.h"
#include "dwgraphics/dwpainter.h"

//------------------------------------------------------------------------------
namespace Gdiplus
{
class Bitmap;
};

//------------------------------------------------------------------------------
class DW_GRAPHICS_EXPORT DwCachedCanvas : public DwCanvas
{
    DW_DISABLE_COPY(DwCachedCanvas);
public:
    DwCachedCanvas(DwSize size);
    virtual ~DwCachedCanvas();

    virtual void resize(const DwSize &size);
    virtual void beginePaint(const DwRegion& region);
    virtual void endPaint();

    inline DwReal opacity() const;
    inline void setOpacity(DwReal opacity);

    inline bool enabled() const;
    inline void setEnabled(bool b);

    // will clone the bitmap
    Gdiplus::Bitmap* toBitmap()const;

    static DwCachedCanvas *cacheOf(const DwString& key);
    static DwCachedCanvas *releaseCache(const DwString& key);
    static void paintCache(DwPainter* painter, DwCachedCanvas* cache);

    // extend for dc
    HDC GetHDC();
    void ReleaseHDC(HDC hdc);

protected:
    DwReal m_opacity;
    bool m_enabled;
    bool m_isBitmapCache;
    Gdiplus::Bitmap *m_bitmapGdi;
};// end of DwCachedCanvas

//------------------------------------------------------------------------------
/**
*/
inline DwReal DwCachedCanvas::opacity() const
{
    return m_opacity;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwCachedCanvas::setOpacity(DwReal opacity)
{
    m_opacity = opacity;
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwCachedCanvas::enabled() const
{
    return m_enabled;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwCachedCanvas::setEnabled(bool b)
{
    m_enabled = b;
}