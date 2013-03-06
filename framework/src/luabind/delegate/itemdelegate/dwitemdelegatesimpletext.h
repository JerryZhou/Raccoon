#pragma once
#include "dwluabind/dwitemdelegate.h"

//------------------------------------------------------------------------------
class DW_LUABIND_EXPORT DwItemDelegateSimpleText : public DwItemDelegate
{
    DW_DECLARE_CLASS(DwItemDelegateSimpleText);
public:
    DwItemDelegateSimpleText();
};// end of DwItemDelegateSimpleText
//------------------------------------------------------------------------------
DW_REGISTER_CLASS(DwItemDelegateSimpleText);