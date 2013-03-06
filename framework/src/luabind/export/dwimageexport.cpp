#include "stable.h"
#include "dwluabind/dwluaengine.h"
#include "dwimageexport.h"
#include "dwimageexport_p.h"

DW_IMPLEMENT_CLASS(DwImageExport, 'IERT', DwLuaExport);

//------------------------------------------------------------------------------
/**
*/
bool DwImageExport::exportd( DwLuaEngine *engine )
{
    luaL_register(engine->luaState(), "Image", imageLib);
    bool isOk = engine->load("script:ui/image.lua");
    isOk = isOk && engine->load("script:ui/imagecom.lua");
    return isOk;
}
