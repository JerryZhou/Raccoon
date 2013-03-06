#pragma once
#include "dwexportutil_p.h"
#include "dwgui/dwassignlib.h"

//------------------------------------------------------------------------------
/**
*/
static int assign_setup(lua_State *L)
{
    DW_UNUSED(L);
    dwAssignLib()->setup();
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
static int assign_discard(lua_State *L)
{
    DW_UNUSED(L);
    dwAssignLib()->discard();
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
static int assign_isValid(lua_State *L)
{
    return _lua_push_value_of<bool>(L, dwAssignLib()->isValid());
}

//------------------------------------------------------------------------------
/**
    setAssign(name, path)
*/
static int assign_setAssign(lua_State *L)
{
    DW_ASSERT(lua_gettop(L) >= 2);

    DwString n = _lua_pop_value_of<DwString>(L , 1);
    DwString p = _lua_pop_value_of<DwString>(L, 2);

    dwAssignLib()->setAssign(DwAssign(n, p));
    return 0;
}

//------------------------------------------------------------------------------
/**
    hasAssign(n)
*/
static int assign_hasAssign(lua_State *L)
{
    DW_ASSERT(lua_gettop(L) == 1);
    DwString n = _lua_pop_value_of<DwString>(L, 1);

    return _lua_push_value_of<bool>(L, dwAssignLib()->hasAssign(n));
}

//------------------------------------------------------------------------------
/**
    assignOf(n)
*/
static int assign_assignOf(lua_State *L)
{
    DW_ASSERT(lua_gettop(L) == 1);
    DwString n = _lua_pop_value_of<DwString>(L, 1);

    return _lua_push_value_of<DwString>(L, dwAssignLib()->assignOf(n));
}

//------------------------------------------------------------------------------
/**
    clearAssign(n)
*/
static int assign_clearAssign(lua_State *L)
{
    DW_ASSERT(lua_gettop(L) == 1);
    DwString n = _lua_pop_value_of<DwString>(L, 1);

    dwAssignLib()->clearAssign(n);
    return 0;
}

//------------------------------------------------------------------------------
/**
    resolveAssignsInString(url)
*/
static int assign_resolveAssignsInString(lua_State *L)
{
    DW_ASSERT(lua_gettop(L) == 1);
    DwString n = _lua_pop_value_of<DwString>(L, 1);

    return _lua_push_value_of<DwString>(L, dwAssignLib()->resolveAssignsInString(n));
}

//------------------------------------------------------------------------------
/**
    localPath(url)
*/
static int assign_localPath(lua_State *L)
{
    DW_ASSERT(lua_gettop(L) == 1);
    DwString n = _lua_pop_value_of<DwString>(L, 1);

    return _lua_push_value_of<DwString>(L, DwUrl(n).localPath());
}

//------------------------------------------------------------------------------
/**
*/
static const struct luaL_reg assignLib [] =
{
    {"setup", assign_setup},
    {"discard", assign_discard},

    {"setAssign", assign_setAssign},
    {"hasAssign", assign_hasAssign},
    {"assignOf", assign_assignOf},
    {"clearAssign", assign_clearAssign},

    {"resolveAssignsInString", assign_resolveAssignsInString},
    {"localPath", assign_localPath},

    {NULL, NULL} /* sentinel */
};