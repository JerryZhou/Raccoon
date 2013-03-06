#pragma once
#include "dwexportutil_p.h"
#include "dwluabind/dwluatimerlib.h"

//------------------------------------------------------------------------------
/**
*/
struct LUA_USERDATA_TIMER
{
    STimerId id;
    LUA_USERDATA_TIMER() : id(0) {}
    LUA_USERDATA_TIMER(STimerId d) : id(d) {}
};

//------------------------------------------------------------------------------
/**
*/
static int _lua_helper_timer_release(lua_State *L)
{
    LUA_USERDATA_TIMER *d = (LUA_USERDATA_TIMER *)lua_touserdata(L, 1);
    dwLuaTimerLib()->deleteTimerLater(d->id, L);
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
template<>
static int _lua_push_value_of<LUA_USERDATA_TIMER>(lua_State *L, const LUA_USERDATA_TIMER &d)
{
    LUA_USERDATA_TIMER *p = (LUA_USERDATA_TIMER*)(lua_newuserdata(L, sizeof(LUA_USERDATA_TIMER)));
    *p = d;

    // meta table
    lua_getglobal(L, "_lua_timer_metatable");
    if(lua_isnil(L, -1))
    {
        lua_pop(L, 1);// pop nil
        luaL_newmetatable(L, "_lua_timer_metatable");

        lua_pushcfunction(L, _lua_helper_timer_release);
        lua_setfield(L, -2, "__gc");
    }
    DW_ASSERT(lua_istable(L, -1));
    lua_setmetatable(L, -2);

    return 1;
}

//------------------------------------------------------------------------------
/**
*/
template<>
static LUA_USERDATA_TIMER _lua_pop_value_of(lua_State *L, int index)
{
    if (lua_isnumber(L, index))
    {
        return LUA_USERDATA_TIMER(lua_tointeger(L, index));
    }
    else
    {
        LUA_USERDATA_TIMER * p = (LUA_USERDATA_TIMER*)luaL_checkudata(L, index, "_lua_timer_metatable");
        if (p)
        {
            return LUA_USERDATA_TIMER(*p);
        }
    }
    return LUA_USERDATA_TIMER();
}

//------------------------------------------------------------------------------
/**
    (mesc)
*/
int timer_create(lua_State *L)
{
    DW_UNUSED(L);
    uint32_t mesc = 0;
    if (lua_gettop(L) >= 1 )
    {
        mesc = _lua_pop_value_of<uint32_t>(L, -1);
    }
    LUA_USERDATA_TIMER timer(dwLuaTimerLib()->newTimer(mesc, L));
    return _lua_push_value_of<LUA_USERDATA_TIMER>(L, timer);
}

//------------------------------------------------------------------------------
/**
    (stimer)
*/
int timer_destroy(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) >= 1);
    DW_ASSERT(lua_isnumber(L, 1));
    STimerId id = _lua_pop_value_of<LUA_USERDATA_TIMER>(L, 1).id;
    bool isOk = dwLuaTimerLib()->deleteTimerLater(id, L);
    return _lua_push_value_of<bool>(L, isOk);
}

//------------------------------------------------------------------------------
/**
    (stimer, mesc)
*/
int timer_start(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) >= 1);

    STimerId id = _lua_pop_value_of<LUA_USERDATA_TIMER>(L, 1).id;
    if (lua_gettop(L) >= 2)
    {
        DW_ASSERT(lua_isnumber(L, 2));
        uint32_t mesc = _lua_pop_value_of<uint32_t>(L, 2);
        bool isOk = dwLuaTimerLib()->startTimer(id, mesc, L);
        return _lua_push_value_of<bool>(L, isOk);
    }

    bool isOk = dwLuaTimerLib()->startTimer(id, L);
    return _lua_push_value_of<bool>(L, isOk);
}

//------------------------------------------------------------------------------
/**
    (stimer)
*/
int timer_stop(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) >= 1);

    STimerId id = _lua_pop_value_of<LUA_USERDATA_TIMER>(L, 1).id;
    bool isOk = dwLuaTimerLib()->stopTimer(id, L);
    return _lua_push_value_of<bool>(L, isOk);
}

//------------------------------------------------------------------------------
/**
    (stimer)
*/
int timer_isActive(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) >= 1);

    STimerId id = _lua_pop_value_of<LUA_USERDATA_TIMER>(L, 1).id;
    bool isOk = dwLuaTimerLib()->isActive(id, L);
    return _lua_push_value_of<bool>(L, isOk);
}

//------------------------------------------------------------------------------
/**
    (stimer, mesc)
*/
int timer_setInterval(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) >= 2);

    STimerId id = _lua_pop_value_of<LUA_USERDATA_TIMER>(L, 1).id;
    DW_ASSERT(lua_isnumber(L, 2));
    uint32_t mesc = _lua_pop_value_of<uint32_t>(L, 2);
    bool isOk = dwLuaTimerLib()->setInterval(id, mesc, L);
    return _lua_push_value_of<bool>(L, isOk);
}

//------------------------------------------------------------------------------
/**
    (stimer, function)
*/
int timer_connect(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) >= 2);
    STimerId id = _lua_pop_value_of<LUA_USERDATA_TIMER>(L, 1).id;
    bool isOk = dwLuaTimerLib()->connect(id, L);
    return _lua_push_value_of<bool>(L, isOk);
}

//------------------------------------------------------------------------------
/**
    (stimer)
*/
int timer_disconnect(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) >= 1);
    STimerId id = _lua_pop_value_of<LUA_USERDATA_TIMER>(L, 1).id;
    bool isOk = dwLuaTimerLib()->disconnect(id, L);
    return _lua_push_value_of<bool>(L, isOk);
}

//------------------------------------------------------------------------------
/**
    (stimer)
*/
int timer_id(lua_State *L)
{
    DW_ASSERT(lua_gettop(L) >= 1);
    STimerId id = _lua_pop_value_of<LUA_USERDATA_TIMER>(L, 1).id;
    return _lua_push_value_of<STimerId>(L, id);
}
//------------------------------------------------------------------------------
/**
*/
static const struct luaL_reg timerLib [] =
{
    {"create", timer_create},
    {"destroy", timer_destroy},
    {"start", timer_start},
    {"stop", timer_stop},
    {"isActive", timer_isActive},
    {"setInterval", timer_setInterval},
    {"connect", timer_connect},
    {"disconnect", timer_disconnect},
    {"id", timer_id},
    {NULL, NULL} /* sentinel */
};