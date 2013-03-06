#pragma once
#include "dwgui/dwrttiobject.h"
#include "dwgui/dwurl.h"
#include "dwcore/dwbytearray.h"

//------------------------------------------------------------------------------
class DwResourceMapping;
class DwEventSignalDispatch;
class DwEvent;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwResourceLib : public DwRttiObject
{
    DW_DECLARE_CLASS(DwResourceLib);
public:
    DwResourceLib();
    virtual ~DwResourceLib();

    // mapping manage
    bool hasMapping(const DwString &scheme) const;
    bool regsterMapping(const DwString &scheme, const DwRtti *mapping);
    DwResourceMapping * mappingOf(const DwString &scheme) const;
    DwVector<DwResourceMapping*> mappings()const;

    // resource manager
    bool hasRes(const DwUrl &url) const;
    DwByteArray lookupRes(const DwUrl &url)const;
    bool appendRes(const DwUrl &url, const DwByteArray &data);

    // async support
    bool isAsyncResource(const DwUrl &url) const;
    DwEventSignalDispatch* eventDispatch() const;
    bool sendEvent(DwEvent *evt);
    void update();

protected:
    DwEventSignalDispatch* m_eventDispatch;
    DwMap<DwString, DwResourceMapping*> m_mappings;
};// end of DwResourceLib

//------------------------------------------------------------------------------
DW_GUI_EXPORT DwResourceLib *dwResourceLib();