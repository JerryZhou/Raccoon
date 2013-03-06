#pragma once

#include "dwapp/dwhandlerthreadbase.h"
#include "dwapp/dwsafequeue.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwBlockingHandlerThread : public DwHandlerThreadBase
{
    DW_DECLARE_CLASS(DwBlockingHandlerThread);
public:
    /// constructor
    DwBlockingHandlerThread();

    /// set optional wait timeout (0 if infinite)
    void setWaitTimeout(int milliSec);
    /// get wait timeout
    int waitTimeout() const;

    /// add a message to be handled (override in subclass!)
    virtual void addMessage(const DwPtr<DwMessage>& msg);
    /// cancel a pending message (override in subclass!)
    virtual void cancelMessage(const DwPtr<DwMessage>& msg);

    /// called if thread needs a wakeup call before stopping
    virtual void emitWakeupSignal();
    /// this method runs in the thread context
    virtual void doWork();

private:
    int m_waitTimeout;
    DwSafeQueue<DwPtr<DwMessage> > m_msgQueue;
};

//------------------------------------------------------------------------------
/**
*/
inline void
DwBlockingHandlerThread::setWaitTimeout(int milliSec)
{
    DW_ASSERT(!this->isRunning());
    this->m_waitTimeout = milliSec;
}

//------------------------------------------------------------------------------
/**
*/
inline int
DwBlockingHandlerThread::waitTimeout() const
{
    return this->m_waitTimeout;
}
