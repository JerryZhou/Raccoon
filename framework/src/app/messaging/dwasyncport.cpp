
#include "stable.h"
#include "dwasyncport.h"

DW_IMPLEMENT_CLASS(DwAsyncPort, 'ASPT', DwRefCounted);


//------------------------------------------------------------------------------
/**
*/
DwAsyncPort::DwAsyncPort() :
    m_isOpen(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
DwAsyncPort::~DwAsyncPort()
{
    if (this->isOpen())
    {
        this->close();
    }
    this->m_thread = 0;
}

//------------------------------------------------------------------------------
/**
    Open the async port. The async port needs a valid name before it
    is opened. Messages can only be sent to an open port.
*/
void
DwAsyncPort::open()
{
    DW_ASSERT(!this->isOpen());
    DW_ASSERT(this->m_thread.isvalid());

    // start the handler thread, and wait until handlers are open
    this->m_thread->start();
    this->m_thread->waitForHandlersOpened();

    this->m_isOpen = true;
}

//------------------------------------------------------------------------------
/**
    Closes the async port.
*/
void
DwAsyncPort::close()
{
    DW_ASSERT(this->isOpen());
    this->m_thread->stop();
    this->m_thread->clearHandlers();
    this->m_isOpen = false;
}

//------------------------------------------------------------------------------
/**
    Add a message handler, this can either be called before the handler
    thread is started, or any time afterwards.
*/
void
DwAsyncPort::attachHandler(const DwPtr<DwHandler>& h)
{
    this->m_thread->attachHandler(h);
}

//------------------------------------------------------------------------------
/**
    Dynamically remove a message handler.
*/
void
DwAsyncPort::removeHandler(const DwPtr<DwHandler>& h)
{
    this->m_thread->removeHandler(h);
}

//------------------------------------------------------------------------------
/**
    Handle an asynchronous message and return immediately. If the caller
    expects any results from the message he can poll with the AsyncPort::Peek()
    method, or he may wait for the message to be handled with the 
    AsyncPort::Wait() method.
*/
void
DwAsyncPort::sendInternal(const DwPtr<DwMessage>& msg)
{
    DW_ASSERT(msg.isvalid());
    DW_ASSERT(this->m_thread.isvalid());
    DW_ASSERT(!msg->handled());
    this->m_thread->addMessage(msg);
}

//------------------------------------------------------------------------------
/**
    Send an asynchronous message and wait until the message has been
    handled.
*/
void
DwAsyncPort::sendWaitInternal(const DwPtr<DwMessage>& msg)
{
    this->send(msg);
    this->wait(msg);
}

//------------------------------------------------------------------------------
/**
    This method peeks whether a message has already been handled. If the
    caller expects any return arguments from the message handling it 
    can use this message to check whether the results are ready using
    this non-blocking method. The caller can also wait for the results
    to become ready using the Wait() method.
*/
bool
DwAsyncPort::peekInternal(const DwPtr<DwMessage>& msg)
{
    DW_ASSERT(msg.isvalid());
    return msg->handled();
}

//------------------------------------------------------------------------------
/**
    This method will cancel a pending message.
*/
void
DwAsyncPort::cancelInternal(const DwPtr<DwMessage>& msg)
{
    DW_ASSERT(msg.isvalid());
    DW_ASSERT(this->m_thread.isvalid());
    this->m_thread->cancelMessage(msg);
}

//------------------------------------------------------------------------------
/**
    This method will wait until a message has been handled. If the caller
    expects any return arguments from the message handling it can use
    this method to wait for the results.
*/
void
DwAsyncPort::waitInternal(const DwPtr<DwMessage>& msg)
{
    DW_ASSERT(msg.isvalid());
    DW_ASSERT(this->m_thread.isvalid());

    // only wait if the message hasn't been handled yet
    if (!msg->handled())
    {
        this->m_thread->waitForMessage(msg);
        DW_ASSERT(msg->handled());
    }
}
