#pragma once
#include "dwgui/dwwidgetitem.h"
#include "dwcore/dwmargins.h"

//------------------------------------------------------------------------------
class DwGraphicsSimpleTextItem;
class DwGraphicsFrameItem;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwLabel : public DwWidgetItem
{
    DW_DECLARE_CLASS(DwLabel);
public:
    DwLabel(DwGraphicsItem *parent = NULL);
    virtual ~DwLabel();

    void setText(const DwString& txt);
    inline DwGraphicsSimpleTextItem *text();

protected:
    void updateBoundingRect();

    DwGraphicsFrameItem *m_background;
    DwGraphicsSimpleTextItem *m_text;
    DwMargins m_margins;
};// end of DwLabel
DW_REGISTER_CLASS(DwLabel);

//------------------------------------------------------------------------------
/**
*/
DwGraphicsSimpleTextItem *DwLabel::text()
{
    return m_text;
}