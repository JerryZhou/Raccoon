#include "stable.h"
#include "dwresourceprotocol.h"

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwRequestResource, 'rrce', DwMessage);
DW_IMPLEMENT_MESSAGEID(DwRequestResource);

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwResourceLoaded, 'rled', DwMessage);
DW_IMPLEMENT_MESSAGEID(DwResourceLoaded);

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwResourceFailed, 'rfed', DwMessage);
DW_IMPLEMENT_MESSAGEID(DwResourceFailed);

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwResourceHeader, 'rher', DwMessage);
DW_IMPLEMENT_MESSAGEID(DwResourceHeader);

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwResourceProgress, 'rpss', DwMessage);
DW_IMPLEMENT_MESSAGEID(DwResourceProgress);