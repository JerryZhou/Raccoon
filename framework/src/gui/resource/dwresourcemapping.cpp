#include "stable.h"
#include "dwresourcemapping.h"
#include "dwresourcelib.h"
#include "dwgui/dweventsignaldispatch.h"

DW_IMPLEMENT_CLASS(DwResourceMapping, 'RMNG', DwRttiObject);

//------------------------------------------------------------------------------
/**
*/
DwResourceMapping::DwResourceMapping()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwResourceMapping::~DwResourceMapping()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
bool DwResourceMapping::hasRes(const DwUrl &url) const
{
    return m_datas.contains(url.asString());
}

//------------------------------------------------------------------------------
/**
*/
DwByteArray DwResourceMapping::lookupRes(const DwUrl &url)const
{
    DwMap<DwString, DwByteArray>::const_iterator ite = m_datas.find(url.asString());
    if (ite != m_datas.end())
    {
        return ite->second;
    }
    // return the empty
    return DwByteArray();
}

//------------------------------------------------------------------------------
/**
*/
bool DwResourceMapping::appendRes(const DwUrl &url, const DwByteArray &data)
{
    m_datas[url.asString()] = data;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
DwByteArray& DwResourceMapping::resOf(const DwUrl &url)
{
    DW_ASSERT(hasRes(url));
    return m_datas.find(url.asString())->second;
}

//------------------------------------------------------------------------------
/**
*/
bool DwResourceMapping::saveRes(const DwUrl &url)const
{
    DW_UNUSED(url);
    DW_ASSERT(!hasRes(url));
    // TODO: every mapping self
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwResourceMapping::isAsyncResource(const DwUrl &url) const
{
    DW_UNUSED(url);
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void DwResourceMapping::update()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwEventSignalDispatch* DwResourceMapping::eventDispatch() const
{
    return dwResourceLib()->eventDispatch();
}

//------------------------------------------------------------------------------
/**
*/
const DwMap<DwString, DwByteArray>& DwResourceMapping::datas()const
{
    return m_datas;
}

//------------------------------------------------------------------------------
/**
*/
DwString DwResourceMapping::scheme()const
{
    return m_scheme;
}

//------------------------------------------------------------------------------
/**
*/
void DwResourceMapping::setResourceScheme(const DwString &s)
{
    m_scheme = s;
}