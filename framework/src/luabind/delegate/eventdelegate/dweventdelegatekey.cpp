#include "stable.h"
#include "dweventdelegatekey.h"
#include "bind/dwluapoppush.h"
#include "dwgui/dwgraphicsevent.h"

DW_IMPLEMENT_CLASS(DwEventDelegateKey, 'EDEY', DwEventDelegateScene);

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_SET(keyCode)
{
    DwKeyEvent * keyEvt = dwsafe_cast<DwKeyEvent>(obj);
    DW_CHECK_ASSERT_RET(keyEvt, 0);

    keyEvt->m_virtualKey = (unsigned int)_lua_pop_value_of<int>(L, -1);
    return 0;
}

DECLARE_DELEGATE_SET(modifier)
{
    DwKeyEvent * keyEvt = dwsafe_cast<DwKeyEvent>(obj);
    DW_CHECK_ASSERT_RET(keyEvt, 0);

    keyEvt->m_modifier = (unsigned int)_lua_pop_value_of<int>(L, -1);
    return 0;
}

DECLARE_DELEGATE_SET(isShfitDown)
{
    DwKeyEvent * keyEvt = dwsafe_cast<DwKeyEvent>(obj);
    DW_CHECK_ASSERT_RET(keyEvt, 0);

    keyEvt->m_isShiftDown = _lua_pop_value_of<bool>(L, -1);
    return 0;
}

DECLARE_DELEGATE_SET(isCtrlDown)
{
    DwKeyEvent * keyEvt = dwsafe_cast<DwKeyEvent>(obj);
    DW_CHECK_ASSERT_RET(keyEvt, 0);

    keyEvt->m_isControlDown = _lua_pop_value_of<bool>(L, -1);
    return 0;
}

DECLARE_DELEGATE_SET(isAltDown)
{
    DwKeyEvent * keyEvt = dwsafe_cast<DwKeyEvent>(obj);
    DW_CHECK_ASSERT_RET(keyEvt, 0);

    keyEvt->m_isAltDown = _lua_pop_value_of<bool>(L, -1);
    return 0;
}

DECLARE_DELEGATE_SET(param)
{
    DwKeyEvent * keyEvt = dwsafe_cast<DwKeyEvent>(obj);
    DW_CHECK_ASSERT_RET(keyEvt, 0);

    keyEvt->m_param = (unsigned int)( _lua_pop_value_of<int>(L, -1) );
    return 0;
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_GET(keyCode)
{
    DwKeyEvent * keyEvt = dwsafe_cast<DwKeyEvent>(obj);
    DW_CHECK_ASSERT_RET(keyEvt, 0);

    return _lua_push_value_of<int>(L, (keyEvt->m_virtualKey));
}

DECLARE_DELEGATE_GET(modifier)
{
    DwKeyEvent * keyEvt = dwsafe_cast<DwKeyEvent>(obj);
    DW_CHECK_ASSERT_RET(keyEvt, 0);

    return _lua_push_value_of<int>(L, (keyEvt->m_modifier));
}

DECLARE_DELEGATE_GET(isShfitDown)
{
    DwKeyEvent * keyEvt = dwsafe_cast<DwKeyEvent>(obj);
    DW_CHECK_ASSERT_RET(keyEvt, 0);

    return _lua_push_value_of<bool>(L, bool(keyEvt->m_isShiftDown));
}

DECLARE_DELEGATE_GET(isCtrlDown)
{
    DwKeyEvent * keyEvt = dwsafe_cast<DwKeyEvent>(obj);
    DW_CHECK_ASSERT_RET(keyEvt, 0);

    return _lua_push_value_of<bool>(L, bool(keyEvt->m_isControlDown));
}

DECLARE_DELEGATE_GET(isAltDown)
{
    DwKeyEvent * keyEvt = dwsafe_cast<DwKeyEvent>(obj);
    DW_CHECK_ASSERT_RET(keyEvt, 0);

    return _lua_push_value_of<bool>(L, bool(keyEvt->m_isAltDown));
}

DECLARE_DELEGATE_GET(isExtendKey)
{
    DwKeyEvent * keyEvt = dwsafe_cast<DwKeyEvent>(obj);
    DW_CHECK_ASSERT_RET(keyEvt, 0);

    return _lua_push_value_of<bool>(L, bool(keyEvt->isExtendKey()));
}

DECLARE_DELEGATE_GET(param)
{
    DwKeyEvent * keyEvt = dwsafe_cast<DwKeyEvent>(obj);
    DW_CHECK_ASSERT_RET(keyEvt, 0);

    return _lua_push_value_of<int>(L, int(keyEvt->m_param));
}


//------------------------------------------------------------------------------
/**
*/
DwEventDelegateKey::DwEventDelegateKey()
{
    DECLARE_ADD_SET(keyCode);
    DECLARE_ADD_SET(modifier);
    DECLARE_ADD_SET(isShfitDown);
    DECLARE_ADD_SET(isCtrlDown);
    DECLARE_ADD_SET(isAltDown);
    DECLARE_ADD_SET(param);

    DECLARE_ADD_GET(keyCode);
    DECLARE_ADD_GET(modifier);
    DECLARE_ADD_GET(isShfitDown);
    DECLARE_ADD_GET(isCtrlDown);
    DECLARE_ADD_GET(isAltDown);
    DECLARE_ADD_GET(isExtendKey);
    DECLARE_ADD_GET(param);
}