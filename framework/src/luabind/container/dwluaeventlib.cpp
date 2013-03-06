#include "stable.h"
#include "dwluaeventlib.h"
#include "bind/dwluapoppush.h"
#include "dwluabind/dwluaengine.h"
#include "dwluabind/dweventexport.h"

DW_IMPLEMENT_CLASS(DwLuaEventLib, 'LEIB', DwRttiObject);

//------------------------------------------------------------------------------
/**
*/
DwLuaEventLib::DwLuaEventLib()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwLuaEventLib::~DwLuaEventLib()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
bool DwLuaEventLib::connect(DwGraphicsItem *item, DwEventId *evtId, lua_State *L)
{
    DwLuaEngine *engine = DwLuaEngine::engineOf(L);
    if (engine)
    {
        DwEventExport *exporter = dwsafe_cast<DwEventExport>(engine->exporterOf(DwEventExport::RTTI()));
        if (exporter)
        {
            return exporter->connect(item, evtId);
        }
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwLuaEventLib::disconnect(DwGraphicsItem *item, DwEventId *evtId, lua_State *L)
{
    DwLuaEngine *engine = DwLuaEngine::engineOf(L);
    if (engine)
    {
        DwEventExport *exporter = dwsafe_cast<DwEventExport>(engine->exporterOf(DwEventExport::RTTI()));
        if (exporter)
        {
            return exporter->disconnect(item, evtId);
        }
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwLuaEventLib::callEvent(DwGraphicsItem *item, DwEvent *evt, lua_State *L)
{
    DwLuaEngine *engine = DwLuaEngine::engineOf(L);
    if (engine)
    {
        DwEventExport *exporter = dwsafe_cast<DwEventExport>(engine->exporterOf(DwEventExport::RTTI()));
        if (exporter)
        {
            return exporter->callEvent(item, evt);
        }
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
static void clearScript(lua_State *L , DwGraphicsItem *item)
{
    lua_getglobal(L, "GUI_CLEAR_PROXY_OF");
    if (lua_isfunction(L, -1))
    {
        lua_pushinteger(L, item->id());
        int res = lua_pcall(L, 1, 0, 0);
        if (res != 0)
        {
            DwString s = lua_tostring(L, -1);
            DW_UI_LOGER() << "script error: " << s << "\n";
            lua_pop(L, 1); //  pop the error
            DW_ASSERT(false); // go on to push id
        }
    }
    else
    {
        lua_pop(L, 1);
    }

    return;
}

//------------------------------------------------------------------------------
/**
*/
DW_GLOBAL_STATIC(DwLuaEventLib, _dw_luaEventLib);
DwLuaEventLib* dwLuaEventLib()
{
    return _dw_luaEventLib();
}