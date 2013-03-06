#include "stable.h"
#include "dwresourcehttpeventport.h"
#include "dwapp/dwptr.h"
#include "dwapp/dwresourceprotocol.h"
#include "dwapp/dwresourcemappinghttp.h"

DW_IMPLEMENT_CLASS(DwResourceHttpEventPort, 'RHET', DwHttpClientEventPort);

//------------------------------------------------------------------------------
/**
*/
DwResourceHttpEventPort::DwResourceHttpEventPort()
: contentLength(0)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwResourceHttpEventPort::~DwResourceHttpEventPort()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwResourceHttpEventPort::report( const DwUrl& url, HttpEvent evt, const DwVariant& v )
{
    if (httpMapping)
    {
        switch (evt)
        {
        case StateChange:
        	break;
        case HeaderResponse:
            {
                contentLength = v.as<int>();
            }
            break;
        case ProgressChange:
            {
                if (httpMapping)
                {
                    DwPtr<DwResourceProgress> progress = DwResourceProgress::create();
                    progress->url = url;
                    progress->contentLength = contentLength;
                    progress->progress = v.as<int>();
                    httpMapping->appendResource(progress.upcast<DwMessage>());
                }
            }
            break;
        default:
            break;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwResourceHttpEventPort::setHttpMapping( DwResourceMappingHttp *mapping )
{
    DwResourceHttpEventPort::httpMapping = mapping;
}

//------------------------------------------------------------------------------
/**
*/
DwResourceMappingHttp *DwResourceHttpEventPort::httpMapping = NULL;