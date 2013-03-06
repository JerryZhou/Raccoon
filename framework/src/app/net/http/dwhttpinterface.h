#pragma once

#include "dwapp/dwinterfacebase.h"
#include "dwapp/dwhttpserverproxy.h"
#include "dwgui/dwtimer.h"
#include "dwapp/dwsinglton.h"

//------------------------------------------------------------------------------
class DwHttpRequestHandlerProxy;
class DwHttpClientRegistry;

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwHttpInterface : public DwInterfaceBase, public DwSinglton<DwHttpInterface>
{
    DW_DECLARE_CLASS(DwHttpInterface);
public:
    /// constructor
    DwHttpInterface();
    /// destructor
    virtual ~DwHttpInterface();
    /// open the interface object
    virtual void open();
    /// close the interface object
    virtual void close();

    /// attach a request handler to the server
    void attachRequestHandler(const DwPtr<DwHttpRequestHandler>& h);
    /// remove a request handler from the server
    void removeRequestHandler(const DwPtr<DwHttpRequestHandler>& h);

private:
    DwPtr<DwHttpServerProxy> m_serverProxy;
    DwPtr<DwHttpRequestHandlerProxy> m_handlerProxy;
    DwPtr<DwHttpClientRegistry> m_clientRegistrys;
    DwTimer timer;
};// end of DwHttpInterface