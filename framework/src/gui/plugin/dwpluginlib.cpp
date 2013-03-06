#include "dwpluginlib.h"
#include "dwgui/dwfswrapper.h"
#include "dwgui/dwprivateprofile.h"
#include "dwgui/dwassignlib.h"
#include "dwgui/dwplugin.h"
#include "dwgui/dwplugineventdispatch.h"
#include "dwgui/dwpluginevent.h"
#include "dwgui/dwplugincontext.h"
#include "dwgui/dwpluginmapping.h"
#include "graphicsview/dwdummys.h"


//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwPluginLib, 'DPLB', DwRttiObject);

//------------------------------------------------------------------------------
/**
*/
DwPluginLib::DwPluginLib()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwPluginLib::~DwPluginLib()
{
    freeAllDesc();
}

//------------------------------------------------------------------------------
/**
*/
bool DwPluginLib::setup()
{
    //dwAssignLib()->setAssign(DwAssign("plugins", "home:plugins"));
    dwAssignLib()->setAssign(DwAssign("plugins", "bin:"));
    return true;
}

//------------------------------------------------------------------------------
/**
*/
int DwPluginLib::pluginsNum()
{
    return m_plugins.size();
}

//------------------------------------------------------------------------------
/**
*/
DwPlugin * DwPluginLib::pluginAt(int idx)const
{
    DW_ASSERT(idx >= 0 && idx < m_plugins.size());
    return m_plugins.at(idx);
}

//------------------------------------------------------------------------------
/**
*/
DwPlugin * DwPluginLib::pluginOf(int id)const
{
    DwMap<int , DwPlugin*>::const_iterator ite = m_id2plugins.find(id);
    if (ite != m_id2plugins.end())
    {
        return ite->second;
    }
    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
DwPlugin * DwPluginLib::pluginOf(const DwString& n)const
{
    DwMap<DwString, DwPlugin*>::const_iterator ite = m_name2plugins.find(n);
    if (ite != m_name2plugins.end())
    {
        return ite->second;
    }
    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
DwVector<DwPlugin *> DwPluginLib::pluginOf(DwRtti *kind)const
{
    DwVector<DwPlugin *> ps;
    for (int i=0; i<m_plugins.size(); ++i)
    {
        DwPlugin* p = m_plugins.at(i);
        if (p && p->isA(kind))
        {
            ps.append(p);
        }
    }
    return ps;
}

//------------------------------------------------------------------------------
/**
*/
const DwVector<DwPlugin*> DwPluginLib::plugins() const
{
    return m_plugins;
}

//------------------------------------------------------------------------------
/**
*/
bool DwPluginLib::loadPlugin(const DwString &name)
{
    DwUrl url("plugins:" + name + ".dll");
    return loadPluginByUrl(url);
}

//------------------------------------------------------------------------------
/**
*/
bool DwPluginLib::loadPluginByUrl(const DwUrl &url)
{
    return loadDescByUrl(url);
}

//------------------------------------------------------------------------------
/**
*/
bool DwPluginLib::unLoadPlugin(const DwString &name)
{
    DwUrl url("plugins:" + name + ".dll");
    return unLoadPluginByUrl(url);
}

//------------------------------------------------------------------------------
/**
*/
bool DwPluginLib::unLoadPluginByUrl(const DwUrl &url)
{
    return freeDescByUrl(url);
}

//------------------------------------------------------------------------------
/**
*/
DwPlugin* DwPluginLib::createPlugin(const DwString &name, const DwString & params)
{
    DwUrl url("plugins:" + name + ".dll");
    return createPluginByUrl(url, params);
}

//------------------------------------------------------------------------------
/**
*/
DwPlugin* DwPluginLib::createPluginByUrl(const DwUrl &url, const DwString & params)
{
    static int idx = 0;
    if (hasDesc(url))
    {
        const DwPluginDesc &desc = descOf(url);
        DwRttiObject *obj = desc.rtti->create();
        DwPlugin *plugin = dwsafe_cast<DwPlugin>(obj);
        if (plugin)
        {
            plugin->m_desc = desc;
            plugin->m_lib = this;
            plugin->m_name = url.localPath();
            plugin->m_id = ++idx;
            plugin->m_version = DwPluginVersion(desc.version);

            if (desc.mappingRtti)
            {
                DwRttiObject *obj = desc.mappingRtti->create();
                plugin->m_mapping = dwsafe_cast<DwPluginMapping>(obj);
                if (plugin->m_mapping)
                {
                    delete obj;
                }
            }

            if (desc.contextRtti)
            {
                DwRttiObject *obj = desc.contextRtti->create();
                plugin->m_context = dwsafe_cast<DwPluginContext>(obj);
                if (plugin->m_context)
                {
                    delete obj;
                }
            }

            if (m_name2plugins.contains(plugin->m_name))
            {
                plugin->m_name.append(plugin->m_id);
            }
            m_plugins.append(plugin);
            m_id2plugins[plugin->m_id] = plugin;
            m_name2plugins[plugin->m_name] = plugin;

            plugin->construct(params);

            DwPluginConstructEvent evt;
            evt.m_cast = Dw::BroadCast;
            evt.m_from = plugin;
            dispatchEvent(&evt);
            return plugin;
        }
        else
        {
            delete plugin;
        }
    }
    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
bool DwPluginLib::destroyPlugin(DwPlugin* p)
{
    DW_UNUSED(p);
    if (p)
    {
        m_plugins.removeOne(p);
        m_id2plugins.remove(p->id());
        m_name2plugins.remove(p->name());

        DwPluginDestructEvent evt;
        evt.m_cast = Dw::BroadCast;
        evt.m_from = p;
        dispatchEvent(&evt);
        p->destruct();
        DW_SAFE_DELETE(p);
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
void DwPluginLib::dispatchEvent(DwPluginEvent *evt)
{
    dwPluginEventDispatch()->dispatchEvent(this, evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwPluginLib::dispatchEvent(DwPlugin * plugin, DwPluginEvent *evt)
{
    dwPluginEventDispatch()->dispatchEvent(plugin, evt);
}

//------------------------------------------------------------------------------
/**
*/
bool DwPluginLib::hasDesc(const DwUrl& url) const
{
    return m_url2descs.contains(url.asString());
}

//------------------------------------------------------------------------------
/**
*/
bool DwPluginLib::addDesc(const DwUrl& url, const DwPluginDesc& desc)
{
    DW_ASSERT(!m_url2descs.contains(url.asString()));
    m_url2descs[url.asString()] = desc;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
const DwPluginDesc& DwPluginLib::descOf(const DwUrl& url) const
{
    DwMap<DwString, DwPluginDesc>::const_iterator ite = m_url2descs.find(url.asString());
    if (ite != m_url2descs.end())
    {
        return ite->second;
    }
    DW_ASSERT(false);
    return ite->second;
}

//------------------------------------------------------------------------------
/**
*/
const DwMap<DwString, DwPluginDesc>& DwPluginLib::descs() const
{
    return m_url2descs;
}

//------------------------------------------------------------------------------
/**
*/
void DwPluginLib::enumPlugins()
{
    DwUrl pluginsUrl("plugins:");
    DwVector<DwString> files = DwFsWrapper::listFiles(pluginsUrl.localPath(), "*.dll");

    if (files.size() > 0)
    {
        for (int i=0; i<files.size(); ++i)
        {
            DwString file = files.at(i);
            loadDesc(file);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
bool DwPluginLib::loadDesc(const DwString& file)
{
    DwUrl pluginUrl = "plugins:" + file;
    return loadDescByUrl(pluginUrl);
}

//------------------------------------------------------------------------------
/**
*/
bool DwPluginLib::loadDescByUrl(const DwUrl& pluginUrl)
{
    if ( m_url2descs.contains(pluginUrl.asString()) )
    {
        return false;
    }

    DwString pluginPath = pluginUrl.localPath();
    DwString file = pluginPath;
    int lastIndex = dwLastSlashIndex(pluginPath);
    if (lastIndex != InvalidIndex)
    {
        file = pluginPath.right(pluginPath.length() - lastIndex - 1);
    }

    HMODULE h = NULL;
    DwMap<DwString, HMODULE>::iterator ite = m_modules.find(pluginPath);
    if (ite != m_modules.end())
    {
        h = ite->second;
    }
    else
    {
        h = ::LoadLibrary(pluginPath.constString());
    }

    if (h)
    {
        DwString entryName = file.replace(".", "_").append("_desc");
        FnPluginDesc entry = (FnPluginDesc)GetProcAddress(h, entryName.toCap().data());
        if (entry)
        {
            DwPluginDesc desc;
            bool isOk = entry(this, &desc);
            if (isOk)
            {
                desc.url = pluginUrl;
                m_modules[pluginPath] = h;
                m_url2descs[pluginUrl.asString()] = desc;
                return true;
            }
        }
        else
        {
            ::FreeLibrary(h);
        }
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwPluginLib::freeDescByUrl(const DwUrl& url)
{
    DwMap<DwString, DwPluginDesc>::iterator ite = m_url2descs.find(url.asString());
    if (ite != m_url2descs.end())
    {
        DwPluginDesc desc = ite->second;
        return freeDesc(desc);
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool DwPluginLib::freeDesc(const DwPluginDesc& desc)
{
    DwVector<DwPlugin*> allPlugins = m_plugins;
    for (int i=0; i<allPlugins.size(); ++i)
    {
        DwPlugin* plugin = allPlugins.at(i);
        if (plugin && plugin->rtti() == desc.rtti)
        {
            destroyPlugin(plugin);
        }
    }

    DwString pluginPath = desc.url.localPath();
    DwMap<DwString, HMODULE>::iterator ite = m_modules.find(pluginPath);
    HMODULE h = NULL;
    if (ite != m_modules.end())
    {
        h = ite->second;
    }
    if (h)
    {
        return !!(::FreeLibrary(h));
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwPluginLib::freeAllDesc()
{
    DwMap<DwString, DwPluginDesc> allDescs = m_url2descs;
    DwMap<DwString, DwPluginDesc>::iterator ite = allDescs.begin();
    while (ite != allDescs.end())
    {
        freeDesc(ite->second);
        ++ite;
    }
}

//------------------------------------------------------------------------------
/**
*/
DW_GLOBAL_STATIC(DwPluginLib, _dw_PluginLib);
DwPluginLib* dwPluginLib()
{
    return _dw_PluginLib();
}