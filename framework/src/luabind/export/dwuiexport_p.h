#pragma once
#include "dwexportutil_p.h"
#include "dwluabind/dwluaitemlib.h"

//------------------------------------------------------------------------------
/**
    createItem(type, parent)
*/
static int ui_createItem(lua_State *L)
{
    DW_ASSERT(lua_gettop(L) == 2);
    DW_ASSERT(lua_isstring(L, -2));
    DW_ASSERT(lua_isnumber(L, -1) || lua_isnil(L, -1) );

    const char * type = lua_tostring(L, -2);
    DwString n(DwString("Dw") + type);
    DwGraphicsItem *parent = _lua_itemOf(L, -1);

    int32_t id = dwLuaItemLib()->createItem(n, parent, L);

    if (id <= 0)
    {
        DW_ASSERT(false && "no exits item");
        return 0;
    }

    lua_pushinteger(L, id);

    return 1;
}

//------------------------------------------------------------------------------
/**
    destroyItem(id)
*/
static int ui_destroyItem(lua_State *L)
{
    DW_ASSERT(lua_gettop(L) == 1);

    DwGraphicsItem *item = _lua_itemOf(L, -1);
    return dwLuaItemLib()->destroyItem(item, L);
}

//------------------------------------------------------------------------------
/**
    setProperty(item, key, v)
*/
static int ui_setProperty(lua_State *L)
{
    DW_ASSERT(lua_gettop(L) == 3);

    DwGraphicsItem *target = _lua_itemOf(L, 1);
    DwString key = _lua_pop_value_of<DwString>(L, 2);

    return dwLuaItemLib()->setProperty(target, key, L);
}

//------------------------------------------------------------------------------
/**
    propertyOf(item, key)
*/
static int ui_propertyOf(lua_State *L)
{
    DW_ASSERT(lua_gettop(L) == 2);

    DwGraphicsItem *target = _lua_itemOf(L, 1);
    DwString key = _lua_pop_value_of<DwString>(L, 2);

    return dwLuaItemLib()->propertyOf(target, key, L);
}

//------------------------------------------------------------------------------
/**
    call(item, entry, args)
*/
static int ui_call(lua_State *L)
{
    DW_ASSERT(lua_gettop(L) >= 2);

    DwGraphicsItem *target = _lua_itemOf(L, 1);
    DwString key = _lua_pop_value_of<DwString>(L, 2);

    return dwLuaItemLib()->call(target, key, L);
}

//------------------------------------------------------------------------------
/**
    register(item)
*/
static int ui_register(lua_State *L)
{
    DW_UNUSED(L);
    DwGraphicsItem *target = _lua_itemOf(L, 1);

    return dwLuaItemLib()->registerItem(target, L);
}

//------------------------------------------------------------------------------
/**
*/
static const struct luaL_reg uiLib [] =
{
    {"createItem", ui_createItem},
    {"destroyItem", ui_destroyItem},
    {"setProperty", ui_setProperty},
    {"propertyOf", ui_propertyOf},
    {"call", ui_call},
    {"register", ui_register},
    {NULL, NULL} /* sentinel */
};