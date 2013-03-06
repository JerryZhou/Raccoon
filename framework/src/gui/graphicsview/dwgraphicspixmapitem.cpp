#include "stable.h"
#include "dwnamespace.h"
#include "dwgraphicspixmapitem.h"
#include "dwgraphics/dwpainter.h"
#include "dwgui/dwresourceinterface.h"

DW_IMPLEMENT_CLASS(DwGraphicsPixmapItem, 'GPPM', DwGraphicsItem);

//------------------------------------------------------------------------------
/**
*/
DwGraphicsPixmapItem::DwGraphicsPixmapItem(DwGraphicsItem *parent/* = NULL*/, DwGraphicsScene *scene/* = NULL*/)
    : DwGraphicsItem(parent, scene)
{

}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsPixmapItem::DwGraphicsPixmapItem(const DwImage& image, DwGraphicsItem *parent/* = NULL*/, DwGraphicsScene *scene/* = NULL*/)
    : DwGraphicsItem(parent, scene)
{
    setPixmap(image);
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsPixmapItem::~DwGraphicsPixmapItem()
{

}

//------------------------------------------------------------------------------
/**
*/
const DwImage& DwGraphicsPixmapItem::pixmap() const
{
    return m_image;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsPixmapItem::setPixmap(const DwImage &pixmap, bool force/* = false*/)
{
    if (m_image != pixmap || force)
    {
        m_image = pixmap;
        setBoundingRect(DwRectF(DwPointF(0., 0.), DwSizeF(m_image.size().width(), m_image.size().height())));
        update();
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsPixmapItem::setPixmap(const DwUrl &url)
{
    setPixmap(DW_PIXMAP_ASYNC(url, this));
}

//------------------------------------------------------------------------------
/**
*/
const DwPointF& DwGraphicsPixmapItem::offset() const
{
    return m_offset;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsPixmapItem::setOffset(const DwPointF &offset)
{
    if(m_offset == offset)
        return;

    m_offset = offset;
    update();
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsPixmapItem::paint(DwPainter *painter, const DwStyleOptionGraphicsItem *option)
{
    DW_UNUSED(option);
    if (!m_image.isNull())
    {
        DwPoint topLeft = m_offset.toPoint();
        DwSize size = boundingRect().size().toSize();
        painter->drawImage(topLeft.x(), topLeft.y(), size.width(), size.height(), m_image);
    }
}

//------------------------------------------------------------------------------
/**
*/
DwString DwGraphicsPixmapItem::accessibleDescription() const
{
    if (!m_accessibleDescription.isNull())
    {
        return m_accessibleDescription;
    }

    if (!m_image.isNull())
    {
        return m_image.path();
    }

    return DwString();
}