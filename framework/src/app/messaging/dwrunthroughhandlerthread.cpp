
#include "stable.h"
#include "dwrunthroughhandlerthread.h"

DW_IMPLEMENT_CLASS(DwRunThroughHandlerThread, 'RTHT', DwHandlerThreadBase);

//------------------------------------------------------------------------------
/**
*/
DwRunThroughHandlerThread::DwRunThroughHandlerThread()
{
    this->m_msgQueue.setSignalOnEnqueueEnabled(false);
}

//------------------------------------------------------------------------------
/**
    This adds a new message to the thread's message queue.
*/
void
DwRunThroughHandlerThread::addMessage(const DwPtr<DwMessage>& msg)
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
DwRunThroughHandlerThread::cancelMessage(const DwPtr<DwMessage>& msg)
{
    DW_ASSERT(msg.isvalid());
    this->m_msgQueue.eraseMatchingElements(msg);
}

//------------------------------------------------------------------------------
/**
    The message processing loop.
*/
void
DwRunThroughHandlerThread::doWork()
{
    this->threadOpenHandlers();
    while (!this->threadStopRequested())
    {
        bool msgHandled = false;

        // do not wait for message, but yield thread on some platforms
        // (the yield is only important on the Wii, and doesn't do
        // anything on other platforms)
        DwThread::yieldThread();

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