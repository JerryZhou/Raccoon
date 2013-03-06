
#include "stable.h"
#include "dwhttpinterface.h"
#include "dwapp/dwhttpmessagehandler.h"
#include "dwapp/dwrunthroughhandlerthread.h"
#include "dwapp/dwhttpclientregistry.h"
#include "dwapp/dwresourcehandlerhttp.h"
#include "dwapp/dwresourcemappinghttp.h"
#include "dwgui/dwresourcelib.h"

/// handlers
#include "net/http/debug/dwhelloworldrequesthandler.h"
#include "net/http/debug/dwsvgtestpagehandler.h"
#include "debug/handlers/dwrttiobjecthandler.h"
#include "debug/handlers/dwresourcehttpdebughandler.h"
#include "debug/handlers/dwringloghttphandler.h"
#include "debug/handlers/dwimageshttphandler.h"

DW_IMPLEMENT_CLASS(DwHttpInterface, 'HTIF', DwInterfaceBase);

class DwHttpRequestHandlerProxy : public DwRefCounted, public sigslot::has_slots
{
    DW_DECLARE_CLASS(DwHttpRequestHandlerProxy);
public:
    DwHttpRequestHandlerProxy()
        : httpMapping(NULL)
    {
        ;
    }
    void open()
    {
        ;
    }

    void close()
    {
        ;
    }

    void setHttpMapping(DwResourceMappingHttp *mapping) { httpMapping = mapping; }
    DwResourceMappingHttp* httpMapping;

    void onPendingRequst()
    {
        DwHttpServerProxy::Instance()->handlePendingRequests();

        // jugle it
        if (httpMapping)
        {
            httpMapping->update();
        }
    }
};
DW_IMPLEMENT_CLASS(DwHttpRequestHandlerProxy, 'RHPY', DwRefCounted);


//------------------------------------------------------------------------------
/**
*/
DwHttpInterface::DwHttpInterface()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwHttpInterface::~DwHttpInterface()
{
    DW_ASSERT(!isOpen());
    m_serverProxy = NULL;
    m_handlerProxy = NULL;
}

//------------------------------------------------------------------------------
/**
*/
void
DwHttpInterface::open()
{
    // setup runthrough handler thread
    DwPtr<DwRunThroughHandlerThread> handlerThread = DwRunThroughHandlerThread::create();
    handlerThread->setName("HttpInterface Thread");
    handlerThread->setCoreId(DwCpu::MiscThreadCore);
    handlerThread->attachHandler(DwHttpMessageHandler::create());
    // deal with http resource
    dwResourceLib()->regsterMapping("http", DwResourceMappingHttp::RTTI());
    DwResourceMappingHttp *mapping = dwsafe_cast<DwResourceMappingHttp>(dwResourceLib()->mappingOf("http"));
    DwPtr<DwResourceHandlerHttp> resourceHttp = DwResourceHandlerHttp::create();
    resourceHttp->setHttpMapping(mapping);
    handlerThread->attachHandler(resourceHttp.cast<DwHandler>());

    this->setHandlerThread(handlerThread.cast<DwHandlerThreadBase>());

    m_serverProxy = DwHttpServerProxy::create();
    m_serverProxy->open();

    m_handlerProxy = DwHttpRequestHandlerProxy::create();
    m_handlerProxy->open();
    m_handlerProxy->setHttpMapping(mapping);

    m_clientRegistrys = DwHttpClientRegistry::create();
    m_clientRegistrys->setup();

    timer.sigTimeout.connect(m_handlerProxy.get(), &DwHttpRequestHandlerProxy::onPendingRequst);
    timer.start(50);

    DwInterfaceBase::open();

    attachRequestHandler(DwRttiObjectHandler::create());
    attachRequestHandler(DwResourceHttpDebugHandler::create());
    attachRequestHandler(DwRingLogHttpHandler::create());
    attachRequestHandler(DwImagesHttpHandler::create());
}

//------------------------------------------------------------------------------
/**
*/
void 
DwHttpInterface::close()
{
    DwInterfaceBase::close();

    m_clientRegistrys->discard();

    m_serverProxy->close();
    m_serverProxy = NULL;

    m_handlerProxy->close();
    m_handlerProxy = NULL;
}

//------------------------------------------------------------------------------
/**
*/
void 
DwHttpInterface::attachRequestHandler(const DwPtr<DwHttpRequestHandler>& h)
{
    if (m_serverProxy)
    {
        m_serverProxy->attachRequestHandler(h);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwHttpInterface::removeRequestHandler(const DwPtr<DwHttpRequestHandler>& h)
{
    if (m_serverProxy)
    {
        m_serverProxy->removeRequestHandler(h);
    }
}
