#include "stable.h"
#include "dwluaexport.h"
#include "dwluabind/dwluaengine.h"

DW_IMPLEMENT_CLASS(DwLuaExport, 'LERT', DwRttiObject);

//------------------------------------------------------------------------------
/**
*/
DwLuaExport::DwLuaExport()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwLuaExport::~DwLuaExport()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
bool DwLuaExport::exportd( DwLuaEngine * )
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwLuaExport::onItemCreate(DwGraphicsItem *)
{
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwLuaExport::onItemDestroy(DwGraphicsItem *)
{
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwLuaExport::onEngineCreate(DwLuaEngine *)
{
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwLuaExport::onEngineDestroy(DwLuaEngine *)
{
    return true;
}

//------------------------------------------------------------------------------
/**
*/
DwLuaEngine *DwLuaExport::engine() const
{
    return m_engine;
}

//------------------------------------------------------------------------------
/**
*/
void DwLuaExport::setCallExportWhen(Dw::CallExportWhen when, bool b/*=true*/)
{
    if (b)
    {
        m_whens |= when;
    }
    else
    {
        m_whens &= (~when);
    }
}

//------------------------------------------------------------------------------
/**
*/
bool DwLuaExport::isCallExportWhen(Dw::CallExportWhen when)const
{
    return !!(m_whens & when);
}

//------------------------------------------------------------------------------
/**
*/
void DwLuaExport::setEngine(DwLuaEngine *e)
{
    DW_ASSERT(e);
    m_engine = e;
}
