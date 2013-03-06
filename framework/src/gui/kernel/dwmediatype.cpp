#include "stable.h"
#include "dwmediatype.h"
#include "graphicsview/dwdummys.h"

//------------------------------------------------------------------------------
/**
*/
DwMediaType::DwMediaType()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwMediaType::DwMediaType(const DwString& str)
{
    set(str);
}

//------------------------------------------------------------------------------
/**
*/
DwMediaType::DwMediaType(const DwString& _type, const DwString& _subType)
{
    set(_type, _subType);
}

//------------------------------------------------------------------------------
/**
*/
DwMediaType::DwMediaType(const DwMediaType& rhs)
{
    *this = rhs;
}

//------------------------------------------------------------------------------
/**
*/
void DwMediaType::operator=(const DwMediaType& rhs)
{
    if(this == &rhs) return;
    set(rhs.m_type, rhs.m_subType);
}

//------------------------------------------------------------------------------
/**
*/
bool DwMediaType::operator==(const DwMediaType& rhs)
{
    if (m_type == rhs.m_type && m_subType == rhs.m_subType)
    {
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwMediaType::operator!=(const DwMediaType& rhs)
{
    return ! (operator==(rhs));
}

//------------------------------------------------------------------------------
/**
*/
bool DwMediaType::isValid() const
{
    return !m_type.isEmpty() && !m_subType.isEmpty();
}

//------------------------------------------------------------------------------
/**
*/
void DwMediaType::clear()
{
    m_type.clear();
    m_subType.clear();
}

//------------------------------------------------------------------------------
/**
*/
void DwMediaType::set(const DwString& str)
{
    int index = str.indexOf(L'/');
    if (index != InvalidIndex)
    {
        DW_ASSERT(str.length() > 3);
        DwString _type = str.left(index);
        DwString _subType = str.right(str.length() - index - 1);
        set(_type, _subType);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwMediaType::set(const DwString& _type, const DwString& _subType)
{
    m_type = _type;
    m_subType = _subType;
}

//------------------------------------------------------------------------------
/**
*/
DwString DwMediaType::asString() const
{
    DwString result;
    result.append(m_type);
    result.append(L'/');
    result.append(m_subType);
    return result;
}

//------------------------------------------------------------------------------
/**
*/
const DwString& DwMediaType::type() const
{
    return m_type;
}

//------------------------------------------------------------------------------
/**
*/
const DwString& DwMediaType::subType() const
{
    return m_subType;
}