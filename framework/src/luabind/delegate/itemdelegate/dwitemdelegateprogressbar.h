#pragma once
#include "dwluabind/dwitemdelegate.h"

//------------------------------------------------------------------------------
class DW_LUABIND_EXPORT DwItemDelegateProgressBar : public DwItemDelegate
{
    DW_DECLARE_CLASS(DwItemDelegateProgressBar);
public:
    DwItemDelegateProgressBar();
protected:
};// end of DwItemDelegateProgressBar
//------------------------------------------------------------------------------
DW_REGISTER_CLASS(DwItemDelegateProgressBar);