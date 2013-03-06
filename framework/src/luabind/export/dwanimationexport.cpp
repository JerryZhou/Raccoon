#include "stable.h"
#include "dwanimationexport.h"
#include "dwanimationexport_p.h"
#include "dwluabind/dwluaengine.h"

DW_IMPLEMENT_CLASS(DwAnimationExport, 'ANIT', DwLuaExport);

//------------------------------------------------------------------------------
/**
*/
bool DwAnimationExport::exportd( DwLuaEngine *engine )
{
    luaL_register(engine->luaState(), "Animation", animationLib);
    bool isOk = engine->load("script:ui/animation.lua");
    isOk = isOk && engine->load("script:ui/animationcom.lua");
    return isOk;
}