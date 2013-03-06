#pragma once

#include "dwapp/dwmessage.h"
#include "dwapp/dwhttprequesthandler.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwAttachRequestHandler : public DwMessage
{
    DW_DECLARE_CLASS(DwAttachRequestHandler);
    DW_DECLARE_MESSAGEID;
public:
    DwAttachRequestHandler() 
    { };
public:
    void SetRequestHandler(const DwPtr<DwHttpRequestHandler>& val)
    {
        this->m_requesthandler = val;
    };
    const DwPtr<DwHttpRequestHandler>& GetRequestHandler() const
    {
        return this->m_requesthandler;
    };
private:
    DwPtr<DwHttpRequestHandler> m_requesthandler;
};// end of DwAttachRequestHandler

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwRemoveRequestHandler : public DwMessage
{
    DW_DECLARE_CLASS(DwRemoveRequestHandler);
    DW_DECLARE_MESSAGEID;
public:
    DwRemoveRequestHandler() 
    { };
public:
    void SetRequestHandler(const DwPtr<DwHttpRequestHandler>& val)
    {
        this->m_requesthandler = val;
    };
    const DwPtr<DwHttpRequestHandler>& GetRequestHandler() const
    {
        return this->m_requesthandler;
    };
private:
    DwPtr<DwHttpRequestHandler> m_requesthandler;
};// end of DwRemoveRequestHandler