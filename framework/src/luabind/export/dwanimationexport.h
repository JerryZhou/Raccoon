#pragma once
#include "dwluabind/dwluaexport.h"

//------------------------------------------------------------------------------
class DW_LUABIND_EXPORT DwAnimationExport : public DwLuaExport
{
    DW_DECLARE_CLASS(DwAnimationExport)
public:
    virtual bool exportd(DwLuaEngine *engine);
protected:
};// end of DwAnimationExport
DW_REGISTER_CLASS(DwAnimationExport);