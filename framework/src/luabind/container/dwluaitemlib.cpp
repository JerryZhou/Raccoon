#include "stable.h"
#include "dwluaitemlib.h"
#include "dwgui/dwframewindow.h"
#include "dwgui/dwgraphicsitem.h"
#include "dwgui/dweventsignaldispatch.h"
#include "dwgui/dwwidgetevent.h"
#include "dwluabind/dwluaengine.h"
#include "dwluabind/dwuiexport.h"

DW_IMPLEMENT_CLASS(DwLuaItemLib, 'WLIB', DwRttiObject);

//------------------------------------------------------------------------------
/**
*/
DwLuaItemLib::DwLuaItemLib()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwLuaItemLib::~DwLuaItemLib()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem *DwLuaItemLib::findItem(uint32_t idx) const
{
    return DwGraphicsItem::findItem(idx);
}

//------------------------------------------------------------------------------
/**
*/
uint32_t DwLuaItemLib::createItem(const DwString &s, DwGraphicsItem *parent, lua_State *L)
{
    DwLuaEngine *engine = DwLuaEngine::engineOf(L);
    if (engine)
    {
        DwUIExport *exporter = dwsafe_cast<DwUIExport>(engine->exporterOf(DwUIExport::RTTI()));
        if (exporter)
        {
            return exporter->createItem(s, parent);
        }
    }

    return 0;
}

//------------------------------------------------------------------------------
/**
*/
int DwLuaItemLib::destroyItem(DwGraphicsItem *item, lua_State *L)
{
    DwLuaEngine *engine = DwLuaEngine::engineOf(L);
    if (engine)
    {
        DwUIExport *exporter = dwsafe_cast<DwUIExport>(engine->exporterOf(DwUIExport::RTTI()));
        if (exporter)
        {
            return exporter->destroyItem(item);
        }
    }

    return 0;
}

//------------------------------------------------------------------------------
/**
*/
int DwLuaItemLib::setProperty(DwGraphicsItem *item, const DwString &key, lua_State *L)
{
    DwLuaEngine *engine = DwLuaEngine::engineOf(L);
    if (engine)
    {
        DwUIExport *exporter = dwsafe_cast<DwUIExport>(engine->exporterOf(DwUIExport::RTTI()));
        if (exporter)
        {
            return exporter->setProperty(item, key);
        }
    }

    return 0;
}

//------------------------------------------------------------------------------
/**
*/
int DwLuaItemLib::propertyOf(DwGraphicsItem *item, const DwString &key, lua_State *L)
{
    DwLuaEngine *engine = DwLuaEngine::engineOf(L);
    if (engine)
    {
        DwUIExport *exporter = dwsafe_cast<DwUIExport>(engine->exporterOf(DwUIExport::RTTI()));
        if (exporter)
        {
            return exporter->propertyOf(item, key);
        }
    }

    return 0;
}

//------------------------------------------------------------------------------
/**
*/
int DwLuaItemLib::call(DwGraphicsItem *item, const DwString &key, lua_State *L)
{
    DwLuaEngine *engine = DwLuaEngine::engineOf(L);
    if (engine)
    {
        DwUIExport *exporter = dwsafe_cast<DwUIExport>(engine->exporterOf(DwUIExport::RTTI()));
        if (exporter)
        {
            return exporter->call(item, key);
        }
    }

    return 0;
}

//------------------------------------------------------------------------------
/**
*/
int DwLuaItemLib::registerItem(DwGraphicsItem *item, lua_State *L)
{
    DwLuaEngine *engine = DwLuaEngine::engineOf(L);
    if (engine)
    {
        DwUIExport *exporter = dwsafe_cast<DwUIExport>(engine->exporterOf(DwUIExport::RTTI()));
        if (exporter)
        {
            return exporter->registerItem(item);
        }
    }

    return 0;
}

//------------------------------------------------------------------------------
/**
*/
DW_GLOBAL_STATIC(DwLuaItemLib, _dw_windowLib);
DwLuaItemLib* dwLuaItemLib()
{
    return _dw_windowLib();
}