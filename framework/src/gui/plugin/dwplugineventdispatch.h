#pragma once
#include "dwgui/dweventdispatch.h"

//------------------------------------------------------------------------------
class DwPlugin;
class DwPluginEvent;
class DwPluginLib;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwPluginEventDispatch : public DwEventDispatch
{
public:
    void dispatchEvent(DwPluginLib *lib, DwPluginEvent *evt);
    void dispatchEvent(DwPlugin * plugin, DwPluginEvent *evt);
protected:
private:
};// end of DwPluginEventDispatch

DW_GUI_EXPORT DwPluginEventDispatch *dwPluginEventDispatch();
