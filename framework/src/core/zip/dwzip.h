#pragma once

#include "dwcore/dwvector.h"
#include "dwcore/dwmap.h"
#include "dwziputils.h"

class DwByteArray;
class DwZipPrivate;

class DW_CORE_EXPORT DwZip
{
    DW_DISABLE_COPY(DwZip)
public:
    explicit DwZip(const DwByteArray& zipName);
    ~DwZip();

    bool zip(const DwVector<DwByteArray>& filesAddToZip, DwZipUtils::ZipMode zipway = DwZipUtils::CreateZip);
    bool unzip(DwMap<DwByteArray, DwByteArray>* fileNameDataMap);
    bool unzip(const DwByteArray& fileToUnzip, DwByteArray& dataOut);
    bool unzip(const DwByteArray& extractDir, bool isOverWrite = false);

    //DwVector<DwByteArray> filesInZip();
    //bool testUnzip();
    bool isExist();

    void setPassword(DwByteArray pwd);
    void setCompressLevel(int level);
    void setZipExcludePath(bool isExclude);
    void setUnzipExcludePath(bool isExclude);

private:
    DwZipPrivate	*d_ptr;
};


//////////////////////////////////////////////////////////////////////////


class DW_CORE_EXPORT DwGZip
{
public:
    DwGZip(int buffLen = 1024);
    ~DwGZip();

    //gzip
    bool ungzip(DwByteArray& gzDataIn, int len, DwByteArray& dataOut);
    bool gzipex(DwByteArray& gzDataIn, int len, DwByteArray& dataOut);
    bool ungzipex(DwByteArray& gzDataIn, int len, DwByteArray& dataOut);

private:
    int				m_buffLen;
    unsigned char	*m_buff;
};
