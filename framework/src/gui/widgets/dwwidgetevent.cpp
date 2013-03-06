#include "stable.h"
#include "dwwidgetevent.h"

//------------------------------------------------------------------------------
DW_IMPLEMENT_EVENTID(DwWidgetEvent, DwSceneEvent);

//------------------------------------------------------------------------------
DW_IMPLEMENT_EVENTID(DwWidgetWindowEvent, DwWidgetEvent);

//------------------------------------------------------------------------------
DW_IMPLEMENT_EVENTID(DwCheckBoxEvent, DwWidgetEvent);

//------------------------------------------------------------------------------
DW_IMPLEMENT_EVENTID(DwTextEditEvent, DwWidgetEvent);

//------------------------------------------------------------------------------
DW_IMPLEMENT_EVENTID(DwMessageBoxEvent, DwWidgetEvent);

//------------------------------------------------------------------------------
DW_IMPLEMENT_EVENTID(DwScrollBarEvent, DwWidgetEvent);

//------------------------------------------------------------------------------
DW_IMPLEMENT_EVENTID(DwScrollAreaEvent, DwWidgetEvent);

//------------------------------------------------------------------------------
DW_IMPLEMENT_EVENTID(DwMenuEvent, DwWidgetEvent);