#pragma once

#include "dwcore/dwstring.h"

//------------------------------------------------------------------------------
class DwFileTime
{
public:
    /// constructor
    DwFileTime();
    /// construct from string
    DwFileTime(const DwString& str);
    /// operator ==
    friend bool operator==(const DwFileTime& a, const DwFileTime& b);
    /// operator !=
    friend bool operator!=(const DwFileTime& a, const DwFileTime& b);
    /// operator >
    friend bool operator>(const DwFileTime& a, const DwFileTime& b);
    /// operator <
    friend bool operator<(const DwFileTime& a, const DwFileTime& b);
    /// convert to string
    DwString AsString() const;

    FILETIME time;
};

//------------------------------------------------------------------------------
/**
*/
inline
DwFileTime::DwFileTime()
{
    time.dwLowDateTime = 0;
    time.dwHighDateTime = 0;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
operator==(const DwFileTime& a, const DwFileTime& b)
{
    return (0 == CompareFileTime(&(a.time), &(b.time)));
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
operator!=(const DwFileTime& a, const DwFileTime& b)
{
    return (0 != CompareFileTime(&(a.time), &(b.time)));
}

//------------------------------------------------------------------------------
/**
*/
inline bool
operator>(const DwFileTime& a, const DwFileTime& b)
{
    return (1 == CompareFileTime(&(a.time), &(b.time)));
}

//------------------------------------------------------------------------------
/**
*/
inline bool
operator <(const DwFileTime& a, const DwFileTime& b)
{
    return (-1 == CompareFileTime(&(a.time), &(b.time)));
}
