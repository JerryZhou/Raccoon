#include "stable.h"
#include "dwresourcemappingzip.h"
#include "dwresourcelib.h"
#include "dwcore/dwzip.h"

DW_IMPLEMENT_CLASS(DwResourceMappingZip, 'RZIP', DwResourceMapping);

//------------------------------------------------------------------------------
/**
*/
bool DwResourceMappingZip::appendArchive( const DwUrl &url )
{
    DW_ASSERT(url.scheme() == "file");
    DwZip zip(url.hostAndLocalPath().toCap());
    if(zip.isExist())
    {
        DwMap<DwByteArray, DwByteArray> datas;
        if(zip.unzip(&datas))
        {
            DwMap<DwByteArray, DwByteArray>::iterator ite = datas.begin();
            while(ite != datas.end())
            {
                if (ite->first.size() > 0)
                {
                    appendRes(DwUrl(DwString("zip:///") + ite->first.data()), ite->second);
                }
                ++ite;
            }

            return true;
        }
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool initZipDwResource(const DwUrl &url)
{
    DwResourceMapping *  mapping = dwResourceLib()->mappingOf("zip");
    if (!mapping)
    {
        dwResourceLib()->regsterMapping("zip", DwResourceMappingZip::RTTI());
    }

    DwResourceMappingZip * zipMapping = dwsafe_cast<DwResourceMappingZip>(dwResourceLib()->mappingOf("zip"));
    if (zipMapping)
    {
        return zipMapping->appendArchive(url);
    }

    return false;
}