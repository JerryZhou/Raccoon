#include "stable.h"
#include "dweventdelegatemousemove.h"
#include "bind/dwluapoppush.h"
#include "dwgui/dwgraphicsevent.h"
#include "dwgui/dwgraphicsitem.h"

DW_IMPLEMENT_CLASS(DwEventDelegateMouseMove, 'EDMC', DwEventDelegateMouse);

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_SET(delta)
{
    DwMouseMoveEvent * mouseEvt = dwsafe_cast<DwMouseMoveEvent>(obj);
    DW_CHECK_ASSERT_RET(mouseEvt, 0);

    mouseEvt->m_delta = _lua_pop_value_of<DwPointF>(L, -1).toPoint();
    return 0;
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_GET(delta)
{
    DwMouseMoveEvent * mouseEvt = dwsafe_cast<DwMouseMoveEvent>(obj);
    DW_CHECK_ASSERT_RET(mouseEvt, 0);

    return _lua_push_value_of<DwPointF>(L, DwPointF(mouseEvt->m_delta));
}


//------------------------------------------------------------------------------
/**
*/
DwEventDelegateMouseMove::DwEventDelegateMouseMove()
{
    DECLARE_ADD_SET(delta);
    DECLARE_ADD_GET(delta);
}