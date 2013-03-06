#pragma once
#include "dwluabind/dwluaengine.h"

//------------------------------------------------------------------------------
class DW_LUABIND_EXPORT DwLuaInterface
{
public:
    static bool register2Lua(DwLuaEngine *engine, const char* lib, const char* name, d_lua_CFunction entry);

    static bool __cdecl callLua(DwLuaEngine *engine, const char* name, const char* proto, ...);

    static bool dostring(DwLuaEngine *engine, const char *buff, int sz, const char *name);

    static bool dostring(DwLuaEngine *engine, const DwString s);

    static bool addInlineExporters(DwLuaEngine *engine);
};// end of DwLuaInterface