#pragma once
#include "dwluabind/dwdelegate.h"
#include "dwgui/dwgraphicsitem.h"

//------------------------------------------------------------------------------
class DW_LUABIND_EXPORT DwItemDelegate : public DwDelegate
{
    DW_DECLARE_CLASS(DwItemDelegate);
public:
    DwItemDelegate();
protected:
};// end of DwItemDelegate
//------------------------------------------------------------------------------
DW_REGISTER_CLASS(DwItemDelegate);