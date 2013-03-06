#include "stable.h"
#include "dwluabind/dwluaengine.h"
#include "dwappexport.h"
#include "dwappexport_p.h"

DW_IMPLEMENT_CLASS(DwAppExport, 'APET', DwLuaExport);

//------------------------------------------------------------------------------
/**
*/
bool DwAppExport::exportd( DwLuaEngine *engine )
{
    luaL_register(engine->luaState(), "App", appLib);
    return true;
}