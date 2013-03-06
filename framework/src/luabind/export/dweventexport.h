#pragma once

#include "dwluabind/dwluaexport.h"
#include "dwgui/dweventid.h"
#include "dwgui/dwevent.h"

//------------------------------------------------------------------------------
class DW_LUABIND_EXPORT DwEventExport : public DwLuaExport, public sigslot::has_slots
{
    DW_DECLARE_CLASS(DwEventExport);
public:
    DwEventExport();
    virtual ~DwEventExport();

    virtual bool exportd(DwLuaEngine *engine);

    // listeners
    virtual bool onEngineDestroy(DwLuaEngine *engine);

    virtual bool onItemDestroy(DwGraphicsItem *item);

    // detail imp
    bool connect(DwGraphicsItem *item, DwEventId *evtId);

    bool disconnect(DwGraphicsItem *item, DwEventId *evtId);

    bool callEvent(DwGraphicsItem *item, DwEvent *evt);

protected:
    // event slots
    void onEvent(DwRttiObject *item, DwEvent *evt);

    int newEvent(DwEvent *evt, lua_State *L);

    typedef DwVector<int> EventHandlers;
    typedef DwMap<DwEventId*, EventHandlers > EventRefMap;
    typedef DwMap<DwGraphicsItem*, EventRefMap > ItemEventRefCountedMap;
    ItemEventRefCountedMap m_itemEventRefMap;
    bool m_isConnected;

    EventHandlers _idRefOf(DwGraphicsItem *item, DwEvent *evt);
    int _connect(DwGraphicsItem *item, DwEventId *evtId);
    bool _disconnect(DwGraphicsItem *item, DwEventId *evtId, const EventHandlers& ns);
    bool _callEvent(DwGraphicsItem *item, DwEvent *evt, int n);
    void _onItemDestroy(DwGraphicsItem *item, const EventRefMap& events);
};// end of DwEventExport
DW_REGISTER_CLASS(DwEventExport);