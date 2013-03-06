#include "stable.h"
#include "dweventdelegatechar.h"
#include "bind/dwluapoppush.h"
#include "dwgui/dwgraphicsevent.h"

DW_IMPLEMENT_CLASS(DwEventDelegateChar, 'EDAR', DwEventDelegateScene);

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_SET(characterCode)
{
    DwCharEvent * charEvt = dwsafe_cast<DwCharEvent>(obj);
    DW_CHECK_ASSERT_RET(charEvt, 0);

    charEvt->m_characterCode = (unsigned int)_lua_pop_value_of<int>(L, -1);
    return 0;
}

DECLARE_DELEGATE_SET(virtualKey)
{
    DwCharEvent * charEvt = dwsafe_cast<DwCharEvent>(obj);
    DW_CHECK_ASSERT_RET(charEvt, 0);

    charEvt->m_virtualKey = (unsigned int)_lua_pop_value_of<int>(L, -1);
    return 0;
}

DECLARE_DELEGATE_SET(param)
{
    DwCharEvent * charEvt = dwsafe_cast<DwCharEvent>(obj);
    DW_CHECK_ASSERT_RET(charEvt, 0);

    charEvt->m_param = (unsigned int)( _lua_pop_value_of<int>(L, -1) );
    return 0;
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_GET(characterCode)
{
    DwCharEvent * charEvt = dwsafe_cast<DwCharEvent>(obj);
    DW_CHECK_ASSERT_RET(charEvt, 0);

    return _lua_push_value_of<int>(L, (int)(charEvt->m_characterCode));
}

DECLARE_DELEGATE_GET(virtualKey)
{
    DwCharEvent * charEvt = dwsafe_cast<DwCharEvent>(obj);
    DW_CHECK_ASSERT_RET(charEvt, 0);

    return _lua_push_value_of<int>(L, (int)(charEvt->m_virtualKey));
}

DECLARE_DELEGATE_GET(param)
{
    DwCharEvent * charEvt = dwsafe_cast<DwCharEvent>(obj);
    DW_CHECK_ASSERT_RET(charEvt, 0);

    return _lua_push_value_of<int>(L, (int)(charEvt->m_param));
}

//------------------------------------------------------------------------------
/**
*/
DwEventDelegateChar::DwEventDelegateChar()
{
    DECLARE_ADD_SET(characterCode);
    DECLARE_ADD_SET(virtualKey);
    DECLARE_ADD_SET(param);

    DECLARE_ADD_GET(characterCode);
    DECLARE_ADD_GET(virtualKey);
    DECLARE_ADD_GET(param);
}