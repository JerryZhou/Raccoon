#pragma once
#include "dwluabind/dwitemdelegatebasepositioner.h"

//------------------------------------------------------------------------------
class DW_LUABIND_EXPORT DwItemDelegateGridPositioner : public DwItemDelegateBasePositioner
{
    DW_DECLARE_CLASS(DwItemDelegateGridPositioner);
public:
    DwItemDelegateGridPositioner();
};// end of DwItemDelegateGridPositioner
//------------------------------------------------------------------------------
DW_REGISTER_CLASS(DwItemDelegateGridPositioner);