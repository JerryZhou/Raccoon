#pragma once

#include "dwapp/dwmessage.h"
#include "dwapp/dwhandler.h"
#include "dwapp/dwhandlerthreadbase.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwAsyncPort : public DwRefCounted
{
    DW_DECLARE_CLASS(DwAsyncPort);
public:
    /// constructor
    DwAsyncPort();
    /// destructor
    virtual ~DwAsyncPort();
    
    /// set pointer to handler thread object (must be derived from HandlerThreadBase)
    void setHandlerThread(const DwPtr<DwHandlerThreadBase>& t);
    /// get pointer to handler thread object
    const DwPtr<DwHandlerThreadBase>& handlerThread() const;
        
    /// attach a handler to the port, may be called before or after Open()
    virtual void attachHandler(const DwPtr<DwHandler>& h);
    /// dynamically remove a handler from the port
    virtual void removeHandler(const DwPtr<DwHandler>& h);

    /// open the async port
    virtual void open();
    /// close the async port
    virtual void close();
    /// return true if port is open
    bool isOpen() const;

    /// send an asynchronous message to the port
    template<class MESSAGETYPE> void send(const DwPtr<MESSAGETYPE>& msg);
    /// send a message and wait for completion
    template<class MESSAGETYPE> void sendWait(const DwPtr<MESSAGETYPE>& msg);
    /// wait for a message to be handled
    template<class MESSAGETYPE> void wait(const DwPtr<MESSAGETYPE>& msg);
    /// peek a message whether it has been handled
    template<class MESSAGETYPE> bool peek(const DwPtr<MESSAGETYPE>& msg);
    /// cancel a pending message
    template<class MESSAGETYPE> void cancel(const DwPtr<MESSAGETYPE>& msg);

private:
    /// send an asynchronous message to the port
    void sendInternal(const DwPtr<DwMessage>& msg);
    /// send a message and wait for completion
    void sendWaitInternal(const DwPtr<DwMessage>& msg);
    /// wait for a message to be handled
    void waitInternal(const DwPtr<DwMessage>& msg);
    /// peek a message whether it has been handled
    bool peekInternal(const DwPtr<DwMessage>& msg);
    /// cancel a pending message
    void cancelInternal(const DwPtr<DwMessage>& msg);

private:
    /// clear all attached message handlers
    void ClearHandlers();

    DwPtr<DwHandlerThreadBase> m_thread;
    bool m_isOpen;
};

//------------------------------------------------------------------------------
/**
*/
inline void
DwAsyncPort::setHandlerThread(const DwPtr<DwHandlerThreadBase>& h)
{
    DW_ASSERT(!this->isOpen());
    this->m_thread = h;
}

//------------------------------------------------------------------------------
/**
*/
inline const DwPtr<DwHandlerThreadBase>&
DwAsyncPort::handlerThread() const
{
    return this->m_thread;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
DwAsyncPort::isOpen() const
{
    return this->m_isOpen;
}

//------------------------------------------------------------------------------
/**
*/
template<class MESSAGETYPE> inline void 
DwAsyncPort::send(const DwPtr<MESSAGETYPE>& msg)
{
    this->sendInternal((const DwPtr<DwMessage>&)msg);
}

//------------------------------------------------------------------------------
/**
*/
template<class MESSAGETYPE> inline void 
DwAsyncPort::sendWait(const DwPtr<MESSAGETYPE>& msg)
{
    this->sendWaitInternal((const DwPtr<DwMessage>&)msg);
}

//------------------------------------------------------------------------------
/**
*/
template<class MESSAGETYPE> inline void 
DwAsyncPort::wait(const DwPtr<MESSAGETYPE>& msg)
{
    this->waitInternal((const DwPtr<DwMessage>&)msg);
}

//------------------------------------------------------------------------------
/**
*/
template<class MESSAGETYPE> inline bool 
DwAsyncPort::peek(const DwPtr<MESSAGETYPE>& msg)
{
    return this->peekInternal((const DwPtr<DwMessage>&)msg);
}

//------------------------------------------------------------------------------
/**
*/
template<class MESSAGETYPE> inline void 
DwAsyncPort::cancel(const DwPtr<MESSAGETYPE>& msg)
{
    this->cancelInternal((const DwPtr<DwMessage>&)msg);
}
