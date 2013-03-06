#pragma once

#include "dwapp/dwhttprequesthandler.h"

//------------------------------------------------------------------------------
class DwHtmlPageWriter;

//------------------------------------------------------------------------------
class DwImagesHttpHandler : public DwHttpRequestHandler
{
    DW_DECLARE_CLASS(DwImagesHttpHandler);
public:
    /// constructor
    DwImagesHttpHandler();
    /// handle a http request, the handler is expected to fill the content stream with response data
    virtual void handleRequest(const DwPtr<DwHttpRequest>& request); 
};// end of DwImagesHttpHandler