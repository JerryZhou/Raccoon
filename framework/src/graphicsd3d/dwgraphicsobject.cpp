#include "stable.h"
#include "dwgraphicsobject.h"
#include "dwgraphicsdatad3d_p.h"
#include "dwgraphicsd3d_p.h"

//------------------------------------------------------------------------------
/**
*/
DwGraphicsObject::DwGraphicsObject()
: m_data(NULL)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsObject::~DwGraphicsObject()
{
    if (m_data)
    {
        m_data->Release();
    }
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsObject::DwGraphicsObject(const DwGraphicsObject& other)
: m_data(NULL)
{
    *this = other;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsObject& DwGraphicsObject::operator=(const DwGraphicsObject &other)
{
    return *this = other.data();
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsObjectData * DwGraphicsObject::data() const
{
    return m_data;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsObject& DwGraphicsObject::operator=(DwGraphicsObjectData* d)
{
    if (d == m_data)
    {
        return *this;
    }

    if (m_data)
    {
        m_data->Release();
    }

    m_data = d;

    if (m_data)
    {
        m_data->AddRef();
    }

    return *this;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsObject::DwGraphicsObject(DwGraphicsObjectData* d)
: m_data(NULL)
{
    *this = d;
}

