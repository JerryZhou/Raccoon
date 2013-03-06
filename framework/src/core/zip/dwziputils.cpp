#include "stable.h"

#include <errno.h>
#include <direct.h>

#include "dwcore/dwfile.h"

#include "dwzip.h"
#include "dwziparchive.h"
#include "dwziputils.h"


namespace DwZipUtils
{
const int KWriteBufferSize = 16384;
const int KMaxFileName = 256;
const int KCaseSensitivity = 0;

//char *fileName;		name of file to get info on
//tm_zip *tmzip;		return value: access, modific. and creation times
//uLong *dosTime;		dostime
bool fileTime(const char *fileName, unsigned long *dosTime)
{
    HANDLE hFind;
    WIN32_FIND_DATAA ff32;
    hFind = FindFirstFileA(fileName,&ff32);
    FILETIME ftLocal;
    if (hFind != INVALID_HANDLE_VALUE)
    {
        FileTimeToLocalFileTime(&(ff32.ftLastWriteTime),&ftLocal);
        FindClose(hFind);
    }
    else
    {
        LPSYSTEMTIME lpSystemTime = NULL;
        LPFILETIME   lpFileTime = NULL;
        GetSystemTime(lpSystemTime);
        SystemTimeToFileTime(lpSystemTime, lpFileTime);
        FileTimeToLocalFileTime(lpFileTime, &ftLocal);
    }
    FileTimeToDosDateTime(&ftLocal,((LPWORD)dosTime) + 1,((LPWORD)dosTime) + 0);
    return true;
}

/* calculate the CRC32 of a file,
 * because to encrypt a file, we need known the CRC32 of the file before
 */
int getFileCrc(const char* fileNameInZip, char* buf, unsigned long bufSize, unsigned long* resCrc)
{
    int err = ZIP_OK;
    DwFile file(fileNameInZip);
    if (file.open(DwIODevice::ReadOnly))
    {
        int64_t readSize = 0;
        //unsigned long total_read = 0;
        unsigned long calcCrc = 0;
        do
        {
            err = ZIP_OK;
            readSize = file.read(buf, bufSize);
            if (readSize < bufSize && !file.atEnd())
            {
                err = ZIP_ERRNO;//error in reading filenameinzip
            }
            if (readSize > 0)
            {
                calcCrc = crc32(calcCrc, (const Bytef *)buf, (uInt)readSize);
            }
            //total_read += size_read;
        }
        while ((err == ZIP_OK) && (readSize>0));
        *resCrc = calcCrc;
    }

    file.close();
    return err;
}

bool isLargeFile(const char* fileName)
{
    bool isLargeFile = false;
    DwFile file(fileName);
    if (file.open(DwIODevice::ReadOnly))
    {
        int64_t pos = file.size();

        if(pos >= 0xffffffff)
        {
            isLargeFile = true;
        }
        file.close();
    }
    return isLargeFile;
}

//change_file_date : change the date/time of a file
//filename : the filename of the file where date/time must be modified
//dosdate : the new date at the MSDos format (4 bytes)
void changeFileDate(const char *fileName, unsigned long dosDate)
{
    HANDLE hFile;
    FILETIME ftm, ftLocal, ftCreate, ftLastAcc, ftLastWrite;

    hFile = CreateFileA(fileName, GENERIC_READ | GENERIC_WRITE,
                        0, NULL, OPEN_EXISTING, 0, NULL);
    GetFileTime(hFile, &ftCreate, &ftLastAcc, &ftLastWrite);
    DosDateTimeToFileTime((WORD)(dosDate>>16), (WORD)dosDate, &ftLocal);
    LocalFileTimeToFileTime(&ftLocal, &ftm);
    SetFileTime(hFile, &ftm,&ftLastAcc, &ftm);
    CloseHandle(hFile);
}

bool makeDir(const char *newDir)
{
    int  len = (int)strlen(newDir);

    if (len <= 0)
    {
        return false;
    }

    char *buffer = new char[len+1];
    if (buffer == NULL)
    {
        return false;//Error allocating memory
    }
    strcpy(buffer, newDir);

    if (buffer[len-1] == '/')
    {
        buffer[len-1] = '\0';
    }
    if (_mkdir(buffer) == 0)
    {
        delete[] buffer;
        return true;
    }

    char *temp = buffer + 1;
    while (true)
    {
        char hold;

        while(*temp && *temp != '\\' && *temp != '/')
        {
            temp++;
        }
        hold = *temp;//hole == '\' or '/'
        *temp = 0;
        if ((_mkdir(buffer) == -1) && (errno == ENOENT))
        {
            delete[] buffer;
            return false;//couldn't create directory
        }
        if (hold == 0)
        {
            break;
        }
        *temp++ = hold;
    }
    delete[] buffer;
    return true;
}

const char* getFileNameWithoutPath(const char* fileName)
{
    //"hello/world.txt" will be "world.txt"
    const char* temp = fileName;
    const char* fileNameWithoutPath = fileName;
    while ((*temp) != '\0')
    {
        if (((*temp) == '/') || ((*temp) == '\\'))
        {
            fileNameWithoutPath = temp+1;
        }
        temp++;
    }
    return fileNameWithoutPath;
}

int lastIndexOfSlash(char* fileName)
{
    //"hello/world.txt" will be "world.txt"
    for (int index = strlen(fileName) - 1; index >= 0; -- index)
    {
        if (((fileName[index] == '/') || (fileName[index] == '\\')))
        {
            return index;
        }
    }
    return -1;
}

//"../test" return 2
int firstIndexOfChar( const char* fileName )
{
    int index = 0;
    while( fileName[index] == '\\' || fileName[index] == '/' || fileName[index] == '.')
    {
        ++index;
    }
    return index - 1;
}

bool doExtractCurrentFile(unzFile uf, bool isExtractWithoutPath, bool isOverWrite, const char* password)
{
    char fileNameInZip[KMaxFileName];
    unz_file_info64 fileInfo;
    int err = unzGetCurrentFileInfo64(uf, &fileInfo, fileNameInZip, sizeof(fileNameInZip), NULL, 0, NULL, 0);

    if (err != UNZ_OK)
    {
        return false;//error with zipfile in unzGetCurrentFileInfo
    }

    //"hello/world.txt" will be "world.txt"
    char* fileNameWithoutPath = fileNameInZip + lastIndexOfSlash(fileNameInZip) + 1;

    //fileNameInZip is "hello/", fileNameWithoutPath is NULL, means create dir "hello"
    if ((*fileNameWithoutPath) == '\0')
    {
        if (!isExtractWithoutPath)
        {
            _mkdir(fileNameInZip);//creating directory: fileNameInZip
        }
    }
    else
    {
        const char* writeFileName;
        if (!isExtractWithoutPath)
        {
            writeFileName = fileNameInZip;
        }
        else
        {
            writeFileName = fileNameWithoutPath;
        }

        err = unzOpenCurrentFilePassword(uf, password);

        //is file is exists and not over write, return
        if ((!isOverWrite) && (err == UNZ_OK))
        {
            if (DwFile::exist(writeFileName))
            {
                return false;
            }
        }

        DwFile fileOut(writeFileName);
        bool hasOpened = false;
        if (err == UNZ_OK)
        {
            hasOpened = fileOut.open(DwIODevice::WriteOnly);
            // some zipfile don't contain directory alone before file
            if ((!hasOpened) && (!isExtractWithoutPath) && (fileNameWithoutPath != (char*)fileNameInZip))
            {
                char c = *(fileNameWithoutPath - 1);
                *(fileNameWithoutPath - 1) = '\0';
                makeDir(writeFileName);
                *(fileNameWithoutPath - 1) = c;
                hasOpened = fileOut.open(DwIODevice::WriteOnly);
            }
        }

        if (hasOpened)
        {
            //extracting: writeFileName
            char buf[KWriteBufferSize] = {0};
            do
            {
                err = unzReadCurrentFile(uf, buf, KWriteBufferSize);
                if (err < 0)
                {
                    break;//error with zipfile in unzReadCurrentFile
                }
                if (err > 0 && fileOut.write(buf, err) < 1 )
                {
                    err = UNZ_ERRNO;
                    break;//error in writing extracted file
                }
            }
            while (err > 0);

            fileOut.close();
            if (err == 0)
            {
                changeFileDate(writeFileName, fileInfo.dosDate);
            }
        }
        err = unzCloseCurrentFile(uf);
    }
    return err == ZIP_OK;
}

bool doExtractCurrentFileToMemery(unzFile uf, const char* password, DwByteArray& outFileName, DwByteArray& outFileData)
{
    char fileNameInZip[KMaxFileName];
    unz_file_info64 fileInfo;
    int err = unzGetCurrentFileInfo64(uf, &fileInfo, fileNameInZip, sizeof(fileNameInZip), NULL, 0, NULL, 0);
    if (err != UNZ_OK)
    {
        return false;//error with zipfile in unzGetCurrentFileInfo
    }
    outFileName.clear();
    outFileName.append(fileNameInZip);

    void* buf = (void*)malloc(KWriteBufferSize);
    if (buf==NULL)
    {
        return false;//printf("Error allocating memory\n");
    }

    err = unzOpenCurrentFilePassword(uf, password);
    if (err == ZIP_OK)
    {
        //extracting to dataOut
        outFileData.clear();
        do
        {
            err = unzReadCurrentFile(uf, buf, KWriteBufferSize);
            if (err < ZIP_OK)
            {
                break;//error with zipfile in unzReadCurrentFile
            }
            outFileData.append((char*)buf, err);
        }
        while (err > 0);
    }
    err = unzCloseCurrentFile(uf);

    free(buf);
    return err == ZIP_OK;
}

bool doExtract(unzFile uf, bool isExtractWithoutPath, bool isOverWrite, const char* password)
{
    unz_global_info64 globalInfo;
    unzGetGlobalInfo64(uf, &globalInfo);
    for (uLong i=0; i < globalInfo.number_entry; ++i)
    {
        if (!doExtractCurrentFile(uf, isExtractWithoutPath, isOverWrite, password))
        {
            return false;
        }

        if ((i+1) < globalInfo.number_entry)
        {
            if (unzGoToNextFile(uf) != UNZ_OK)
            {
                return false;//error with zipfile in unzGoToNextFile
            }
        }
    }
    return true;
}

bool doExtractToMemery(unzFile uf, const char* password, DwMap<DwByteArray, DwByteArray>* fileNameDataMap)
{
    unz_global_info64 globalInfo;

    int err = unzGetGlobalInfo64(uf, &globalInfo);
    DwByteArray fileName;
    DwByteArray fileData;
    for (uLong i=0; i < globalInfo.number_entry; ++i)
    {
        if (!doExtractCurrentFileToMemery(uf, password, fileName, fileData))
        {
            return false;
        }
        fileNameDataMap->insert(fileName, fileData);

        if ((i+1) < globalInfo.number_entry)
        {
            err = unzGoToNextFile(uf);
            if (err != UNZ_OK)
            {
                return false;//error with zipfile in unzGoToNextFile
            }
        }
    }
    return true;
}

bool doExtractOneFile(unzFile uf, const char* filename, bool isExtractWithoutPath, bool isOverWrite, const char* password)
{
    if (unzLocateFile(uf, filename + firstIndexOfChar(filename) + 1, KCaseSensitivity) == UNZ_OK)
    {
        return doExtractCurrentFile(uf, isExtractWithoutPath, isOverWrite, password);
    }
    //file filename not found in the zipfile
    return false;
}

bool doExtractOneFileToMemery(unzFile uf, const char* filename, const char* password, DwByteArray& outData)
{
    if (unzLocateFile(uf, filename + firstIndexOfChar(filename) + 1, KCaseSensitivity) == UNZ_OK)
    {
        DwByteArray tempName;
        return doExtractCurrentFileToMemery(uf, password, tempName, outData);
    }
    //file filename not found in the zipfile
    return false;
}

void getUnzFile(const char *zipFileName, unzFile &uf)
{
    char fileNameTry[KMaxFileName + 16];
    strncpy(fileNameTry, zipFileName, KMaxFileName - 1);
    //strncpy doesnt append the trailing NULL, of the string is too long.
    fileNameTry[ KMaxFileName ] = '\0';

    zlib_filefunc64_def ffunc;
    fill_win32_filefunc64A(&ffunc);
    uf = unzOpen2_64(zipFileName, &ffunc);

    if (uf == NULL)
    {
        strcat(fileNameTry, ".zip");
        uf = unzOpen2_64(fileNameTry, &ffunc);
    }
}

zipFile getZipFile( const char* zipName, int overWriteOption )
{
    if (zipName == NULL)
    {
        return NULL;
    }
    DwByteArray fileNameTry = zipName;
    bool hasZipSuffix = fileNameTry.endsWith(".zip");
    if (!hasZipSuffix)
    {
        fileNameTry.append(".zip");
    }

    //2: append files to .zip	1: over write	0: exit.
    zlib_filefunc64_def ffunc;
    fill_win32_filefunc64A(&ffunc);
    return zipOpen2_64(fileNameTry.data(), overWriteOption, NULL, &ffunc);
}

int openNewFileInZip( zipFile zf, const char *realName, bool isExcludePath, int compressLevel, const char* pwd)
{
    zip_fileinfo zi;
    zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour = 0;
    zi.tmz_date.tm_mday = zi.tmz_date.tm_mon = zi.tmz_date.tm_year = 0;
    zi.dosDate = 0;
    zi.internal_fa = 0;
    zi.external_fa = 0;
    fileTime(realName, &zi.dosDate);

    //The path name saved, should not include a leading slash.
    //if it did, windows/xp and dynazip couldn't read the zip file.
    const char *saveNameInZip = realName + firstIndexOfChar(realName) + 1;

    //should the zip file contain any path at all?.
    //if not. "hello/world.txt" will be saved to "world.txt"
    if( isExcludePath )
    {
        saveNameInZip = getFileNameWithoutPath(realName);
    }

    char buf[KWriteBufferSize] = {0};
    unsigned long crcFile = 0;
    int err = ZIP_OK;
    if (pwd != NULL)
    {
        err = getFileCrc(realName, buf, KWriteBufferSize, &crcFile);
    }

    bool isZip64 = isLargeFile(realName);
    err = zipOpenNewFileInZip3_64(zf, saveNameInZip, &zi,
                                  NULL, 0, NULL, 0, NULL,
                                  (compressLevel != 0) ? Z_DEFLATED : 0,
                                  compressLevel, 0,
                                  -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
                                  pwd, crcFile, isZip64);

    return err;
}

template<typename DEVICE>
int writeFileInZip(zipFile zf, DEVICE& fileIn)
{
    int err = ZIP_ERRNO;
    if (fileIn.open(DwIODevice::ReadOnly))
    {
        int64_t readSize;
        char buf[KWriteBufferSize] = {0};
        do
        {
            err = ZIP_OK;
            readSize = fileIn.read(buf, KWriteBufferSize);
            if (readSize < KWriteBufferSize && !fileIn.atEnd() )
            {
                err = ZIP_ERRNO;//error in reading fileNameInZip
            }

            if (readSize > 0)
            {
                err = zipWriteInFileInZip (zf, buf, (unsigned int)readSize);//if (err < 0)	error in writing fileNameInZip in the zipfile
            }
        }
        while ((err == ZIP_OK) && (readSize > 0));
        fileIn.close();
    }
    return err;
}

int addFileToZip( zipFile zf, const char *realFileName, bool isZipExcludePath, int compressLevel, const char* password )
{
    int err = openNewFileInZip(zf, realFileName, isZipExcludePath, compressLevel, password);

    if (err == ZIP_OK)
    {
        DwFile fileIn(realFileName);
        err = writeFileInZip(zf, fileIn);
    }
    if (err >= 0)
    {
        err = zipCloseFileInZip(zf);
    }
    return err;
}

int addDataToZip( zipFile zf, const char *realFileName, bool isZipExcludePath, int compressLevel, const char* password, DwZipFile& fileIn )
{
    int err = openNewFileInZip(zf, realFileName, isZipExcludePath, compressLevel, password);

    if (err == ZIP_OK)
    {
        err = writeFileInZip(zf, fileIn);
    }
    if (err >= 0)
    {
        err = zipCloseFileInZip(zf);
    }
    return err;
}

int zipModeToInt( const DwByteArray& zipName, ZipMode mode )
{
    int value = 0;
    switch(mode)
    {
    case CreateZip:
        value = 0;
        break;
    case OverWriteZip:
        value = 1;
        break;
    case AppendToZip:
        //if the file don't exist, we not append file
        value = DwFile::exist(zipName.data()) ? 2 : 1;
        break;
    default:
        break;
    }
    //if open as OverWriteZip but not add file to zip, the zip file will be broken. so we don't return 1.
    return value == 2 ? 2 : 0;
}

bool delFileInZip( unzFile srcUnzFile, const DwByteArray& zipName, const DwByteArray& locateName, const DwByteArray& comment )
{
    //http://www.winimage.com/zLibDll/del.cpp
    //create a temp zip to contain the files no need to delete.
    bool someWasDel = false;

    // change name for temp file
    DwByteArray tempName(zipName);
    tempName.append(".tmp.zip");

    // open source and destination file
    zipFile destZip = DwZipUtils::getZipFile(tempName.data(), 0);
    if (!destZip)
    {
        return false;
    }

    // copying files
    int filesCount = 0;

    const char *locateNameWithoutPoint = locateName.data() + firstIndexOfChar(locateName.data()) + 1;
    int ret = unzGoToFirstFile(srcUnzFile);
    while (ret == UNZ_OK)
    {
        // get zipped file info
        unz_file_info64 unzInfo;
        char nameInZip[KMaxFileName];
        if (unzGetCurrentFileInfo64(srcUnzFile, &unzInfo, nameInZip, KMaxFileName, NULL, 0, NULL, 0) != UNZ_OK)
        {
            break;
        }

        // if need delete this file

        if (_stricmp(nameInZip, locateNameWithoutPoint) == 0) // lowercase comparison
        {
            someWasDel = true;
        }
        else
        {
            char* extrafield = new char[unzInfo.size_file_extra];
            if ((extrafield==NULL) && (unzInfo.size_file_extra != 0))
            {
                break;
            }
            char* commentary = new char[unzInfo.size_file_comment];
            if ((commentary==NULL) && (unzInfo.size_file_comment != 0))
            {
                DW_SAFE_DELETE_ARRAY(extrafield);
                break;
            }

            if (unzGetCurrentFileInfo64(srcUnzFile, &unzInfo, nameInZip, KMaxFileName, extrafield, unzInfo.size_file_extra, commentary, unzInfo.size_file_comment) != UNZ_OK)
            {
                DW_SAFE_DELETE_ARRAY(extrafield);
                DW_SAFE_DELETE_ARRAY(commentary);
                break;
            }

            // open file for RAW reading, not uncompress
            int method;
            int level;
            if (unzOpenCurrentFile2(srcUnzFile, &method, &level, 1) != UNZ_OK)
            {
                DW_SAFE_DELETE_ARRAY(extrafield);
                DW_SAFE_DELETE_ARRAY(commentary);
                break;
            }

            int localExtraSize = unzGetLocalExtrafield(srcUnzFile, NULL, 0);
            if (localExtraSize < 0)
            {
                DW_SAFE_DELETE_ARRAY(extrafield);
                DW_SAFE_DELETE_ARRAY(commentary);
                break;
            }
            void* localExtra = malloc(localExtraSize);
            if ((localExtra == NULL) && (localExtraSize != 0))
            {
                DW_SAFE_DELETE_ARRAY(extrafield);
                DW_SAFE_DELETE_ARRAY(commentary);
                break;
            }

            if (unzGetLocalExtrafield(srcUnzFile, localExtra, localExtraSize) < 0)
            {
                DW_SAFE_DELETE_ARRAY(extrafield);
                DW_SAFE_DELETE_ARRAY(commentary);
                free(localExtra);
                break;
            }

            // this malloc may fail if file very large
            size_t allocSize = (size_t)unzInfo.compressed_size;
            void* buf = malloc(allocSize);
            if ((buf == NULL) && (unzInfo.compressed_size != 0))
            {
                DW_SAFE_DELETE_ARRAY(extrafield);
                DW_SAFE_DELETE_ARRAY(commentary);
                free(localExtra);
                break;
            }

            // read file

            int sz = unzReadCurrentFile(srcUnzFile, buf, (unsigned int)unzInfo.compressed_size);
            if ((unsigned int)sz != unzInfo.compressed_size)
            {
                DW_SAFE_DELETE_ARRAY(extrafield);
                DW_SAFE_DELETE_ARRAY(commentary);
                free(localExtra);
                free(buf);
                break;
            }

            // open destination file
            zip_fileinfo destZipInfo;
            memcpy (&destZipInfo.tmz_date, &unzInfo.tmu_date, sizeof(tm_unz));
            destZipInfo.dosDate = unzInfo.dosDate;
            destZipInfo.internal_fa = unzInfo.internal_fa;
            destZipInfo.external_fa = unzInfo.external_fa;

            if (zipOpenNewFileInZip2(destZip, nameInZip, &destZipInfo, localExtra, localExtraSize, extrafield, unzInfo.size_file_extra, commentary, method, level, 1) != UNZ_OK)
            {
                DW_SAFE_DELETE_ARRAY(extrafield);
                DW_SAFE_DELETE_ARRAY(commentary);
                free(localExtra);
                free(buf);
                break;
            }

            // write file
            if (zipWriteInFileInZip(destZip, buf, (unsigned int)unzInfo.compressed_size) != UNZ_OK)
            {
                DW_SAFE_DELETE_ARRAY(extrafield);
                DW_SAFE_DELETE_ARRAY(commentary);
                free(localExtra);
                free(buf);
                break;
            }

            if (zipCloseFileInZipRaw(destZip, (unsigned int)unzInfo.uncompressed_size, unzInfo.crc) != UNZ_OK)
            {
                DW_SAFE_DELETE_ARRAY(extrafield);
                DW_SAFE_DELETE_ARRAY(commentary);
                free(localExtra);
                free(buf);
                break;
            }

            if (unzCloseCurrentFile(srcUnzFile) == UNZ_CRCERROR)
            {
                DW_SAFE_DELETE_ARRAY(extrafield);
                DW_SAFE_DELETE_ARRAY(commentary);
                free(localExtra);
                free(buf);
                break;
            }

            DW_SAFE_DELETE_ARRAY(commentary);
            DW_SAFE_DELETE_ARRAY(extrafield);
            free(buf);
            free(localExtra);

            filesCount ++;
        }

        ret = unzGoToNextFile(srcUnzFile);
    }

    zipClose(destZip, comment.isEmpty() ? NULL : comment.data());
    unzClose(srcUnzFile);

    // if fail
    wchar_t *tempWStr = DwString(tempName.data()).constString();
    wchar_t *zipWStr = DwString(zipName.data()).constString();
    if ( (!someWasDel) || (ret!=UNZ_END_OF_LIST_OF_FILE) )
    {
        ::DeleteFile(tempWStr);
        return false;
    }

    ::DeleteFile(zipWStr);
    if (!::MoveFile(tempWStr, zipWStr))
    {
        return false;
    }

    // if all files were deleted
    if (filesCount==0)
    {
        ::DeleteFile(zipWStr);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////

bool UnZipGzip(DwByteArray& gzDataIn, int len, DwByteArray& dataOut)
{
    return DwGZip().ungzip(gzDataIn, len, dataOut);
}

bool ZipGzipEx(DwByteArray& gzDataIn, int len, DwByteArray& dataOut)
{
    return DwGZip().gzipex(gzDataIn, len, dataOut);
}

bool UnZipGzipEx(DwByteArray& gzDataIn, int len, DwByteArray& dataOut)
{
    return DwGZip().ungzipex(gzDataIn, len, dataOut);
}

bool Zip(const DwByteArray& zipFileName, const DwVector<DwByteArray> &filesAddToZip)
{
    //压缩操作
    //使用zipOpen64来打开/创建一个ZIP文件，然后开始遍历要被放到压缩包中去的文件.
    //针对每个文件，先调用一次zipOpenNewFileInZip，然后开始读原始文件数据，使用zipWriteInFileInZip来写入到ZIP文件中去.
    //zipOpenNewFileInZip的第三个参数是一个zip_fileinfo结构，该结构数据可全部置零，
    //其中 dosDate可用于填入一个时间（LastModificationTime）.
    //它的第二个参数是ZIP中的文件名，若要保持目录结构，该参数中可以保留路径，如foo/bar.txt。

    zipFile zf = getZipFile(zipFileName.data(), 0);
    if (zf == NULL)
    {
        return false;//error opening fileNameTry
    }

    DwVector<const DwByteArray>::const_iterator filesIter;
    int defaultCompressLevel = 6;
    int err = ZIP_OK;
    for (filesIter = filesAddToZip.begin(); filesIter != filesAddToZip.end() && (err == ZIP_OK); ++ filesIter)
    {
        err = addFileToZip(zf, (*filesIter).data(), false, defaultCompressLevel, NULL);
    }

    return zipClose(zf, NULL) == ZIP_OK;
}

bool Unzip(const DwByteArray& zipFileName, const DwByteArray& extractDir)
{
    if (zipFileName.isEmpty())
    {
        return false;
    }
    unzFile uf = NULL;
    getUnzFile(zipFileName.data(), uf);

    if (uf == NULL)
    {
        return false;//Cannot open zipFileName or zipFileName.zip
    }

    char currentDir[KMaxFileName];
    _getcwd(currentDir, KMaxFileName);
    if (_chdir(extractDir.data()) == -1)
    {
        if (makeDir(extractDir.data()))
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

    bool retValue = doExtract(uf, false, false, NULL);
    unzClose(uf);
    _chdir(currentDir);

    return retValue;
}

bool UnzipToMemery(const DwByteArray& zipFileName, DwMap<DwByteArray, DwByteArray>* fileNameDataMap)
{
    if (!zipFileName.isNull())
    {
        unzFile uf = NULL;
        getUnzFile(zipFileName.data(), uf);
        if (uf)
        {
            bool retValue = doExtractToMemery(uf, NULL, fileNameDataMap);
            unzClose(uf);
            return retValue;
        }
    }
    return false;
}

bool UnzipOneFileToMemery(const DwByteArray& zipFileName, const DwByteArray& fileName, DwByteArray& dataOut)
{
    if (!zipFileName.isNull())
    {
        unzFile uf = NULL;
        getUnzFile(zipFileName.data(), uf);
        if (uf)
        {
            bool retValue = doExtractOneFileToMemery(uf, fileName.data(), NULL, dataOut);
            unzClose(uf);
            return retValue;
        }
    }
    return false;
}
}
