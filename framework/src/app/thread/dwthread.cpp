#include "stable.h"
#include "dwthread.h"

DW_IMPLEMENT_CLASS(DwThread, 'THRD', DwRttiObject);

//------------------------------------------------------------------------------
/**
*/
DwThread::DwThread() :
    m_threadHandle(0),
    m_priority(Normal),
    m_stackSize(65536),
    m_coreId(DwCpu::InvalidCoreId),
    m_ThreadName(0)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwThread::~DwThread()
{
    if (this->isRunning())
    {
        this->stop();
    }
}

//------------------------------------------------------------------------------
/**
    Start the thread, this creates a Win32 thread and calls the static
    ThreadProc, which in turn calls the virtual DoWork() class of this object.
    The method waits for the thread to start and then returns.
*/
void DwThread::start()
{
    DW_ASSERT(!this->isRunning());
    DW_ASSERT(0 == this->m_threadHandle);
    this->m_threadHandle = ::CreateThread(NULL,             // lpThreadAttributes
                                      this->m_stackSize,  // dwStackSize
                                      ThreadProc,       // lpStartAddress
                                      (LPVOID) this,    // lpParameter
                                      CREATE_SUSPENDED, // dwCreationFlags
                                      NULL);            // lpThreadId
    DW_ASSERT(NULL != this->m_threadHandle);

    // apply thread priority
    switch (this->m_priority)
    {
        case Low:   
            SetThreadPriority(this->m_threadHandle, THREAD_PRIORITY_BELOW_NORMAL);
            break;

        case Normal:
            SetThreadPriority(this->m_threadHandle, THREAD_PRIORITY_NORMAL);
            break;

        case High:
            SetThreadPriority(this->m_threadHandle, THREAD_PRIORITY_ABOVE_NORMAL);
            break;
    }

    // resume thread (since it was actived in suspended state)
    ResumeThread(this->m_threadHandle);

    // wait for the thread to start
    this->m_threadStartedEvent.wait();
}

//------------------------------------------------------------------------------
/**
    This method is called by Thread::Stop() after setting the 
    stopRequest event and before waiting for the thread to stop. If your
    thread runs a loop and waits for jobs it may need an extra wakeup
    signal to stop waiting and check for the ThreadStopRequested() event. In
    this case, override this method and signal your event object.
*/
void DwThread::emitWakeupSignal()
{
    // empty, override in subclass!
}

//------------------------------------------------------------------------------
/**
    This stops the thread by signalling the stopRequestEvent and waits for the
    thread to actually quit. If the thread code runs in a loop it should use the 
    IsStopRequested() method to see if the thread object wants it to shutdown. 
    If so DoWork() should simply return.
*/
void DwThread::stop()
{
    DW_ASSERT(this->isRunning());
    DW_ASSERT(0 != this->m_threadHandle);

    // signal the thread to stop
    this->m_stopRequestEvent.signal();

    // call the wakeup-thread method, may be derived in a subclass
    // if the threads needs to be woken up, it is important that this
    // method is called AFTER the stopRequestEvent is signalled!
    this->emitWakeupSignal();

    // wait for the thread to terminate
    WaitForSingleObject(this->m_threadHandle, INFINITE);
    CloseHandle(this->m_threadHandle);
    this->m_threadHandle = 0;
}

//------------------------------------------------------------------------------
/**
    Returns true if the thread is currently running.
*/
bool DwThread::isRunning() const
{
    if (0 != this->m_threadHandle)
    {
        DWORD exitCode = 0;
        if (GetExitCodeThread(this->m_threadHandle, &exitCode))
        {
            if (STILL_ACTIVE == exitCode)
            {
                return true;
            }
        }
    }
    // fallthrough: thread not yet, or no longer
    return false;
}

//------------------------------------------------------------------------------
/**
    This method should be derived in a Thread subclass and contains the
    actual code which is run in the thread. The method must not call
    C-Lib functions under Win32. To terminate the thread, just return from
    this function. If DoWork() runs in an infinite loop, call ThreadStopRequested()
    to check whether the Thread object wants the thread code to quit.
*/
void DwThread::doWork()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    Internal static helper method. This is called by CreateThread() and
    simply calls the virtual DoWork() method on the thread object.
*/
DWORD WINAPI DwThread::ThreadProc(LPVOID self)
{
    DW_ASSERT(0 != self);

    DwThread* threadObj = (DwThread*) self;
    DwThread::setMyThreadName(threadObj->name().toCap().data());
    threadObj->m_threadStartedEvent.signal();
    threadObj->doWork();

    return 0;
}

//------------------------------------------------------------------------------
/**
    Static method which sets the name of this thread. This is called from
    within ThreadProc. The string pointed to must remain valid until
    the thread is terminated!
*/
void DwThread::setMyThreadName(const char* n)
{
    // first update our own internal thread-name pointer
    // m_ThreadName = n;

    // update the Windows thread name so that it shows up correctly
    // in the Debugger
    struct THREADNAME_INFO
    {
        DWORD dwType;     // must be 0x1000
        LPCSTR szName;    // pointer to name (in user address space)
        DWORD dwThreadID; // thread ID (-1 = caller thread)
        DWORD dwFlags;    // reserved for future use, must be zero
    };

    THREADNAME_INFO info;
    info.dwType     = 0x1000;
    info.szName     = n;
    info.dwThreadID = ::GetCurrentThreadId();
    info.dwFlags    = 0;
    __try
    {
        RaiseException( 0x406D1388, 0, sizeof(info) / sizeof(DWORD), (DWORD*)&info );
    }
    __except( EXCEPTION_CONTINUE_EXECUTION ) 
    {
    }
}

//------------------------------------------------------------------------------
/**
    Static method to obtain the current thread name from anywhere
    in the thread's code.
*/
const char* DwThread::myThreadName()
{
    //return m_ThreadName;
    return NULL;
}

//------------------------------------------------------------------------------
/**
    Static method which returns the ThreadId of this thread.
*/
DwThreadId DwThread::myThreadId()
{
    return ::GetCurrentThreadId();
}

//------------------------------------------------------------------------------
/**
    The yield function is empty on Win32 and Xbox360.
*/
void DwThread::yieldThread()
{
    // empty
}