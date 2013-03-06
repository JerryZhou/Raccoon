#include "dwgui/dwtextlayoutengine.h"

//------------------------------------------------------------------------------
class DwTextFormat;
class DwCharFormat;
class DwRichChar;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwMultiTextLayoutEngine : public DwTextLayoutEngine
{
    DW_DECLARE_CLASS(DwMultiTextLayoutEngine);
    typedef DwTextLayoutEngine supperClass;
public:
    // invalidate the cache
    virtual bool invalidateLayoutCache(int c);

    // the text content
    virtual bool layout(DwTextDocument * doc, const DwRectF &rect, DwVector<DwString>& texts, DwVector<DwRectF> &rects, DwVector<DwCharFormat*> &formats) const;
    // the flash cursor
    virtual bool layout(DwTextDocument * doc, const DwRectF &rect, DwPointF &from, DwPointF &to) const;
    // the select rect
    virtual bool layout(DwTextDocument * doc, const DwRectF &rect, DwVector<DwRectF> &rects) const;
    // calcute the num of char from begin to p, int rect
    virtual int numOfPrintChar(DwTextDocument * doc, const DwRectF &rect, const DwPointF &p) const;
    // calcute the text content rect
    virtual bool contentRect(DwTextDocument * doc, const DwRectF &rect, DwRectF & textRect);
    // add a character
    virtual bool onAdd(DwTextDocument * doc, const DwRectF &rect, DwRichChar *c, int pos = -1);
    // remove a character
    virtual bool onDel(DwTextDocument * doc, const DwRectF &rect, DwRichChar *c, int pos = -1);

protected:
    virtual bool layoutCursor(DwTextDocument * doc, const DwRectF &rect, const DwTextCursor& cursor, DwPointF &from, DwPointF &to) const;

    // calcute the layout
    void layoutCalcuteLine(DwTextDocument * doc, const DwRectF &rect, DwTextFormat* textFormat, DwCharFormat * charFormat, int start, int end, DwString &s,
                           DwPointF &topLeft , DwReal &maxLineHeight, int &lineStartIndex, int &lineEndIndex, DwVector<DwString>& texts,
                           DwVector<DwRectF> &rects, DwVector<DwCharFormat*> &formats)const;

    void adjustLineHeight(DwVector<DwRectF> &rects, int lineStartIndex, int lineEndIndex, DwReal maxLineHeight)const;

    // the whole document layout information
    DwVector<DwString> m_texts;
    DwVector<DwCharFormat*> m_formats;
    DwVector<DwRectF> m_rects;

    // current input cursor information
    DwPointF m_from;
    DwPointF m_to;

    // current select information
    DwVector<DwRectF> m_selectRects;
};// end of DwMultiTextLayoutEngine
DW_REGISTER_CLASS(DwMultiTextLayoutEngine);