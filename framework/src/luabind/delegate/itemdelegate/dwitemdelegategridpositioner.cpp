#include "stable.h"
#include "dwitemdelegategridpositioner.h"
#include "bind/dwluapoppush.h"
#include "dwgui/dwresourceinterface.h"
#include "dwgui/dwgridpositioner.h"

DW_IMPLEMENT_CLASS(DwItemDelegateGridPositioner, 'DGPR', DwItemDelegateBasePositioner);

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_SET(rows)
{
    DwGridPositioner *positioner = dwsafe_cast<DwGridPositioner>(obj);
    positioner->setRows(_lua_pop_value_of<int>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(columns)
{
    DwGridPositioner *positioner = dwsafe_cast<DwGridPositioner>(obj);
    positioner->setColumns(_lua_pop_value_of<int>(L, -1));
    return 0;
}

DECLARE_DELEGATE_SET(flow)
{
    DwGridPositioner *positioner = dwsafe_cast<DwGridPositioner>(obj);
    positioner->setFlow(DwGridPositioner::Flow(_lua_pop_value_of<int>(L, -1)));
    return 0;
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_GET(rows)
{
    DwGridPositioner *positioner = dwsafe_cast<DwGridPositioner>(obj);
    return _lua_push_value_of<int>(L, positioner->rows());
}

DECLARE_DELEGATE_GET(columns)
{
    DwGridPositioner *positioner = dwsafe_cast<DwGridPositioner>(obj);
    return _lua_push_value_of<int>(L, positioner->columns());
}

DECLARE_DELEGATE_GET(flow)
{
    DwGridPositioner *positioner = dwsafe_cast<DwGridPositioner>(obj);
    return _lua_push_value_of<int>(L, (int)(positioner->flow()));
}

//////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_CALL(setRows)
{
    DwGridPositioner *positioner = dwsafe_cast<DwGridPositioner>(obj);
    positioner->setRows(_lua_pop_value_of<int>(L, -1));
    return 0;
}

DECLARE_DELEGATE_CALL(setColumns)
{
    DwGridPositioner *positioner = dwsafe_cast<DwGridPositioner>(obj);
    positioner->setColumns(_lua_pop_value_of<int>(L, -1));
    return 0;
}

DECLARE_DELEGATE_CALL(setFlow)
{
    DwGridPositioner *positioner = dwsafe_cast<DwGridPositioner>(obj);
    positioner->setFlow(DwGridPositioner::Flow(_lua_pop_value_of<int>(L, -1)));
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
DwItemDelegateGridPositioner::DwItemDelegateGridPositioner()
{
    DECLARE_ADD_SET(rows);
    DECLARE_ADD_SET(columns);
    DECLARE_ADD_SET(flow);

    DECLARE_ADD_GET(rows);
    DECLARE_ADD_GET(columns);
    DECLARE_ADD_GET(flow);

    DECLARE_ADD_CALL(setRows);
    DECLARE_ADD_CALL(setColumns);
    DECLARE_ADD_CALL(setFlow);
}