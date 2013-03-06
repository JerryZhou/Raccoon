#include "stable.h"
#include "dwmessage.h"

DW_IMPLEMENT_CLASS(DwMessage, 'MAGE', DwRefCounted);
DW_IMPLEMENT_MESSAGEID(DwMessage);


//------------------------------------------------------------------------------
/**
*/
DwMessage::DwMessage() :
m_handled(0),
m_deferred(false),
m_deferredHandled(false)
{
    // empty
}
