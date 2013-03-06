#include "stable.h"
#include "dweventdelegatedragmove.h"
#include "bind/dwluapoppush.h"
#include "dwgui/dwgraphicsevent.h"

DW_IMPLEMENT_CLASS(DwEventDelegateDragMove, 'EDDM', DwEventDelegateDrag);

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_SET(delta)
{
    DwDragMoveEvent * dragEvt = dwsafe_cast<DwDragMoveEvent>(obj);
    DW_CHECK_ASSERT_RET(dragEvt, 0);

    dragEvt->m_delta = _lua_pop_value_of<DwPointF>(L, -1).toPoint();
    return 0;
}

DECLARE_DELEGATE_SET(curMousePoint)
{
    DwDragMoveEvent * dragEvt = dwsafe_cast<DwDragMoveEvent>(obj);
    DW_CHECK_ASSERT_RET(dragEvt, 0);

    dragEvt->m_curMousePoint = _lua_pop_value_of<DwPointF>(L, -1).toPoint();
    return 0;
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_GET(delta)
{
    DwDragMoveEvent * dragEvt = dwsafe_cast<DwDragMoveEvent>(obj);
    DW_CHECK_ASSERT_RET(dragEvt, 0);

    return _lua_push_value_of<DwPointF>(L, (DwPointF)(dragEvt->m_delta));
}

DECLARE_DELEGATE_GET(curMousePoint)
{
    DwDragMoveEvent * dragEvt = dwsafe_cast<DwDragMoveEvent>(obj);
    DW_CHECK_ASSERT_RET(dragEvt, 0);

    return _lua_push_value_of<DwPointF>(L, (DwPointF)(dragEvt->m_curMousePoint));
}

//------------------------------------------------------------------------------
/**
*/
DwEventDelegateDragMove::DwEventDelegateDragMove()
{
    DECLARE_ADD_SET(delta);
    DECLARE_ADD_SET(curMousePoint);

    DECLARE_ADD_GET(delta);
    DECLARE_ADD_GET(curMousePoint);
}