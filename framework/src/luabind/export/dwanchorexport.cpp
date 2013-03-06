#include "stable.h"
#include "dwanchorexport.h"
#include "dwanchorexport_p.h"
#include "dwluabind/dwluaengine.h"

DW_IMPLEMENT_CLASS(DwAnchorExport, 'AERT', DwLuaExport);

//------------------------------------------------------------------------------
/**
*/
bool DwAnchorExport::exportd( DwLuaEngine *engine )
{
    luaL_register(engine->luaState(), "Anchor", anchorLib);
    bool isOk = engine->load("script:ui/anchor.lua");
    isOk = isOk && engine->load("script:ui/anchorcom.lua");
    return isOk;
}