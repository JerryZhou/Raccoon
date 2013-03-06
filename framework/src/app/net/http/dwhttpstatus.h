#pragma once

#include "dwcore/dwstring.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwHttpStatus
{
public:
    /// status codes
    enum Code
    {
        Continue                        = 100,
        OK                              = 200,
        BadRequest                      = 400,
        Forbidden                       = 403,
        NotFound                        = 404,
        MethodNotAllowed                = 405,
        NotAcceptable                   = 406,
        InternalServerError             = 500,
        NotImplemented                  = 501,
        ServiceUnavailable              = 503,

        InvalidHttpStatus = 0xffff
    };

    /// convert from string
    static Code fromString(const DwString& str);
    /// convert to string
    static DwString toString(Code c);
    /// convert code to human readable string
    static DwString toHumanReadableString(Code c);
};// end of DwHttpStatus
