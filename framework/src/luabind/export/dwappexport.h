#pragma once

#include "dwluabind/dwluaexport.h"

//------------------------------------------------------------------------------
class DW_LUABIND_EXPORT DwAppExport : public DwLuaExport
{
    DW_DECLARE_CLASS(DwAppExport);
public:
    virtual bool exportd(DwLuaEngine *engine);
protected:
};// end of DwAppExport
DW_REGISTER_CLASS(DwAppExport);