#pragma once

#include "../3rdparty/zlib/minizip/zip.h"
#include "../3rdparty/zlib/minizip/unzip.h"
#include "dwziputils.h"

class DwByteArray;
class DwBuffer;

class DwZipFilePrivate;

class DW_CORE_EXPORT DwZipFile
{
public:
    explicit DwZipFile(const DwByteArray& fileName);
    ~DwZipFile();

    bool	open(DwIODevice::OpenMode flags);
    void	close();
    int64_t	size() const;
    int64_t	pos() const;
    bool	seek(int64_t pos);
    bool	atEnd() const;

    int64_t	read(char *data, const int64_t len);
    int64_t	write(const DwByteArray& data);
    const DwByteArray &readAll() const;
    void	setData( DwByteArray &data );
    int64_t	saveToDisk(const DwByteArray& dir);

    DwByteArray	name() const;
    void	setName(const DwByteArray& fileName);

private:
    DwZipFilePrivate	*d_ptr;
};

//////////////////////////////////////////////////////////////////////////
class DwZipArchivePrivate;

class DW_CORE_EXPORT DwZipArchive
{
public:
    //using the archive for:
    enum OpenMode
    {
        View = 0,
        Zip,
        UnZip
    };

    explicit DwZipArchive(const DwByteArray& zipName);
    ~DwZipArchive();

    bool		open(const OpenMode mode = View);
    bool		close();

    bool		addFile(const DwByteArray& fileName);
    bool		addFile(DwZipFile& zipFile);
    //bool		addEmptyDir(const DwByteArray& dirName);
    bool		delFile(const int index);
    bool		delFile(const DwByteArray& locateName);
    bool		extractTo(const DwByteArray& dirName, bool isOverWrite = false);
    bool		extractOneFileTo(const DwByteArray& dirName, const DwByteArray& locateName, bool isOverWrite = false);

    int			count() const;
    bool		fileByName(const DwByteArray& locateName, DwZipFile& fileOut) const;
    DwByteArray	nameByIndex(const int index);
    int			indexByName(const DwByteArray& locateName) const;
    //DwVector<DwByteArray>	allFilesName();
    DwByteArray	archiveComment() const;
    DwByteArray currentPath();

    void		setZipMode(DwZipUtils::ZipMode mode);

private:
    DwByteArray _currentFileName();
    bool		_changeToDir(const DwByteArray& dirName, DwByteArray& curDirName);

private:
    DwZipArchivePrivate	*d_ptr;
};