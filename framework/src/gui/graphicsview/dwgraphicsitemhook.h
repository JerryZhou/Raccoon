#pragma once
#include "dwnamespace.h"
#include "dwcore/dwvariant.h"
#include "dwgui/dwgraphicshook.h"

//------------------------------------------------------------------------------
class DwGraphicsItem;
typedef bool (*dw_hook_api)(Dw::ItemHook hook, DwGraphicsItem* item, const DwVariant& v);

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwGraphicsItemHook : public DwGraphicsHook
{
    DW_DECLARE_CLASS(DwGraphicsItemHook);
public:
    DwGraphicsItemHook(DwGraphicsHook* next = NULL);

    void setHookApi(dw_hook_api api);
    dw_hook_api hookApi() const;

protected:
    dw_hook_api m_api;
};// end of DwGraphicsItemHook