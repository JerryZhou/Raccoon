#include "stable.h"

#include "dwpainter.h"
#include "dwgraphics/dwninepatch.h"

//------------------------------------------------------------------------------
/**
*/
DwPainter::DwPainter(DwCanvas *canvas)
    : m_canvas(canvas)
{
}

//------------------------------------------------------------------------------
/**
*/
void DwPainter::fillRect(const DwRectF &rect, const DwColor &color)
{
    m_canvas->fillRect(rect, color);
}

//------------------------------------------------------------------------------
/**
*/
void DwPainter::fillRegion(const DwRegion &region, const DwColor &color)
{
    m_canvas->fillRegion(region, color);
}

//------------------------------------------------------------------------------
/**
*/
void DwPainter::drawRect(const DwRectF &rect, const DwColor &color)
{
    m_canvas->drawRect(rect, color);
}

//------------------------------------------------------------------------------
/**
*/
void DwPainter::drawImage(DwReal x, DwReal y, const DwImage &image)
{
    m_canvas->drawImage(x, y, image);
}

//------------------------------------------------------------------------------
/**
*/
void DwPainter::drawImage(const DwPointF &p, const DwImage &image)
{
    m_canvas->drawImage(p.x(), p.y(), image);
}

//------------------------------------------------------------------------------
/**
*/
void DwPainter::drawImage(DwReal x, DwReal y, DwReal width, DwReal height, const DwImage &image)
{
    m_canvas->drawImage(x, y, width, height, image);
}

//------------------------------------------------------------------------------
/**
*/
void DwPainter::drawImage(const DwPointF &p, const DwSizeF &size, const DwImage &image)
{
    m_canvas->drawImage(p.x(), p.y(), size.width(), size.height(), image);
}

//------------------------------------------------------------------------------
/**
*/
void DwPainter::drawImageScaled(const DwRect &destRect, const DwImage &image, const DwRect &srcRect)
{
    m_canvas->drawImageScaled(destRect, image, srcRect);
}

//------------------------------------------------------------------------------
/**
*/
void DwPainter::drawImageScaledFlipXY(const DwRect &destRect, const DwImage &image, const DwRect &srcRect)
{
    m_canvas->drawImageScaledFlipXY(destRect, image, srcRect);
}

//------------------------------------------------------------------------------
/**
*/
void DwPainter::drawLine(const DwPoint &from, const DwPoint &to, const DwColor &color)
{
    m_canvas->drawLine(from, to, color);
}

//------------------------------------------------------------------------------
/**
*/
void DwPainter::drawBorderImage(const DwRect &rect, const DwImage &image, const DwMargins &margins)
{
    m_canvas->drawBorderImage(rect, image, margins);
}

//------------------------------------------------------------------------------
/**
*/
void DwPainter::drawBorderImage(const DwRect &dstRect, const DwRect &sourceRect, const DwImage &image, const DwMargins &margins)
{
    m_canvas->drawBorderImage(dstRect, sourceRect, image, margins);
}

//------------------------------------------------------------------------------
/**
*/
void DwPainter::drawNinePatchs(const DwRect &dstRect, int idx, const DwImage &image, bool center/* = false*/)
{
    if (image.isNull())
    {
        return;
    }

    DwNinePatch::NinePatchInfo info = DwNinePatch::ninePatchInfo(image, idx);
    
    // draw border
    drawBorderImage(dstRect, info.srcRect, image, info.margins);

    // draw center
    if (center)
    {
        int mleft = info.margins.left();
        int mtop = info.margins.top();
        int mright = info.margins.right();
        int mbottom = info.margins.bottom();
#define _v_len(len) ((len) ? ((len) + 1) : (len) )

        DwRect cdstRect = dstRect;
        cdstRect.adjust(mleft, mtop, -mright, -mbottom);
        DwRect sourceRect = info.srcRect;
        sourceRect.adjust(_v_len(mleft), _v_len(mtop), -_v_len(mright), -_v_len(mbottom));

        if (sourceRect.width() > 1 && sourceRect.height() > 1)
        {
            // set a offset to revise the tile: SHAME, I DO NOT KNOW WHY
            if (cdstRect.width() != sourceRect.width() && mright > 0 && sourceRect.width() > 1)
            {
                sourceRect.adjust(0, 0, -1, 0);
            }

            if (cdstRect.height() != sourceRect.height() && mbottom > 0 && sourceRect.height() > 1)
            {
                sourceRect.adjust(0, 0, 0, -1);
            }

            drawImageScaled(cdstRect, image, sourceRect);
        }
        else
        {
            drawImageScaledFlipXY(cdstRect, image, sourceRect);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwPainter::drawText(const DwRectF &rect, const DwString &text)
{
    m_canvas->drawText(rect, text);
}

//------------------------------------------------------------------------------
/**
*/
void DwPainter::drawText(const DwRectF &rect, const DwString &text, const DwString &fontName, int fontSize)
{
    m_canvas->drawText(rect, text, fontName, fontSize);
}

//------------------------------------------------------------------------------
/**
*/
void DwPainter::drawText(const DwRectF &rect, const DwString &text, const DwFont &font)
{
    m_canvas->drawText(rect, text, font);
}

//------------------------------------------------------------------------------
/**
*/
void DwPainter::save()
{
    m_canvas->save();
}

//------------------------------------------------------------------------------
/**
*/
void DwPainter::restrore()
{
    m_canvas->restore();
}

//------------------------------------------------------------------------------
/**
*/
void DwPainter::setWorldTransform(const DwTransform& transform)
{
    m_canvas->setWorldTransform(transform);

}

//------------------------------------------------------------------------------
/**
*/
int DwPainter::width()
{
    return m_canvas->size().width();
}

//------------------------------------------------------------------------------
/**
*/
int DwPainter::height()
{
    return m_canvas->size().height();
}

//------------------------------------------------------------------------------
/**
*/
DwRect DwPainter::clipRect() const
{
    return m_canvas->clipRect().toRect();
}

//------------------------------------------------------------------------------
/**
*/
void DwPainter::setClipRect(const DwRect &rect, Dw::ClipOperation op)
{
    m_canvas->setClipRect(rect, op);
}

//------------------------------------------------------------------------------
/**
*/
void DwPainter::setClip(const DwRegion *region, Dw::ClipOperation op/* = Dw::ReplaceClip*/)
{
    m_canvas->setClip(region, op);
}

//------------------------------------------------------------------------------
/**
*/
Dw::CompositingMode DwPainter::compositingMode() const
{
    return m_canvas->compositingMode();
}

//------------------------------------------------------------------------------
/**
*/
void DwPainter::setCompositingMode(Dw::CompositingMode mode)
{
    m_canvas->setCompositingMode(mode);
}

//------------------------------------------------------------------------------
/**
*/
DwCanvas *DwPainter::canvas() const
{
    return m_canvas;
}
