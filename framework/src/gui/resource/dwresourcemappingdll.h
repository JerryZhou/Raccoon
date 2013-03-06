#pragma once
#include "dwgui/dwresourcemapping.h"

// dll:///localpath/*.*
//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwResourceMappingDLL : public DwResourceMapping
{
    DW_DECLARE_CLASS(DwResourceMappingDLL);
public:
    bool appendFrom(int num, const char** keys, const unsigned char* datas, int* lens);
protected:
private:
};// end of DwResourceMappingDLL

//------------------------------------------------------------------------------
DW_GUI_EXPORT bool initDllDwResource(int num, const char** keys, const unsigned char* datas, int* lens);