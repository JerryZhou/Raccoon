#pragma once
#include "dwluabind/dwluaexport.h"

//------------------------------------------------------------------------------
class DW_LUABIND_EXPORT DwImageExport : public DwLuaExport
{
    DW_DECLARE_CLASS(DwImageExport);
public:
    virtual bool exportd(DwLuaEngine *engine);
protected:
};// end of DwImageExport
DW_REGISTER_CLASS(DwImageExport);