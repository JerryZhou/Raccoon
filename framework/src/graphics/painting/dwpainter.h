#pragma once

#include <dwcore/dwobject.h>
#include <dwcore/dwrect.h>
#include <dwcore/dwpoint.h>
#include <dwcore/dwsize.h>
#include <dwgraphics/dwcanvas.h>
#include <dwgraphics/dwtransform.h>
#include <dwgraphics/dwfont.h>

//------------------------------------------------------------------------------
class DW_GRAPHICS_EXPORT DwPainter : public DwObject
{
public:
    DwPainter(DwCanvas *canvas);

public:
    void drawLine(const DwPoint &from, const DwPoint &to, const DwColor &color);
    void fillRect(const DwRectF &rect, const DwColor &color);
    void fillRegion(const DwRegion &region, const DwColor &color);
    void drawRect(const DwRectF &rect, const DwColor &color);

    void drawImage(const DwPointF &p, const DwImage &image);
    void drawImage(DwReal x, DwReal y, const DwImage &image);
    void drawImage(const DwPointF &p, const DwSizeF &size, const DwImage &image);
    void drawImage(DwReal x, DwReal y, DwReal width, DwReal height, const DwImage &image);
    void drawImageScaled(const DwRect &destRect, const DwImage &image, const DwRect &srcRect);
    void drawImageScaledFlipXY(const DwRect &destRect, const DwImage &image, const DwRect &srcRect);
    void drawBorderImage(const DwRect &rect, const DwImage &image, const DwMargins &margins);
    void drawBorderImage(const DwRect &dstRect, const DwRect &sourceRect, const DwImage &image, const DwMargins &margins);
    void drawNinePatchs(const DwRect &dstRect, int idx, const DwImage &image, bool center = false);

    void drawText(const DwRectF &rect, const DwString &text);
    void drawText(const DwRectF &rect, const DwString &text, const DwString &fontName, int fontSize);
    void drawText(const DwRectF &rect, const DwString &text, const DwFont &font);

    void save();
    void restrore();
    void setWorldTransform(const DwTransform& transform);

    DwRect clipRect() const;
    void setClipRect(const DwRect &rect, Dw::ClipOperation op = Dw::ReplaceClip);
    void setClip(const DwRegion *region, Dw::ClipOperation op = Dw::ReplaceClip);

    Dw::CompositingMode compositingMode() const;
    void setCompositingMode(Dw::CompositingMode mode);

    int width();
    int height();

    DwCanvas *canvas() const ;

private:
    DwCanvas *m_canvas;
};// end of DwPainter
