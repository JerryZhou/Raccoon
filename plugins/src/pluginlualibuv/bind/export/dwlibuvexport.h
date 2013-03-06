#pragma once
#include "dwluabind/dwluaexport.h"

//------------------------------------------------------------------------------
class DwLibuvExport :public DwLuaExport
{
    DW_DECLARE_CLASS(DwLibuvExport)
public:
    DwLibuvExport();
    virtual ~DwLibuvExport();

    virtual bool exportd(DwLuaEngine *e);

    bool run();
    bool run_once();
protected:
private:
};// end of DwLibuvExport