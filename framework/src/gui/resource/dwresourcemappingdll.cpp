#include "stable.h"
#include "dwresourcemappingdll.h"
#include "dwresourcelib.h"

DW_IMPLEMENT_CLASS(DwResourceMappingDLL, 'RMLL', DwResourceMapping);

//------------------------------------------------------------------------------
/**
*/
bool DwResourceMappingDLL::appendFrom(int num, const char** keys, const unsigned char* datas, int* lens)
{
    int offset = 0;
    for (int i=0; i<num; ++i)
    {
        DwByteArray data((char*)(datas + offset), lens[i]);
        if (data.size() > 0)
        {
            appendRes(DwUrl(DwString("dll:///") + keys[i]), data);
        }
        offset += lens[i];
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool initDllDwResource(int num, const char** keys, const unsigned char* datas, int* lens)
{
    DwResourceMapping *  mapping = dwResourceLib()->mappingOf("dll");
    if (!mapping)
    {
        dwResourceLib()->regsterMapping("dll", DwResourceMappingDLL::RTTI());
    }

    DwResourceMappingDLL * dllMapping = dwsafe_cast<DwResourceMappingDLL>(dwResourceLib()->mappingOf("dll"));
    if (dllMapping)
    {
        dllMapping->appendFrom(num, keys, datas, lens);
    }

    return true;
}