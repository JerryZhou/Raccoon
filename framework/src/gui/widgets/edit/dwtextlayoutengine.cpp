#include "stable.h"
#include "dwtextlayoutengine.h"
#include "dwtextdocument.h"
#include "dwtextcursor.h"
#include "graphicsview/dwdummys.h"

//------------------------------------------------------------------------------
/**
*/
DW_IMPLEMENT_CLASS(DwTextLayoutEngine, 'TLEE', DwRttiObject);

//------------------------------------------------------------------------------
/**
*/
DwTextLayoutEngine::DwTextLayoutEngine()
    : m_validateCache(0)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwTextLayoutEngine::~DwTextLayoutEngine()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
bool DwTextLayoutEngine::invalidateLayoutCache(int c)
{
    if (c & Content)
    {
        setCacheValidate(Content, false);
    }
    if (c & Cursor)
    {
        setCacheValidate(Cursor, false);
    }
    if (c & Select)
    {
        setCacheValidate(Select, false);
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwTextLayoutEngine::layout( DwTextDocument * doc, const DwRectF &rect, DwVector<DwString>& texts,
                                 DwVector<DwRectF> &rects, DwVector<DwCharFormat*> &formats ) const
{
    DW_CHECK_RET(doc->length() > 0, true);

    DwPointF topLeft = rect.topLeft();
    for (int i=0; i < doc->m_charFormatIndexs.size(); ++i)
    {
        int formatIndex = doc->m_charContentsFormatIndexs.at(i);
        DwCharFormat* charFormat = doc->m_charFormats.at(formatIndex);
        int start = doc->m_charFormatIndexs.at(i);
        int end =  (i < (doc->m_charFormatIndexs.size() - 1)) ?
                   doc->m_charFormatIndexs.at(i+1) : (doc->m_chars.size());

        DwString s;
        for (int j=start; j<end; ++j)
        {
            s.append(doc->m_chars.at(j).character());
        }
        DwSizeF size = charFormat->pixelMetric(s);

        texts.append(s);
        rects.append(DwRectF(topLeft, size));
        formats.append(charFormat);

        topLeft.setX(topLeft.x() + size.width());
        if (topLeft.x() > rect.width())
        {
            topLeft.setY(topLeft.y() + size.height());
            topLeft.setX(rect.x());
        }
    }

    return texts.size() > 0;
}

//------------------------------------------------------------------------------
/**
*/
bool DwTextLayoutEngine::layout( DwTextDocument * doc, const DwRectF &rect,
                                 DwPointF &from, DwPointF &to ) const
{
    return layoutCursor(doc, rect, doc->m_inputCursor, from, to);
}

//------------------------------------------------------------------------------
/**
*/
bool DwTextLayoutEngine::layout( DwTextDocument * doc, const DwRectF &rect,
                                 DwVector<DwRectF> &rects ) const
{
    DW_UNUSED(rects);
    if (doc->m_inputCursor.cur() == doc->m_selectCursor.cur())
    {
        return false;
    }

    DwPointF inputFrom;
    DwPointF inputTo;
    bool isOk = layoutCursor(doc, rect, doc->m_inputCursor, inputFrom, inputTo);
    if (isOk)
    {
        DwPointF selectFrom;
        DwPointF selectTo;

        isOk = layoutCursor(doc, rect, doc->m_selectCursor, selectFrom, selectTo);

        if (selectFrom.x() > inputFrom.x())
        {
            rects.append(DwRectF(inputFrom, DwSizeF(selectFrom.x() - inputFrom.x() + 1, selectTo.y() - selectFrom.y() + 1)));
        }
        else
        {
            rects.append(DwRectF( selectFrom, DwSizeF(inputFrom.x() - selectFrom.x() + 1, inputTo.y() - inputFrom.y() + 1)));
        }
    }

    return isOk;
}

//------------------------------------------------------------------------------
/**
*/
int DwTextLayoutEngine::numOfPrintChar(DwTextDocument * doc, const DwRectF &rect, const DwPointF &p) const
{
    DwPointF topLeft = rect.topLeft() + DwPointF(1, 1);
    for (int i=0; i < doc->m_charFormatIndexs.size(); ++i)
    {
        int formatIndex = doc->m_charContentsFormatIndexs.at(i);
        DwCharFormat* charFormat = doc->m_charFormats.at(formatIndex);
        int start = doc->m_charFormatIndexs.at(i);
        int end =  (i < (doc->m_charFormatIndexs.size() - 1)) ?
                   doc->m_charFormatIndexs.at(i+1) : (doc->m_chars.size());

        DwString s;
        for (int j=start; j<end; ++j)
        {
            s.append(doc->m_chars.at(j).character());
            DwSizeF size = charFormat->pixelMetric(s);

            if (topLeft.x() + size.width() - 2 >= p.x())
            {
                return j;
            }
        }
    }
    return doc->length();
}

//------------------------------------------------------------------------------
/**
*/
bool DwTextLayoutEngine::contentRect(DwTextDocument * doc, const DwRectF &rect, DwRectF & textRect)
{
    textRect = m_orgContentRect;

    DwVector<DwString> texts;
    DwVector<DwCharFormat*> formats;
    DwVector<DwRectF> rects;
    bool isOk = layout(doc, rect, texts, rects, formats);
    if (isOk)
    {
        for (int i = 0; i<rects.size(); ++i)
        {
            textRect |= rects.at(i);
        }
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwTextLayoutEngine::onAdd(DwTextDocument * doc, const DwRectF &rect, DwRichChar *c, int pos /*= -1*/)
{
    DW_UNUSED(doc);
    DW_UNUSED(rect);
    DW_UNUSED(c);
    DW_UNUSED(pos);
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwTextLayoutEngine::onDel(DwTextDocument * doc, const DwRectF &rect, DwRichChar *c, int pos /*= -1*/)
{
    DW_UNUSED(doc);
    DW_UNUSED(rect);
    DW_UNUSED(c);
    DW_UNUSED(pos);
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void DwTextLayoutEngine::setOrgContentRect(const DwRectF &r)
{
    m_orgContentRect = r;
}

//------------------------------------------------------------------------------
/**
*/
DwRectF DwTextLayoutEngine::orgContentRect() const
{
    return m_orgContentRect;
}

//------------------------------------------------------------------------------
/**
*/
bool DwTextLayoutEngine::layoutCursor(DwTextDocument * doc, const DwRectF &rect, const DwTextCursor& cursor,
                                      DwPointF &from, DwPointF &to)const
{
    DwPointF topLeft = rect.topLeft();
    int cursorEnd = cursor.cur();

    from = rect.topLeft().toPoint();
    to = rect.bottomLeft().toPoint();

    if (doc->length() == 0 || cursorEnd == 0)
    {
        from.setX(from.x() + 1);
        to.setX(to.x() + 1);
        return true;
    }

    for (int i=0; i < doc->m_charFormatIndexs.size(); ++i)
    {
        int formatIndex = doc->m_charContentsFormatIndexs.at(i);
        DwCharFormat* charFormat = doc->m_charFormats.at(formatIndex);
        int start = doc->m_charFormatIndexs.at(i);
        int end =  (i < (doc->m_charFormatIndexs.size() - 1)) ?
                   doc->m_charFormatIndexs.at(i+1) : (doc->m_chars.size());

        DwString s;
        int j=start;
        for (start; j<end && j<cursorEnd; ++j)
        {
            s.append(doc->m_chars.at(j).character());
        }
        DwSizeF size = charFormat->pixelMetric(s);

        if (cursorEnd == j)
        {
            DwRectF inputRect = DwRectF(topLeft, size);
            from = inputRect.topRight();
            to = inputRect.bottomRight();

            break;
        }

        topLeft.setX(topLeft.x() + size.width());
        if (topLeft.x() > rect.width())
        {
            topLeft.setY(topLeft.y() + size.height());
            topLeft.setX(rect.x());
        }
    }

    from.setX(from.x() - 2);
    to.setX(to.x() - 2);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void DwTextLayoutEngine::setCacheValidate(LayoutCache c, bool b/* = true*/)
{
    if (b)
    {
        m_validateCache |= c;
    }
    else
    {
        m_validateCache &= (~c);
    }
}

//------------------------------------------------------------------------------
/**
*/
bool DwTextLayoutEngine::isCachedValidate(LayoutCache c) const
{
    return !!(m_validateCache & c);
}
