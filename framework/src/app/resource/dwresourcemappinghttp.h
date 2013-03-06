#pragma once
#include "dwgui/dwresourcemapping.h"
#include "dwapp/dwresourceprotocol.h"
#include "dwapp/dwsafequeue.h"
#include "dwapp/dwptr.h"

// http://host/localpath/*.*
//------------------------------------------------------------------------------
class DW_APP_EXPORT DwResourceMappingHttp : public DwResourceMapping
{
    DW_DECLARE_CLASS(DwResourceMappingHttp);
public:
    // imp async http resource
    virtual DwByteArray lookupRes(const DwUrl &url)const;
    virtual bool isAsyncResource(const DwUrl &url) const;
    virtual void update();

    // warn the resource
    bool acquireLink(const DwUrl &url);

    // append resource from http
    void appendResource(const DwPtr<DwMessage> &res);
    void appendResource(const DwVector<DwPtr<DwMessage> > &ress);

protected:
    // run in ui thread
    void handlePendingResources();

private:
    DwHash<DwString, bool> m_pendingRequireLinks;
    DwSafeQueue<DwPtr<DwMessage> > m_pendingResources;
};// end of DwResourceMappingHttp