#pragma once
#include "dwluabind/dwluaexport.h"

//------------------------------------------------------------------------------
class DW_LUABIND_EXPORT DwUIExport : public DwLuaExport
{
    DW_DECLARE_CLASS(DwUIExport);
public:
    DwUIExport();
    virtual ~DwUIExport();

    /// export interface to script
    virtual bool exportd(DwLuaEngine *engine);

    /// listeners
    virtual bool onItemDestroy(DwGraphicsItem *item);

    /// create item in script
    uint32_t createItem(const DwString &s, DwGraphicsItem *parent);
    /// destroy item by script
    int destroyItem(DwGraphicsItem *item);
    /// set property of item by script
    int setProperty(DwGraphicsItem *item, const DwString &key);
    /// get property of item by script
    int propertyOf(DwGraphicsItem *item, const DwString &key);
    /// call function of item by script
    int call(DwGraphicsItem *item, const DwString &key);
    /// register item by script
    int registerItem(DwGraphicsItem *item);
private:
};// end of DwUIExport
DW_REGISTER_CLASS(DwUIExport);