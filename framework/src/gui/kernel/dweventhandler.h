#pragma once
#include "dwevent.h"
#include "dwgui/dwrttiobject.h"

//------------------------------------------------------------------------------
class DwEventPort;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwEventHandler : public DwRttiObject
{
    DW_DECLARE_CLASS(DwEventHandler);
public:
    DwEventHandler();
    virtual ~DwEventHandler();

    virtual void onEvent(DwEvent* evt);

    virtual void onAttachd(DwEventPort* port);

    virtual void onDetach(DwEventPort* port);

protected:
private:
};// end of DwEventHandler
DW_REGISTER_CLASS(DwEventHandler);