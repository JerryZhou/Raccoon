
#include "stable.h"
#include "dwhandlerthreadbase.h"
#include "dwapp/dwutil.h"

DW_IMPLEMENT_CLASS(DwHandlerThreadBase, 'HTBS', DwThread);


//------------------------------------------------------------------------------
/**
*/
DwHandlerThreadBase::DwHandlerThreadBase()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    Attach a message handler to the port. This method may be called from
    any thread.
*/
void
DwHandlerThreadBase::attachHandler(const DwPtr<DwHandler>& h)
{
    DW_ASSERT(h.isvalid());    
    this->m_handlersCritSect.lock();    
    DW_ASSERT(InvalidIndex == this->m_handlers.indexOf(h));
    this->m_handlers.append(h);
    this->m_handlersCritSect.unlock();
}

//------------------------------------------------------------------------------
/**
    Remove a message handler. This method may be called form any thread.
*/
void
DwHandlerThreadBase::removeHandler(const DwPtr<DwHandler>& h)
{
    DW_ASSERT(h.isvalid());

    this->m_handlersCritSect.lock();    
    int index = this->m_handlers.indexOf(h);
    this->m_handlers.remove(index);
    this->m_handlersCritSect.unlock();
}

//------------------------------------------------------------------------------
/**
    This clears all attached message handlers.
*/
void
DwHandlerThreadBase::clearHandlers()
{
    this->m_handlersCritSect.lock();
    this->m_handlers.clear();
    this->m_handlersCritSect.unlock();
}

//------------------------------------------------------------------------------
/**
    Wait on the handlers-opened event (will be signalled by the 
    ThreadOpenHandlers method.
*/
void
DwHandlerThreadBase::waitForHandlersOpened()
{
    DW_ASSERT(this->isRunning());
    this->m_handlersOpenedEvent.wait();
}

//------------------------------------------------------------------------------
/**
    This adds a new message to the thread's message queue. In the base class
    this message is empty and must be implemented in a subclass.
*/
void
DwHandlerThreadBase::addMessage(const DwPtr<DwMessage>& )
{
    // empty, override in subclass!
}

//------------------------------------------------------------------------------
/**
    This removes a message from the thread's message queue, regardless
    of its state. Override this method in a subclass.
*/
void
DwHandlerThreadBase::cancelMessage(const DwPtr<DwMessage>& )
{
    // empty, override in subclass!
}

//------------------------------------------------------------------------------
/**
    This waits until the message given as argument has been handled. In 
    order for this message to work, the ThreadSignalMessageHandled() must
    be called from within the handler thread context. Note that 
    subclasses may override this method if they need to.
*/
void
DwHandlerThreadBase::waitForMessage(const DwPtr<DwMessage>& msg)
{
    while (!msg->handled())
    {
        this->m_msgHandledEvent.wait();
    }
}

//------------------------------------------------------------------------------
/**
    Open attached message handlers. This method must be called at the
    start of the handler thread.
*/
void
DwHandlerThreadBase::threadOpenHandlers()
{
    this->m_handlersCritSect.lock();
    DwVector<DwPtr<DwHandler> >::iterator iter;
    for (iter = this->m_handlers.begin(); iter < this->m_handlers.end(); iter++)
    {
        (*iter)->open();
    }
    this->m_handlersCritSect.unlock();
    this->m_handlersOpenedEvent.signal();
}

//------------------------------------------------------------------------------
/**
    Close attached message handlers. This method must be called right
    before the handler thread shuts down.
*/
void
DwHandlerThreadBase::threadCloseHandlers()
{
    this->m_handlersCritSect.lock();
    DwVector<DwPtr<DwHandler> >::iterator iter;
    for (iter = this->m_handlers.begin(); iter < this->m_handlers.end(); iter++)
    {
        (*iter)->close();
    }
    this->m_handlersCritSect.unlock();
}

//------------------------------------------------------------------------------
/**
    Do per-frame update of attached handlers. This will open handlers
    which have been added late, and call the DoWork() method on 
    handlers from within the thread context.
*/
void
DwHandlerThreadBase::threadUpdateHandlers()
{
    this->m_handlersCritSect.lock();
    DwVector<DwPtr<DwHandler> >::iterator iter;
    for (iter = this->m_handlers.begin(); iter < this->m_handlers.end(); iter++)
    {
        // make sure the handler is open, it may happen that handlers
        // are added as the result of a message, and such a handler
        // may not have been opened yet
        if (!(*iter)->isOpen())
        {
            (*iter)->open();
        }
        (*iter)->doWork();
    }
    this->m_handlersCritSect.unlock();
}

//------------------------------------------------------------------------------
/**
    This checks every message in the deferred message array whether it
    has been handled yet, if yes, the message's actual handled flag
    will be set, and the message will be removed from the deferred
    handled array. If at least one message has been handled, the method
    will return true, if no message has been handled, the method returns
    false. If message have been handled, don't forget to call
    ThreadSignalMessageHandled() later!
*/
bool
DwHandlerThreadBase::threadUpdateDeferredMessages()
{
    bool msgHandled = false;
    int deferredIndex;
    for (deferredIndex = this->m_deferredMessages.size() - 1; deferredIndex != InvalidIndex; deferredIndex--)
    {
        DwPtr<DwMessage> msg = this->m_deferredMessages[deferredIndex];
        if (msg->deferredHandled())
        {
            this->m_deferredMessages.remove(deferredIndex);
            msg->setHandled(true);
            msgHandled = true;
        }
    }
    return msgHandled;
}

//------------------------------------------------------------------------------
/**
    This clears any leftover deferred messages. Call right before shutdown
    of the handler thread.
*/
void
DwHandlerThreadBase::threadDiscardDeferredMessages()
{
    this->m_deferredMessages.clear();
}

//------------------------------------------------------------------------------
/**
    Handle all message in the provided message array. Supports batched
    and deferred messages. Calls ThreadHandleSingleMessage(). If at least
    one message has been handled, the method returns true.
*/
bool
DwHandlerThreadBase::threadHandleMessages(const DwVector<DwPtr<DwMessage> >& msgArray)
{
    this->m_handlersCritSect.lock();

    bool msgHandled = false;
    int msgIdx;
    for (msgIdx = 0; msgIdx < msgArray.size(); ++msgIdx)
    {
        const DwPtr<DwMessage>& msg = msgArray[msgIdx];
        // special case: batch message?
        // a normal message
        msgHandled |= this->threadHandleSingleMessage(msg);
    }
    this->m_handlersCritSect.unlock();
    return msgHandled;
}

//------------------------------------------------------------------------------
/**
    Handle a single message, called by ThreadHandleMessages(). Return true
    if message has been handled.
    This method MUST be called from ThreadHandleMessages(), since this
    method will not explicitely take the handlers array critical section.
*/
bool
DwHandlerThreadBase::threadHandleSingleMessage(const DwPtr<DwMessage>& msg)
{
    bool msgHandled = false;

    // let each handler look at the message
    int i;
    for (i = 0; i < this->m_handlers.size(); i++)
    {    
        const DwPtr<DwHandler>& curHandler = this->m_handlers[i];
        
        // open handler on demand (a handler may have
        // been added as the result of a message call,
        // so let's make sure it will be able to handle 
        // message as soon as possible
        if (!curHandler->isOpen())
        {
            curHandler->open();
        }
    
        // let handler handle the current message
        if (curHandler->handleMessage(msg))
        {
            // message has been accepted by this handler
            if (msg->isDeferred())
            {
                // message has been defered by the handler
                // and will be handled at some later time,
                // put the message into the deferred queue
                // and handle at a later time
                this->m_deferredMessages.append(msg);
            }
            else
            {
                // message has been handled normally
                msg->setHandled(true);
                msgHandled = true;
            }
            
            // break after the first handler accepts the message
            break;
        }
    }
    return msgHandled;
}

//------------------------------------------------------------------------------
/**
    Signal the message-handled flag. Call this method once per
    handler-loop if either ThreadUpdateDeferredMessages or 
    ThreadHandleMessages returns true!
*/
void
DwHandlerThreadBase::threadSignalMessageHandled()
{
    this->m_msgHandledEvent.signal();    
}