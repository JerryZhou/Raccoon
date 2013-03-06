#pragma once

#include "dwluabind/dwluaexport.h"

//------------------------------------------------------------------------------
class DW_LUABIND_EXPORT DwFontExport : public DwLuaExport
{
    DW_DECLARE_CLASS(DwFontExport);
public:
    virtual bool exportd(DwLuaEngine *engine);
protected:
};// end of DwFontExport
DW_REGISTER_CLASS(DwFontExport);