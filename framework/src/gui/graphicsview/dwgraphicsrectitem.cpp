#include "stable.h"

#include "dwgraphics/dwpainter.h"
#include "dwgraphicsrectitem.h"

DW_IMPLEMENT_CLASS(DwGraphicsRectItem, 'GRTM', DwGraphicsItem);

//------------------------------------------------------------------------------
/**
*/
DwGraphicsRectItem::DwGraphicsRectItem(DwGraphicsItem *parent/* = NULL*/, DwGraphicsScene *scene/* = NULL*/)
    : DwGraphicsItem(parent, scene)
    , m_color(255, 255, 255)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsRectItem::DwGraphicsRectItem(const DwRectF& rect, DwGraphicsItem *parent/* = NULL*/, DwGraphicsScene *scene/* = NULL*/)
    : DwGraphicsItem(parent, scene)
    , m_color(255, 255, 255)
{
    setRect(rect);
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsRectItem::DwGraphicsRectItem(DwReal x, DwReal y, DwReal w, DwReal h, DwGraphicsItem *parent/* = NULL*/, DwGraphicsScene *scene/* = NULL*/)
    : DwGraphicsItem(parent, scene)
    , m_color(255, 255, 255)
{
    setRect(x, y, w, h);
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsRectItem::~DwGraphicsRectItem()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwColor DwGraphicsRectItem::color() const
{
    return m_color;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsRectItem::setColor(const DwColor &c)
{
    if(m_color == c)
        return;

    m_color = c;
    update();
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsRectItem::paint(DwPainter *painter, const DwStyleOptionGraphicsItem *option)
{
    DW_UNUSED(option);
    painter->fillRect(boundingRect(), m_color);
}