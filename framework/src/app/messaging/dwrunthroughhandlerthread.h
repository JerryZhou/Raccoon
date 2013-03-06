#pragma once

#include "dwapp/dwhandlerthreadbase.h"
#include "dwapp/dwsafequeue.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwRunThroughHandlerThread : public DwHandlerThreadBase
{
    DW_DECLARE_CLASS(DwRunThroughHandlerThread);
public:
    /// constructor
    DwRunThroughHandlerThread();

    /// add a message to be handled (override in subclass!)
    virtual void addMessage(const DwPtr<DwMessage>& msg);
    /// cancel a pending message (override in subclass!)
    virtual void cancelMessage(const DwPtr<DwMessage>& msg);

    /// this method runs in the thread context
    virtual void doWork();

private:
    DwSafeQueue<DwPtr<DwMessage> > m_msgQueue;
};// end of DwRunThroughHandlerThread