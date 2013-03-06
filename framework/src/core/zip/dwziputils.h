#pragma once

#include "../3rdparty/zlib/minizip/zip.h"
#include "../3rdparty/zlib/minizip/unzip.h"
#include "../3rdparty/zlib/minizip/iowin32.h"

#include "dwcore/dwvector.h"
#include "dwcore/dwmap.h"

class DwByteArray;
class DwZipFile;

namespace DwZipUtils
{
enum ZipMode
{
    CreateZip = 0,
    OverWriteZip,
    AppendToZip
};

//gzip
bool DW_CORE_EXPORT UnZipGzip(DwByteArray& gzDataIn, int len, DwByteArray& dataOut);
bool DW_CORE_EXPORT ZipGzipEx(DwByteArray& gzDataIn, int len, DwByteArray& dataOut);
bool DW_CORE_EXPORT UnZipGzipEx(DwByteArray& gzDataIn, int len, DwByteArray& dataOut);

//压缩一系列文件到zip文件中，如果zip已经存在，则附加，否则新建。
//zip
//文件若存在，则返回；压缩系数为6
bool DW_CORE_EXPORT Zip(const DwByteArray& zipFileName, const DwVector<DwByteArray>& filesAddToZip);
//包含目录结构，不覆盖已有文件
bool DW_CORE_EXPORT Unzip(const DwByteArray& zipFileName, const DwByteArray& extractDir);
bool DW_CORE_EXPORT UnzipToMemery(const DwByteArray& zipFileName, DwMap<DwByteArray, DwByteArray>* fileNameDataMap);
bool DW_CORE_EXPORT UnzipOneFileToMemery(const DwByteArray& zipFileName, const DwByteArray& fileName, DwByteArray& dataOut);

//////////////////////////////////////////////////////////////////////////
//no export
bool	fileTime(const char *fileName, unsigned long *dosTime);
int		getFileCrc(const char* fileNameInZip, char* buf, unsigned long bufSize, unsigned long* resCrc);
bool	isLargeFile(const char* fileName);
void	changeFileDate(const char *fileName, unsigned long dosDate);
bool	makeDir(const char *newDir);
const char* getFileNameWithoutPath(const char* fileName);
int		lastIndexOfSlash(char* fileName);
int		firstIndexOfChar(const char* fileName);
bool	doExtractCurrentFile(unzFile uf, bool isExtractWithoutPath, bool isOverWrite, const char* password);
bool	doExtractCurrentFileToMemery(unzFile uf, const char* password, DwByteArray& outFileName, DwByteArray& outFileData);
bool	doExtract(unzFile uf, bool isExtractWithoutPath, bool isOverWrite, const char* password);
bool	doExtractToMemery(unzFile uf, const char* password, DwMap<DwByteArray, DwByteArray>* fileNameDataMap);
bool	doExtractOneFile(unzFile uf, const char* filename, bool isExtractWithoutPath, bool isOverWrite, const char* password);
bool	doExtractOneFileToMemery(unzFile uf, const char* filename, const char* password, DwByteArray& outData);
zipFile	getZipFile(const char* zipName, int overWriteOption);
void	getUnzFile(const char *zipFileName, unzFile &uf);
int		zipModeToInt(const DwByteArray& zipName, ZipMode zipway);
int		addFileToZip(zipFile zf, const char *realFileName, bool isZipExcludePath, int compressLevel, const char* password );
int		addDataToZip(zipFile zf, const char *realFileName, bool isZipExcludePath, int compressLevel, const char* password, DwZipFile& fileIn );
int		openNewFileInZip(zipFile zf, const char *realFileName, bool isZipExcludePath, int compressLevel, const char* password);
bool	delFileInZip(unzFile srcUnzFile, const DwByteArray& zipName, const DwByteArray& locateName, const DwByteArray& comment);

template<typename DEVICE>
int writeFileInZip(zipFile zf, DEVICE& fileIn);
}