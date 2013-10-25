#include "stable.h"
#include "dwcharformat.h"
#include "dwrichchar.h"
#include "dwgraphics/dwpainter.h"

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwCharFormat, 'CFMT', DwRttiObject);

//------------------------------------------------------------------------------
/**
*/
DwCharFormat::DwCharFormat()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwCharFormat::~DwCharFormat()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwFont & DwCharFormat::font()
{
    return m_font;
}

//------------------------------------------------------------------------------
/**
*/
const DwFont & DwCharFormat::font() const
{
    return m_font;
}

//------------------------------------------------------------------------------
/**
*/
DwSizeF DwCharFormat::pixelMetric(DwRichChar *c) const
{
    return pixelMetric(DwString(c->character()));
}

//------------------------------------------------------------------------------
/**
*/
DwSizeF DwCharFormat::pixelMetric(DwRichChar *chars, int num) const
{
    DwString s;
    for (int i=0; i<num; ++i)
    {
        s.append((chars + i)->character());
    }
    return pixelMetric(s);
}

//------------------------------------------------------------------------------
/**
*/
DwSizeF DwCharFormat::pixelMetric(const DwString& s) const
{
    return m_font.MeasureString(s).size();
}

//------------------------------------------------------------------------------
/**
*/
DwSizeF DwCharFormat::paint(DwPainter *p, DwPointF topLeft, DwRichChar *c)
{
    DwString s(c->character());
    DwSizeF size = pixelMetric(s);
    DwRect rect(topLeft.toPoint(), size.toSize());
    return paint(p, rect, s);
}

//------------------------------------------------------------------------------
/**
*/
DwSizeF DwCharFormat::paint(DwPainter *p, DwPointF topLeft, DwRichChar *chars, int num)
{
    DwString s;
    for (int i=0; i<num; ++i)
    {
        DwRichChar * pChar = chars + i;
        s.append(pChar->character());
    }

    DwSizeF size = pixelMetric(s);
    DwRect rect(topLeft.toPoint(), size.toSize());
    return paint(p, rect, s);
}

//------------------------------------------------------------------------------
/**
*/
DwSizeF DwCharFormat::paint(DwPainter *p, DwRectF rect, const DwString& s)
{
    if (!rect.isEmpty())
    {
        p->drawText(rect, s, m_font);
    }
    return rect.size();
}

//------------------------------------------------------------------------------
/**
*/
void DwCharFormat::onAdd(const DwRichChar *c)
{
    DW_UNUSED(c);
}

//------------------------------------------------------------------------------
/**
*/
void DwCharFormat::onDel(const DwRichChar *c)
{
    DW_UNUSED(c);
}

//------------------------------------------------------------------------------
/**
*/
void DwCharFormat::onLayout(const DwRectF &rect, const DwString &s)
{
    DW_UNUSED(rect);
    DW_UNUSED(s);
}

//------------------------------------------------------------------------------
/**
*/
void DwCharFormat::refresh()
{

}
