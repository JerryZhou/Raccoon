#include "dwgui/dwsimpleediteventhandler.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwMultiEditEventHandler : public DwSimpleEditEventHandler
{
    DW_DECLARE_CLASS(DwMultiEditEventHandler);

public:
    DwMultiEditEventHandler(DwSimpleEdit* edit = NULL, DwGraphicsBackgroundItem* back = NULL);

    virtual void onKeyDown(DwKeyDownEvent* evt);
    virtual void onCharEvent(DwCharEvent* evt);

protected:
    virtual void updateGraphicsEditToLookCursor();
};// end of DwMultiEditEventHandler
DW_REGISTER_CLASS(DwMultiEditEventHandler);