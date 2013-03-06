
#include "stable.h"
#include "dwhttpstatus.h"


//------------------------------------------------------------------------------
/**
*/
DwHttpStatus::Code
DwHttpStatus::fromString(const DwString& str)
{
    if (str == "100")      return Continue;
    else if (str == "200") return OK;
    else if (str == "400") return BadRequest;
    else if (str == "403") return Forbidden;
    else if (str == "404") return NotFound;
    else if (str == "405") return MethodNotAllowed;
    else if (str == "406") return NotAcceptable;
    else if (str == "500") return InternalServerError;
    else if (str == "503") return ServiceUnavailable;
    else
    {
        return InvalidHttpStatus;
    }
}

//------------------------------------------------------------------------------
/**
*/
DwString
DwHttpStatus::toString(Code c)
{
    switch (c)
    {
        case Continue:                      return "100";
        case OK:                            return "200";
        case BadRequest:                    return "400";
        case Forbidden:                     return "403";
        case NotFound:                      return "404";
        case InternalServerError:           return "500";
        case NotImplemented:                return "501";
        case ServiceUnavailable:            return "503";
        default:
            DW_ASSERT("Invalid HTTP status code!");
            return "";
    }
}

//------------------------------------------------------------------------------
/**
*/
DwString
DwHttpStatus::toHumanReadableString(Code c)
{
    switch (c)
    {
        case Continue:                      return "Continue";
        case OK:                            return "OK";
        case BadRequest:                    return "Bad Request";
        case Forbidden:                     return "Forbidden";
        case NotFound:                      return "Not Found";
        case InternalServerError:           return "Internal Server Error";
        case NotImplemented:                return "Not Implemented";
        case ServiceUnavailable:            return "Service Unavailable";
        default:
            return "Invalid Status Code";
    }
}
