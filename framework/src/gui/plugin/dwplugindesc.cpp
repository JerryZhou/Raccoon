#include "stable.h"
#include "dwplugindesc.h"

//------------------------------------------------------------------------------
/**
*/
bool DwPluginDesc::isValid(const DwPluginDesc& idesc)
{
    return !idesc.url.isEmpty()
           && !idesc.rtti
           && !idesc.mappingRtti;
}