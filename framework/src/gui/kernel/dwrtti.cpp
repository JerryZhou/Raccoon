#include "stable.h"
#include "dwrtti.h"
#include "dwrttiobject.h"
#include "dwcore/dwmempool.h"


//------------------------------------------------------------------------------
/**
*/
static DwFourCC _unique_fourcc()
{
    static DwFourCC _u_fourcc = '_0';
    return ++_u_fourcc;
}

//------------------------------------------------------------------------------
/**
*/
DwRtti::DwRtti(const char* className, DwFourCC fcc, pfnCreator creatorFunc, const DwRtti* parentClass, int instSize)
{
    construct(className, fcc, creatorFunc, parentClass, instSize);
}

//------------------------------------------------------------------------------
/**
*/
DwRtti::DwRtti(const char* className, pfnCreator creatorFunc, const DwRtti* parentClass, int instSize)
{
    construct(className, 0, creatorFunc, parentClass, instSize);
}

//------------------------------------------------------------------------------
/**
*/
bool DwRtti::operator==(const DwRtti& rhs) const
{
    return this == &rhs;
}

//------------------------------------------------------------------------------
/**
*/
bool DwRtti::operator!=(const DwRtti& rhs) const
{
    return this != &rhs;
}

//------------------------------------------------------------------------------
/**
*/
DwRttiObject* DwRtti::create() const
{
    if (m_creator)
    {
        return m_creator();
    }
    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
bool DwRtti::isDerivedFrom(const DwRtti& other) const
{
    return isDerivedFrom(&other);
}

//------------------------------------------------------------------------------
/**
*/
bool DwRtti::isDerivedFrom(const DwRtti* other) const
{
    const DwRtti* cur;
    for (cur = this; cur != 0; cur = cur->parent())
    {
        if (cur == other)
        {
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwRtti::isDerivedFrom(const DwString& otherClassName) const
{
    const DwRtti* other = dwFactory()->classRttiOf(otherClassName);
    return isDerivedFrom(other);
}

//------------------------------------------------------------------------------
/**
*/
bool DwRtti::isDerivedFrom(const DwFourCC& otherClassFourCC) const
{
    const DwRtti* other = dwFactory()->classRttiOf(otherClassFourCC);
    return isDerivedFrom(other);
}

void* DwRtti::allocInstanceMemory()
{
    //return mp_alloc(m_instanceSize);
    return dwMalloc(m_instanceSize);
}

//------------------------------------------------------------------------------
/**
*/
void DwRtti::freeInstanceMemory(void* ptr)
{
    //mp_free(ptr);
    dwFree(ptr);
}

//------------------------------------------------------------------------------
/**
*/
void DwRtti::construct(const char* className, DwFourCC fcc, pfnCreator creatorFunc, const DwRtti* parentClass, int instSize)
{
    // hijack the auto fourcc
    if (!fcc)
    {
        fcc = _unique_fourcc();
    }

    // setup members
    m_parent = parentClass;
    m_fourCC = fcc;     // NOTE: may be 0
    m_creator = creatorFunc;
    m_instanceSize = instSize;

    // register class with factory
    m_className = className;

    // FourCC code valid
    if (fcc && !dwFactory()->classExists(fcc))
    {
        dwFactory()->registerClass(this, name(), fcc);
    }
}