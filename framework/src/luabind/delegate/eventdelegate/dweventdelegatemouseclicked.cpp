#include "stable.h"
#include "dweventdelegatemouseclicked.h"
#include "bind/dwluapoppush.h"
#include "dwgui/dwgraphicsevent.h"
#include "dwgui/dwgraphicsitem.h"

DW_IMPLEMENT_CLASS(DwEventDelegateMouseClicked, 'EDMC', DwEventDelegateMouse);

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_SET(clickedItem)
{
    DwMouseClickedEvent * mouseEvt = dwsafe_cast<DwMouseClickedEvent>(obj);
    DW_CHECK_ASSERT_RET(mouseEvt, 0);

    mouseEvt->m_clickedItem = _lua_pop_value_of<LUA_GRAPHICSITEM_PTR>(L, -1);
    return 0;
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_GET(clickedItem)
{
    DwMouseClickedEvent * mouseEvt = dwsafe_cast<DwMouseClickedEvent>(obj);
    DW_CHECK_ASSERT_RET(mouseEvt, 0);

    return _lua_push_value_of<LUA_GRAPHICSITEM_PTR>(L, mouseEvt->m_clickedItem);
}

//------------------------------------------------------------------------------
/**
*/
DwEventDelegateMouseClicked::DwEventDelegateMouseClicked()
{
    DECLARE_ADD_SET(clickedItem);
    DECLARE_ADD_GET(clickedItem);
}