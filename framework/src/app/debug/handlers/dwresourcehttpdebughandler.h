#pragma once

#include "dwapp/dwhttprequesthandler.h"

//------------------------------------------------------------------------------
class DwHtmlPageWriter;
class DwResourceMapping;

//------------------------------------------------------------------------------
class DwResourceHttpDebugHandler : public DwHttpRequestHandler
{
    DW_DECLARE_CLASS(DwResourceHttpDebugHandler);
public:
    /// constructor
    DwResourceHttpDebugHandler();

protected:
    /// handle a http request, overwrite this method in you subclass
    virtual void handleRequest(const DwPtr<DwHttpRequest>& request);
    // handle the resource main page
    virtual void handleMainPage(const DwPtr<DwHtmlPageWriter>& htmlWriter);
    // handle the detail of resource mapping
    virtual void handleDetailOfResourceMapping(DwResourceMapping* mapping, const DwPtr<DwHtmlPageWriter>& htmlWriter);
};// end of DwResourceHttpDebugHandler