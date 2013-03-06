#pragma once

#include "dwapp/dwmessage.h"
#include "dwapp/dwhttprequesthandler.h"
#include "dwapp/dwhttpstatus.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwRequestResource : public DwMessage
{
    DW_DECLARE_CLASS(DwRequestResource);
    DW_DECLARE_MESSAGEID;
public:
    DwRequestResource() 
    { };

    void setURL(const DwUrl &u) { url = u; }

    DwUrl url;
};// end of DwRequestResource

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwResourceLoaded : public DwMessage
{
    DW_DECLARE_CLASS(DwResourceLoaded);
    DW_DECLARE_MESSAGEID;
public:
    DwResourceLoaded() 
    { result = true; };

    DwUrl url;
    DwByteArray data;
    bool result;
};// end of DwResourceLoaded

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwResourceFailed : public DwMessage
{
    DW_DECLARE_CLASS(DwResourceFailed);
    DW_DECLARE_MESSAGEID;
public:
    DwResourceFailed() 
    {
        status = DwHttpStatus::InvalidHttpStatus;
    }
    DwUrl url;
    DwHttpStatus::Code status;
};// end of DwResourceFailed

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwResourceHeader : public DwMessage
{
    DW_DECLARE_CLASS(DwResourceHeader);
    DW_DECLARE_MESSAGEID;
public:
    DwResourceHeader() 
    {
        contentLength = 0;
    }

    DwUrl url;
    int contentLength;
};// end of DwResourceHeader

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwResourceProgress : public DwMessage
{
    DW_DECLARE_CLASS(DwResourceProgress);
    DW_DECLARE_MESSAGEID;
public:
    DwResourceProgress() 
    {
        contentLength = 0;
        progress = 0;
    }

    DwUrl url;
    int contentLength;
    int progress;
};// end of DwResourceProgress