#include "dwapp/dwhttpclienteventport.h"

//------------------------------------------------------------------------------
class DwResourceMappingHttp;

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwResourceHttpEventPort : public DwHttpClientEventPort
{
    DW_DECLARE_CLASS(DwResourceHttpEventPort);
public:
    /// constructor
    DwResourceHttpEventPort();
    /// destructor
    virtual ~DwResourceHttpEventPort();

    /// do inhert, will tell resource mapping what happen
    virtual void report(const DwUrl& url, HttpEvent evt, const DwVariant& v);
    /// config the out put place
    static void setHttpMapping(DwResourceMappingHttp *mapping);

protected:
    int contentLength;
    static DwResourceMappingHttp *httpMapping;
};// end of DwResourceHttpEventPort