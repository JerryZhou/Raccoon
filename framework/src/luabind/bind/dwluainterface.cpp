#include "stable.h"
#include "dwluainterface.h"
#include "dwgui/dwringlogcase.h"
#include "dwluabind/dwluaengine.h"
#include "dwluabind/dwuiexport.h"
#include "dwluabind/dwanchorexport.h"
#include "dwluabind/dweventexport.h"
#include "dwluabind/dwanimationexport.h"
#include "dwluabind/dwtimerexport.h"
#include "dwluabind/dwimageexport.h"
#include "dwluabind/dwfontexport.h"
#include "dwluabind/dwassignexport.h"
#include "dwluabind/dwappexport.h"

//------------------------------------------------------------------------------
/**
*/
bool DwLuaInterface::register2Lua(DwLuaEngine *engine, const char* lib, const char* name, lua_CFunction entry )
{
    DW_UNUSED(engine);
    DW_UNUSED(lib);
    DW_UNUSED(name);
    DW_UNUSED(entry);
    engine->registerScript(entry, name, lib);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool __cdecl DwLuaInterface::callLua(DwLuaEngine *engine, const char* name, const char* proto, ... )
{
    DW_UNUSED(engine);
    DW_UNUSED(name);
    DW_UNUSED(proto);
    va_list _va_list;
    va_start(_va_list, proto);
    engine->call(name, proto, _va_list);
    va_end(_va_list);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwLuaInterface::dostring(DwLuaEngine *engine, const char *buff, int sz, const char *name)
{
    return engine->load(buff, sz, name);
}

//------------------------------------------------------------------------------
/**
*/
bool DwLuaInterface::dostring(DwLuaEngine *engine, const DwString s)
{
    DwByteArray data = s.toUtf8();
    if (data.size() > 0)
    {
        return dostring(engine, data.data(), data.size(), "_dw_interface");
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwLuaInterface::addInlineExporters(DwLuaEngine *engine)
{
    DW_ASSERT(engine);
    bool isOk = true;

    // export the ui part to the script
    isOk = isOk && engine->addExporter(DwUIExport::RTTI());
    DW_ASSERT(isOk && "failed to exporte script ");

    // export the anchor part to the script
    isOk = isOk && engine->addExporter(DwAnchorExport::RTTI());
    DW_ASSERT(isOk && "failed to exporte script ");

    // export the event part to the script
    isOk = isOk && engine->addExporter(DwEventExport::RTTI());
    DW_ASSERT(isOk && "failed to exporte script ");

    // export the animation to the script
    isOk = isOk && engine->addExporter(DwAnimationExport::RTTI());
    DW_ASSERT(isOk && "failed to exporte script ");

    // export the timer to the script
    isOk = isOk && engine->addExporter(DwTimerExport::RTTI());
    DW_ASSERT(isOk && "failed to exporte script ");

    // export the image to the script
    isOk = isOk && engine->addExporter(DwImageExport::RTTI());
    DW_ASSERT(isOk && "failed to exporte script ");

    // export the font to the script
    isOk = isOk && engine->addExporter(DwFontExport::RTTI());
    DW_ASSERT(isOk && "failed to exporte script ");

    // export the assign to the script
    isOk = isOk && engine->addExporter(DwAssignExport::RTTI());
    DW_ASSERT(isOk && "failed to exporte script ");

    // export the app to the script
    isOk = isOk && engine->addExporter(DwAppExport::RTTI());
    DW_ASSERT(isOk && "failed to exporte script ");

    return isOk;
}
