#pragma once

#include "dwapp/dwthread.h"
#include "dwapp/dwsyncevent.h"
#include "dwcore/dwlocks.h"
#include "dwapp/dwhandler.h"
#include "dwapp/dwmessage.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwHandlerThreadBase : public DwThread
{
    DW_DECLARE_CLASS(DwHandlerThreadBase);
public:
    /// constructor
    DwHandlerThreadBase();
    
    /// attach a message handler
    void attachHandler(const DwPtr<DwHandler>& h);
    /// dynamically remove a message handler
    void removeHandler(const DwPtr<DwHandler>& h);
    /// clear all attached message handlers
    void clearHandlers();
    /// wait until handlers have been opened
    void waitForHandlersOpened();

    /// add a message to be handled (override in subclass!)
    virtual void addMessage(const DwPtr<DwMessage>& msg);
    /// cancel a pending message (override in subclass!)
    virtual void cancelMessage(const DwPtr<DwMessage>& msg);
    /// wait for message to be handled  (optionally override in subclass!)
    virtual void waitForMessage(const DwPtr<DwMessage>& msg);

protected:
    /// open message handlers
    void threadOpenHandlers();
    /// close message handlers
    void threadCloseHandlers();
    /// open dynamically added handlers, and call DoWork() on all attached handlers
    void threadUpdateHandlers();
    /// update deferred messages, return true if at least one message has been handled
    bool threadUpdateDeferredMessages();
    /// clear leftover deferred messages
    void threadDiscardDeferredMessages();
    /// handle messages in array, return true if at least one message has been handled
    bool threadHandleMessages(const DwVector<DwPtr<DwMessage> >& msgArray);
    /// handle a single message without deferred support, return if message has been handled
    bool threadHandleSingleMessage(const DwPtr<DwMessage>& msg);
    /// signal message handled event (only call if at least one message has been handled)
    void threadSignalMessageHandled();

    DwSyncEvent m_msgHandledEvent;
    DwSyncEvent m_handlersOpenedEvent;
    DwCriticalSection m_handlersCritSect;
    DwVector<DwPtr<DwHandler> > m_handlers;
    DwVector<DwPtr<DwMessage> > m_deferredMessages;
};// end of DwHandlerThreadBase
