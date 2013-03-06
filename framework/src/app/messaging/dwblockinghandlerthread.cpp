
#include "stable.h"
#include "dwblockinghandlerthread.h"

DW_IMPLEMENT_CLASS(DwBlockingHandlerThread, 'BLHT', DwHandlerThreadBase);

//------------------------------------------------------------------------------
/**
*/
DwBlockingHandlerThread::DwBlockingHandlerThread() :
    m_waitTimeout(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This adds a new message to the thread's message queue.
*/
void
DwBlockingHandlerThread::addMessage(const DwPtr<DwMessage>& msg)
{
    DW_ASSERT(msg.isvalid());
    this->m_msgQueue.enqueue(msg);
}

//------------------------------------------------------------------------------
/**
    This removes a message from the thread's message queue, regardless
    of its state.
*/
void
DwBlockingHandlerThread::cancelMessage(const DwPtr<DwMessage>& msg)
{
    DW_ASSERT(msg.isvalid());
    this->m_msgQueue.eraseMatchingElements(msg);
}

//------------------------------------------------------------------------------
/**
    This method is called by Thread::Stop() after setting the 
    stopRequest event and before waiting for the thread to stop.
*/
void
DwBlockingHandlerThread::emitWakeupSignal()
{
    // wake up our thread if it's waiting for messages
    this->m_msgQueue.signal();
}

//------------------------------------------------------------------------------
/**
    The message processing loop.
*/
void
DwBlockingHandlerThread::doWork()
{
    this->threadOpenHandlers();
    while (!this->threadStopRequested())
    {
        // wait for next message, or timeout
        bool msgHandled = false;
        if (this->m_waitTimeout > 0)
        {
            this->m_msgQueue.waitTimeout(this->m_waitTimeout);
        }
        else
        {
            this->m_msgQueue.wait();
        }
        
        // update state of deferred messages
        msgHandled = this->threadUpdateDeferredMessages();

        // process messages
        if (!this->m_msgQueue.isEmpty())
        {
            DwVector<DwPtr<DwMessage> > msgArray;
            this->m_msgQueue.dequeueAll(msgArray);
            msgHandled |= this->threadHandleMessages(msgArray);
        }

        // signal if at least one message has been handled
        if (msgHandled)
        {
            this->threadSignalMessageHandled();
        }

        // do per-frame update on attached handlers
        this->threadUpdateHandlers();
    }

    // cleanup and exit thread
    this->threadDiscardDeferredMessages();
    this->threadCloseHandlers();
}