#pragma once
#include "dwgui/dwaccessibleitem.h"
#include "dwgui/dwframewindow.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwAccessibleWindow : public DwAccessibleItem
{
    DW_DECLARE_CLASS(DwAccessibleWindow);
public:
    explicit DwAccessibleWindow(DwGraphicsItem *o=NULL, Role r = Client, const DwString& name = DwString());
protected:
    virtual DwVector<DwGraphicsItem*> childWidgets()const;
    DwFrameWindow* window()const;
};// end of DwAccessibleWindow