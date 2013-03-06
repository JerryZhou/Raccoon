#include "dwstable.h"
#include "dwresourcemappingfile.h"

DW_IMPLEMENT_CLASS(DwResourceMappingFile, 'RMFE', DwResourceMapping);

//------------------------------------------------------------------------------
/**
*/
bool DwResourceMappingFile::hasRes(const DwUrl &url) const
{
    return DwFile::exist(url.localPath());
}

//------------------------------------------------------------------------------
/**
*/
DwByteArray DwResourceMappingFile::lookupRes(const DwUrl &url) const
{
    DwMap<DwString, DwByteArray>::const_iterator ite = m_datas.find(url.asString());
    if (ite != m_datas.end())
    {
        return ite->second;
    }
    DwFile file(url.localPath());
    bool isOk = file.open(DwIODevice::ReadOnly);
    DwByteArray data;
    if (isOk)
    {
        data = file.readAll();
        // we do not cache the resource,
        // if there are some effective important res,
        // suggest cached handful
        const_cast<DwResourceMappingFile*>(this)->appendRes(url, data);
        file.close();
    }

    return data;
}