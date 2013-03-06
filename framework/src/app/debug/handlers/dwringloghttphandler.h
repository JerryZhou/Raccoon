#pragma once

#include "dwapp/dwhttprequesthandler.h"

//------------------------------------------------------------------------------
class DwHtmlPageWriter;

//------------------------------------------------------------------------------
class DwRingLogHttpHandler : public DwHttpRequestHandler
{
    DW_DECLARE_CLASS(DwRingLogHttpHandler);
public:
    /// constructor
    DwRingLogHttpHandler();
    /// handle a http request, the handler is expected to fill the content stream with response data
    virtual void handleRequest(const DwPtr<DwHttpRequest>& request); 

private:
    /// will do the main thread page
    void handleMainPage(const DwPtr<DwHtmlPageWriter>& writer);
    /// will do the detail information about thread
    void handleThreadPage(DWORD id, const DwPtr<DwHtmlPageWriter>& writer);
};// end of DwRingLogHttpHandler