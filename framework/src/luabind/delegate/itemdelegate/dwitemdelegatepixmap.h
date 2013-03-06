#pragma once
#include "dwluabind/dwitemdelegate.h"

//------------------------------------------------------------------------------
class DW_LUABIND_EXPORT DwItemDelegatePixmap : public DwItemDelegate
{
    DW_DECLARE_CLASS(DwItemDelegatePixmap);
public:
    DwItemDelegatePixmap();
protected:
};// end of DwItemDelegatePixmap
//------------------------------------------------------------------------------
DW_REGISTER_CLASS(DwItemDelegatePixmap);