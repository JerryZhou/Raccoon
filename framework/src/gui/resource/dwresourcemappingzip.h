#pragma once
#include "dwgui/dwresourcemapping.h"

// zip:///localpath/*.*
//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwResourceMappingZip : public DwResourceMapping
{
    DW_DECLARE_CLASS(DwResourceMappingZip);
public:
    // zip Archive, url must be the file:/// scheme
    bool appendArchive(const DwUrl &url);
protected:
private:
};// end of DwResourceMappingZip

//------------------------------------------------------------------------------
DW_GUI_EXPORT bool initZipDwResource(const DwUrl &url);