#pragma once

#include "dwapp/dwinterfacehandlerbase.h"
#include "dwapp/dwmessage.h"
#include "dwapp/dwhttpserver.h"
#include "dwapp/dwhttpprotocol.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwHttpMessageHandler : public DwInterfaceHandlerBase
{
    DW_DECLARE_CLASS(DwHttpMessageHandler);
public:
    /// constructor
    DwHttpMessageHandler();
    /// destructor
    virtual ~DwHttpMessageHandler();

    /// open the handler
    virtual void open();
    /// close the handler
    virtual void close();
    /// handle a message, return true if handled
    virtual bool handleMessage(const DwPtr<DwMessage>& msg);
    /// do per-frame work
    virtual void doWork();

private:
    static const uint16_t DefaultTcpPort = 2100;

    /// handle AttachRequestHandler message
    void onAttachRequestHandler(const DwPtr<DwAttachRequestHandler>& msg);
    /// handle RemoveRequestHandler message
    void onRemoveRequestHandler(const DwPtr<DwRemoveRequestHandler>& msg);

    DwPtr<DwHttpServer> m_httpServer;
};// end of DwHttpMessageHandler
