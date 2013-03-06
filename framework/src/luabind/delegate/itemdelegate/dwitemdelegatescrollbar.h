#pragma once
#include "dwluabind/dwitemdelegate.h"

//------------------------------------------------------------------------------
class DW_LUABIND_EXPORT DwItemDelegateScrollBar : public DwItemDelegate
{
    DW_DECLARE_CLASS(DwItemDelegateScrollBar);
public:
    DwItemDelegateScrollBar();
protected:
};// end of DwItemDelegateScrollBar
//------------------------------------------------------------------------------
DW_REGISTER_CLASS(DwItemDelegateScrollBar);