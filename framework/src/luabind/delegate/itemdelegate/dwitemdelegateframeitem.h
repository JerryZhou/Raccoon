#pragma once
#include "dwluabind/dwitemdelegate.h"

//------------------------------------------------------------------------------
class DW_LUABIND_EXPORT DwItemDelegateFrameItem : public DwItemDelegate
{
    DW_DECLARE_CLASS(DwItemDelegateFrameItem);
public:
    DwItemDelegateFrameItem();
};// end of DwItemDelegateFrameItem
//------------------------------------------------------------------------------
DW_REGISTER_CLASS(DwItemDelegateFrameItem);