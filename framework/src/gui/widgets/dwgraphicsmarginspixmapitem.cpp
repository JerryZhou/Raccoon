#include "stable.h"
#include "dwgraphicsmarginspixmapitem.h"
#include "dwgraphics/dwpainter.h"
#include "dwgui/dwresourceinterface.h"

DW_IMPLEMENT_CLASS(DwGraphicsMarginsPixmapItem, 'GMPP', DwGraphicsItem);

//------------------------------------------------------------------------------
/**
*/
DwGraphicsMarginsPixmapItem::DwGraphicsMarginsPixmapItem(DwGraphicsItem *parent/* = NULL*/)
    : DwGraphicsItem(parent)
    , m_needDrawCenter(true)
{
    // accessible
    setAccessibleName(tr("raccoon margins pixmap item"));
    setAccessibleDescription(tr("raccoon margins pixmap item"));
    setWhatsThis(tr("raccoon base margins pixmap item"));
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsMarginsPixmapItem::~DwGraphicsMarginsPixmapItem()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
const DwImage& DwGraphicsMarginsPixmapItem::pixmap() const
{
    return m_pixmap;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsMarginsPixmapItem::setPixmap(const DwImage &p)
{
    m_pixmap = p;
    setSize(p.size());
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsMarginsPixmapItem::setPixmap(const DwUrl &url)
{
    setPixmap(DW_PIXMAP(url));
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsMarginsPixmapItem::setMargins(const DwMargins& m)
{
    m_margins = m;
    update();
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsMarginsPixmapItem::setDrawCenter(bool b)
{
    m_needDrawCenter = b;
}

//------------------------------------------------------------------------------
/**
*/
bool DwGraphicsMarginsPixmapItem::isDrawCenter() const
{
    return m_needDrawCenter;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsMarginsPixmapItem::paint(DwPainter *painter, const DwStyleOptionGraphicsItem *option)
{
    DW_UNUSED(option);
    // null
    if (m_pixmap.isNull() || width() <= 0.0 || height() <= 0.0)
    {
        return;
    }

    // no margins or no need to extends
    if ((m_margins.left() == 0 && m_margins.right() == 0
            && m_margins.top() == 0 && m_margins.bottom() == 0)
            || m_localBoundingRect.size().toSize() == m_pixmap.size())
    {
        painter->drawImage(m_localBoundingRect.topLeft(), m_localBoundingRect.size(), m_pixmap);
    }
    else
    {
        DwRect destRect = m_localBoundingRect.toRect();

        // draw boder
        painter->drawBorderImage(destRect, m_pixmap, m_margins);

        // draw center
        if (m_needDrawCenter)
        {
            destRect.adjust(m_margins.left(), m_margins.top(), -m_margins.right(), -m_margins.bottom());
            DwRect sourceRect = DwRect(0, 0, m_pixmap.size().width(), m_pixmap.size().height());
            sourceRect.adjust(m_margins.left(), m_margins.top(), -m_margins.right(), -m_margins.bottom());

            // set a offset to revise the tile: SHAME, I DO NOT KNOW WHY
            if (destRect.width() != sourceRect.width() && m_margins.right() > 0 && sourceRect.width() > 1)
            {
                sourceRect.adjust(0, 0, -1, 0);
            }

            if (destRect.height() != sourceRect.height() && m_margins.bottom() > 0 && sourceRect.height() > 1)
            {
                sourceRect.adjust(0, 0, 0, -1);
            }

            painter->drawImageScaled(destRect, m_pixmap, sourceRect);
        }
    }
}