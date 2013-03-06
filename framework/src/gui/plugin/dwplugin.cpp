#include "stable.h"
#include "dwplugin.h"
#include "dwgui/dwplugincontext.h"
#include "dwgui/dwpluginmapping.h"
#include "graphicsview/dwdummys.h"

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwPlugin, 'DPIN', DwRttiObject);
DW_PLUGIN_IMPLEMENT_PROPERTY(DwPlugin);

//------------------------------------------------------------------------------
/**
*/
DwPlugin::DwPlugin()
    : m_lib(NULL)
    , m_context(NULL)
    , m_mapping(NULL)
    , m_id(0)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwPlugin::~DwPlugin()
{
    m_lib = NULL;
    DW_SAFE_DELETE(m_context);
    DW_SAFE_DELETE(m_mapping);
}

//------------------------------------------------------------------------------
/**
*/
bool DwPlugin::construct(const DwString &params)
{
    DW_ASSERT(m_lib);
    DW_UNUSED(params);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwPlugin::destruct()
{
    DW_ASSERT(m_lib);
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void DwPlugin::onEvent(DwPluginEvent *evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwPlugin::onInvokeEvent(DwPluginInvokeEvent *evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwPlugin::onDestructEvent(DwPluginDestructEvent *evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwPlugin::onConstructEvent(DwPluginConstructEvent *evt)
{
    DW_UNUSED(evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwPlugin::onEventEn(DwPluginEvent *evt)
{
    DW_EVENT_BEGIN();
    DW_EVENT_DISPATCH(evt, DwPluginInvokeEvent, onInvokeEvent);
    DW_EVENT_DISPATCH(evt, DwPluginDestructEvent, onDestructEvent);
    DW_EVENT_DISPATCH(evt, DwPluginConstructEvent, onConstructEvent);
    DW_EVENT_END();
    DW_EVENT_CHECK(evt);

    onEvent(evt);
}

//------------------------------------------------------------------------------
/**
*/
DwPluginLib* DwPlugin::lib()const
{
    return m_lib;
}

//------------------------------------------------------------------------------
/**
*/
const DwPluginVersion& DwPlugin::version() const
{
    return m_version;
}

//------------------------------------------------------------------------------
/**
*/
const DwPluginDesc& DwPlugin::desc() const
{
    return m_desc;
}

//------------------------------------------------------------------------------
/**
*/
const DwString& DwPlugin::name() const
{
    return m_name;
}

//------------------------------------------------------------------------------
/**
*/
int DwPlugin::id() const
{
    return m_id;
}

//------------------------------------------------------------------------------
/**
*/
DwPluginContext* DwPlugin::context() const
{
    return m_context;
}

//------------------------------------------------------------------------------
/**
*/
DwPluginMapping* DwPlugin::mapping() const
{
    return m_mapping;
}

//------------------------------------------------------------------------------
/**
*/
void DwPlugin::accept(DwEventId* evt, bool batched/* = false*/)
{
    if (batched)
    {
        int idx = m_acceptBatchIds.indexOf(evt);
        if (idx == InvalidIndex)
        {
            m_acceptBatchIds.append(evt);
        }
    }
    else
    {
        int idx = m_acceptEventIds.indexOf(evt);
        if (idx == InvalidIndex)
        {
            m_acceptEventIds.append(evt);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwPlugin::refuse(DwEventId* evt, bool batched/* = false*/)
{
    if (batched)
    {
        int idx = m_acceptBatchIds.indexOf(evt);
        if (idx != InvalidIndex)
        {
            m_acceptBatchIds.remove(idx);
        }
    }
    else
    {
        int idx = m_acceptEventIds.indexOf(evt);
        if (idx != InvalidIndex)
        {
            m_acceptEventIds.remove(idx);
        }
    }
}

bool DwPlugin::isAccept(DwEventId* evt) const
{
    if (m_acceptEventIds.size() > 0)
    {
        int idx = m_acceptBatchIds.indexOf(evt);
        if (idx != InvalidIndex)
        {
            return true;
        }
    }

    if (m_acceptBatchIds.size() > 0)
    {
        for (int i=0; i<m_acceptBatchIds.size(); ++i)
        {
            if (evt->isDerivedFrom(m_acceptBatchIds.at(i)))
            {
                return true;
            }
        }
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
const DwVector<DwEventId*>& DwPlugin::acceptEventIds()const
{
    return m_acceptEventIds;
}

//------------------------------------------------------------------------------
/**
*/
const DwVector<DwEventId*> DwPlugin::acceptBatchIds() const
{
    return m_acceptBatchIds;
}

//------------------------------------------------------------------------------
/**
*/
bool DwPlugin::isInterestBy(DwPluginEvent* evt) const
{
    DW_CHECK_RET(evt, false);
    if (evt->m_cast == Dw::UniCast)
    {
        return true;
    }

    return isAccept(evt->eventId());
}