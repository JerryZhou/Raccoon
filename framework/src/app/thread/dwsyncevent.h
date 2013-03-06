#pragma once

#include "dwgui/dwrttiobject.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwSyncEvent
{
public:
    /// constructor
    DwSyncEvent(bool manualReset=false);
    /// destructor
    ~DwSyncEvent();
    /// signal the event
    void signal();
    /// reset the event (only if manual reset)
    void reset();
    /// wait for the event to become signalled
    void wait() const;
    /// wait for the event with timeout in millisecs
    bool waitTimeout(int ms) const;
    /// check if event is signalled
    bool peek() const;
private:
    HANDLE event;
};// end of DwSyncEvent

//------------------------------------------------------------------------------
/**
*/
inline
DwSyncEvent::DwSyncEvent(bool manualReset)
{
    this->event = ::CreateEvent(NULL, manualReset, FALSE, NULL);
    DW_ASSERT(0 != this->event);
}

//------------------------------------------------------------------------------
/**
*/
inline
DwSyncEvent::~DwSyncEvent()
{
    ::CloseHandle(this->event);
    this->event = 0;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DwSyncEvent::signal()
{
    ::SetEvent(this->event);
}

//------------------------------------------------------------------------------
/**
*/
inline void
DwSyncEvent::reset()
{
    ::ResetEvent(this->event);
}

//------------------------------------------------------------------------------
/**
*/
inline void
DwSyncEvent::wait() const
{
    ::WaitForSingleObject(this->event, INFINITE);
}

//------------------------------------------------------------------------------
/**
    Waits for the event to become signaled with a specified timeout
    in milli seconds. If the method times out it will return false,
    if the event becomes signalled within the timeout it will return 
    true.
*/
inline bool
DwSyncEvent::waitTimeout(int timeoutInMilliSec) const
{
    DWORD res = ::WaitForSingleObject(this->event, timeoutInMilliSec);
    return (WAIT_TIMEOUT == res) ? false : true;
}

//------------------------------------------------------------------------------
/**
    This checks if the event is signalled and returnes immediately.
*/
inline bool
DwSyncEvent::peek() const
{
    DWORD res = ::WaitForSingleObject(this->event, 0);
    return (WAIT_TIMEOUT == res) ? false : true;
}