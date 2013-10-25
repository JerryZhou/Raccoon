#pragma once
#include "dwgraphics/dwfont.h"
#include "dwgui/dwrttiobject.h"
#include "dwcore/dwsize.h"

//------------------------------------------------------------------------------
class DwRichChar;
class DwPainter;

//------------------------------------------------------------------------------
class  DW_GUI_EXPORT DwCharFormat : public DwRttiObject
{
    DW_DECLARE_CLASS(DwCharFormat);
public:
    DwCharFormat();
    virtual ~DwCharFormat();

    DwFont & font() ;
    const DwFont & font() const;
    virtual DwSizeF pixelMetric(DwRichChar *c) const;
    virtual DwSizeF pixelMetric(DwRichChar *chars, int num) const;
    virtual DwSizeF pixelMetric(const DwString& s) const;

    virtual DwSizeF paint(DwPainter *p, DwPointF topLeft, DwRichChar *c);
    virtual DwSizeF paint(DwPainter *p, DwPointF topLeft, DwRichChar *chars, int num);
    virtual DwSizeF paint(DwPainter *p, DwRectF rect, const DwString& s);

    virtual void onAdd(const DwRichChar *c);
    virtual void onDel(const DwRichChar *c);
    virtual void onLayout(const DwRectF &rect, const DwString &s);
    virtual void refresh();

protected:
    DwFont m_font;
};// end of DwCharFormat
DW_REGISTER_CLASS(DwCharFormat);
