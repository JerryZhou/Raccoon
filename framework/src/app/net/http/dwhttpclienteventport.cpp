#include "stable.h"
#include "dwhttpclienteventport.h"

DW_IMPLEMENT_CLASS(DwHttpClientEventPort, 'HCEP', DwRefCounted);

//------------------------------------------------------------------------------
/**
*/
DwHttpClientEventPort::DwHttpClientEventPort()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwHttpClientEventPort::~DwHttpClientEventPort()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwHttpClientEventPort::report( const DwUrl& url, HttpEvent evt, const DwVariant& v )
{
    DW_UNUSED(url);
    DW_UNUSED(evt);
    DW_UNUSED(v);
}
