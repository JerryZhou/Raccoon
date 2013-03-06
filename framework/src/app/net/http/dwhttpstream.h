#pragma once

#include "dwapp/dwmemorystream.h"
#include "dwapp/dwhttpclient.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwHttpStream : public DwMemoryStream
{
    DW_DECLARE_CLASS(DwHttpStream);
public:
    /// constructor
    DwHttpStream();
    /// destructor
    virtual ~DwHttpStream();
    
    /// open the stream
    virtual bool open();

    /// set the http client event port
    void setEventPort(const DwRtti* rti);

protected:
    DwRtti* eventPort;
};// end of DwHttpStream