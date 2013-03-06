#include "stable.h"
#include "dwversion.h"

//------------------------------------------------------------------------------
/**
*/
bool DwVersion::operator != (const DwVersion &rfs) const
{
    return version != rfs.version;
}

//------------------------------------------------------------------------------
/**
*/
bool DwVersion::operator == (const DwVersion &rfs) const
{
    return version == rfs.version;
}

//------------------------------------------------------------------------------
/**
*/
bool DwVersion::operator <= (const DwVersion &rfs) const
{
    return version <= rfs.version;
}

//------------------------------------------------------------------------------
/**
*/
bool DwVersion::operator < (const DwVersion &rfs) const
{
    return version < rfs.version;
}

//------------------------------------------------------------------------------
/**
*/
bool DwVersion::operator >(const DwVersion &rfs) const
{
    return version > rfs.version;
}

//------------------------------------------------------------------------------
/**
*/
bool DwVersion::operator >=(const DwVersion &rfs) const
{
    return version >= rfs.version;
}