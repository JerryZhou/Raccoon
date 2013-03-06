#pragma once
#include "dwluabind/dwitemdelegatewidgetitem.h"

//------------------------------------------------------------------------------
class DW_LUABIND_EXPORT DwItemDelegateFrameWindow : public DwItemDelegateWidgetItem
{
    DW_DECLARE_CLASS(DwItemDelegateFrameWindow);
public:
    DwItemDelegateFrameWindow();
};// end of DwItemDelegateFrameWindow
//------------------------------------------------------------------------------
DW_REGISTER_CLASS(DwItemDelegateFrameWindow);