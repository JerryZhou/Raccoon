#include "stable.h"
#include "dwluabind/dwluaengine.h"
#include "dwassignexport.h"
#include "dwassignexport_p.h"

DW_IMPLEMENT_CLASS(DwAssignExport, 'ANRT', DwLuaExport);

//------------------------------------------------------------------------------
/**
*/
bool DwAssignExport::exportd( DwLuaEngine *engine )
{
    luaL_register(engine->luaState(), "Assign", assignLib);
    bool isOk = engine->load("script:ui/assign.lua");
    isOk = isOk && engine->load("script:ui/assigncom.lua");
    return isOk;
}