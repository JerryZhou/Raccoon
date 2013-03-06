#pragma once
#include "dwgui/dwrttiobject.h"
#include "dwgui/dwplugindesc.h"
#include "dwgui/dwpluginpropertys.h"
#include "dwgui/dwpluginversion.h"
#include "dwgui/dwpluginevent.h"

//------------------------------------------------------------------------------
class DwPluginLib;
class DwPluginEvent;
class DwPluginMapping;
class DwPluginContext;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwPlugin : public DwRttiObject
{
    DW_DECLARE_CLASS(DwPlugin);
public:
    DwPlugin();
    virtual ~DwPlugin();

    virtual bool construct(const DwString &params);

    virtual bool destruct();

    virtual bool isInterestBy(DwPluginEvent* evt) const;

    virtual void onEvent(DwPluginEvent *evt);

    const DwPluginVersion& version() const;
    DwPluginLib* lib()const;
    const DwPluginDesc& desc() const;
    const DwString& name() const;
    int id() const;
    DwPluginContext* context() const;
    DwPluginMapping* mapping() const;

    bool isAccept(DwEventId* evt) const;
    void accept(DwEventId* evt, bool batched = false);
    void refuse(DwEventId* evt, bool batched = false);

    const DwVector<DwEventId*>& acceptEventIds()const;
    const DwVector<DwEventId*> acceptBatchIds() const;

protected:
    void onEventEn(DwPluginEvent *evt);
    virtual void onInvokeEvent(DwPluginInvokeEvent *evt);
    virtual void onDestructEvent(DwPluginDestructEvent *evt);
    virtual void onConstructEvent(DwPluginConstructEvent *evt);

    int m_id;
    DwString m_name;
    DwPluginDesc m_desc;
    DwPluginLib *m_lib;
    DwPluginContext *m_context;
    DwPluginMapping *m_mapping;
    DwPluginVersion m_version;
    DwVector<DwEventId*> m_acceptEventIds;
    DwVector<DwEventId*> m_acceptBatchIds;
private:
    friend class DwPluginLib;
    friend class DwPluginEventDispatch;

    DW_PLUGIN_DECLARE_PROPERTY(DwPlugin);
    DW_PLUGIN_ROOT_PROPERTY_BEGIN(DwPlugin);
    DW_PLUGIN_PROPERTY_END();
};// end of DwPlugin
DW_REGISTER_CLASS(DwPlugin);

//------------------------------------------------------------------------------
typedef bool (*FnPluginDesc)(DwPluginLib* lib, DwPluginDesc *desc);