#pragma once
#include "dwgui/dwrttiobject.h"
#include "dwluabind/dwbindnamespace.h"

//------------------------------------------------------------------------------
class DwLuaEngine;
class DwGraphicsItem;

//------------------------------------------------------------------------------
class DW_LUABIND_EXPORT DwLuaExport : public DwRttiObject
{
    DW_DECLARE_CLASS(DwLuaExport)
public:
    DwLuaExport();
    virtual ~DwLuaExport();

    virtual bool exportd(DwLuaEngine *e);

    virtual bool onItemCreate(DwGraphicsItem *item);
    virtual bool onItemDestroy(DwGraphicsItem *item);
    virtual bool onEngineCreate(DwLuaEngine *e);
    virtual bool onEngineDestroy(DwLuaEngine *e);

    DwLuaEngine *engine() const;

    void setCallExportWhen(Dw::CallExportWhen, bool b=true);
    bool isCallExportWhen(Dw::CallExportWhen)const;

protected:
    friend class DwLuaEngine;
    void setEngine(DwLuaEngine *e);

    DwLuaEngine *m_engine;
    uint32_t m_whens;
};// end of DwLuaExport