#pragma once

#include <dwcore/dwsize.h>
#include <dwcore/dwrect.h>
#include <dwcore/dwmargins.h>
#include <dwcore/dwvector.h>
#include <dwgraphics/dwimage.h>
#include <dwgraphics/dwcolor.h>
#include <dwgraphics/dwtransform.h>
#include <dwgraphics/dwfont.h>
#include "dwgraphics/dwregion.h"

//------------------------------------------------------------------------------
namespace Gdiplus
{
class Graphics;
}

//------------------------------------------------------------------------------
class DW_GRAPHICS_EXPORT DwCanvas
{
    DW_DISABLE_COPY(DwCanvas);
public:
    DwCanvas();
    virtual ~DwCanvas();

    DwSize size() const;
    virtual void resize(const DwSize &s);
    virtual void beginePaint(const DwRegion& region);
    virtual void endPaint();

    void drawLine(const DwPointF &from, const DwPointF &to, const DwColor &color);
    void fillRect(const DwRectF &rect, const DwColor &color);
    void fillRegion(const DwRegion &region, const DwColor &color);
    void drawRect(const DwRectF &rect, const DwColor &color);
    void drawImage(double x, double y, const DwImage &image);
    void drawImage(double x, double y, double width, double height, const DwImage &image);
    void drawImageScaled(const DwRectF &destRect, const DwImage &image, const DwRectF &srcRect);
    void drawImageScaledFlipXY(const DwRectF &destRect, const DwImage &image, const DwRectF &srcRect);
    void drawBorderImage(const DwRect &rect, const DwImage &image, const DwMargins &margins);
    void drawBorderImage(const DwRect &dstRect, const DwRect &sourceRect, const DwImage &image, const DwMargins &margins);

    void drawText(const DwRectF &rect, const DwString &text);
    void drawText(const DwRectF &rect, const DwString &text, const DwString &fontName, int fontSize);
    void drawText(const DwRectF &rect, const DwString &text, const DwFont &font);

    DwRectF clipRect() const;
    void setClipRect(const DwRectF &rect, Dw::ClipOperation op);
    void setClip(const DwRegion *region, Dw::ClipOperation op);
    void resetClipRect();

    void setWorldTransform(const DwTransform& transform);
    void resetWoldTransform();

    Dw::CompositingMode compositingMode() const;
    void setCompositingMode(Dw::CompositingMode mode);

    void save();
    void restore();

    DwColor pixel(const DwPoint &p) const;

    Gdiplus::Graphics *graphics() const;
    inline HDC memDC() const;

    // for global config
    static void setDefaultTextRenderingHint(Dw::TextRenderingHint hint);
    static Dw::TextRenderingHint defaultTextRenderingHint();

    static void setDefaultInterpolationMode(Dw::InterpolationMode mode);
    static Dw::InterpolationMode defaultInterpolationMode();

    static void setDefaultCompositingQuality(Dw::CompositingQuality quality);
    static Dw::CompositingQuality defaultCompositingQuality();

    static Gdiplus::Graphics *defaultGraphics() ; 
    static void initCanvas();
    static void unInitCanvas();

protected:
    void clear(const DwRegion& region, const DwColor& c);

    DwSize m_canvasSize;
    Gdiplus::Graphics *m_graphics;
    HDC m_memDC;
    HBITMAP m_bitmap;
    HGDIOBJ m_oldBitmap;
    DwVector< unsigned int > m_savedStates;
    bool m_dirty;
};// end of DwCanvas

//------------------------------------------------------------------------------
/**
*/
inline HDC DwCanvas::memDC() const
{
    return m_memDC;
};