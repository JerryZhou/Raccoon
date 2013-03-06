#pragma once
#include "dwgui/dwrttiobject.h"
#include "dwapp/dwinterlocked.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwRefCounted : public DwRttiObject
{
    DW_DECLARE_CLASS(DwRefCounted);
public:
    /// constructor
    DwRefCounted();
    /// get the current refcount
    int refCount() const;
    /// increment refcount by one
    void addRef();
    /// decrement refcount and destroy object if refcount is zero
    void release();

protected:
    /// destructor (called when refcount reaches zero)
    virtual ~DwRefCounted();

private:
    volatile int m_refCount;
};// end of DwRefCounted

//------------------------------------------------------------------------------
/**
*/
inline DwRefCounted::DwRefCounted()
: m_refCount(0)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
inline int DwRefCounted::refCount() const
{
    return m_refCount;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwRefCounted::addRef()
{
    DwInterlocked::increment(m_refCount);
}

//------------------------------------------------------------------------------
/**
*/
inline void DwRefCounted::release()
{
    if (0 == DwInterlocked::decrement(this->m_refCount))
    {
        delete (this);
    }
}