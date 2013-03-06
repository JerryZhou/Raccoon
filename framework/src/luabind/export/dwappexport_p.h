#pragma once
#include "dwexportutil_p.h"
#include "dwgui/dwapplication.h"

//------------------------------------------------------------------------------
/**
*/
static int app_quit(lua_State *L)
{
    DW_ASSERT(dwApp());
    dwApp()->quit();
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
static int app_setFlushLog(lua_State *L)
{
    DW_ASSERT(dwApp());
    if(lua_gettop(L))
    {
        dwApp()->setFlushLog(_lua_pop_value_of<bool>(L, -1));
    }else
    {
        dwApp()->setFlushLog(false);
    }

    return 0;
}

//------------------------------------------------------------------------------
/**
*/
static const struct luaL_reg appLib [] =
{
    {"quit", app_quit},

    {"setFlushLog", app_setFlushLog},

    {NULL, NULL} /* sentinel */
};