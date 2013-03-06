#include "stable.h"
#include "dwresourcelib.h"
#include "dwgui/dweventsignaldispatch.h"
#include "dwresourcemapping.h"
#if DW_ENABLE_RESOURCE_SCHEME_DLL
#include "dwresourcemappingdll.h"
#endif
#if DW_ENABLE_RESOURCE_SCHEME_FILE
#include "dwresourcemappingfile.h"
#endif
#if DW_ENABLE_RESOURCE_SCHEME_ZIP
#include "dwresourcemappingzip.h"
#endif
#if DW_ENABLE_RESOURCE_SCHEME_HTTP
// move to app
// #include "dwresourcemappinghttp.h"
#endif

DW_IMPLEMENT_CLASS(DwResourceLib, 'RLIB', DwRttiObject);

//------------------------------------------------------------------------------
/**
*/
DwResourceLib::DwResourceLib()
{
    // to send resource event
    m_eventDispatch = new DwEventSignalDispatch(this);

#if DW_ENABLE_RESOURCE_SCHEME_DLL
    regsterMapping("dll", DwResourceMappingDLL::RTTI());
#endif
#if DW_ENABLE_RESOURCE_SCHEME_FILE
    regsterMapping("file", DwResourceMappingFile::RTTI());
#endif
#if DW_ENABLE_RESOURCE_SCHEME_ZIP
    regsterMapping("zip", DwResourceMappingZip::RTTI());
#endif
#if DW_ENABLE_RESOURCE_SCHEME_HTTP
    // move to app
    // regsterMapping("http", DwResourceMappingHttp::RTTI());
#endif
}

//------------------------------------------------------------------------------
/**
*/
DwResourceLib::~DwResourceLib()
{
    DwMap<DwString, DwResourceMapping*>::iterator ite = m_mappings.begin();
    while(ite != m_mappings.end())
    {
        DW_SAFE_DELETE(ite->second);
        ++ite;
    }
    m_mappings.clear();

    DW_SAFE_DELETE(m_eventDispatch);
}

//------------------------------------------------------------------------------
/**
*/
bool DwResourceLib::hasMapping(const DwString &scheme) const
{
    return m_mappings.contains(scheme);
}

//------------------------------------------------------------------------------
/**
*/
bool DwResourceLib::regsterMapping(const DwString &scheme, const DwRtti *mapping)
{
    if (!m_mappings.contains(scheme) &&
            mapping && mapping->isDerivedFrom(DwResourceMapping::RTTI()))
    {
        m_mappings [scheme] = dwsafe_cast<DwResourceMapping>(mapping->create());
        m_mappings [scheme]->setResourceScheme(scheme);
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
DwResourceMapping * DwResourceLib::mappingOf(const DwString &scheme) const
{
    DwMap<DwString, DwResourceMapping*>::const_iterator ite = m_mappings.find(scheme);
    if (ite != m_mappings.end())
    {
        return ite->second;
    }
    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
DwVector<DwResourceMapping*> DwResourceLib::mappings()const
{
    DwVector<DwResourceMapping*> maps;
    DwMap<DwString, DwResourceMapping*>::const_iterator ite = m_mappings.begin();
    while (ite != m_mappings.end())
    {
        maps.append((DwResourceMapping*)ite->second);
        ++ite;
    }
    return maps;
}

//------------------------------------------------------------------------------
/**
*/
bool DwResourceLib::hasRes(const DwUrl &url) const
{
    DwResourceMapping *mapping = mappingOf(url.scheme());
    if (mapping)
    {
        return mapping->hasRes(url);
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwResourceLib::appendRes(const DwUrl &url, const DwByteArray &data)
{
    DwResourceMapping *mapping = mappingOf(url.scheme());
    if (mapping)
    {
        return mapping->appendRes(url, data);
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
DwByteArray DwResourceLib::lookupRes(const DwUrl &url)const
{
    DwResourceMapping *mapping = mappingOf(url.scheme());
    if (mapping)
    {
        return mapping->lookupRes(url);
    }
    else
    {
        return DwByteArray();
    }
}

bool DwResourceLib::isAsyncResource(const DwUrl &url) const
{
    DwResourceMapping *mapping = mappingOf(url.scheme());
    if (mapping)
    {
        return mapping->isAsyncResource(url);
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
DwEventSignalDispatch* DwResourceLib::eventDispatch() const
{
    return m_eventDispatch;
}

//------------------------------------------------------------------------------
/**
*/
bool DwResourceLib::sendEvent(DwEvent *evt)
{
    return m_eventDispatch->dispatchEvent(this, evt);
}

//------------------------------------------------------------------------------
/**
*/
void DwResourceLib::update()
{
    DwMap<DwString, DwResourceMapping*>::iterator ite = m_mappings.begin();
    while (ite != m_mappings.end())
    {
        // NB! do not delete/reg/unreg in update
        ite->second->update();
        ++ite;
    }
}

//------------------------------------------------------------------------------
/**
*/
DW_GLOBAL_STATIC(DwResourceLib, _dw_ResourceLib);
DwResourceLib *dwResourceLib()
{
    return _dw_ResourceLib();
}