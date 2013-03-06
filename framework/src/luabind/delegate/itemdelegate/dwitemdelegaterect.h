#pragma once
#include "dwluabind/dwitemdelegate.h"

//------------------------------------------------------------------------------
class DW_LUABIND_EXPORT DwItemDelegateRect : public DwItemDelegate
{
    DW_DECLARE_CLASS(DwItemDelegateRect);
public:
    DwItemDelegateRect();
protected:
};// end of DwItemDelegateRect
//------------------------------------------------------------------------------
DW_REGISTER_CLASS(DwItemDelegateRect);