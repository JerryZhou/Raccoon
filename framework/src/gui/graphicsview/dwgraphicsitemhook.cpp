#include "stable.h"
#include "dwgraphicsitemhook.h"

DW_IMPLEMENT_CLASS(DwGraphicsItemHook, 'GIHK', DwGraphicsHook);

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItemHook::DwGraphicsItemHook(DwGraphicsHook* next/* = NULL*/)
    : DwGraphicsHook(next)
    , m_api(NULL)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItemHook::setHookApi(dw_hook_api api)
{
    m_api = api;
}

//------------------------------------------------------------------------------
/**
*/
dw_hook_api DwGraphicsItemHook::hookApi() const
{
    return m_api;
}