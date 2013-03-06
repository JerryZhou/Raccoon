#pragma once
#include "dwgui/dwrttiobject.h"
#include "dwgui/dwrttiobjectptr.h"
#include "dwcore/dwvector.h"
#include "dwgui/dwurl.h"
#include "dwgui/dwplugindesc.h"

//------------------------------------------------------------------------------
class DwPlugin;
class DwPluginEvent;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwPluginLib : public DwRttiObject
{
    DW_DECLARE_CLASS(DwPluginLib);
public:
    DwPluginLib();
    virtual ~DwPluginLib();

    // setup
    bool setup();

    // Plugin Manage Stuffs
    int pluginsNum();
    DwPlugin * pluginAt(int idx)const;
    DwPlugin * pluginOf(int id)const;
    DwPlugin * pluginOf(const DwString& n)const;
    DwVector<DwPlugin *> pluginOf(DwRtti *kind)const;
    const DwVector<DwPlugin*> plugins() const;

    // Load, UnLoad Stuffs
    bool loadPlugin(const DwString &name);
    bool loadPluginByUrl(const DwUrl &url);
    bool unLoadPlugin(const DwString &name);
    bool unLoadPluginByUrl(const DwUrl &url);

    // create and destroy
    DwPlugin* createPlugin(const DwString &name, const DwString & params);
    DwPlugin* createPluginByUrl(const DwUrl &url, const DwString & params);
    bool destroyPlugin(DwPlugin* p);

    // Event Stuffs
    void dispatchEvent(DwPluginEvent *evt);
    void dispatchEvent(DwPlugin * plugin, DwPluginEvent *evt);

    // Container
    bool hasDesc(const DwUrl& url) const;
    bool addDesc(const DwUrl& url, const DwPluginDesc& desc);
    const DwPluginDesc& descOf(const DwUrl& url) const;
    const DwMap<DwString, DwPluginDesc>& descs() const;

    bool loadDesc(const DwString& file);
    bool loadDescByUrl(const DwUrl& url);
    bool freeDescByUrl(const DwUrl& url);
    bool freeDesc(const DwPluginDesc& desc);
    void freeAllDesc();
    void enumPlugins();

protected:
    DwVector<DwPlugin*> m_plugins;
    DwMap<int , DwPlugin*> m_id2plugins;
    DwMap<DwString, DwPlugin*> m_name2plugins;
    DwMap<DwString, DwPluginDesc> m_url2descs;
    DwMap<DwString, HMODULE> m_modules;
};// end of DwPluginLib

//------------------------------------------------------------------------------
DW_GUI_EXPORT DwPluginLib* dwPluginLib();