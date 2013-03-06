#include "stable.h"
#include "dwgraphicsedittext.h"
#include "dwgui/dwgraphicspixmapitem.h"
#include "dwgui/dwgraphicssimpletextitem.h"
#include "dwgui/dwanchors.h"
#include "dwgui/dwclipboard.h"
#include "dwgraphics/dwimage.h"
#include "dwgraphics/dwpainter.h"
#include "dwinputfocuspainter.h"

DW_IMPLEMENT_CLASS(DwGraphicsEditText, 'GETT', DwGraphicsItem);

//------------------------------------------------------------------------------
/**
*/
DwGraphicsEditText::DwGraphicsEditText(DwGraphicsItem *parent/* = NULL*/)
    : DwGraphicsItem(parent)
    , m_doc (NULL)
    , m_layoutEngine (NULL)
    , m_xDisplayOffset (0)
    , m_yDisplayOffset (0)
{
    setName(L"graphicsEditText");
    setFlag(Dw::ItemClipsToShape);

    m_doc = new DwTextDocument();
    m_layoutEngine = new DwTextLayoutEngine();
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsEditText::~DwGraphicsEditText()
{
    DW_SAFE_DELETE(m_doc);
    DW_SAFE_DELETE(m_layoutEngine);
}

//------------------------------------------------------------------------------
/**
*/
DwTextDocument& DwGraphicsEditText::doc()
{
    return *m_doc;
}

//------------------------------------------------------------------------------
/**
*/
DwTextLayoutEngine & DwGraphicsEditText::layoutEngine()
{
    return *m_layoutEngine;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsEditText::paint(DwPainter *painter, const DwStyleOptionGraphicsItem *option)
{
    DW_UNUSED(option);
    // draw text
    DwVector<DwString> texts;
    DwVector<DwCharFormat*> formats;
    DwVector<DwRectF> rects;
    bool isOk = m_layoutEngine->layout(m_doc, boundingRect(), texts, rects, formats);
    if (isOk)
    {
        for (int i = 0; i<texts.size(); ++i)
        {
            formats.at(i)->paint(painter, rects.at(i).translated(offset()), texts.at(i));
        }
    }

    // draw cursor
    DwPointF from;
    DwPointF to;
    isOk = m_layoutEngine->layout(m_doc, boundingRect(), from, to);
    if (isOk)
    {
        dwInputFocusPainter()->paint(painter, this, from + offset(), to + offset(), DwColor(0, 0, 0, 255));
    }

    // draw select
    DwVector<DwRectF> selectRects;
    isOk = m_layoutEngine->layout(m_doc, boundingRect(), selectRects);
    if (isOk && selectRects.size() > 0)
    {
        for (int i=0; i<selectRects.size(); ++i)
        {
            painter->fillRect(selectRects.at(i).translated(offset()), DwColor(0, 0, 151, 64));
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsEditText::notifyItemChange(Dw::GraphicsItemChange change, const DwVariant& newOne, const DwVariant& oldOne)
{
    if (change == Dw::ItemGemoryChange)
    {
        DwRectF newRect = newOne.as<DwRectF>();
        DwRectF oldRect = oldOne.as<DwRectF>();
        if (newRect.width() != oldRect.width() && m_doc->textFormatAt(0)->isAutoLineBreak())
        {
            m_layoutEngine->invalidateLayoutCache(DwTextLayoutEngine::Content);
        }
    }

    __super::notifyItemChange(change, newOne, oldOne);
}

//------------------------------------------------------------------------------
/**
*/
DwPoint DwGraphicsEditText::offset() const
{
    return DwPointF(m_xDisplayOffset, m_yDisplayOffset).toPoint();
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwGraphicsEditText::offsetX() const
{
    return m_xDisplayOffset;
}

//------------------------------------------------------------------------------
/**
*/
DwReal DwGraphicsEditText::offsetY() const
{
    return m_yDisplayOffset;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsEditText::setDisplayOffsetX(DwReal x)
{
    m_xDisplayOffset = x;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsEditText::setDisplayOffsetY(DwReal y)
{
    m_yDisplayOffset = y;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsEditText::setDocument(DwTextDocument *d)
{
    if (m_doc == d)
    {
        return ;
    }

    if (m_doc)
    {
        DW_SAFE_DELETE(m_doc);
    }

    m_doc = d;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsEditText::setLayoutEngine(DwTextLayoutEngine *engine)
{
    if (m_layoutEngine == engine)
    {
        return;
    }

    if (m_layoutEngine)
    {
        DW_SAFE_DELETE(m_layoutEngine);
    }

    m_layoutEngine = engine;
}