#pragma once

#include "dwcore/dwlocks.h"
#include "dwcore/dwvector.h"
#include "dwapp/dwsyncevent.h"

//------------------------------------------------------------------------------
template<class TYPE> class DwSafeQueue
{
public:
    /// constructor
    DwSafeQueue();
    /// copy constructor
    DwSafeQueue(const DwSafeQueue<TYPE>& rhs);

    /// assignment operator
    void operator=(const DwSafeQueue<TYPE>& rhs);
    /// enable/disable signalling on Enqueue() (default is enabled)
    void setSignalOnEnqueueEnabled(bool b);
    /// return signalling-on-Enqueue() flag
    bool isSignalOnEnqueueEnabled() const;
    /// returns number of elements in the queue
    size_t size() const;
    /// return true if queue is empty
    bool isEmpty() const;
    /// remove all elements from the queue
    void clear();
    /// add element to the back of the queue
    void enqueue(const TYPE& e);
    /// enqueue an array of elements
    void enqueueArray(const DwVector<TYPE>& a);
    /// remove the element from the front of the queue
    TYPE dequeue();
    /// dequeue all events (only requires one lock)
    void dequeueAll(DwVector<TYPE>& outArray);
    /// access to element at front of queue without removing it
    TYPE peek() const;
    /// wait until queue contains at least one element
    void wait();
    /// wait until queue contains at least one element, or time-out happens
    void waitTimeout(int ms);
    /// signal the internal event, so that Wait() will return
    void signal();
    /// erase all matching elements
    void eraseMatchingElements(const TYPE& e);

protected:
    DwCriticalSection m_criticalSection;
    DwSyncEvent m_enqueueEvent;
    bool m_signalOnEnqueueEnabled;
    DwVector<TYPE> m_datas;
};// end of DwSafeQueue

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
DwSafeQueue<TYPE>::DwSafeQueue() :
    m_signalOnEnqueueEnabled(true)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
DwSafeQueue<TYPE>::DwSafeQueue(const DwSafeQueue<TYPE>& rhs)
{
    this->m_criticalSection.lock();
    this->m_datas = rhs.m_datas;
    this->m_signalOnEnqueueEnabled = rhs.m_signalOnEnqueueEnabled;
    this->m_criticalSection.unlock();
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
DwSafeQueue<TYPE>::operator=(const DwSafeQueue<TYPE>& rhs)
{
    this->m_criticalSection.lock();
    this->m_datas = rhs.m_datas;
    this->m_signalOnEnqueueEnabled = rhs.m_signalOnEnqueueEnabled;
    this->m_criticalSection.unlock();
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
DwSafeQueue<TYPE>::setSignalOnEnqueueEnabled(bool b)
{
    this->m_criticalSection.lock();
    this->m_signalOnEnqueueEnabled = b;
    this->m_criticalSection.unlock();
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
DwSafeQueue<TYPE>::clear()
{
    this->m_criticalSection.lock();
    this->m_datas.clear();
    this->m_criticalSection.unlock();
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> size_t
DwSafeQueue<TYPE>::size() const
{
    return this->m_datas.size();
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
DwSafeQueue<TYPE>::isEmpty() const
{
    this->m_criticalSection.lock();
    bool b = this->m_datas.isEmpty();
    this->m_criticalSection.unlock();
    return b;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
DwSafeQueue<TYPE>::enqueue(const TYPE& e)
{
    this->m_criticalSection.lock();
    this->m_datas.append(e);
    this->m_criticalSection.unlock();
    if (this->m_signalOnEnqueueEnabled)
    {
        this->m_enqueueEvent.signal();
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
DwSafeQueue<TYPE>::enqueueArray(const DwVector<TYPE>& a)
{
    this->m_criticalSection.lock();
    for (int i=0; i<a.size(); ++i)
    {
        this->m_datas.append(a[i]);
    }
    this->m_criticalSection.unlock();
    if (this->m_signalOnEnqueueEnabled)
    {
        this->m_enqueueEvent.signal();
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> TYPE
DwSafeQueue<TYPE>::dequeue()
{
    this->m_criticalSection.lock();
    TYPE e = this->m_datas.front();
    this->m_datas.remove(0);
    this->m_criticalSection.unlock();
    return e;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
DwSafeQueue<TYPE>::dequeueAll(DwVector<TYPE>& outArray)
{
    this->m_criticalSection.lock();
    outArray = this->m_datas;
    this->m_datas.clear();
    this->m_criticalSection.unlock();
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> TYPE
DwSafeQueue<TYPE>::peek() const
{
    this->m_criticalSection.lock();
    TYPE e = this->m_datas.front();
    this->m_criticalSection.unlock();
    return e;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
DwSafeQueue<TYPE>::wait()
{
    if (this->m_signalOnEnqueueEnabled && this->isEmpty())
    {
        this->m_enqueueEvent.wait();
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
DwSafeQueue<TYPE>::waitTimeout(int ms)
{
    if (this->m_signalOnEnqueueEnabled && this->isEmpty())
    {
        this->m_enqueueEvent.waitTimeout(ms);
    }
}

//------------------------------------------------------------------------------
/**
    This signals the internal event object, on which Wait() may be waiting.
    This method may be useful to wake up a thread waiting for events
    when it should stop.
*/
template<class TYPE> void
DwSafeQueue<TYPE>::signal()
{
    this->m_enqueueEvent.signal();
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
DwSafeQueue<TYPE>::eraseMatchingElements(const TYPE& e)
{
    this->m_criticalSection.lock();
    int i;
    for (i = this->m_datas.size() - 1; i >= 0; i--)
    {
        if (e == this->m_datas[i])
        {
            this->m_datas.remove(i);
        }
    }
    this->m_criticalSection.unlock();
}