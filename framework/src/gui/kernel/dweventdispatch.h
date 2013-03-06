#pragma once
#include "dwcore/dwvector.h"

//------------------------------------------------------------------------------
class DwEvent;
class DwEventHandler;

// TODO: abstract a conception : port , to delegating the item, view, and scene
// dispatch --> port ---> handler
//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwEventDispatch
{
public:
    DwEventDispatch();
    virtual ~DwEventDispatch();
};// end of DwEventDispatch