#pragma once
#include "dwgui/dwevent.h"
#include "dwgui/dweventid.h"
#include "dwcore/dwvector.h"
#include "dwgui/dwrttiobjectptr.h"
#include "dwgui/dwpluginenum.h"

//------------------------------------------------------------------------------
class DwPlugin;
class DwPluginLib;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwPluginEvent : public DwEvent
{
    DW_DECLARE_EVENTID(DwPluginEvent);
public:
    DwPluginEvent();
    virtual ~DwPluginEvent();

    // index a event accept plugin
    virtual const DwVector<DwPlugin*> indexFromLib(DwPluginLib *lib);
    // is plugin meet the condition
    virtual bool isInterestBy(DwPlugin *plugin)const;
    // event order
    virtual int priority(DwPlugin *p);

    // content and result
    const DwString &content()const;
    const DwString &result()const;

    // event flag
    bool isNeedOrdered() const;

    // dispatch stuffs
    Dw::EventCast m_cast;
    DwString m_addr;
    bool m_needOrdered;

    // event contains
    DwRttiObjectPtr<DwPlugin> m_from;
    DwRttiObjectPtr<DwPlugin> m_to;
    DwRttiObjectPtr<DwPlugin> m_donePlugin;
    DwString m_content;
    DwString m_result;
};// end of DwPluginEvent
DW_REGISTER_CLASS(DwPluginEvent);

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwPluginConstructEvent : public DwPluginEvent
{
    DW_DECLARE_EVENTID(DwPluginConstructEvent);
public:
    // is plugin meet the condition
    virtual bool isInterestBy(DwPlugin *plugin)const;
protected:
private:
};
DW_REGISTER_CLASS(DwPluginConstructEvent);

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwPluginDestructEvent : public DwPluginEvent
{
    DW_DECLARE_EVENTID(DwPluginDestructEvent);
public:
    // is plugin meet the condition
    virtual bool isInterestBy(DwPlugin *plugin)const;
protected:
private:
};
DW_REGISTER_CLASS(DwPluginDestructEvent);

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwPluginInvokeEvent : public DwPluginEvent
{
    DW_DECLARE_EVENTID(DwPluginInvokeEvent);
public:
    // is plugin meet the condition
    virtual bool isInterestBy(DwPlugin *plugin)const;
protected:
private:
};
DW_REGISTER_CLASS(DwPluginInvokeEvent);