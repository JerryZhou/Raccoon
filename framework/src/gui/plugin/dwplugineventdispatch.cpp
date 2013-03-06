#include "stable.h"
#include "dwplugineventdispatch.h"
#include "dwplugin.h"
#include "dwpluginevent.h"
#include "graphicsview/dwdummys.h"

//------------------------------------------------------------------------------
struct DwPluginEventCompareGreaterThan
{
    DwPluginEventCompareGreaterThan(DwPluginEvent *evt)
        : m_evt(evt)
    {
        DW_ASSERT(m_evt);
    }
    bool operator () (DwPlugin* lfs, DwPlugin *rfs)
    {
        int lfsOrder = m_evt->priority(lfs);
        int rfsOrder = m_evt->priority(rfs);
        return lfsOrder > rfsOrder;
    }
    DwPluginEvent *m_evt;
};

//------------------------------------------------------------------------------
/**
*/
#include <algorithm>
#define dwSort std::sort
void DwPluginEventDispatch::dispatchEvent(DwPluginLib *lib, DwPluginEvent *evt)
{
    DwVector<DwPlugin*> plugins = evt->indexFromLib(lib);
    if (plugins.size() > 0)
    {
        if (plugins.size() > 1 && evt->isNeedOrdered())
        {
            dwSort(plugins.begin(), plugins.end(), DwPluginEventCompareGreaterThan(evt));
        }
        for (int i=0; i<plugins.size(); ++i)
        {
            DwPlugin *plugin = plugins.at(i);
            if (evt->isInterestBy(plugin))
            {
                dispatchEvent(plugin, evt);
            }

            if (evt->isAccepted())
            {
                evt->m_donePlugin = plugin;
                break;
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwPluginEventDispatch::dispatchEvent(DwPlugin * plugin, DwPluginEvent *evt)
{
    DW_CHECK(plugin);
    DW_CHECK(evt);
    plugin->onEventEn(evt);
}

//------------------------------------------------------------------------------
/**
*/
DW_GLOBAL_STATIC(DwPluginEventDispatch, _dw_PluginEventDispatch);
DwPluginEventDispatch *dwPluginEventDispatch()
{
    return _dw_PluginEventDispatch();
}
