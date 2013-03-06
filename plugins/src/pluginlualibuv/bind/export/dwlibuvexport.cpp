#include "stable.h"
#include "dwlibuvexport.h"
#include "dwluabind/dwluaengine.h"

DW_IMPLEMENT_CLASS(DwLibuvExport, 'LUVT', DwLuaExport);

#ifdef  __cplusplus
extern "C" {
#endif
#include "luv.h"
#include "luv_misc.h"
extern int lua_open_libvit(lua_State *L);
#ifdef  __cplusplus
}
#endif

//------------------------------------------------------------------------------
/**
*/
DwLibuvExport::DwLibuvExport()
{

}

//------------------------------------------------------------------------------
/**
*/
DwLibuvExport::~DwLibuvExport()
{

}

//------------------------------------------------------------------------------
/**
*/
bool DwLibuvExport::exportd(DwLuaEngine *e)
{
    DW_UNUSED(e);
    lua_State *L = e->luaState();
    lua_open_libvit(L);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwLibuvExport::run()
{
    lua_State *L = m_engine->luaState();
    return luv_run(L);
}

//------------------------------------------------------------------------------
/**
*/
bool DwLibuvExport::run_once()
{
    lua_State *L = m_engine->luaState();
    return luv_run_once(L);
}