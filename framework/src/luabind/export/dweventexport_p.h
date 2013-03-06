#pragma once
#include "dwexportutil_p.h"
#include "dwluabind/dwluaeventlib.h"
#include "dwgui/dwappeventdispatch.h"

//------------------------------------------------------------------------------
/**
    connect(item, funcName, func)
*/
static int event_connect(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) == 3);
    bool isOk = false;
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item)
    {
        DwEventId*  evtId = _lua_event_id(L, 2);
        if (evtId)
        {
            isOk = dwLuaEventLib()->connect(item, evtId, L);
        }
        else
        {
            DW_UI_LOGER() << "event_connect cannot find event" << _lua_pop_value_of<DwString>(L, 2) << "\n";
            DW_ASSERT(false);
        }
    }

    lua_pushboolean(L, isOk);
    return 1;
}

//------------------------------------------------------------------------------
/**
    disconnect(item, funcName)
*/
static int event_disconnect(lua_State *L)
{
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) == 2);
    bool isOk = false;
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item)
    {
        DwEventId*  evtId = _lua_event_id(L, 2);
        if (evtId)
        {
            isOk = dwLuaEventLib()->disconnect(item, evtId, L);
        }
        else
        {
            DW_UI_LOGER() << "event_disconnect cannot find event" << _lua_pop_value_of<DwString>(L, 2) << "\n";
            DW_ASSERT(false);
        }
    }

    lua_pushboolean(L, isOk);
    return 1;
}

//------------------------------------------------------------------------------
/**
    call(item, funcName, arg)
*/
static int event_call(lua_State *L)
{
    DW_ASSERT(lua_gettop(L) >= 2);
    bool isOk = false;
    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item)
    {
        DwEventId*  evtId = _lua_event_id(L, 2);
        if (evtId)
        {
            DwRttiObject *obj = evtId->create();
            DwEvent *evt = dwsafe_cast<DwEvent>(obj);

            // TODO: Construct Event From LuaState
            if (evt)
            {
                isOk = dwLuaEventLib()->callEvent(item, evt, L);
            }
            else
            {
                DW_UI_LOGER() << "event_call cannot find event" << _lua_pop_value_of<DwString>(L, 2) << "\n";
                DW_ASSERT(false);
            }

            DW_SAFE_DELETE(obj);
        }
    }

    lua_pushboolean(L, isOk);
    return 1;
}

//------------------------------------------------------------------------------
/**
    dispatch(item, event)
*/
static int event_dispatch(lua_State *L)
{
    DW_ASSERT(lua_gettop(L) >= 2);

    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item)
    {
        DwEvent * evt = _lua_eventOf(L, 2);
        if (evt)
        {
            dwAppEventDispatch()->dispatchEvent(evt);
        }
        else
        {
            DW_UI_LOGER() << "event_dispatch cannot find event" << _lua_pop_value_of<DwString>(L, 2) << "\n";
            DW_ASSERT(false);
        }
        _lua_event_release(evt);
    }

    return 0;
}

//------------------------------------------------------------------------------
/**
    dispatchAsync(item, event, delay)
*/
static int event_dispatchAsync(lua_State *L)
{
    DW_ASSERT(lua_gettop(L) >= 3);

    DwGraphicsItem *item = _lua_itemOf(L, 1);
    if (item)
    {
        DwEvent * evt = _lua_eventOf(L, 2);
        if (evt)
        {
            int delay = _lua_pop_value_of<int>(L, 3);
            dwAppEventDispatch()->dispatchEventAsync(evt, delay);
        }
        else
        {
            DW_UI_LOGER() << "event_dispatchAsync cannot find event" << _lua_pop_value_of<DwString>(L, 2) << "\n";
            DW_ASSERT(false);
        }
        _lua_event_release(evt);
    }

    return 0;
}

//------------------------------------------------------------------------------
/**
*/
static DwEvent * _lua_event_of(lua_State *L, int index)
{
    DwEvent * evt = NULL;
    DW_ASSERT(lua_istable(L, index));

    lua_pushstring(L, "_event_pointer");
    lua_rawget(L, index);

    if(lua_islightuserdata(L, -1))
    {
        evt = (DwEvent*)(lua_topointer(L, -1));
    }
    lua_pop(L, 1);

    return evt;
}

//------------------------------------------------------------------------------
/**
*/
static int event_call_func(lua_State* L)
{
    DW_ASSERT(lua_gettop(L) >= 2);

    int n = 0;
    DwEvent * target = _lua_event_of(L, 1);
    DwString key = _lua_pop_value_of<DwString>(L, 2);
    DwString v = _lua_pop_value_of<DwString>(L, 3);
    if (target)
    {
        DwDelegate* dgate = _delegateOf(target->rtti());
        while (dgate && dgate->rtti()->isDerivedFrom(DwEventDelegate::RTTI()))
        {
            DwDelegate::fn_delegate_call get =  dgate->entryCall(key);
            if (get)
            {
                n = get(target, L);
                break;
            }

            dgate = dgate->parent();
            if (!dgate)
            {
                DW_ASSERT(false && "not exits of call");
                DW_UI_LOGER() << "_lua_event_call error, not exits of call: " << key << "\n";
            }
        }
    }
    return n;
}

//------------------------------------------------------------------------------
/**
*/
static const struct luaL_reg eventLib [] =
{
    {"connect", event_connect},
    {"disconnect", event_disconnect},
    {"call", event_call},
    {"dispatch", event_dispatch},
    {"dispatchAsync", event_dispatchAsync},
    {"invoke", event_call_func},
    {NULL, NULL} /* sentinel */
};