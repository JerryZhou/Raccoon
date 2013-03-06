#include "stable.h"
#include "dwmultitextlayoutengine.h"
#include "dwtextdocument.h"
#include "dwtextcursor.h"
#include "dwtextformat.h"
#include "dwcharformat.h"
#include "graphicsview/dwdummys.h"

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwMultiTextLayoutEngine, 'MTLE', DwTextLayoutEngine);

//------------------------------------------------------------------------------
/**
*/
bool DwMultiTextLayoutEngine::invalidateLayoutCache(int c)
{
    __super::invalidateLayoutCache(c);
    if (c & Content)
    {
        m_texts.clear();
        m_formats.clear();
        m_rects.clear();
    }
    if (c & Cursor)
    {
        m_from = DwPoint();
        m_to = DwPoint();
    }
    if (c & Select)
    {
        m_selectRects.clear();
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void DwMultiTextLayoutEngine::layoutCalcuteLine(DwTextDocument * doc, const DwRectF &rect,
        DwTextFormat* textFormat, DwCharFormat * charFormat,
        int start, int end, DwString &s,
        DwPointF &topLeft , DwReal &maxLineHeight, int &lineStartIndex, int &lineEndIndex,
        DwVector<DwString>& texts, DwVector<DwRectF> &rects, DwVector<DwCharFormat*> &formats)const
{
    int j=start;
    while (j < end)
    {
        for (; j<end; ++j)
        {
            wchar_t c = doc->m_chars.at(j).character();
            if (c == '\n')
            {
                DwSizeF size;
                if (!s.isEmpty())
                {
                    size = charFormat->pixelMetric(s);
                    texts.append(s);
                    rects.append(DwRect(topLeft.toPoint(), size.toSize()));
                    formats.append(charFormat);
                    s.clear();
                }

                size.setWidth(0);
                if(size.height() == 0)
                {
                    DwSizeF sf = charFormat->pixelMetric(DwString(wchar_t('a')));
                    size.setHeight(sf.height());
                }

                if (maxLineHeight < size.height())
                {
                    maxLineHeight = size.height();
                }

                topLeft.setX(rect.topLeft().x());
                topLeft.setY(topLeft.y() + maxLineHeight + textFormat->lineSpacing());

                s.append(c);
                texts.append(s);
                rects.append(DwRectF(topLeft, size));
                formats.append(charFormat);
                s.clear();

                // adjust the line
                lineEndIndex = texts.size();
                for (int n=lineStartIndex; n<lineEndIndex; ++n)
                {
                    DwRectF& r = rects[n];
                    int dis = (int)(maxLineHeight) - (int)r.height();
                    if (dis > 0)
                    {
                        r.translate(0, dis/2);
                    }
                }

                // set the next line
                lineStartIndex = lineEndIndex;
                maxLineHeight = size.height();

                ++j;
                break;
            }
            else
            {
                s.append(c);
            }

            DwSizeF size = charFormat->pixelMetric(s);
            if (topLeft.x() + size.width() > rect.width() && textFormat->isAutoLineBreak())
            {
                s = s.remove(s.length()-1, 1);

                DwSizeF size;
                if (!s.isEmpty())
                {
                    size = charFormat->pixelMetric(s);
                    texts.append(s);
                    rects.append(DwRectF(topLeft, size));
                    formats.append(charFormat);
                    s.clear();
                }
                if(size.height() == 0)
                {
                    DwSizeF sf = charFormat->pixelMetric(DwString(wchar_t('a')));
                    size.setHeight(sf.height());
                }

                if (maxLineHeight < size.height())
                {
                    maxLineHeight = size.height();
                }

                // set the next line topleft
                topLeft.setX(rect.topLeft().x());
                topLeft.setY(topLeft.y() + maxLineHeight + textFormat->lineSpacing());

                // adjust the line
                lineEndIndex = texts.size();
                adjustLineHeight(rects, lineStartIndex, lineEndIndex, maxLineHeight);

                // set the next line
                lineStartIndex = lineEndIndex;
                maxLineHeight = 0;
                break;
            }
        }
    }

    if (!s.isEmpty())
    {
        DwSizeF size = charFormat->pixelMetric(s);
        texts.append(s);
        rects.append(DwRectF(topLeft, size));
        formats.append(charFormat);

        if (maxLineHeight < size.height())
        {
            maxLineHeight = size.height();
        }

        topLeft.setX(topLeft.x() + size.width() - 5);
    }
}

void DwMultiTextLayoutEngine::adjustLineHeight(DwVector<DwRectF> &rects,
        int lineStartIndex, int lineEndIndex,
        DwReal maxLineHeight)const
{
    for (int n=lineStartIndex; n<lineEndIndex; ++n)
    {
        DwRectF& r = rects[n];
        int dis = (int)(maxLineHeight) - (int)r.height();
        if (dis > 0)
        {
            r.translate(0, dis/2);
        }
    }
}


//------------------------------------------------------------------------------
/**
*/
bool DwMultiTextLayoutEngine::layout(DwTextDocument * doc, const DwRectF &rect, DwVector<DwString>& texts,
                                     DwVector<DwRectF> &rects, DwVector<DwCharFormat*> &formats) const
{
    DW_CHECK_RET(doc->length() > 0, true);

    if (isCachedValidate(Content))
    {
        texts = m_texts;
        rects = m_rects;
        formats = m_formats;
        return true;
    }

    int lineStartIndex = 0;
    int lineEndIndex = 0;
    DwReal maxLineHeight = 0.0;

    // there should be a text format
    DwTextFormat* textFormat = doc->textFormatAt(0);
    DwPointF topLeft = rect.topLeft();
    for (int i=0; i < doc->m_charFormatIndexs.size(); ++i)
    {
        int formatIndex = doc->m_charContentsFormatIndexs.at(i);
        DwCharFormat* charFormat = doc->m_charFormats.at(formatIndex);
        int start = doc->m_charFormatIndexs.at(i);
        int end =  (i < (doc->m_charFormatIndexs.size() - 1)) ?
                   doc->m_charFormatIndexs.at(i+1) : (doc->m_chars.size());

        DwString s;
        layoutCalcuteLine(doc, rect, textFormat, charFormat,
                          start, end,
                          s, topLeft, maxLineHeight, lineStartIndex, lineEndIndex,
                          texts, rects, formats);
    }

    // adjust the line
    lineEndIndex = texts.size();
    adjustLineHeight(rects, lineStartIndex, lineEndIndex, maxLineHeight);

    // call in formats
    for (int m=0; m < texts.size(); ++m)
    {
        formats.at(m)->onLayout(rects.at(m), texts.at(m));
    }

    const_cast<DwMultiTextLayoutEngine*>(this)->setCacheValidate(Content);
    const_cast<DwMultiTextLayoutEngine*>(this)->m_texts = texts;
    const_cast<DwMultiTextLayoutEngine*>(this)->m_rects = rects;
    const_cast<DwMultiTextLayoutEngine*>(this)->m_formats = formats;

    return texts.size() > 0;
}

//------------------------------------------------------------------------------
/**
*/
bool DwMultiTextLayoutEngine::layout(DwTextDocument * doc, const DwRectF &rect, DwPointF &from, DwPointF &to) const
{
    if (isCachedValidate(Cursor))
    {
        from = m_from;
        to = m_to;
        return true;
    }

    bool isOk = layoutCursor(doc, rect, doc->m_inputCursor, from, to);

    const_cast<DwMultiTextLayoutEngine*>(this)->setCacheValidate(Cursor);
    const_cast<DwMultiTextLayoutEngine*>(this)->m_from = from;
    const_cast<DwMultiTextLayoutEngine*>(this)->m_to = to;
    return isOk;
}

//------------------------------------------------------------------------------
/**
*/
bool DwMultiTextLayoutEngine::layout(DwTextDocument * doc, const DwRectF &rect, DwVector<DwRectF> &rects) const
{
    DW_UNUSED(rects);
    if (doc->m_inputCursor.cur() == doc->m_selectCursor.cur())
    {
        return false;
    }

    if (isCachedValidate(Select))
    {
        rects = m_selectRects;
        return true;
    }

    DwPointF inputFrom;
    DwPointF inputTo;
    bool isOk = layoutCursor(doc, rect, doc->m_inputCursor, inputFrom, inputTo);
    if (isOk)
    {
        DwPointF selectFrom;
        DwPointF selectTo;

        isOk = layoutCursor(doc, rect, doc->m_selectCursor, selectFrom, selectTo);

        if (selectFrom.y() != inputFrom.y())
        {
            if (selectFrom.y() > inputFrom.y())
            {
                rects.append(DwRectF(inputFrom, DwSizeF((int)rect.width() - 2 - inputFrom.x(), inputTo.y() - inputFrom.y())));
                rects.append(DwRectF(DwPointF(1, inputTo.y()), DwSizeF((int)(rect.width() - 4), selectFrom.y() - inputTo.y())));
                rects.append(DwRectF(DwPointF(1, selectFrom.y()), DwSizeF(selectFrom.x() - 2 + 1, selectTo.y() - selectFrom.y() + 1)));
            }
            else
            {
                rects.append(DwRectF(selectFrom, DwSizeF((int)rect.width() - 2 - selectFrom.x(), selectTo.y() - selectFrom.y())));
                rects.append(DwRectF(DwPointF(1, selectTo.y()), DwSizeF((int)(rect.width() - 4), inputFrom.y() - selectTo.y())));
                rects.append(DwRectF(DwPointF(1, inputFrom.y()), DwSizeF(inputFrom.x() - 2 + 1, inputTo.y() - inputFrom.y() + 1)));
            }
        }
        else
        {
            if (selectFrom.x() > inputFrom.x())
            {
                rects.append(DwRectF(inputFrom, DwSizeF(selectFrom.x() - inputFrom.x() + 1, selectTo.y() - selectFrom.y() + 1)));
            }
            else
            {
                rects.append(DwRectF( selectFrom, DwSizeF(inputFrom.x() - selectFrom.x() + 1, inputTo.y() - inputFrom.y() + 1)));
            }
        }
    }

    const_cast<DwMultiTextLayoutEngine*>(this)->setCacheValidate(Select);
    const_cast<DwMultiTextLayoutEngine*>(this)->m_selectRects = rects;

    return isOk;
}

//------------------------------------------------------------------------------
/**
*/
int DwMultiTextLayoutEngine::numOfPrintChar(DwTextDocument * doc, const DwRectF &rect, const DwPointF &p) const
{
    DwPointF topLeft = rect.topLeft() + DwPointF(1, 1);
    DwVector<DwString> texts;
    DwVector<DwCharFormat*> formats;
    DwVector<DwRectF> rects;
    bool isOk = layout(doc, rect, texts, rects, formats);
    if (isOk)
    {
        int n = 0;
        int i=0;
        for (; i<rects.size(); ++i)
        {
            DwCharFormat* charFormat = formats.at(i);
            int start = n;
            DwString ls = texts.at(i);
            n += ls.length();
            DwRectF lr = rects.at(i);
            if (lr.contains(p))
            {
                topLeft = lr.topLeft();
                DwString s;
                int end = n;
                for (int j=start; j<end; ++j)
                {
                    s.append(doc->m_chars.at(j).character());
                    DwSizeF size = charFormat->pixelMetric(s);

                    if (topLeft.x() + size.width() - 2 >= p.x())
                    {
                        return j;
                    }
                }

                return n;
            }
            else if (i<rects.size()-1)
            {
                DwRectF lrNext = rects.at(i+1);
                if (lrNext.topLeft().y() >= lr.bottomRight().y())
                {
                    // next line
                    if(lr.bottomRight().y() > p.y() && lr.width() > 0)
                    {
                        if (lr.bottomRight().x() < p.x())
                        {
                            return n;
                        }
                        else
                        {
                            return start;
                        }
                    }
                }
                else
                {
                    ;
                }
            }
            else if(lr.bottomRight().y() > p.y() && lr.width() > 0)
            {
                if (lr.bottomRight().x() < p.x())
                {
                    return n;
                }
                else
                {
                    return start;
                }
            }
            else
            {
                ;
            }
        }
    }

    return doc->length();
}

//------------------------------------------------------------------------------
/**
*/
bool DwMultiTextLayoutEngine::contentRect(DwTextDocument * doc, const DwRectF &rect, DwRectF & textRect)
{
    return supperClass::contentRect(doc, rect, textRect);
}


//------------------------------------------------------------------------------
/**
*/
bool DwMultiTextLayoutEngine::onAdd(DwTextDocument * doc,const DwRectF &rect, DwRichChar *c, int pos /*= -1*/)
{
    DW_UNUSED(c);
    DW_UNUSED(pos);

    if (pos == -1)
    {
        pos = doc->length();
    }

    if (pos < 0 || pos > doc->length())
    {
        return false;
    }

    if (pos == doc->length()-1)
    {
        DwTextFormat *textFormat = doc->textFormatAt(0);
        DwCharFormat *charFormat = doc->charFormatAt(doc->charFormatNum()-1);

        int start = pos;
        int end = doc->length();
        int lineStartIndex = m_rects.size() > 0 ? m_rects.size() -1 : 0;
        int lineEndIndex = lineStartIndex;
        DwString s = m_texts.size() > 0 ? m_texts.back() : DwString();
        DwPointF topLeft = m_rects.size() > 0 ? m_rects.back().topLeft() : rect.topLeft();
        DwReal maxLineHeight = m_rects.size() > 0 ? m_rects.back().height() : 0.0;
        int m=lineStartIndex;

        DwCharFormat *lastCharFormat = m_formats.size() > 0 ? m_formats.back() : charFormat;
        if (charFormat != lastCharFormat)
        {
            DwSizeF size = lastCharFormat->pixelMetric(s);
            topLeft.setX(topLeft.x() + size.width() - 5);

            s = DwString();
        }
        else
        {
            if (m_rects.size() > 0)
            {
                m_rects.pop_back();
                m_texts.pop_back();
                m_formats.pop_back();
            }
        }

        // calculate the layout of last char
        layoutCalcuteLine(doc, rect, textFormat, charFormat,
                          start, end, s, topLeft, maxLineHeight, lineStartIndex, lineEndIndex,
                          m_texts, m_rects, m_formats);

        // adjust the line
        lineEndIndex = m_texts.size();
        adjustLineHeight(m_rects, lineStartIndex, lineEndIndex, maxLineHeight);

        // call in formats
        for (; m < m_texts.size(); ++m)
        {
            m_formats.at(m)->onLayout(m_rects.at(m), m_texts.at(m));
        }
    }
    else
    {
        invalidateLayoutCache(Content);
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwMultiTextLayoutEngine::onDel(DwTextDocument * doc,const DwRectF &rect, DwRichChar *c, int pos /*= -1*/)
{
    DW_UNUSED(c);
    DW_UNUSED(pos);
    DW_UNUSED(rect);

    if (pos == -1)
    {
        pos = doc->length();
    }

    if (pos < 0 || pos > doc->length())
    {
        return false;
    }

    // TODO: xx
    if (pos == doc->length())
    {
        invalidateLayoutCache(Content);
    }
    else
    {
        invalidateLayoutCache(Content);
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwMultiTextLayoutEngine::layoutCursor(DwTextDocument * doc, const DwRectF &rect, const DwTextCursor& cursor, DwPointF &from, DwPointF &to) const
{
    DwPointF topLeft = rect.topLeft();
    int cursorEnd = cursor.cur();

    from = rect.topLeft().toPoint();
    to = rect.bottomLeft().toPoint();

    if (doc->length() == 0)
    {
        from.setX(from.x() + 1);
        to.setX(to.x() + 1);
        return true;
    }

    // draw text
    DwVector<DwString> texts;
    DwVector<DwCharFormat*> formats;
    DwVector<DwRectF> rects;
    bool isOk = layout(doc, rect, texts, rects, formats);
    if (isOk)
    {
        int n = 0;
        int i=0;
        for (; i<texts.size(); ++i)
        {
            int start = n;
            DwString ls = texts.at(i);
            n += ls.length();
            if (n >= cursorEnd)
            {
                DwString s;
                int j=start;
                for (; j<cursorEnd; ++j)
                {
                    s.append(ls.at(j-start));
                }
                DwSizeF size (0, 0);
                if(!s.isEmpty())
                {
                    size = formats.at(i)->pixelMetric(s);

                    if (size.width() > rects.at(i).width())
                    {
                        size.setWidth(rects.at(i).width());
                    }
                    else if (size.height() > rects.at(i).height())
                    {
                        size.setHeight(rects.at(i).height());
                    }
                }
                else
                {
                    size.setWidth(0);
                    size.setHeight(rects.at(i).height());
                }

                if (cursorEnd == j)
                {
                    topLeft = rects.at(i).topLeft();
                    DwRectF inputRect = DwRectF(topLeft, size);
                    from = inputRect.topRight();
                    to = inputRect.bottomRight();

                    break;
                }
            }
        }
    }

    if (from.x() > 0)
    {
        from.setX(from.x() - 2);
        to.setX(to.x() - 2);
    }
    else
    {
        from.setX(rect.topLeft().toPoint().x() + 1);
        to.setX(rect.bottomLeft().toPoint().x() + 1);
    }

    return isOk;
}