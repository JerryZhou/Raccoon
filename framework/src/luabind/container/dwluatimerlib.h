#pragma once
#include "dwgui/dwrttiobject.h"
#include "dwcore/sigslot.h"

//------------------------------------------------------------------------------
struct lua_State;

//------------------------------------------------------------------------------
class DwLuaTimerLib : public DwRttiObject, public sigslot::has_slots
{
    DW_DECLARE_CLASS(DwLuaTimerLib);
public:
    DwLuaTimerLib();

    virtual ~DwLuaTimerLib();

    uint32_t newTimer(uint32_t mesc, lua_State *L);

    bool deleteTimer(uint32_t id, lua_State *L);
    bool deleteTimerLater(uint32_t id, lua_State *L);

    bool startTimer(uint32_t id, lua_State *L);
    bool startTimer(uint32_t id, uint32_t mesc, lua_State *L);

    bool stopTimer(uint32_t id, lua_State *L);

    bool isActive(uint32_t id, lua_State *L) const;

    bool setInterval(uint32_t id, uint32_t mesc, lua_State *L);

    bool connect(uint32_t id, lua_State *L);

    bool disconnect(uint32_t id, lua_State *L);
};// end of DwLuaTimerLib

//------------------------------------------------------------------------------
DW_LUABIND_EXPORT DwLuaTimerLib* dwLuaTimerLib();