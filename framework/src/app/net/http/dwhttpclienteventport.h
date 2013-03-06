#pragma once

#include "dwcore/dwvariant.h"
#include "dwapp/dwrefcounted.h"
#include "dwgui/dwurl.h"
#include "dwgui/dwmediatype.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwHttpClientEventPort : public DwRefCounted
{
    DW_DECLARE_CLASS(DwHttpClientEventPort);
public:
    enum HttpEvent{StateChange, HeaderResponse, ProgressChange};
    DwHttpClientEventPort();
    virtual ~DwHttpClientEventPort();

    virtual void report(const DwUrl& url, HttpEvent evt, const DwVariant& v);
};// end of DwHttpClientEventPort