#include "dwpluginevent.h"
#include "graphicsview/dwdummys.h"
#include "dwpluginlib.h"
#include "dwplugin.h"

//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_EVENTID(DwPluginEvent, DwEvent);

//------------------------------------------------------------------------------
/**
*/
DwPluginEvent::DwPluginEvent()
    : m_cast(Dw::UniCast)
    , m_needOrdered(false)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwPluginEvent::~DwPluginEvent()
{
    m_donePlugin = NULL;
}

//------------------------------------------------------------------------------
/**
*/
const DwVector<DwPlugin*> DwPluginEvent::indexFromLib(DwPluginLib *lib)
{
    if (m_cast == Dw::BroadCast)
    {
        return lib->plugins();
    }

    DwVector<DwPlugin*> plugins;
    if (m_cast == Dw::MultiCast)
    {
        DwVector<DwString> addrs = dwTokenize(m_addr, L" ,", '"');
        if (addrs.size() > 0)
        {
            for (int i=0; i<addrs.size(); ++i)
            {
                DwPlugin* plugin = lib->pluginOf(addrs.at(i));
                if (plugin)
                {
                    plugins.append(plugin);
                }
            }
        }
    }
    else
    {
        DW_ASSERT(m_cast == Dw::UniCast);
        if (m_to)
        {
            plugins.append(m_to);
        }
        else
        {
            DwPlugin* plugin = lib->pluginOf(m_addr);
            if (plugin)
            {
                plugins.append(plugin);
            }
        }
    }
    return plugins;
}

//------------------------------------------------------------------------------
/**
*/
bool DwPluginEvent::isInterestBy(DwPlugin *p)const
{
    DW_UNUSED(p);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
int DwPluginEvent::priority(DwPlugin *p)
{
    DW_UNUSED(p);
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
const DwString &DwPluginEvent::content()const
{
    return m_content;
}

//------------------------------------------------------------------------------
/**
*/
const DwString &DwPluginEvent::result()const
{
    return m_result;
}

//------------------------------------------------------------------------------
/**
*/
bool DwPluginEvent::isNeedOrdered() const
{
    return m_needOrdered;
}

//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_EVENTID(DwPluginConstructEvent, DwPluginEvent);

//------------------------------------------------------------------------------
/**
*/
bool DwPluginConstructEvent::isInterestBy(DwPlugin *plugin)const
{
    if (plugin && plugin != m_from)
    {
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_EVENTID(DwPluginDestructEvent, DwPluginEvent);

//------------------------------------------------------------------------------
/**
*/
bool DwPluginDestructEvent::isInterestBy(DwPlugin *plugin)const
{
    if (plugin && plugin != m_from)
    {
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////////
DW_IMPLEMENT_EVENTID(DwPluginInvokeEvent, DwPluginEvent);

//------------------------------------------------------------------------------
/**
*/
bool DwPluginInvokeEvent::isInterestBy( DwPlugin * ) const
{
    return true;
}
