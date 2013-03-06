#include "stable.h"
#include "dwitemdelegatebasepositioner.h"
#include "bind/dwluapoppush.h"
#include "dwgui/dwresourceinterface.h"
#include "dwgui/dwbasepositioner.h"

DW_IMPLEMENT_CLASS(DwItemDelegateBasePositioner, 'DBPR', DwItemDelegate);

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_SET(spacing)
{
    DW_UNUSED(obj);
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) >= 2 );

    DwBasePositioner *positioner = dwsafe_cast<DwBasePositioner>(obj);
    DW_CHECK_ASSERT_RET(positioner, 0);
    positioner->setSpacing(_lua_pop_value_of<DwReal>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(applyImplicitWidth)
{
    DW_UNUSED(obj);
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) >= 2 );

    DwBasePositioner *positioner = dwsafe_cast<DwBasePositioner>(obj);
    DW_CHECK_ASSERT_RET(positioner, 0);
    positioner->setApplyImplicitWidth(_lua_pop_value_of<bool>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(applyImplicitHeight)
{
    DW_UNUSED(obj);
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) >= 2 );

    DwBasePositioner *positioner = dwsafe_cast<DwBasePositioner>(obj);
    DW_CHECK_ASSERT_RET(positioner, 0);
    positioner->setApplyImplicitHeight(_lua_pop_value_of<bool>(L, -1));
    return 0;
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_GET(spacing)
{
    DwBasePositioner *positioner = dwsafe_cast<DwBasePositioner>(obj);
    DW_CHECK_ASSERT_RET(positioner, 0);

    return _lua_push_value_of<DwReal>(L, positioner->spacing());
}

DECLARE_DELEGATE_GET(applyImplicitWidth)
{
    DwBasePositioner *positioner = dwsafe_cast<DwBasePositioner>(obj);
    DW_CHECK_ASSERT_RET(positioner, 0);

    return _lua_push_value_of<bool>(L, positioner->isApplyImplicitWidth());
}

DECLARE_DELEGATE_GET(applyImplicitHeight)
{
    DwBasePositioner *positioner = dwsafe_cast<DwBasePositioner>(obj);
    DW_CHECK_ASSERT_RET(positioner, 0);

    return _lua_push_value_of<bool>(L, positioner->isApplayImplicitHeight());
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_CALL(setSpacing)
{
    DwBasePositioner *positioner = dwsafe_cast<DwBasePositioner>(obj);
    DW_CHECK_ASSERT_RET(positioner, 0);
    positioner->setSpacing(_lua_pop_value_of<DwReal>(L, 3));
    return 0;
}

DECLARE_DELEGATE_CALL(setApplyImplicitWidth)
{
    DW_UNUSED(obj);
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) >= 3 );

    DwBasePositioner *positioner = dwsafe_cast<DwBasePositioner>(obj);
    DW_CHECK_ASSERT_RET(positioner, 0);
    positioner->setApplyImplicitWidth(_lua_pop_value_of<bool>(L, 3));
    return 0;
}

DECLARE_DELEGATE_CALL(setApplyImplicitHeight)
{
    DW_UNUSED(obj);
    DW_UNUSED(L);
    DW_ASSERT(lua_gettop(L) >= 3 );

    DwBasePositioner *positioner = dwsafe_cast<DwBasePositioner>(obj);
    DW_CHECK_ASSERT_RET(positioner, 0);
    positioner->setApplyImplicitHeight(_lua_pop_value_of<bool>(L, 3));
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
DwItemDelegateBasePositioner::DwItemDelegateBasePositioner()
{
    DECLARE_ADD_SET(spacing);
    DECLARE_ADD_SET(applyImplicitWidth);
    DECLARE_ADD_SET(applyImplicitHeight);

    DECLARE_ADD_GET(spacing);
    DECLARE_ADD_GET(applyImplicitWidth);
    DECLARE_ADD_GET(applyImplicitHeight);

    DECLARE_ADD_CALL(setSpacing);
    DECLARE_ADD_CALL(setApplyImplicitWidth);
    DECLARE_ADD_CALL(setApplyImplicitHeight);
}