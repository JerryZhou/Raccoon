#include "stable.h"

#include <direct.h>

#include "dwcore/dwbytearray.h"

#include "dwcore/dwfile.h"
#include "dwcore/dwbuffer.h"
#include "dwziparchive.h"

//////////////////////////////////////////////////////////////////////////

class DwZipFilePrivate
{
public:
    DwZipFilePrivate(const DwByteArray& fileName);

    DwByteArray	m_fileName;
    DwBuffer	m_buffer;
};

DwZipFilePrivate::DwZipFilePrivate( const DwByteArray& fileName )
    : m_fileName(fileName)
{

}

//////////////////////////////////////////////////////////////////////////

DwZipFile::DwZipFile(const DwByteArray& fileName)
    : d_ptr(new DwZipFilePrivate(fileName))
{

}

DwZipFile::~DwZipFile()
{
    if (d_ptr->m_buffer.isOpen())
    {
        close();
    }
    DW_SAFE_DELETE(d_ptr);
}

bool DwZipFile::open( DwIODevice::OpenMode flags )
{
    return d_ptr->m_buffer.open(flags);
}

void DwZipFile::close()
{
    return d_ptr->m_buffer.close();
}

int64_t DwZipFile::size() const
{
    return d_ptr->m_buffer.size();
}

int64_t DwZipFile::pos() const
{
    return d_ptr->m_buffer.pos();
}

bool DwZipFile::seek( int64_t pos )
{
    return d_ptr->m_buffer.seek(pos);
}

bool DwZipFile::atEnd() const
{
    return d_ptr->m_buffer.atEnd();
}

int64_t DwZipFile::read(char *data, const int64_t len )
{
    return d_ptr->m_buffer.read(data, len);
}

int64_t DwZipFile::write( const DwByteArray& data )
{
    return d_ptr->m_buffer.write(data);
}

int64_t DwZipFile::saveToDisk( const DwByteArray& dir )
{
    DwByteArray path = dir + (dir.isEmpty() ? "" : "/") + d_ptr->m_fileName;
    DwFile file(path.data());
    if (file.open(DwIODevice::WriteOnly))
    {
        int64_t len = file.write(d_ptr->m_buffer.buffer());
        file.close();
        return len;
    }
    return false;
}

const DwByteArray & DwZipFile::readAll() const
{
    return d_ptr->m_buffer.buffer();
}

void DwZipFile::setData( DwByteArray &data )
{
    d_ptr->m_buffer.setBuffer(&data);
}

void DwZipFile::setName( const DwByteArray& fileName )
{
    d_ptr->m_fileName = fileName;
}

DwByteArray DwZipFile::name() const
{
    return d_ptr->m_fileName;
}


//////////////////////////////////////////////////////////////////////////

const int KDefaultCompressLevel = 6;
const bool KIsExcludePath		= false;
const bool KCaseSensitivity		= false;
const int KMaxFileName			= 256;


class DwZipArchivePrivate
{
public:
    DwZipArchivePrivate(const DwByteArray& zipName);
    ~DwZipArchivePrivate();

    DwByteArray	m_zipName;
    zipFile		m_zipFile;
    unzFile		m_unzFile;
    DwZipUtils::ZipMode	m_zipMode;
    int			m_filesCount;
    unsigned long m_commentSize;
};

DwZipArchivePrivate::DwZipArchivePrivate( const DwByteArray& zipName )
    : m_zipName(zipName)
    , m_zipFile(NULL)
    , m_unzFile(NULL)
    , m_zipMode(DwZipUtils::AppendToZip)
    , m_filesCount(-1)
    , m_commentSize(-1)
{

}

DwZipArchivePrivate::~DwZipArchivePrivate()
{

}


//////////////////////////////////////////////////////////////////////////


DwZipArchive::DwZipArchive(const DwByteArray& zipName)
    : d_ptr(new DwZipArchivePrivate(zipName))
{
}

DwZipArchive::~DwZipArchive()
{
    close();
    DW_SAFE_DELETE(d_ptr);
}

bool DwZipArchive::open( const OpenMode mode /*= View*/ )
{
    if (d_ptr->m_zipName.isEmpty())
    {
        return false;
    }
    switch(mode)
    {
    case View:
    case UnZip:
        if (DwFile::exist(d_ptr->m_zipName.data()))
        {
            DwZipUtils::getUnzFile(d_ptr->m_zipName.data(), d_ptr->m_unzFile);
            if (d_ptr->m_unzFile)
            {
                unz_global_info64 globalInfo;
                unzGetGlobalInfo64(d_ptr->m_unzFile, &globalInfo);
                d_ptr->m_filesCount = (int)globalInfo.number_entry;
                d_ptr->m_commentSize = globalInfo.size_comment;
            }
            return d_ptr->m_unzFile != NULL;
        }
        break;
    case Zip:
    {
        int overWriteOption = DwZipUtils::zipModeToInt(d_ptr->m_zipName, d_ptr->m_zipMode);
        d_ptr->m_zipFile = DwZipUtils::getZipFile(d_ptr->m_zipName.data(), overWriteOption);
        return d_ptr->m_zipFile != NULL;
    }
    default:
        break;
    }
    return false;
}

bool DwZipArchive::close()
{
    int zipRet = ZIP_OK;
    int unzipRet = UNZ_OK;
    if (d_ptr->m_zipFile)
    {
        zipRet = zipClose(d_ptr->m_zipFile, NULL);
        d_ptr->m_zipFile = NULL;
    }
    if (d_ptr->m_unzFile)
    {
        unzipRet = unzClose(d_ptr->m_unzFile);
        d_ptr->m_unzFile = NULL;
    }
    return  zipRet == UNZ_OK && unzipRet == UNZ_OK;
}

bool DwZipArchive::addFile( const DwByteArray& fileName )
{
    if (!d_ptr->m_zipFile)
    {
        return false;
    }
    if (fileName.isEmpty() || !DwFile::exist(fileName.data()))
    {
        return false;
    }

    return ZIP_OK == DwZipUtils::addFileToZip(d_ptr->m_zipFile, fileName.data(), KIsExcludePath, KDefaultCompressLevel, NULL);
}

bool DwZipArchive::addFile( DwZipFile& zipFile )
{
    if (!d_ptr->m_zipFile)
    {
        return false;
    }
    if (zipFile.size() <= 0)
    {
        return false;
    }
    return ZIP_OK == DwZipUtils::addDataToZip(d_ptr->m_zipFile, zipFile.name().data(), KIsExcludePath, KDefaultCompressLevel, NULL, zipFile);
}

bool DwZipArchive::delFile( const int index )
{
    return delFile( nameByIndex(index) );
}

bool DwZipArchive::delFile( const DwByteArray& locateName )
{
    if (!d_ptr->m_unzFile)
    {
        return false;
    }

    bool suc = DwZipUtils::delFileInZip(d_ptr->m_unzFile, d_ptr->m_zipName, locateName, archiveComment());
    //delFileInZip() has deleted zipFile£¬ need to re-open.
    if(!open(UnZip))
    {
        d_ptr->m_zipFile = NULL;
        d_ptr->m_unzFile = NULL;
        d_ptr->m_filesCount = -1;
        d_ptr->m_commentSize = -1;
    }
    return suc;
}

bool DwZipArchive::extractTo( const DwByteArray& dirName, bool isOverWrite /* = false*/)
{
    if (!d_ptr->m_unzFile)
    {
        return false;
    }

    DwByteArray curDirName;
    if (!_changeToDir(dirName, curDirName))
    {
        return false;
    }
    bool retValue = DwZipUtils::doExtract(d_ptr->m_unzFile, false, isOverWrite, NULL);
    bool chToOldDir = _chdir(curDirName.data()) == 0;
    return retValue && chToOldDir;
}

bool DwZipArchive::extractOneFileTo( const DwByteArray& dirName, const DwByteArray& locateName, bool isOverWrite /*= false*/ )
{
    if (!d_ptr->m_unzFile)
    {
        return false;
    }

    DwByteArray curDirName;
    if (!_changeToDir(dirName, curDirName))
    {
        return false;
    }
    bool retValue = DwZipUtils::doExtractOneFile(d_ptr->m_unzFile, locateName.data(), false, isOverWrite, NULL);
    bool chToOldDir = _chdir(curDirName.data()) == 0;
    return retValue && chToOldDir;
}

void DwZipArchive::setZipMode( DwZipUtils::ZipMode mode )
{
    d_ptr->m_zipMode = mode;
}

int DwZipArchive::count() const
{
    return d_ptr->m_filesCount;
}

bool DwZipArchive::fileByName( const DwByteArray& locateName, DwZipFile& fileOut ) const
{
    DwByteArray *dataOut = new DwByteArray;
    if (d_ptr->m_unzFile)
    {
        if ( DwZipUtils::doExtractOneFileToMemery(d_ptr->m_unzFile, locateName.data(), NULL, *dataOut) )
        {
            if (fileOut.name().isEmpty())
            {
                fileOut.setName(locateName);
            }
            fileOut.setData(*dataOut);
            return true;
        }
    }
    return false;
}

DwByteArray DwZipArchive::nameByIndex( const int index )
{
    if(index < 0 || index >= count())
    {
        return DwByteArray();
    }
    if (d_ptr->m_unzFile)
    {
        unzGoToFirstFile(d_ptr->m_unzFile);
        for (int i = 0; i < index; ++i)
        {
            if (unzGoToNextFile(d_ptr->m_unzFile) != UNZ_OK)
            {
                return DwByteArray();
            }
        }
        return _currentFileName();
    }

    return DwByteArray();
}

int DwZipArchive::indexByName( const DwByteArray& locateName ) const
{
    if (d_ptr->m_unzFile)
    {
        const char *fileInZip = locateName.data();
        if (unzLocateFile(d_ptr->m_unzFile, fileInZip + DwZipUtils::firstIndexOfChar(locateName.data()) + 1, KCaseSensitivity) == UNZ_OK)
        {
            unz64_file_pos filePos;
            unzGetFilePos64(d_ptr->m_unzFile, &filePos);
            return (int)filePos.num_of_file;
        }
    }
    return -1;
}

DwByteArray DwZipArchive::archiveComment() const
{
    char* tmpCom = NULL;
    if (d_ptr->m_commentSize > 0 && d_ptr->m_unzFile)
    {
        tmpCom = new char[d_ptr->m_commentSize + 1];
        if (!tmpCom)
        {
            if ((unsigned int)unzGetGlobalComment(d_ptr->m_unzFile, tmpCom, d_ptr->m_commentSize + 1) == d_ptr->m_commentSize)
            {
                DwByteArray comment(tmpCom);
                DW_SAFE_DELETE_ARRAY(tmpCom);
                return comment;
            }
        }
    }
    return DwByteArray();
}

DwByteArray DwZipArchive::currentPath()
{
    DwByteArray fullName = _currentFileName();
    if (!fullName.isEmpty())
    {
        int index = fullName.lastIndexOf("/");
        return index > 0 ? fullName.left(index) : fullName;
    }
    return DwByteArray();
}

DwByteArray DwZipArchive::_currentFileName()
{
    if (d_ptr->m_unzFile)
    {
        char fileNameInZip[KMaxFileName] = {0};
        unz_file_info64 fileInfo;
        unzGetCurrentFileInfo64(d_ptr->m_unzFile, &fileInfo, fileNameInZip, sizeof(fileNameInZip), NULL, 0, NULL, 0);
        return DwByteArray(fileNameInZip);
    }
    return DwByteArray();
}

bool DwZipArchive::_changeToDir( const DwByteArray& dirName, DwByteArray& curDirName )
{
    char curDir[KMaxFileName + 1] = {0};
    _getcwd(curDir, KMaxFileName);
    curDirName = curDir;
    if (_chdir(dirName.data()) == -1)
    {
        if (DwZipUtils::makeDir(dirName.data()))
        {
            if (_chdir(dirName.data()) == -1)
            {
                return false;//Error changing into extractDir, aborting
            }
        }
        else
        {
            return false;//Error changing into extractDir, aborting
        }
    }
    return true;
}

//DwVector<DwByteArray> DwZipArchive::allFilesName()
//{
//	int fileCount = count();
//	if (fileCount > 0)
//	{
//
//	}
//}
