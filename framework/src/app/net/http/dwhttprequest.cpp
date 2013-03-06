
#include "stable.h"
#include "dwhttprequest.h"


DW_IMPLEMENT_CLASS(DwHttpRequest, 'HTRQ', DwMessage);
DW_IMPLEMENT_MESSAGEID(DwHttpRequest);

//------------------------------------------------------------------------------
/**
*/
DwHttpRequest::DwHttpRequest() :
    m_method(DwHttpMethod::InvalidHttpMethod),
    m_status(DwHttpStatus::InvalidHttpStatus)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
DwHttpRequest::~DwHttpRequest()
{
    // empty
}
