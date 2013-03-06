#include "stable.h"
#include "dwpluginversion.h"

//------------------------------------------------------------------------------
/**
*/
DwPluginVersion::DwPluginVersion()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwPluginVersion::DwPluginVersion(const DwVersion& other)
{
    version = other.version;
}

//------------------------------------------------------------------------------
/**
*/
bool DwPluginVersion::isCompatibleWith(const DwPluginVersion &ver) const
{
    return isCompatibleWith((const DwVersion&)ver);
}

//------------------------------------------------------------------------------
/**
    NOW WE DO NOT BINARY COMPATIBLE, SO WE ONLY CAPATIBLE THE SAME VERSION
*/
bool DwPluginVersion::isCompatibleWith(const DwVersion& ver) const
{
    if (ver.version == version)
    {
        return true;
    }
    return false;
}