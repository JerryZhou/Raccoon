#pragma once
#include "dwapp/dwrefcounted.h"
#include "dwapp/dwcpuid.h"
#include "dwapp/dwsyncevent.h"
#include "dwapp/dwthreadid.h"

class DW_APP_EXPORT DwThread : public DwRefCounted
{
    DW_DECLARE_CLASS(DwThread);
public:
    /// thread priorities
    enum Priority
    {
        Low,
        Normal,
        High,
    };

    /// constructor
    DwThread();
    /// destructor
    virtual ~DwThread();
    /// set the thread priority
    void setPriority(Priority p);
    /// get the thread priority
    Priority priority() const;
    /// set cpu core on which the thread should be running
    void setCoreId(DwCpu::CoreId coreId);
    /// get the cpu core on which the thread should be running
    DwCpu::CoreId coreId() const;
    /// set stack size in bytes (default is 4 KByte)
    void setStackSize(size_t s);
    /// get stack size
    size_t stackSize() const;
    /// set thread name
    void setName(const DwString& n);
    /// get thread name
    const DwString& name() const;

    /// start executing the thread code, returns when thread has actually started
    void start();
    /// request threading code to stop, returns when thread has actually finished
    void stop();
    /// return true if thread has been started
    bool isRunning() const;
    
    /// yield the thread (gives up current time slice)
    static void yieldThread();
    /// set thread name from within thread context
    static void setMyThreadName(const char* n);
    /// obtain name of thread from within thread context
    static const char* myThreadName();
    /// get the thread ID of this thread
    static DwThreadId myThreadId();

protected:
    /// override this method if your thread loop needs a wakeup call before stopping
    virtual void emitWakeupSignal();
    /// this method runs in the thread context
    virtual void doWork();
    /// check if stop is requested, call from DoWork() to see if the thread proc should quit
    bool threadStopRequested() const;

private:
    /// internal thread proc helper function
    static DWORD WINAPI ThreadProc(LPVOID self);

    HANDLE m_threadHandle;
    DwSyncEvent m_threadStartedEvent;
    DwSyncEvent m_stopRequestEvent;
    Priority m_priority;
    size_t m_stackSize;
    DwString m_name;
    DwCpu::CoreId m_coreId;
    const char* m_ThreadName;
};// end of DwThread

//------------------------------------------------------------------------------
/**
*/
inline void DwThread::setPriority(Priority p)
{
    this->m_priority = p;
}

//------------------------------------------------------------------------------
/**
*/
inline DwThread::Priority DwThread::priority() const
{
    return this->m_priority;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwThread::setStackSize(size_t s)
{
    this->m_stackSize = s;
}

//------------------------------------------------------------------------------
/**
*/
inline size_t DwThread::stackSize() const
{
    return this->m_stackSize;
}

//------------------------------------------------------------------------------
/**
    If the derived DoWork() method is running in a loop it must regularly
    check if the process wants the thread to terminate by calling
    ThreadStopRequested() and simply return if the result is true. This
    will cause the thread to shut down.
*/
inline bool DwThread::threadStopRequested() const
{
    return this->m_stopRequestEvent.peek();
}

//------------------------------------------------------------------------------
/**
    Set the thread's name. To obtain the current thread's name from anywhere
    in the thread's execution context, call the static method
    Thread::GetMyThreadName().
*/
inline void DwThread::setName(const DwString& n)
{
    DW_ASSERT(!n.isNull());
    this->m_name = n;
}

//------------------------------------------------------------------------------
/**
    Get the thread's name. This is the vanilla method which
    returns the name member. To obtain the current thread's name from anywhere
    in the thread's execution context, call the static method
    Thread::GetMyThreadName().
*/
inline const DwString& DwThread::name() const
{
    return this->m_name;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwThread::setCoreId(DwCpu::CoreId id)
{
    this->m_coreId = id;
}

//------------------------------------------------------------------------------
/**
*/
inline DwCpu::CoreId DwThread::coreId() const
{
    return this->m_coreId;
}