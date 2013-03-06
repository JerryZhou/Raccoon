#pragma once

#include "dwluabind/dwluaexport.h"

//------------------------------------------------------------------------------
class DW_LUABIND_EXPORT DwAssignExport : public DwLuaExport
{
    DW_DECLARE_CLASS(DwAssignExport);
public:
    virtual bool exportd(DwLuaEngine *engine);
protected:
};// end of DwAssignExport
DW_REGISTER_CLASS(DwAssignExport);