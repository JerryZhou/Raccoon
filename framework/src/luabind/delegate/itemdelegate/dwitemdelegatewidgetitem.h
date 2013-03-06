#pragma once
#include "dwluabind/dwitemdelegate.h"

//------------------------------------------------------------------------------
class DW_LUABIND_EXPORT DwItemDelegateWidgetItem : public DwItemDelegate
{
    DW_DECLARE_CLASS(DwItemDelegateWidgetItem);
public:
    DwItemDelegateWidgetItem();
};// end of DwItemDelegateWidgetItem
//------------------------------------------------------------------------------
DW_REGISTER_CLASS(DwItemDelegateWidgetItem);