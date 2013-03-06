#pragma once
#include "dwgui/dwrttiobject.h"
#include "dwgui/dwurl.h"
#include "dwluabind/dwbindnamespace.h"
#include "dwluabind/dwlua.h"

//------------------------------------------------------------------------------
struct lua_State;
class DwLuaExport;
typedef int (*d_lua_CFunction) (lua_State *L);

//------------------------------------------------------------------------------
class DW_LUABIND_EXPORT DwLuaEngine : public DwRttiObject
{
    DW_DECLARE_CLASS(DwLuaEngine);
    DW_DISABLE_COPY(DwLuaEngine);
public:
    DwLuaEngine();
    virtual ~DwLuaEngine();

    bool setup();
    bool setupWithAllExporters();
    bool addExporter(const DwString &s);
    bool addExporter(const DwFourCC &c);
    bool addExporter(const DwRtti *c);
    DwLuaExport* exporterOf(const DwRtti *c)const;

    bool load(const DwUrl &url);
    bool load(const char *buff, int sz, const char *name);
    void __cdecl call(const char *f, const char *sig, ...);
    bool run(const char *buff);
    void registerScript(d_lua_CFunction cf, const char *f, const char *ns=NULL);

    lua_State *luaState() const;

    void callExportersWhen(Dw::CallExportWhen when, DwRttiObject *target);

    static DwLuaEngine* engineOf(const lua_State *L);

protected:
    void callExporterWhen(Dw::CallExportWhen when, DwLuaExport* exporter, DwRttiObject *target);
    void addExporterToEngine(DwLuaExport *exporter);
    void removeExporterFromEngine(const DwRtti* c);
    void removeExporterFromEngine(DwLuaExport *exporter);
    void removeAllExporters();

    typedef DwMap<DwRtti*, DwLuaExport*> ExporterMap;
    lua_State *m_L;
    ExporterMap m_exporters;
};// end of DwLuaEngine