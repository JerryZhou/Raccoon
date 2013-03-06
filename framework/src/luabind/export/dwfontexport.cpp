#include "stable.h"
#include "dwluabind/dwluaengine.h"
#include "dwfontexport.h"
#include "dwfontexport_p.h"

DW_IMPLEMENT_CLASS(DwFontExport, 'FERT', DwLuaExport);

//------------------------------------------------------------------------------
/**
*/
bool DwFontExport::exportd( DwLuaEngine *engine )
{
    luaL_register(engine->luaState(), "Font", fontLib);
    bool isOk = engine->load("script:ui/font.lua");
    isOk = isOk && engine->load("script:ui/fontcom.lua");
    return isOk;
}
