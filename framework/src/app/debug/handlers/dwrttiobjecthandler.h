#pragma once

#include "dwapp/dwhttprequesthandler.h"

//------------------------------------------------------------------------------
class DwHtmlPageWriter;

//------------------------------------------------------------------------------
class DwRttiObjectHandler : public DwHttpRequestHandler
{
    DW_DECLARE_CLASS(DwRttiObjectHandler);
public:
    /// constructor
    DwRttiObjectHandler();
    /// handle a http request, the handler is expected to fill the content stream with response data
    virtual void handleRequest(const DwPtr<DwHttpRequest>& request); 

private:
    /// will do the main object page
    void handleMainPage(const DwPtr<DwHtmlPageWriter>& writer);
    /// will do the detail information about class
    void handleClassPage(const DwRtti* c, const DwPtr<DwHtmlPageWriter>& writer);
    /// will do the detail information about object
    void handleObjectPage(const DwRtti* obj, const DwPtr<DwHtmlPageWriter>& writer);
};// end of DwRttiObjectHandler