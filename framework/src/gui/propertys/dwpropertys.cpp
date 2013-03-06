#include "stable.h"
#include "dwpropertys.h"

//------------------------------------------------------------------------------
/**
*/
DwPropertys::DwPropertys()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwPropertys::~DwPropertys()
{
    DwMap<DwString, DwPropertyBase* >::iterator ite = m_propertys.begin();
    while(ite != m_propertys.end())
    {
        delete (*ite).second;
        (*ite).second = NULL;
        ++ite;
    }
    m_propertys.clear();
}

//------------------------------------------------------------------------------
/**
*/
bool DwPropertys::registerProperty(DwString id, DwPropertyBase* p)
{
    addProperty(id, p);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void DwPropertys::addProperty(DwString id, DwPropertyBase* p)
{
    DW_ASSERT(!m_propertys.contains(id));
    m_propertys[id] = p;
}

//------------------------------------------------------------------------------
/**
*/
void DwPropertys::setProperty(void* obj, DwString id, const DwVariant& v)
{
    m_propertys[id]->setProperty(obj, v);
}

//------------------------------------------------------------------------------
/**
*/
DwVariant DwPropertys::propertyOf(const void* obj, DwString id)const
{
    return m_propertys[id]->propertyOf(obj);
}

//------------------------------------------------------------------------------
/**
*/
bool DwPropertys::hasProperty(DwString id) const
{
    return m_propertys.contains(id);
}
