#include "stable.h"
#include "dweventdelegate.h"
#include "bind/dwluapoppush.h"

DW_IMPLEMENT_CLASS(DwEventDelegate, 'EDTE', DwDelegate);

// Set
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_SET(accept)
{
    DwEvent *evt = dwsafe_cast<DwEvent>(obj);
    evt->setAccepted(_lua_pop_value_of<bool>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(ignore)
{
    DwEvent *evt = dwsafe_cast<DwEvent>(obj);
    evt->setAccepted(!(_lua_pop_value_of<bool>(L, -1)));
    return 0;
}

DECLARE_DELEGATE_SET(up)
{
    DwEvent *evt = dwsafe_cast<DwEvent>(obj);
    evt->setUp(!(_lua_pop_value_of<bool>(L, -1)));
    return 0;
}

DECLARE_DELEGATE_SET(backtrace)
{
    DwEvent *evt = dwsafe_cast<DwEvent>(obj);
    evt->setBacktrace((_lua_pop_value_of<bool>(L, -1)));
    return 0;
}

DECLARE_DELEGATE_SET(traced)
{
    DwEvent *evt = dwsafe_cast<DwEvent>(obj);
    evt->setTraced((_lua_pop_value_of<bool>(L, -1)));
    return 0;
}

DECLARE_DELEGATE_SET(marked)
{
    DwEvent *evt = dwsafe_cast<DwEvent>(obj);
    evt->setMarked((_lua_pop_value_of<bool>(L, -1)));
    return 0;
}

// Get
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_GET(accept)
{
    DwEvent *evt = dwsafe_cast<DwEvent>(obj);
    return _lua_push_value_of<bool>(L, evt->isAccepted());
}

DECLARE_DELEGATE_GET(ignore)
{
    DwEvent *evt = dwsafe_cast<DwEvent>(obj);
    return _lua_push_value_of<bool>(L, !evt->isAccepted());
}

DECLARE_DELEGATE_GET(up)
{
    DwEvent *evt = dwsafe_cast<DwEvent>(obj);
    return _lua_push_value_of<bool>(L, evt->isUp());
}

DECLARE_DELEGATE_GET(backtrace)
{
    DwEvent *evt = dwsafe_cast<DwEvent>(obj);
    return _lua_push_value_of<bool>(L, evt->isBacktrace());
}

DECLARE_DELEGATE_GET(traced)
{
    DwEvent *evt = dwsafe_cast<DwEvent>(obj);
    return _lua_push_value_of<bool>(L, evt->isTraced());
}

DECLARE_DELEGATE_GET(marked)
{
    DwEvent *evt = dwsafe_cast<DwEvent>(obj);
    return _lua_push_value_of<bool>(L, evt->isAccepted());
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_CALL(isA)
{
    DW_UNUSED(L);
    DwEvent *evt = dwsafe_cast<DwEvent>(obj);
    DwRtti *info = _lua_pop_value_of<LUA_RTTI_PTR>(L, 3);

    return _lua_push_value_of<bool>(L, evt->isA(info));
}

DECLARE_DELEGATE_CALL(isAccurateA)
{
    DW_UNUSED(L);
    DwEvent *evt = dwsafe_cast<DwEvent>(obj);
    DwRtti *info = _lua_pop_value_of<LUA_RTTI_PTR>(L, 3);

    return _lua_push_value_of<bool>(L, evt->rtti() == info);
}

DECLARE_DELEGATE_CALL(eventName)
{
    DW_UNUSED(L);
    DwEvent *evt = dwsafe_cast<DwEvent>(obj);

    return _lua_push_value_of<DwString>(L, evt->rtti()->name());
}

//------------------------------------------------------------------------------
/**
*/
DwEventDelegate::DwEventDelegate()
{
    DECLARE_ADD_SET(accept);
    DECLARE_ADD_SET(ignore);
    DECLARE_ADD_SET(up);
    DECLARE_ADD_SET(backtrace);
    DECLARE_ADD_SET(traced);
    DECLARE_ADD_SET(marked);

    DECLARE_ADD_GET(accept);
    DECLARE_ADD_GET(ignore);
    DECLARE_ADD_GET(up);
    DECLARE_ADD_GET(backtrace);
    DECLARE_ADD_GET(traced);
    DECLARE_ADD_GET(marked);

    DECLARE_ADD_CALL(isA);
    DECLARE_ADD_CALL(isAccurateA);
    DECLARE_ADD_CALL(eventName);
}