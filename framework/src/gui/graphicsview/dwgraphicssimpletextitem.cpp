#include "stable.h"
#include "dwgraphicssimpletextitem.h"
#include "dwgraphics/dwpainter.h"

DW_IMPLEMENT_CLASS(DwGraphicsSimpleTextItem, 'GSTM', DwGraphicsItem);

//------------------------------------------------------------------------------
/**
*/
DwGraphicsSimpleTextItem::DwGraphicsSimpleTextItem(DwGraphicsItem *parent/* = NULL*/, DwGraphicsScene *scene/* = NULL*/)
    : DwGraphicsItem(parent, scene)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsSimpleTextItem::DwGraphicsSimpleTextItem(const DwString& text, DwGraphicsItem *parent/* = NULL*/, DwGraphicsScene *scene/* = NULL*/)
    : DwGraphicsItem(parent, scene)
{
    setText(text);
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsSimpleTextItem::~DwGraphicsSimpleTextItem()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwString DwGraphicsSimpleTextItem::text() const
{
    return m_text;
}

//------------------------------------------------------------------------------
/**
    Simple just set text then adjust bounding rect,
    The original point bounding rect is (0, 0)
*/
void DwGraphicsSimpleTextItem::setText(const DwString& txt)
{
    m_text = txt;
    updateBoundingRect();
}

//------------------------------------------------------------------------------
/**
*/
DwFont DwGraphicsSimpleTextItem::font() const
{
    return m_font;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSimpleTextItem::setFont(const DwFont &f)
{
    m_font = f;
    updateBoundingRect();
}

//------------------------------------------------------------------------------
/**
*/
int DwGraphicsSimpleTextItem::fontSize() const
{
    return (int)(m_font.size());
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSimpleTextItem::setFontSize(int size)
{
    m_font.setSize(size);
    updateBoundingRect();
}

//------------------------------------------------------------------------------
/**
*/
DwColor DwGraphicsSimpleTextItem::fontColor() const
{
    return m_font.color();
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSimpleTextItem::setFontColor(const DwColor& c)
{
    m_font.setColor(c);
    update();
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSimpleTextItem::updateBoundingRect()
{
    setBoundingRect(m_font.MeasureString(m_text));
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSimpleTextItem::paint(DwPainter *painter, const DwStyleOptionGraphicsItem *option)
{
    DW_UNUSED(option);
    //painter->fillRect(boundingRect(), DwColor(128, 145, 0, 128));
    painter->drawText(boundingRect(), m_text, m_font);
}

//------------------------------------------------------------------------------
/**
*/
DwString DwGraphicsSimpleTextItem::accessibleDescription() const
{
    if (!m_accessibleDescription.isNull())
    {
        return m_accessibleDescription;
    }

    return m_text;
}