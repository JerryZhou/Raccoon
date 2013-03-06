#pragma once
#include "dwapp/dwrefcounted.h"
#include "dwapp/dwmessageid.h"
#include "dwapp/dwinterlocked.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwMessage : public DwRefCounted
{
    DW_DECLARE_CLASS(DwMessage);
    DW_DECLARE_MESSAGEID;
public:
    /// constructor
    DwMessage();
    /// return true if message is of the given id
    bool checkId(const DwMessageId& id) const;
    /// set the handled flag
    void setHandled(bool b);
    /// return true if the message has been handled
    bool handled() const;
    /// set deferred flag
    void setDeferred(bool b);
    /// get deferred flag
    bool isDeferred() const;
    /// set the deferred handled flag
    void setDeferredHandled(bool b);
    /// get the deferred handled flag
    bool deferredHandled() const;
protected:
    volatile int m_handled;
    bool m_deferred;
    bool m_deferredHandled;
};// end of DwMessage

//------------------------------------------------------------------------------
/**
*/
inline bool
DwMessage::checkId(const DwMessageId& id) const
{
    return (id == this->GetId());
}

//------------------------------------------------------------------------------
/**
*/
inline void
DwMessage::setHandled(bool b)
{
    DwInterlocked::exchange(&this->m_handled, (int)b);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
DwMessage::handled() const
{
    return 0 != this->m_handled;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DwMessage::setDeferred(bool b)
{
    this->m_deferred = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
DwMessage::isDeferred() const
{
    return this->m_deferred;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DwMessage::setDeferredHandled(bool b)
{
    this->m_deferredHandled = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
DwMessage::deferredHandled() const
{
    return this->m_deferredHandled;
}