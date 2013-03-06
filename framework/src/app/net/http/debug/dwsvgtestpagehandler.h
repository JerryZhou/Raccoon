#pragma once

#include "dwapp/dwhttprequesthandler.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwSvgTestPageHandler : public DwHttpRequestHandler
{
    DW_DECLARE_CLASS(DwSvgTestPageHandler);
public:
    /// constructor
    DwSvgTestPageHandler();
    /// handle a http request, the handler is expected to fill the content stream with response data
    virtual void handleRequest(const DwPtr<DwHttpRequest>& request); 

private:
    /// test shape rendering
    bool TestShapeRendering(const DwPtr<DwHttpRequest>& request);
    /// test line chart rendering
    bool TestLineChartRendering(const DwPtr<DwHttpRequest>& request);
};// end of DwSvgTestPageHandler