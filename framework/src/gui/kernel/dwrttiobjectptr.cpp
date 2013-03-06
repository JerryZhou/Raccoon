#pragma once
#include "stable.h"
#include "dwrttiobjectptr.h"

// USED TO EXPORT TEMPLATE CLASS
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
*/
DwRRttiObjectPtr::DwRRttiObjectPtr()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwRRttiObjectPtr::DwRRttiObjectPtr(const DwRttiObject* ptr) : DwWeakPtr(ptr)
{
    ;
}