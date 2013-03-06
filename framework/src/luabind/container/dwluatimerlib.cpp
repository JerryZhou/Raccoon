#include "stable.h"
#include "dwluatimerlib.h"
#include "dwgui/dwringlogcase.h"
#include "dwluabind/dwluaengine.h"
#include "dwluabind/dwtimerexport.h"

DW_IMPLEMENT_CLASS(DwLuaTimerLib, 'DLTL', DwRttiObject);

//------------------------------------------------------------------------------
/**
*/
DwLuaTimerLib::DwLuaTimerLib()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwLuaTimerLib::~DwLuaTimerLib()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
STimerId DwLuaTimerLib::newTimer(uint32_t mesc, lua_State *L)
{
    DwLuaEngine *engine = DwLuaEngine::engineOf(L);
    if (engine)
    {
        DwTimerExport *exporter = dwsafe_cast<DwTimerExport>(engine->exporterOf(DwTimerExport::RTTI()));
        if (exporter)
        {
            return exporter->newTimer(mesc);
        }
    }

    return 0;
}

//------------------------------------------------------------------------------
/**
*/
bool DwLuaTimerLib::deleteTimer(STimerId id, lua_State *L)
{
    DwLuaEngine *engine = DwLuaEngine::engineOf(L);
    if (engine)
    {
        DwTimerExport *exporter = dwsafe_cast<DwTimerExport>(engine->exporterOf(DwTimerExport::RTTI()));
        if (exporter)
        {
            return exporter->deleteTimer(id);
        }
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwLuaTimerLib::deleteTimerLater(STimerId id, lua_State *L)
{
    DwLuaEngine *engine = DwLuaEngine::engineOf(L);
    if (engine)
    {
        DwTimerExport *exporter = dwsafe_cast<DwTimerExport>(engine->exporterOf(DwTimerExport::RTTI()));
        if (exporter)
        {
            return exporter->deleteTimerLater(id);
        }
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwLuaTimerLib::startTimer(STimerId id, lua_State *L)
{
    DwLuaEngine *engine = DwLuaEngine::engineOf(L);
    if (engine)
    {
        DwTimerExport *exporter = dwsafe_cast<DwTimerExport>(engine->exporterOf(DwTimerExport::RTTI()));
        if (exporter)
        {
            return exporter->startTimer(id);
        }
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwLuaTimerLib::startTimer(STimerId id, uint32_t mesc, lua_State *L)
{
    DwLuaEngine *engine = DwLuaEngine::engineOf(L);
    if (engine)
    {
        DwTimerExport *exporter = dwsafe_cast<DwTimerExport>(engine->exporterOf(DwTimerExport::RTTI()));
        if (exporter)
        {
            return exporter->startTimer(id, mesc);
        }
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwLuaTimerLib::stopTimer(STimerId id, lua_State *L)
{
    DwLuaEngine *engine = DwLuaEngine::engineOf(L);
    if (engine)
    {
        DwTimerExport *exporter = dwsafe_cast<DwTimerExport>(engine->exporterOf(DwTimerExport::RTTI()));
        if (exporter)
        {
            return exporter->stopTimer(id);
        }
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwLuaTimerLib::isActive(uint32_t id, lua_State *L) const
{
    DwLuaEngine *engine = DwLuaEngine::engineOf(L);
    if (engine)
    {
        DwTimerExport *exporter = dwsafe_cast<DwTimerExport>(engine->exporterOf(DwTimerExport::RTTI()));
        if (exporter)
        {
            return exporter->isActive(id);
        }
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwLuaTimerLib::setInterval(STimerId id, uint32_t mesc, lua_State *L)
{
    DwLuaEngine *engine = DwLuaEngine::engineOf(L);
    if (engine)
    {
        DwTimerExport *exporter = dwsafe_cast<DwTimerExport>(engine->exporterOf(DwTimerExport::RTTI()));
        if (exporter)
        {
            return exporter->setInterval(id, mesc);
        }
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwLuaTimerLib::connect(STimerId id, lua_State *L)
{
    DwLuaEngine *engine = DwLuaEngine::engineOf(L);
    if (engine)
    {
        DwTimerExport *exporter = dwsafe_cast<DwTimerExport>(engine->exporterOf(DwTimerExport::RTTI()));
        if (exporter)
        {
            return exporter->connect(id);
        }
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwLuaTimerLib::disconnect(STimerId id, lua_State *L)
{
    DwLuaEngine *engine = DwLuaEngine::engineOf(L);
    if (engine)
    {
        DwTimerExport *exporter = dwsafe_cast<DwTimerExport>(engine->exporterOf(DwTimerExport::RTTI()));
        if (exporter)
        {
            return exporter->disconnect(id);
        }
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
DW_GLOBAL_STATIC(DwLuaTimerLib, _dw_LuaTimerLib);
DwLuaTimerLib* dwLuaTimerLib()
{
    return _dw_LuaTimerLib();
}