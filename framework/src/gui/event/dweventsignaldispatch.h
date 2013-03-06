#pragma once

#include "dwgui/dwevent.h"
#include "dwgui/dwrttiobject.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwEventSignalDispatch : public DwRttiObject
{
    DW_DECLARE_CLASS(DwEventSignalDispatch);
    DW_DISABLE_COPY(DwEventSignalDispatch);
public:
    explicit DwEventSignalDispatch(DwRttiObject *obj = NULL);
    virtual ~DwEventSignalDispatch();

    bool dispatchEvent(DwRttiObject *obj, DwEvent* evt) const;

    template<class dest_type>
    void connect(DwEventId * evtId, dest_type* pclass, void (dest_type::*pmemfun)(DwRttiObject *, DwEvent* evt));

    template<class dest_type>
    void disconnect(DwEventId * evtId, dest_type* pclass);

    // when dump the objects , will output related information
    virtual void debugString(DwDebug &stream)const;

protected:
    typedef sigslot::signal2<DwRttiObject *, DwEvent* > EventSignal;
    typedef DwMap<DwEventId *, EventSignal* > EventSignalMap;

    DwRttiObject *m_obj;
    EventSignalMap m_eventSignals;
};// end of DwEventSignalDispatch
DW_REGISTER_CLASS(DwEventSignalDispatch);

//------------------------------------------------------------------------------
/**
*/
template<class dest_type>
inline void DwEventSignalDispatch::connect(DwEventId * evtId,
        dest_type* pclass, void (dest_type::*pmemfun)(DwRttiObject *, DwEvent* evt))
{
    EventSignalMap::iterator ite = m_eventSignals.find(evtId);
    if (ite != m_eventSignals.end())
    {
        EventSignal *signal = ite->second;
        signal->connect(pclass, pmemfun);
    }
    else
    {
        EventSignal *signal = new EventSignal();
        m_eventSignals.insert(evtId, signal);
        signal->connect(pclass, pmemfun);
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class dest_type>
inline void DwEventSignalDispatch::disconnect(DwEventId * evtId, dest_type* pclass)
{
    EventSignalMap::iterator ite = m_eventSignals.find(evtId);
    if (ite != m_eventSignals.end())
    {
        EventSignal *signal = ite->second;
        signal->disconnect(pclass);
    }
}