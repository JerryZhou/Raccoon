#include "stable.h"

#include "3rdparty/md4.h"
#include "3rdparty/md5.h"
#include "3rdparty/sha1.h"

#include "dwcryptographichash.h"

class DwCryptographicHashPrivate
{
public:
    DwCryptographicHash::Algorithm m_method;
    union
    {
        Dw::MD5Context m_md5Context;
        Dw::MD4Context m_md4Context;
        Dw::Sha1State m_sha1Context;
    };
    DwByteArray m_result;
};


DwCryptographicHash::DwCryptographicHash( Algorithm method )
    : d_ptr(new DwCryptographicHashPrivate)
{
    d_ptr->m_method = method;
    reset();
}

DwCryptographicHash::~DwCryptographicHash()
{
    DW_SAFE_DELETE(d_ptr);
}

void DwCryptographicHash::reset()
{
    switch (d_ptr->m_method)
    {
    case Md4:
        MD4Init(&d_ptr->m_md4Context);
        break;
    case Md5:
        MD5Init(&d_ptr->m_md5Context);
        break;
    case Sha1:
        Sha1InitState(&d_ptr->m_sha1Context);
        break;
    default:
        break;
    }
    d_ptr->m_result.clear();
}

void DwCryptographicHash::addData( const char *data, int length )
{
    switch (d_ptr->m_method)
    {
    case Md4:
        MD4Update(&d_ptr->m_md4Context, (const unsigned char *)data, length);
        break;
    case Md5:
        MD5Update(&d_ptr->m_md5Context, (const unsigned char *)data, length);
        break;
    case Sha1:
        Sha1Update(&d_ptr->m_sha1Context, (const unsigned char *)data, length);
        break;
    }
    d_ptr->m_result.clear();
}

void DwCryptographicHash::addData( const DwByteArray &data )
{
    addData(data.constData(), data.count());
}

DwByteArray DwCryptographicHash::result() const
{
    if (!d_ptr->m_result.isEmpty())
    {
        return d_ptr->m_result;
    }

    switch (d_ptr->m_method)
    {
    case Md4:
    {
        Dw::MD4Context copy = d_ptr->m_md4Context;
        d_ptr->m_result.resize(Dw::KMD4ResultLen);
        MD4Final(&copy, (unsigned char *)d_ptr->m_result.data());
        break;
    }
    case Md5:
    {
        Dw::MD5Context copy = d_ptr->m_md5Context;
        d_ptr->m_result.resize(16);
        MD5Final(&copy, (unsigned char *)d_ptr->m_result.data());
        break;
    }
    case Sha1:
    {
        Dw::Sha1State copy = d_ptr->m_sha1Context;
        d_ptr->m_result.resize(20);
        Sha1FinalizeState(&copy);
        Sha1ToHash(&copy, (unsigned char *)d_ptr->m_result.data());
        break;
    }
    }

    return d_ptr->m_result;
}

//static
DwByteArray DwCryptographicHash::hash( const DwByteArray &data, Algorithm method )
{
    DwCryptographicHash hash(method);
    hash.addData(data);
    return hash.result();
}

bool DwCryptographicHash::fileMd5( const wchar_t *fileName, DwByteArray &md5Hex )
{
    HANDLE fileHandle = CreateFile(fileName,
                                   GENERIC_READ,
                                   FILE_SHARE_READ,
                                   NULL,
                                   OPEN_EXISTING,
                                   FILE_FLAG_SEQUENTIAL_SCAN,
                                   NULL);
    if(INVALID_HANDLE_VALUE == fileHandle)
    {
        return false;
    }

    LARGE_INTEGER fileSize = {0};
    if(!GetFileSizeEx(fileHandle, &fileSize))
    {
        return false;
    }

    HANDLE fileMapping = CreateFileMappingW(fileHandle,
                                            NULL,
                                            PAGE_READONLY,
                                            fileSize.HighPart,
                                            fileSize.LowPart,
                                            NULL);
    if(NULL == fileMapping)
    {
        CloseHandle(fileHandle);
        return false;
    }

    const unsigned int blockSize = 65536;
    unsigned int curBlockSize = 0;
    LARGE_INTEGER fileOffset = {0};

    DwCryptographicHash hashData(Md5);

    bool isSuccess = true;

    while(fileOffset.QuadPart < fileSize.QuadPart)
    {
        if(fileSize.QuadPart - fileOffset.QuadPart > blockSize)
        {
            curBlockSize = blockSize;
        }
        else
        {
            curBlockSize = static_cast<unsigned int>(fileSize.QuadPart - fileOffset.QuadPart);
        }

        const char *buf = (const char*)MapViewOfFile(fileMapping,
                          FILE_MAP_READ,
                          fileOffset.HighPart,
                          fileOffset.LowPart,
                          curBlockSize);

        if(buf)
        {
            hashData.addData((const char *)(buf), curBlockSize);
            UnmapViewOfFile(buf);
        }
        else
        {
            isSuccess = false;
            break;
        }

        fileOffset.QuadPart += curBlockSize;
    }

    CloseHandle(fileMapping);
    CloseHandle(fileHandle);

    md5Hex = hashData.result().toHex();

    return isSuccess;
}
