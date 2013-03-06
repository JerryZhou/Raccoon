#pragma once
#include "dwluabind/dwluaexport.h"

//------------------------------------------------------------------------------
class DW_LUABIND_EXPORT DwAnchorExport : public DwLuaExport
{
    DW_DECLARE_CLASS(DwAnchorExport)
public:
    virtual bool exportd(DwLuaEngine *engine);
protected:
};// end of DwAnchorExport
DW_REGISTER_CLASS(DwAnchorExport);