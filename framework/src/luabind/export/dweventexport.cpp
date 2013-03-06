#include "stable.h"
#include "dwluabind/dwluaengine.h"
#include "dwluabind/dweventdelegate.h"
#include "dwluabind/dwdelegatelib.h"
#include "dwgui/dwgraphicseventhandler.h"
#include "dwgui/dweventsignaldispatch.h"
#include "dweventexport.h"
#include "dweventexport_p.h"

DW_IMPLEMENT_CLASS(DwEventExport, 'EERT', DwLuaExport);

static int _lua_event_setProperty(lua_State *L);
static int _lua_event_propertyOf(lua_State *L);
static int _lua_event_call(lua_State *L);

//------------------------------------------------------------------------------
/**
*/
DwEventExport::DwEventExport()
    : m_isConnected (false)
{
    setCallExportWhen(Dw::CallWhenEngineDestroy);
    setCallExportWhen(Dw::CallWhenItemDestroy);
}

//------------------------------------------------------------------------------
/**
*/
DwEventExport::~DwEventExport()
{
    DW_ASSERT(m_itemEventRefMap.count() == 0);
}

//------------------------------------------------------------------------------
/**
*/
bool DwEventExport::exportd( DwLuaEngine *engine )
{
    luaL_register(engine->luaState(), "Event", eventLib);
    bool isOk = engine->load("script:ui/event.lua");
    isOk = isOk && engine->load("script:ui/eventcom.lua");
    return isOk;
}

//------------------------------------------------------------------------------
/**
*/
bool DwEventExport::onEngineDestroy(DwLuaEngine *engine)
{
    if (engine == m_engine)
    {
        ItemEventRefCountedMap::iterator ite = m_itemEventRefMap.begin();
        while(ite != m_itemEventRefMap.end())
        {
            _onItemDestroy(ite->first, ite->second);
            ++ite;
        }
        m_itemEventRefMap.clear();
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwEventExport::onItemDestroy(DwGraphicsItem *item)
{
    ItemEventRefCountedMap::iterator ite = m_itemEventRefMap.find(item);
    if (ite != m_itemEventRefMap.end())
    {
        _onItemDestroy(item, ite->second);
        m_itemEventRefMap.remove(ite);
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwEventExport::connect(DwGraphicsItem *item, DwEventId *evtId)
{
    // we only accept the destroy and create multi connect
    if (!evtId->isDerivedFrom(DwItemDestroyEvent::EventId())
            && !evtId->isDerivedFrom(DwItemCreateEvent::EventId()))
    {
        disconnect(item, evtId);
    }

    if (item->eventHandler())
    {
        int idRef = _connect(item, evtId);
        if (idRef != LUA_REFNIL)
        {
            m_itemEventRefMap[item][evtId].append(idRef);

            return true;
        }
        else
        {
            DW_ASSERT(false && "error in lua to ref a object");
        }
    }
    else
    {
        DW_ASSERT(false && "have not set event handler to the item");
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwEventExport::disconnect(DwGraphicsItem *item, DwEventId *evtId)
{
    ItemEventRefCountedMap::iterator ite = m_itemEventRefMap.find(item);
    if (ite != m_itemEventRefMap.end())
    {
        EventRefMap &events = ite->second;
        EventRefMap::iterator iteEvt = events.find(evtId);
        if (iteEvt != events.end())
        {
            _disconnect(item, evtId, iteEvt->second);
            events.remove(iteEvt);

            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwEventExport::callEvent(DwGraphicsItem *item, DwEvent *evt)
{
    bool isOk = true;
    EventHandlers ns = _idRefOf(item, evt);
    if (!ns.isEmpty())
    {
        // NB! DO NOT CALL CREATE EVENT DIRECTLY
        // call script from back to front
        for (int i=ns.size()-1; i>=0 && !evt->isAccepted() && isOk; --i)
        {
            isOk = _callEvent(item, evt, ns.at(i));
        }
    }
    return isOk;
}

//------------------------------------------------------------------------------
/**
*/
void DwEventExport::onEvent(DwRttiObject *obj, DwEvent *evt)
{
    DwGraphicsItem *item = dwsafe_cast<DwGraphicsItem>(obj);
    if (item)
    {
        callEvent(item, evt);
    }
}

//------------------------------------------------------------------------------
/**
*/
DwEventExport::EventHandlers DwEventExport::_idRefOf(DwGraphicsItem *item, DwEvent *evt)
{
    ItemEventRefCountedMap::iterator ite = m_itemEventRefMap.find(item);
    if (ite != m_itemEventRefMap.end())
    {
        EventRefMap &events = ite->second;
        EventRefMap::iterator iteEvt = events.find(evt->eventId());
        if (iteEvt != events.end())
        {
            return iteEvt->second;
        }
    }
    return DwEventExport::EventHandlers();
}

//------------------------------------------------------------------------------
/**
*/
int DwEventExport::_connect(DwGraphicsItem *item, DwEventId *evtId)
{
    lua_State *L = m_engine->luaState();

    DW_ASSERT(lua_isfunction(L, -1));
    int idRef = luaL_ref(L, LUA_REGISTRYINDEX);
    if (idRef != LUA_REFNIL)
    {
        if (m_itemEventRefMap[item][evtId].isEmpty())
        {
            item->eventHandler()->dispatcher()->connect(evtId, this, &DwEventExport::onEvent);
        }
    }

    return idRef;
}

//------------------------------------------------------------------------------
/**
*/
bool DwEventExport::_disconnect(DwGraphicsItem *item, DwEventId *evtId, const DwEventExport::EventHandlers& ns)
{
    if (item && item->eventHandler())
    {
        item->eventHandler()->dispatcher()->disconnect(evtId , this);
    }

    if (!ns.isEmpty())
    {
        lua_State *L = m_engine->luaState();
        for (int i=0; i<ns.size(); ++i)
        {
            luaL_unref(L, LUA_REGISTRYINDEX, ns.at(i));
        }
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwEventExport::_callEvent(DwGraphicsItem *item, DwEvent *evt, int n)
{
    lua_State *L = m_engine->luaState();

    if (n != LUA_REFNIL)
    {
        lua_rawgeti(L, LUA_REGISTRYINDEX, n);
        DW_ASSERT(lua_isfunction(L, -1));
        _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, item);
        newEvent(evt, L);
        int res = lua_pcall(L, 2, 0, 0);
        if (0 != res)
        {
            DwString s = lua_tostring(L, -1);
            DW_UI_LOGER() << "script error when call event: "
                          << item->name() << " : "
                          << evt->eventId()->name()
                          << "\n";
            lua_pop(L, 1); // pop the error
            DW_ASSERT(false);
            return false;
        }

        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void DwEventExport::_onItemDestroy(DwGraphicsItem *item, const DwEventExport::EventRefMap& events)
{
    if (item && events.count() > 0)
    {
        EventRefMap::const_iterator ite = events.begin();
        while(ite != events.end())
        {
            _disconnect(item, ite->first, ite->second);
            ++ite;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
int DwEventExport::newEvent(DwEvent *evt, lua_State *L)
{
    DW_ASSERT(evt);
    DW_ASSERT(L);

    lua_newtable(L);

    // pointer
    lua_pushlightuserdata(L, (void*)evt);
    lua_setfield(L, -2, "_event_pointer");

    // name
    lua_pushstring(L, evt->eventId()->name().toUtf8().data());
    lua_setfield(L, -2, "_event_name");

    // meta table
    lua_getglobal(L, "_lua_event_metatable");
    if(lua_isnil(L, -1))
    {
        lua_pop(L, 1);// pop nil
        luaL_newmetatable(L, "_lua_event_metatable");

        lua_pushcfunction(L, _lua_event_setProperty);
        lua_setfield(L, -2, "__newindex");

        lua_pushcfunction(L, _lua_event_propertyOf);
        lua_setfield(L, -2, "__index");
    }
    DW_ASSERT(lua_istable(L, -1));
    lua_setmetatable(L, -2);

    return 1;
}

//------------------------------------------------------------------------------
/**
    (table, k, v)
*/
int _lua_event_setProperty(lua_State *L)
{
    DW_ASSERT(lua_gettop(L) >= 3);

    int n = 0;
    DwEvent * target = _lua_event_of(L, 1);
    DwString key = _lua_pop_value_of<DwString>(L, 2);

    if (target)
    {
        DwDelegate* dgate = _delegateOf(target->rtti());
        while (dgate && dgate->rtti()->isDerivedFrom(DwEventDelegate::RTTI()))
        {
            DwDelegate::fn_delegate_set set =  dgate->entrySet(key);
            if (set)
            {
                n = set(target, L);
                break;
            }

            dgate = dgate->parent();
            if (!dgate)
            {
                DW_ASSERT(false && "not exits of property");
                DW_UI_LOGER() << "_lua_event_setProperty error, not exits of property: " << key << "\n";
            }
        }
    }
    return n;
}

//------------------------------------------------------------------------------
/**
    (table, k, self, v)
*/
int _lua_event_call(lua_State *L)
{
    DW_ASSERT(lua_gettop(L) >= 2);

    return event_call_func(L);
}

//------------------------------------------------------------------------------
/**
    (table, k)
*/
int _lua_event_propertyOf(lua_State *L)
{
    DW_ASSERT(lua_gettop(L) >= 2);

    int n = 0;
    DwEvent * target = _lua_event_of(L, 1);
    DwString key = _lua_pop_value_of<DwString>(L, 2);

    if (target)
    {
        DwDelegate* dgate = _delegateOf(target->rtti());
        while (dgate && dgate->rtti()->isDerivedFrom(DwEventDelegate::RTTI()))
        {
            DwDelegate::fn_delegate_get get =  dgate->entryGet(key);
            if (get)
            {
                n = get(target, L);
                break;
            }

            dgate = dgate->parent();
            if (!dgate)
            {
                DW_ASSERT(false && "not exits of property");
                DW_UI_LOGER() << "_lua_event_propertyOf error, not exits of property: " << key << "\n";
            }
        }
    }
    return n;
}

