#pragma once
#include "dwluabind/dwitemdelegate.h"

//------------------------------------------------------------------------------
class DW_LUABIND_EXPORT DwItemDelegateBasePositioner : public DwItemDelegate
{
    DW_DECLARE_CLASS(DwItemDelegateBasePositioner);
public:
    DwItemDelegateBasePositioner();
};// end of DwItemDelegateBasePositioner
//------------------------------------------------------------------------------
DW_REGISTER_CLASS(DwItemDelegateBasePositioner);