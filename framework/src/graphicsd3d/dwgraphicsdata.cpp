#include "stable.h"
#include "dwgraphicsdata.h"

//------------------------------------------------------------------------------
/**
*/
DwGraphicsObjectData::DwGraphicsObjectData()
: m_ref(0)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsObjectData::~DwGraphicsObjectData()
{
    DW_ASSERT(!m_ref);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsObjectData::AddRef()
{
    ++m_ref;
}


//------------------------------------------------------------------------------
/**
*/
void DwGraphicsObjectData::Release()
{
    if (!(--m_ref))
    {
        delete this;
    }
}