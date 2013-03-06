#pragma once

#include "dwapp/dwhttprequesthandler.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwDefaultHttpRequestHandler : public DwHttpRequestHandler
{
    DW_DECLARE_CLASS(DwDefaultHttpRequestHandler);
public:
    /// handle a http request, the handler is expected to fill the content stream with response data
    virtual void handleRequest(const DwPtr<DwHttpRequest>& request);
};// end of DwDefaultHttpRequestHandler