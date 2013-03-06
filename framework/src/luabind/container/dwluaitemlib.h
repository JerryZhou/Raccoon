#pragma once
#include "dwgui/dwrttiobject.h"
#include "dwcore/sigslot.h"
#include "dwcore/dwvector.h"
#include "dwgui/dwgraphicsitemhook.h"

//------------------------------------------------------------------------------
class DwFrameWindow;
class DwGraphicsItem;
class DwLuaEngine;
class DwEvent;

//------------------------------------------------------------------------------
class DW_LUABIND_EXPORT DwLuaItemLib : public DwRttiObject
{
    DW_DECLARE_CLASS(DwLuaItemLib);
public:
    DwLuaItemLib();
    virtual ~DwLuaItemLib();

    DwGraphicsItem *findItem(uint32_t idx) const;
    /// create item in script
    uint32_t createItem(const DwString &s, DwGraphicsItem *parent, lua_State *L);
    /// destroy item by script
    int destroyItem(DwGraphicsItem *item, lua_State *L);
    /// set property of item by script
    int setProperty(DwGraphicsItem *item, const DwString &key, lua_State *L);
    /// get property of item by script
    int propertyOf(DwGraphicsItem *item, const DwString &key, lua_State *L);
    /// call function of item by script
    int call(DwGraphicsItem *item, const DwString &key, lua_State *L);
    /// register item by script
    int registerItem(DwGraphicsItem *item, lua_State *L);
};// end of DwWindowLib

//------------------------------------------------------------------------------
DW_LUABIND_EXPORT DwLuaItemLib* dwLuaItemLib();