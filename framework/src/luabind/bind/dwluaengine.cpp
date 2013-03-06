#include "stable.h"
#include "dwluaengine.h"
#include "dwluaengine_p.h"
#include "dwgui/dwresourceinterface.h"
#include "dwgui/dwringlogcase.h"
#include "dwgui/dwassignlib.h"
#include "dwgui/dwgraphicsitem.h"
#include "dwgui/dwgraphicsitemhook.h"
#include "dwluabind/dwluainterface.h"
#include "dwluabind/dwluaexport.h"
#include "dwcore/dwlocks.h"

DW_IMPLEMENT_CLASS(DwLuaEngine, 'LENE', DwRttiObject);
DW_DECLARE_DLLRESOURCE(luabind, theme);

static bool _item_hook_create_destroy(Dw::ItemHook h, DwGraphicsItem *item, const DwVariant &v);
static char engine_register[1];

//------------------------------------------------------------------------------
/**
*/
struct LuaEngineLib
{
    typedef DwMap<lua_State*, DwLuaEngine*> EngineMap;
    EngineMap engines;
    DwCriticalSection criticalSection;

    LuaEngineLib()
    {
        DwGraphicsItemHook*hook = new DwGraphicsItemHook();
        hook->setHookApi(_item_hook_create_destroy);

        DwGraphicsItem::addHook(hook);
    }

    ~LuaEngineLib()
    {
        ;
    }

    void addEngine(const DwLuaEngine *e)
    {
        DW_ASSERT(e);
        lua_State* L = e->luaState();
        lua_pushlightuserdata(L, engine_register);
        lua_pushlightuserdata(L, (void*)e);
        lua_rawset(L, LUA_REGISTRYINDEX);

        DwCSLocker locks(&criticalSection);
        engines[L] = (DwLuaEngine *)e;
    }

    void removeEngine(const DwLuaEngine *e)
    {
        DW_ASSERT(e);
        lua_State* L = e->luaState();
        lua_pushlightuserdata(L, engine_register);
        lua_pushnil(L);
        lua_rawset(L, LUA_REGISTRYINDEX);

        DwCSLocker locks(&criticalSection);
        engines.remove(L);
    }

    const DwLuaEngine* engineOf(const lua_State* L)const
    {
        DW_ASSERT(L);
        DwLuaEngine* engine = NULL;
        lua_pushlightuserdata((lua_State*)L, engine_register);
        
        lua_rawget((lua_State*)L, LUA_REGISTRYINDEX);
        if (lua_islightuserdata((lua_State*)L, -1))
        {
            engine = (DwLuaEngine*)lua_touserdata((lua_State*)L, -1);
        }
        lua_pop((lua_State*)L, 1);
        return engine;
    }

    void onItemCreate(DwGraphicsItem *item, const DwVariant &v)
    {
        DW_UNUSED(v);
        DwCSLocker locks(&criticalSection);

        EngineMap::iterator ite = engines.begin();
        while(ite != engines.end())
        {
            ite->second->callExportersWhen(Dw::CallWhenItemCreate, item);
            ++ite;
        }
    }

    void onItemDestroy(DwGraphicsItem *item, const DwVariant &v)
    {
        DW_UNUSED(v);
        DwCSLocker locks(&criticalSection);

        EngineMap::iterator ite = engines.begin();
        while(ite != engines.end())
        {
            ite->second->callExportersWhen(Dw::CallWhenItemDestroy, item);
            ++ite;
        }
    }
};
DW_GLOBAL_STATIC(LuaEngineLib, _dw_LuaEngineLib);

//------------------------------------------------------------------------------
/**
*/
DwLuaEngine::DwLuaEngine()
    : m_L(NULL)
{
    m_L = luaL_newstate();
    luaL_openlibs(m_L);
    _user_replace_loader(m_L);

    static bool setupAssign = false;
    if (!setupAssign)
    {
        setupAssign = true;
        //dwAssignLib()->setAssign(DwAssign("script", "dll:///theme/script"));
        dwAssignLib()->setAssign(DwAssign("script", "home:theme/script"));
        DW_MOUNTED_DLLRESOURCE(luabind, theme);
    }

    _dw_LuaEngineLib()->addEngine(this);
}

//------------------------------------------------------------------------------
/**
*/
DwLuaEngine::~DwLuaEngine()
{
    removeAllExporters();

    _dw_LuaEngineLib()->removeEngine(this);

    if (m_L)
    {
        lua_close(m_L);
        m_L = NULL;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool DwLuaEngine::setup()
{
    bool isOk = load("script:ui/enginesetup.lua");
    isOk = isOk && load("script:ui/namespace.lua");
    isOk = isOk && load("script:ui/sigslot.lua");
    isOk = isOk && load("script:ui/sigslotcom.lua");
    isOk = isOk && load("script:ui/variant.lua");
    isOk = isOk && load("script:ui/variantcom.lua");
    isOk = isOk && load("script:ui/point.lua");

    

    return isOk;
}

//------------------------------------------------------------------------------
/**
*/
bool DwLuaEngine::setupWithAllExporters()
{
    bool isOk = setup();
    isOk = isOk && DwLuaInterface::addInlineExporters(this);
    
    return isOk;
}

//------------------------------------------------------------------------------
/**
*/
bool DwLuaEngine::addExporter(const DwString &s)
{
    const DwRtti *cinfo = dwFactory()->classRttiOf(s);
    if (!cinfo)
    {
        DW_UI_LOGER() << "exporter not exist: " << s << "\n";
        DW_ASSERT("exporter not exist that is exported to engine" && false);
    }

    return addExporter(cinfo);
}

//------------------------------------------------------------------------------
/**
*/
bool DwLuaEngine::addExporter(const DwFourCC &c)
{
    const DwRtti *cinfo = dwFactory()->classRttiOf(c);
    if (!cinfo)
    {
        DW_UI_LOGER() << "exporter not exist: " << c << "\n";
        DW_ASSERT("exporter not exist that is exported to engine" && false);
    }

    return addExporter(cinfo);
}

//------------------------------------------------------------------------------
/**
*/
bool DwLuaEngine::addExporter(const DwRtti *c)
{
    bool isOk = false;
    if (c)
    {
        if(c->isDerivedFrom(DwLuaExport::RTTI()))
        {
            DwLuaExport *obj = dwsafe_cast<DwLuaExport>(c->create());
            addExporterToEngine(obj);
            isOk = obj->exportd(this);
        }
        else
        {
            DW_UI_LOGER() << "wrong exporter" << c->name() << "\n";
            DW_ASSERT("wrong script exporter is exported to engine" && false);
        }
    }

    return isOk;
}

//------------------------------------------------------------------------------
/**
*/
DwLuaExport* DwLuaEngine::exporterOf(const DwRtti *c)const
{
    return m_exporters.value((DwRtti *)c, NULL);
}

//------------------------------------------------------------------------------
/**
*/
bool DwLuaEngine::load( const DwUrl &url )
{
    DwByteArray data = DwResourceInterface::acquireData(url);
    if (data.size() > 0)
    {
        return load(data.constData(), data.size(), url.localPath().toCap().data());
    }
    else
    {
        DW_UI_LOGER() << "script engine: can not find script" << url.localPath() << "\n";
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwLuaEngine::load( const char *buff, int sz, const char *name )
{
    DW_ASSERT(buff);
    DW_ASSERT(sz);
    DW_ASSERT(name);

    int res = luaL_loadbuffer(m_L, buff, sz, name);
    if (0 != res)
    {
        DwString s = lua_tostring(m_L, -1);
        DW_UI_LOGER() << "script error: " << s << "\n";
        lua_pop(m_L, 1); //  pop the error
        DW_ASSERT(false);
        return false;
    }
    res = lua_pcall(m_L, 0, 0, 0);
    if (0 != res)
    {
        DwString s = lua_tostring(m_L, -1);
        DW_UI_LOGER() << "script error: " << s << "\n";
        lua_pop(m_L, 1); //  pop the error
        DW_ASSERT(false);
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwLuaEngine::run(const char *buff)
{
    if (buff)
    {
        int len = strlen(buff);
        return load(buff, len, "_engine_run");
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void __cdecl DwLuaEngine::call(const char *func, const char *sig, ...)
{
    va_list vl;
    int narg, nres; /* number of arguments and results */
    va_start(vl, sig);
    lua_getglobal(m_L, func); /* get function */
    if (!lua_isfunction(m_L, -1))
    {
        DW_ASSERT(false);
        lua_pop(m_L, 1);
        va_end(vl);
        return;
    }
    /* push arguments */
    narg = 0;
    while (*sig)
    {
        /* push arguments */
        switch (*sig++)
        {
        case 'd': /* double argument */
            lua_pushnumber(m_L, va_arg(vl, double));
            break;
        case 'i': /* int argument */
            lua_pushnumber(m_L, va_arg(vl, int));
            break;
        case 's': /* string argument */
            lua_pushstring(m_L, va_arg(vl, char *));
            break;
        case '>':
            goto endwhile;
        default:
            DW_ASSERT(false);
            DW_UI_LOGER() << "invalid option (%c)" << *(sig - 1) << "\n";
        }
        narg++;
        luaL_checkstack(m_L, 1, "too many arguments");
    }

endwhile:
    /* do the call */
    nres = strlen(sig); /* number of expected results */
    if (lua_pcall(m_L, narg, nres, 0) != 0) /* do the call */
    {
        DW_ASSERT(false);
        DW_UI_LOG("error running function `%s': %s", func, lua_tostring(m_L, -1));
        lua_pop(m_L, 1);
        va_end(vl);
        return;
    }

    /* retrieve results */
    nres = -nres; /* stack index of first result */
    while (*sig)
    {
        /* get results */
        switch (*sig++)
        {
        case 'd': /* double result */
            if (!lua_isnumber(m_L, nres))
            {
                DW_ASSERT(false);
                DW_UI_LOGER() << "wrong result type" << "\n";
            }
            *va_arg(vl, double *) = lua_tonumber(m_L, nres);
            break;
        case 'i': /* int result */
            if (!lua_isnumber(m_L, nres))
            {
                DW_ASSERT(false);
                DW_UI_LOGER() << "wrong result type" << "\n";
            }
            *va_arg(vl, int *) = (int)lua_tonumber(m_L, nres);
            break;
        case 's': /* string result */
            if (!lua_isstring(m_L, nres))
            {
                DW_ASSERT(false);
                DW_UI_LOGER() << "wrong result type" << "\n";
            }
            *va_arg(vl, const char **) = lua_tostring(m_L, nres);
            break;
        default:
            DW_ASSERT(false);
            DW_UI_LOGER() << "invalid option (%c)" << *(sig - 1) << "\n";
        }
        nres++;
    }
    va_end(vl);
}

//------------------------------------------------------------------------------
/**
*/
void DwLuaEngine::registerScript(lua_CFunction cf, const char *f, const char *ns/*=NULL*/)
{
    DW_ASSERT(cf);
    DW_ASSERT(f);

    if (!ns)
    {
        lua_pushcfunction(m_L, cf);
        lua_setglobal(m_L, f);
    }
    else
    {
        lua_getglobal(m_L, ns);
        if (lua_istable(m_L, -1))
        {
            lua_pushcfunction(m_L, cf);
            lua_setfield(m_L, -2, f);
        }
        else
        {
            lua_pop(m_L, 1);

            lua_newtable(m_L);
            lua_setglobal(m_L, ns);

            lua_getglobal(m_L, ns);
            lua_pushcfunction(m_L, cf);
            lua_setfield(m_L, -2, f);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
lua_State * DwLuaEngine::luaState() const
{
    return m_L;
}

//------------------------------------------------------------------------------
/**
*/
void DwLuaEngine::callExportersWhen(Dw::CallExportWhen when, DwRttiObject *target)
{
    ExporterMap::iterator ite = m_exporters.begin();
    while (ite != m_exporters.end())
    {
        callExporterWhen(when, ite->second, target);
        ++ite;
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwLuaEngine::callExporterWhen(Dw::CallExportWhen when, DwLuaExport* exporter, DwRttiObject *target)
{
    DW_ASSERT(exporter);
    if (exporter && exporter->isCallExportWhen(when))
    {
        switch(when)
        {
        case Dw::CallWhenEngineCreate:
            exporter->onEngineCreate(this);
            break;
        case Dw::CallWhenEngineDestroy:
            exporter->onEngineDestroy(this);
            break;
        case Dw::CallWhenItemCreate:
            exporter->onItemCreate(dwsafe_cast<DwGraphicsItem>(target));
            break;
        case Dw::CallWhenItemDestroy:
            exporter->onItemDestroy(dwsafe_cast<DwGraphicsItem>(target));
            break;
        default:
            DW_UI_LOGER() << "wrong time point in DwLuaEngine::callExporterWhen:"
                          << exporter->rtti()->name()
                          << " when:"
                          << (int)when << "\n";
            DW_ASSERT("wrong time point in DwLuaEngine::callExporterWhen" && false);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwLuaEngine::addExporterToEngine(DwLuaExport *exporter)
{
    if( exporterOf(exporter->rtti()) != exporter)
    {
        removeExporterFromEngine(exporter->rtti());

        if (exporter)
        {
            m_exporters[exporter->rtti()] = exporter;
            exporter->setEngine(this);

            callExporterWhen(Dw::CallWhenEngineCreate, exporter, NULL);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwLuaEngine::removeExporterFromEngine(const DwRtti* c)
{
    if (c && m_exporters.contains((DwRtti*)c))
    {
        DwLuaExport *exporter = m_exporters.value((DwRtti*)c, NULL);
        callExporterWhen(Dw::CallWhenEngineDestroy, exporter, NULL);
        m_exporters.remove((DwRtti*)c);
        DW_SAFE_DELETE(exporter);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwLuaEngine::removeExporterFromEngine(DwLuaExport *exporter)
{
    removeExporterFromEngine(exporter->rtti());
}

//------------------------------------------------------------------------------
/**
*/
void DwLuaEngine::removeAllExporters()
{
    callExportersWhen(Dw::CallWhenEngineDestroy, NULL);
    ExporterMap::iterator ite = m_exporters.begin();
    while (ite != m_exporters.end())
    {
        DW_SAFE_DELETE(ite->second);
        ++ite;
    }
}

//------------------------------------------------------------------------------
/**
*/
DwLuaEngine* DwLuaEngine::engineOf(const lua_State *L)
{
    return (DwLuaEngine*)(_dw_LuaEngineLib()->engineOf(L));
}


//------------------------------------------------------------------------------
/**
*/
bool _item_hook_create_destroy(Dw::ItemHook h, DwGraphicsItem *item, const DwVariant &v)
{
    DW_UNUSED(v);
    if (h == Dw::HookCreate)
    {
        _dw_LuaEngineLib()->onItemCreate(item, v);
    }
    else if (h == Dw::HookDestroy)
    {
        _dw_LuaEngineLib()->onItemDestroy(item, v);
    }

    return false;
}
