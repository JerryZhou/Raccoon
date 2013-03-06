#include "stable.h"
#include "dwfactory.h"
#include "dwrtti.h"
#include "dwcore/dwglobalstatic.h"



//------------------------------------------------------------------------------
/**
*/
DwFactory::DwFactory()
{
    ;//empty
}

//------------------------------------------------------------------------------
/**
*/
DwFactory::~DwFactory()
{
    m_nameTable.clear();
    m_fourccTable.clear();
}

//------------------------------------------------------------------------------
/**
*/
void
DwFactory::registerClass(const DwRtti* rtti, const DwString& className, const DwFourCC& classFourCC)
{
    DwCSLocker locks(&m_criticalSection);

    // check if class name already exists
    if (classExists(className))
    {
        DW_ASSERT("class name is exists");
        return;
    }

    // check if class fourcc already exists
    if (classExists(classFourCC))
    {
        DW_ASSERT("class fourcc is exists");
        return;
    }

    // register with lookup tables
    m_nameTable.insert(className, rtti);
    m_fourccTable.insert(classFourCC, rtti);
}

//------------------------------------------------------------------------------
/**
*/
void
DwFactory::registerClass(const DwRtti* rtti, const DwString& className)
{
    DwCSLocker locks(&m_criticalSection);

    // check if class name already exists
    if (classExists(className))
    {
        DW_ASSERT("class name is exists");
        return;
    }

    // register with lookup tables
    m_nameTable.insert(className, rtti);
}

//------------------------------------------------------------------------------
/**
    This method checks if a class with the given name has been registered.
*/
bool
DwFactory::classExists(const DwString& className) const
{
    DwCSLocker locks(&m_criticalSection);

    return m_nameTable.contains(className);
}

//------------------------------------------------------------------------------
/**
*/
bool
DwFactory::classExists(const DwFourCC fourCC) const
{
    DwCSLocker locks(&m_criticalSection);

    return m_fourccTable.contains(fourCC);
}

//------------------------------------------------------------------------------
/**
*/
const DwRtti*
DwFactory::classRttiOf(const DwString& className) const
{
    DwCSLocker locks(&m_criticalSection);

    return m_nameTable.value(className, NULL);
}

//------------------------------------------------------------------------------
/**
*/
const DwRtti*
DwFactory::classRttiOf(const DwFourCC& classFourCC) const
{
    DwCSLocker locks(&m_criticalSection);

    return m_fourccTable.value(classFourCC, NULL);
}

//------------------------------------------------------------------------------
/**
    Create an object by class name.
*/
DwRttiObject*
DwFactory::create(const DwString& className) const
{
    DwCSLocker locks(&m_criticalSection);

    // check if class exists, otherwise give a meaningful error
    if (!classExists(className))
    {
        return 0;
    }

    // lookup RTTI object of class through hash table and create new object
    const DwRtti* rtti = m_nameTable.value(className);
    DwRttiObject* newObject = rtti->create();
    return newObject;
}

//------------------------------------------------------------------------------
/**
    Create an object by FourCC code.
*/
DwRttiObject*
DwFactory::create(const DwFourCC classFourCC) const
{
    DwCSLocker locks(&m_criticalSection);

    // check if class exists, otherwise give meaningful error
    if (!classExists(classFourCC))
    {
        return 0;
    }

    // lookup RTTI object of class through hash table and create new object
    const DwRtti* rtti = m_fourccTable.value(classFourCC);
    DwRttiObject* newObject = rtti->create();
    return newObject;
}

//------------------------------------------------------------------------------
/**
*/
DwVector<DwRtti*> 
DwFactory::rttis()const
{
    DwCSLocker locks(&m_criticalSection);

    DwVector<DwRtti*> infos;
    DwMap<DwFourCC, const DwRtti*>::const_iterator ite = m_fourccTable.begin();
    while(ite != m_fourccTable.end())
    {
        infos.append((DwRtti*)ite->second);
        ++ite;
    }
    return infos;
}

//------------------------------------------------------------------------------
/**
    Create DwFactory
*/
DW_GLOBAL_STATIC(DwFactory, _dw_factory);
DwFactory* dwFactory()
{
    return _dw_factory();
}