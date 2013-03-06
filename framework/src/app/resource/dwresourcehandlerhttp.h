#pragma once

#include "dwapp/dwinterfacehandlerbase.h"
#include "dwapp/dwmessage.h"
#include "dwapp/dwhttpserver.h"
#include "dwapp/dwhttpprotocol.h"
#include "dwapp/dwresourceprotocol.h"

//------------------------------------------------------------------------------
class DwResourceMappingHttp;

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwResourceHandlerHttp : public DwInterfaceHandlerBase
{
    DW_DECLARE_CLASS(DwResourceHandlerHttp);
public:
    /// constructor
    DwResourceHandlerHttp();
    /// destructor
    virtual ~DwResourceHandlerHttp();

    /// open the handler
    virtual void open();
    /// close the handler
    virtual void close();
    /// handle a message, return true if handled
    virtual bool handleMessage(const DwPtr<DwMessage>& msg);
    /// do per-frame work
    virtual void doWork();

    /// config the out put place
    void setHttpMapping(DwResourceMappingHttp *mapping);

private:
    DwResourceMappingHttp *httpMapping;
    DwSafeQueue<DwPtr<DwRequestResource> > pendingRequests;
};// end of DwResourceHandlerHttp
