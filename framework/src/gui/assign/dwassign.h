#pragma once

#include "dwcore/dwstring.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwAssign : public eastl::pair<DwString, DwString>
{
public:
    /// default constructor
    DwAssign();
    /// constructor with assign name and path
    DwAssign(const DwString& n, const DwString& p);
    /// get name of assign
    const DwString& name() const;
    /// get path of assign
    const DwString& path() const;
};

//------------------------------------------------------------------------------
/**
*/
inline DwAssign::DwAssign()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
inline DwAssign::DwAssign(const DwString& n, const DwString& p)
{
    first = n;
    second = p;
}

//------------------------------------------------------------------------------
/**
*/
inline const DwString& DwAssign::name()const
{
    return first;
}

//------------------------------------------------------------------------------
/**
*/
inline const DwString& DwAssign::path()const
{
    return second;
}