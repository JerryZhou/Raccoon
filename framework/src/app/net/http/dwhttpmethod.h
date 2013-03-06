#pragma once

#include "dwcore/dwstring.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwHttpMethod
{
public:
    /// http methods
    enum Code
    {
        Options,
        Get,
        Head,
        Post,
        Put,
        Delete,
        Trace,
        Connect,
        
        NumHttpMethods,
        InvalidHttpMethod,
    };
    
    /// convert from string
    static Code fromString(const DwString& str);
    /// convert to string
    static DwString toString(Code c);
};// end of DwHttpMethod

//------------------------------------------------------------------------------
/**
*/
inline DwHttpMethod::Code
DwHttpMethod::fromString(const DwString& str)
{
    if (str == "OPTIONS") return Options;
    else if (str == "GET") return Get;
    else if (str == "HEAD") return Head;
    else if (str == "POST") return Post;
    else if (str == "PUT") return Put;
    else if (str == "DELETE") return Delete;
    else if (str == "TRACE") return Trace;
    else if (str == "CONNECT") return Connect;
    else
    {
        return InvalidHttpMethod;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline DwString
DwHttpMethod::toString(Code c)
{
    switch (c)
    {
        case Options:   return "OPTIONS";
        case Get:       return "GET";
        case Head:      return "HEAD";
        case Post:      return "POST";
        case Put:       return "PUT";
        case Delete:    return "DELETE";
        case Trace:     return "TRACE";
        case Connect:   return "CONNECT";
        default:
            return "InvalidHttpMethod";
    }
}
    