#pragma once
#include "dwcore/dwmap.h"
#include "dwgui/dwrttiobject.h"
#include "dwgui/dwgraphicsevent.h"
#include "dwgui/dwgraphicsitemhook.h"

//------------------------------------------------------------------------------
class DwGraphicsItem;
class DwLuaEngine;

//------------------------------------------------------------------------------
class DW_LUABIND_EXPORT DwLuaEventLib : public DwRttiObject
{
    DW_DECLARE_CLASS(DwLuaEventLib);
public:
    DwLuaEventLib();
    virtual ~DwLuaEventLib();

    bool connect(DwGraphicsItem *item, DwEventId *evtId, lua_State *L);

    bool disconnect(DwGraphicsItem *item, DwEventId *evtId, lua_State *L);

    bool callEvent(DwGraphicsItem *item, DwEvent *evt, lua_State *L);
};// end of DwLuaEventLib

//------------------------------------------------------------------------------
DW_LUABIND_EXPORT DwLuaEventLib* dwLuaEventLib();