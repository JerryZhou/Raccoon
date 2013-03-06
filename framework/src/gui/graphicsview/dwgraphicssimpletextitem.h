#pragma once
#include "dwcore/dwstring.h"
#include "dwgraphicsitem.h"
#include "dwgraphics/dwfont.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwGraphicsSimpleTextItem : public DwGraphicsItem
{
    DW_DECLARE_CLASS(DwGraphicsSimpleTextItem);
public:
    DwGraphicsSimpleTextItem(DwGraphicsItem *parent = NULL, DwGraphicsScene *scene = NULL);
    explicit DwGraphicsSimpleTextItem(const DwString& text, DwGraphicsItem *parent = NULL, DwGraphicsScene *scene = NULL);
    virtual ~DwGraphicsSimpleTextItem();

    DwString text() const;
    void setText(const DwString& txt);

    DwFont font() const;
    void setFont(const DwFont &f);

    int fontSize() const;
    void setFontSize(int size);

    DwColor fontColor() const;
    void setFontColor(const DwColor& c);

    void updateBoundingRect();

    virtual void paint(DwPainter *painter, const DwStyleOptionGraphicsItem *option);

    // accessible
    virtual DwString accessibleDescription() const;

protected:
    DwString m_text;
    DwFont m_font;
};// end of DwGraphicsSimpleTextItem
DW_REGISTER_CLASS(DwGraphicsSimpleTextItem);