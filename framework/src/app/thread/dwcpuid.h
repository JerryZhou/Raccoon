#pragma once

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwCpu
{
public:
    typedef unsigned int CoreId;

    /// core identifiers, under Win32, we basically don't care...
    static const CoreId InvalidCoreId       = 0xffffffff;
    static const CoreId MainThreadCore      = 0;
    static const CoreId IoThreadCore        = 2;
    static const CoreId RenderThreadCore    = 1;
    static const CoreId AudioThreadCore     = 3;
    static const CoreId MiscThreadCore      = 4;
    static const CoreId NetworkThreadCore   = 5;

    static const CoreId JobThreadFirstCore  = 6;
};// end of DwCpu