#include "stable.h"
#include "dweventsignaldispatch.h"

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwEventSignalDispatch, 'ESDH', DwRttiObject);

//------------------------------------------------------------------------------
/**
*/
DwEventSignalDispatch::DwEventSignalDispatch(DwRttiObject *obj/* = NULL*/)
    : m_obj(obj)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwEventSignalDispatch::~DwEventSignalDispatch()
{
    EventSignalMap::iterator ite = m_eventSignals.begin();
    while(ite != m_eventSignals.end())
    {
        DW_SAFE_DELETE(ite->second);
        ++ite;
    }
    m_eventSignals.clear();
}

//------------------------------------------------------------------------------
/**
*/
bool DwEventSignalDispatch::dispatchEvent(DwRttiObject *obj, DwEvent* evt) const
{
    EventSignalMap::const_iterator ite = m_eventSignals.find(evt->eventId());
    if (ite != m_eventSignals.end())
    {
        ite->second->emit(obj, evt);

        return evt->isAccepted();
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void DwEventSignalDispatch::debugString(DwDebug &stream)const
{
    stream <<" " << this->rtti()->name() << "["
           <<" event signals:" << m_eventSignals.count();

#ifdef _DEBUG
    EventSignalMap::const_iterator ite = m_eventSignals.begin();
    while (ite != m_eventSignals.end())
    {
        DwEventId * evtId = ite->first;
        stream << " Event: "<< evtId->name();
        ++ite;
    }
#endif
    stream << "]";
}