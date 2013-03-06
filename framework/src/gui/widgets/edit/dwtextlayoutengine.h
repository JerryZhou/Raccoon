#pragma once
#include "dwgui/dwrttiobject.h"
#include "dwcore/dwvector.h"
#include "dwcore/dwrect.h"
#include "dwgui/dwcharformat.h"

//------------------------------------------------------------------------------
class DwTextDocument;
class DwTextCursor;

// ONLY SUPPORT THE ONE LINE LAYOUT
//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwTextLayoutEngine : public DwRttiObject
{
    DW_DECLARE_CLASS(DwTextLayoutEngine);
public:
    enum LayoutCache { Content = 0x01, Cursor = 0x02, Select = 0x04, ALL = 0xFFFF, };
    DwTextLayoutEngine();
    virtual ~DwTextLayoutEngine();

    // invalidate the cache
    virtual bool invalidateLayoutCache(int c);

    // the text content
    virtual bool layout(DwTextDocument * doc, const DwRectF &rect, DwVector<DwString>& strs, DwVector<DwRectF> &rects, DwVector<DwCharFormat*> &format) const;
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

    // set the org conetnt rect
    void setOrgContentRect(const DwRectF &r);
    // the org content rect
    DwRectF orgContentRect() const;

protected:
    // caculate the cursor point
    virtual bool layoutCursor(DwTextDocument * doc, const DwRectF &rect, const DwTextCursor& cursor, DwPointF &from, DwPointF &to) const;
    // set the flag of cache
    void setCacheValidate(LayoutCache, bool b = true);
    // require the cached
    bool isCachedValidate(LayoutCache) const;

    DwRectF m_orgContentRect;
    int m_validateCache;
};// end of DwTextLayoutEngine
DW_REGISTER_CLASS(DwTextLayoutEngine);