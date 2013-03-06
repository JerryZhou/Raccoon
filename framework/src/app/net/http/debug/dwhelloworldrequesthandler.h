#pragma once
//------------------------------------------------------------------------------
/**
    http://127.0.0.1:2100/helloworld
*/    
#include "dwapp/dwhttprequesthandler.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwHelloWorldRequestHandler : public DwHttpRequestHandler
{
    DW_DECLARE_CLASS(DwHelloWorldRequestHandler);
public:
    /// constructor
    DwHelloWorldRequestHandler();
    /// handle a http request
    virtual void handleRequest(const DwPtr<DwHttpRequest>& request);
};// end of DwHelloWorldRequestHandler