#pragma once
#include "dwgui/dwscrollarea.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwTreeWidget : public DwScrollArea
{
    DW_DECLARE_CLASS(DwTreeWidget);
public:
    explicit DwTreeWidget(DwGraphicsItem *parent = NULL);
    virtual ~DwTreeWidget();

protected:
private:
};// end of DwTreeWidget