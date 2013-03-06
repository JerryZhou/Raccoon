#pragma once
#include "dwluabind/dwitemdelegate.h"

//------------------------------------------------------------------------------
class DW_LUABIND_EXPORT DwItemDelegateBackgroundItem : public DwItemDelegate
{
    DW_DECLARE_CLASS(DwItemDelegateBackgroundItem);
public:
    DwItemDelegateBackgroundItem();
protected:
};// end of DwItemDelegatePixmap
//------------------------------------------------------------------------------
DW_REGISTER_CLASS(DwItemDelegateBackgroundItem);