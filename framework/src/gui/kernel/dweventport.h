#pragma once
#include "dwcore/dwvector.h"

//------------------------------------------------------------------------------
class DwEvent;
class DwEventId;
class DwEventHandler;

// TODO: xx
//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwEventPort
{
public:
    DwEventPort();
    virtual ~DwEventPort();

    void sendEvent(DwEvent *evt);

    void attachHandler(DwEventHandler* handler);

    void detachHandler(DwEventHandler* handler);

    bool isAcceptEvent(DwEvent *evt) const;

protected:
    friend class DwEventHandler;

    void registerEvent(DwEventId* id, DwEventHandler* handler);

    void unRegisterEvent(DwEventId* id, DwEventHandler* handler);

private:
    DwMap<DwEventId*, DwVector<int> > m_eventHandlerMap;
    DwVector<DwEventHandler *> m_handlers;
};// end of DwEventPort