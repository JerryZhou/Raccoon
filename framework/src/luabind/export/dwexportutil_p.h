#pragma once
#include "dwgui/dwgraphicsitem.h"
#include "dwluabind/dwluaitemlib.h"
#include "dwluabind/dwdelegatelib.h"
#include "dwluabind/dwluabind.h"
#include "dwgui/dwevent.h"

//------------------------------------------------------------------------------
/**
*/
static DwString _lua_seqName(lua_State *L)
{
    DwString name;
    DW_ASSERT(lua_istable(L, -1));
    lua_pushstring(L, "name");
    lua_rawget(L, -2);    //.name
    if (lua_isstring(L, -1))
    {
        name = lua_tostring(L, -1);
    }
    lua_pop(L, 2);

    return name;
}

//------------------------------------------------------------------------------
/**
*/
static bool _lua_seqParent(lua_State *L)
{
    bool isOk = false;
    DW_ASSERT(lua_istable(L, -1));
    lua_pushstring(L, "parent");
    lua_rawget(L, -2);    //.parent
    if (lua_istable(L, -1))
    {
        isOk = true;
        lua_remove(L, -2);  // remove "parent"
        lua_remove(L, -3);  // remove table
    }
    else
    {
        lua_pop(L, 2);
    }

    return isOk;
}

//------------------------------------------------------------------------------
/**
*/
static DwGraphicsItem *_lua_itemOf(lua_State *L, int index)
{
    return _lua_pop_value_of<LUA_GRAPHICSITEM_PTR>(L, index);
}

//------------------------------------------------------------------------------
/**
*/
static DwDelegate * _delegateOf(DwRtti *i)
{
    DwDelegate* dgate = NULL;

    do
    {
        dgate = dwDelegateLib()->delegateOf(i);
        i = (DwRtti*)(i->parent());
    }
    while(i && !dgate);

    return dgate;
}

//------------------------------------------------------------------------------
/**
*/
static DwEventId *_lua_event_id(lua_State *L, int index)
{
    DwString  s = _lua_pop_value_of<DwString>(L, index);
    if ( s.startsWith("on") )
    {
        DwString evtN = s.right(s.length()-2);
        DwString evtName = DwString("Dw") + evtN + DwString("Event");
        if (dwFactory()->classExists(evtName))
        {
            const DwEventId* evtId = dwFactory()->classRttiOf(evtName);
            if (evtId->isDerivedFrom(DwEvent::EventId()))
            {
                return (DwEventId*)evtId;
            }
        }
    }
    DW_UI_LOGER() << "script error: wrong event id in " << s << "\n";
    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
static DwEvent *_lua_eventOf(lua_State *L, int index)
{
    DW_UNUSED(L);
    DW_UNUSED(index);
    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
static void _lua_event_release(DwEvent *evt)
{
    DW_SAFE_DELETE(evt);
}