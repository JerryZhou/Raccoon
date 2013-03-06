#pragma once

#include "dwapp/dwrefcounted.h"
#include "dwapp/dwptr.h"
#include "dwapp/dwmessage.h"

//-----------------------------------------------------------------------------
class DW_APP_EXPORT DwHandler : public DwRefCounted
{
    DW_DECLARE_CLASS(DwHandler);
public:
    /// constructor
    DwHandler();
    /// destructor
    virtual ~DwHandler();
    /// called once on startup 
    virtual void open();
    /// called once before shutdown
    virtual void close();
    /// return true if open
    bool isOpen() const;
    /// handle a message, return true if handled
    virtual bool handleMessage(const DwPtr<DwMessage>& msg);
    /// optional "per-frame" DoWork method for continuous handlers
    virtual void doWork();
protected:
    bool m_isOpen;
};// end of DwHandler

//-----------------------------------------------------------------------------
/**
*/
inline bool
DwHandler::isOpen() const
{
    return this->m_isOpen;
}