#pragma once
#include "dwgui/dwrttiobject.h"
#include "dwgui/dwurl.h"
#include "dwcore/dwmap.h"

//------------------------------------------------------------------------------
class DwResourceLib;
class DwEventSignalDispatch;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwResourceMapping : public DwRttiObject
{
    DW_DECLARE_CLASS(DwResourceMapping);
public:
    DwResourceMapping();
    virtual ~DwResourceMapping();

    virtual bool hasRes(const DwUrl &url) const;
    virtual DwByteArray lookupRes(const DwUrl &url)const;

    virtual bool appendRes(const DwUrl &url, const DwByteArray &data);
    virtual DwByteArray& resOf(const DwUrl &url);
    virtual bool saveRes(const DwUrl &url)const;

    virtual bool isAsyncResource(const DwUrl &url) const;
    virtual void update();

    const DwMap<DwString, DwByteArray>& datas()const;
    DwString scheme()const;

    DwEventSignalDispatch* eventDispatch() const;

protected:
    friend class DwResourceLib;
    
    void setResourceScheme(const DwString &s);

    DwMap<DwString, DwByteArray> m_datas;
    DwString m_scheme;
};// end of DwResourceMapping