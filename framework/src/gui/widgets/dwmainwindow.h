#pragma once
#include "dwgui/dwframewindow.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwMainWindow : public DwFrameWindow
{
    DW_DECLARE_CLASS(DwMainWindow);
public:
    explicit DwMainWindow(DwObject *parent=NULL);
    virtual ~DwMainWindow();
protected:
private:
};// end of DwMainWindow
DW_REGISTER_CLASS(DwMainWindow);