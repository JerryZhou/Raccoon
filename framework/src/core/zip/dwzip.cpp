#include "stable.h"

#include <direct.h>

#include "../3rdparty/zlib/zlib.h"

#include "dwcore/dwbytearray.h"
#include "dwcore/dwfile.h"

#include "dwzip.h"

//////////////////////////////////////////////////////////////////////////
class DwZipPrivate
{
public:
    DwZipPrivate(const DwByteArray& zipName);
    ~DwZipPrivate();

    DwByteArray m_zipName;
    DwByteArray m_curDir;
    DwByteArray	m_password;
    int			m_compressLevel;
    bool		m_isZipExcludePath;
    bool		m_isUnzipExcludePath;
};

DwZipPrivate::DwZipPrivate(const DwByteArray& zipName)
    : m_zipName(zipName)
    , m_compressLevel(6)
    , m_isZipExcludePath(false)
    , m_isUnzipExcludePath(false)
{

}

DwZipPrivate::~DwZipPrivate()
{

}


//////////////////////////////////////////////////////////////////////////

const int KMaxFileName = 256;
const int KWriteBufferSize = 16384;

DwZip::DwZip( const DwByteArray& zipName )
    : d_ptr(new DwZipPrivate(zipName))
{
}

DwZip::~DwZip()
{
    DW_SAFE_DELETE(d_ptr);
}

void DwZip::setPassword( DwByteArray pwd )
{
    d_ptr->m_password = pwd;
}

void DwZip::setCompressLevel( int level )
{
    d_ptr->m_compressLevel = level;
}

void DwZip::setZipExcludePath( bool isExclude )
{
    d_ptr->m_isZipExcludePath = isExclude;
}

void DwZip::setUnzipExcludePath(bool isExclude)
{
    d_ptr->m_isUnzipExcludePath = isExclude;
}

bool DwZip::isExist()
{
    return DwFile::exist(d_ptr->m_zipName.data());
}

bool DwZip::zip( const DwVector<DwByteArray>& filesAddToZip, DwZipUtils::ZipMode zipMode /*= DwZipUtils::CreateZip*/ )
{
    int overWriteOption = DwZipUtils::zipModeToInt(d_ptr->m_zipName, zipMode);
    zipFile zf = DwZipUtils::getZipFile(d_ptr->m_zipName.data(), overWriteOption);
    if (zf == NULL)
    {
        return false;//error opening fileNameTry
    }

    DwVector<const DwByteArray>::const_iterator filesIter;
    int err = ZIP_OK;
    for (filesIter = filesAddToZip.begin(); filesIter != filesAddToZip.end() && (err == ZIP_OK); ++ filesIter)
    {
        err = DwZipUtils::addFileToZip(zf, (*filesIter).data(), d_ptr->m_isZipExcludePath,
                                       d_ptr->m_compressLevel, d_ptr->m_password.isNull() ? NULL : d_ptr->m_password.data());
    }
    return zipClose(zf, NULL) == ZIP_OK;;
}

bool DwZip::unzip( DwMap<DwByteArray, DwByteArray>* fileNameDataMap )
{
    if (!d_ptr->m_zipName.isNull())
    {
        unzFile uf = NULL;
        DwZipUtils::getUnzFile(d_ptr->m_zipName.data(), uf);
        if (uf)
        {
            bool retValue = DwZipUtils::doExtractToMemery(uf, d_ptr->m_password.isNull() ? NULL : d_ptr->m_password.data(), fileNameDataMap);
            unzClose(uf);
            return retValue;
        }
    }
    return false;
}

bool DwZip::unzip( const DwByteArray& fileToUnzip, DwByteArray& dataOut )
{
    if (!d_ptr->m_zipName.isNull())
    {
        unzFile uf = NULL;
        DwZipUtils::getUnzFile(d_ptr->m_zipName.data(), uf);
        if (uf)
        {
            bool retValue = DwZipUtils::doExtractOneFileToMemery(uf, fileToUnzip.data(), d_ptr->m_password.isNull() ? NULL : d_ptr->m_password.data(), dataOut);
            unzClose(uf);
            return retValue;
        }
    }
    return false;
}

bool DwZip::unzip( const DwByteArray& extractDir, bool isOverWrite /*= false*/ )
{
    if (!d_ptr->m_zipName.isNull())
    {
        unzFile uf = NULL;
        DwZipUtils::getUnzFile(d_ptr->m_zipName.data(), uf);
        if (uf == NULL)
        {
            return false;
        }

        char buf[KMaxFileName + 1] = {0};
        _getcwd(buf, KMaxFileName);
        d_ptr->m_curDir = buf;
        if (_chdir(extractDir.data()) == -1)
        {
            if (DwZipUtils::makeDir(extractDir.data()))
            {
                if (_chdir(extractDir.data()) == -1)
                {
                    unzClose(uf);
                    return false;//Error changing into extractDir, aborting
                }
            }
            else
            {
                unzClose(uf);
                return false;//Error changing into extractDir, aborting
            }
        }
        bool retValue = DwZipUtils::doExtract(uf, d_ptr->m_isUnzipExcludePath, isOverWrite, d_ptr->m_password.isNull() ? NULL : d_ptr->m_password.data());
        _chdir(d_ptr->m_curDir.data());
        unzClose(uf);
        return retValue;
    }
    return false;
}


//////////////////////////////////////////////////////////////////////////


DwGZip::DwGZip(int buffLen)
    : m_buffLen(buffLen)
    , m_buff(NULL)
{
    m_buff = new unsigned char[buffLen];
}

DwGZip::~DwGZip()
{
    DW_SAFE_DELETE_ARRAY(m_buff);
}

bool DwGZip::ungzip(DwByteArray& gzDataIn, int len, DwByteArray& dataOut)
{
    int err = 0;
    unsigned long out_count = 0;
    z_stream d_stream = {0}; /* decompression stream */

    d_stream.zalloc = (alloc_func)0;
    d_stream.zfree = (free_func)0;
    d_stream.opaque = (voidpf)0;
    d_stream.next_in = (Bytef*)gzDataIn.data();
    d_stream.avail_in = len;
    d_stream.avail_out = m_buffLen;
    d_stream.next_out = m_buff;

    if(inflateInit2(&d_stream, 47) != Z_OK)
    {
        return false;
    }

    dataOut.clear();

    uLong uLen = (uLong)len;
    while (d_stream.total_in < uLen)
    {
        if((err = inflate(&d_stream, Z_SYNC_FLUSH)) == Z_STREAM_END)
        {
            dataOut.append((const char*)m_buff, d_stream.total_out - out_count);
            err = inflateEnd(&d_stream);
            break;
        }

        if(err == Z_OK)
        {
            dataOut.append((const char*)m_buff, d_stream.total_out - out_count);
            out_count = d_stream.total_out;
            d_stream.avail_out = m_buffLen;
            d_stream.next_out = m_buff;
        }
        else
        {
            return false;
        }
    }
    return err == Z_OK;
}

bool DwGZip::gzipex(DwByteArray& gzDataIn, int len, DwByteArray& dataOut)
{
    int err = 0;
    z_stream c_stream = { 0 }; /* compression stream */

    int destlen = len + (len >> 12) + (len >> 14) + (len >> 25) + 13;

    if (gzDataIn.isNull() || len <= 0)
    {
        return false;
    }
    dataOut.clear();
    char *buf = new char[destlen];

    c_stream.zalloc = (alloc_func)0;
    c_stream.zfree = (free_func)0;
    c_stream.opaque = (voidpf)0;
    c_stream.next_in = (Bytef*)gzDataIn.data();
    c_stream.avail_in = len;
    c_stream.avail_out = destlen;
    c_stream.next_out = (Bytef*)buf;

    err = deflateInit(&c_stream, Z_BEST_COMPRESSION);
    if (err == Z_OK)
    {
        err = deflate(&c_stream, Z_FINISH);
        if (err != Z_STREAM_END)
        {
            deflateEnd(&c_stream);
            //if (Z_OK == err)
            //{
            //	err = Z_BUF_ERROR;
            //}
            DW_SAFE_DELETE_ARRAY(buf);
            return false;
        }

        err = deflateEnd(&c_stream);
        if (err == Z_OK)
        {
            dataOut.append(buf, c_stream.total_out);
            dataOut.append((const char*)&len, sizeof(len));
        }
    }

    DW_SAFE_DELETE_ARRAY(buf);
    return err == Z_OK;
}

bool DwGZip::ungzipex(DwByteArray& gzDataIn, int len, DwByteArray& dataOut)
{
    z_stream stream = { 0 };
    int err = Z_OK;
    int destlen = 0;

    if (gzDataIn.isNull() ||len <= sizeof(stream.total_out))
    {
        return false;
    }
    dataOut.clear();
    ::memcpy(&destlen, (void*)&(gzDataIn.data())[len - sizeof(stream.total_out)], sizeof(stream.total_out));
    if (destlen <= 0 || destlen > 0x0fffffff) // 255 MB
    {
        return false;
    }
    char *buf = new char[destlen];

    stream.next_in = (Bytef*)gzDataIn.data();
    stream.avail_in = (uInt)len - sizeof(stream.total_out);
    stream.next_out = (Bytef*)buf;
    stream.avail_out = (uInt)destlen;
    stream.zalloc = (alloc_func)0;
    stream.zfree = (free_func)0;

    err = inflateInit(&stream);
    if (err == Z_OK)
    {
        err = inflate(&stream, Z_FINISH);
        if (err != Z_STREAM_END)
        {
            inflateEnd(&stream);
            //if (err == Z_NEED_DICT || (err == Z_BUF_ERROR && stream.avail_in == 0))
            //{
            //	err = Z_DATA_ERROR;
            //}
            DW_SAFE_DELETE_ARRAY(buf);
            return false;//err == Z_OK;
        }

        err = inflateEnd(&stream);
        if (Z_OK == err)
        {
            dataOut.append(buf, stream.total_out);
        }
    }

    DW_SAFE_DELETE_ARRAY(buf);
    return err == Z_OK;
}