#pragma once
#include "dwgui/dwresourcemapping.h"

// file:///localpath/*.*
//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwResourceMappingFile : public DwResourceMapping
{
    DW_DECLARE_CLASS(DwResourceMappingFile);
public:
    virtual bool hasRes(const DwUrl &url) const;
    virtual DwByteArray lookupRes(const DwUrl &url)const;
protected:
private:
};// end of DwResourceMappingFile