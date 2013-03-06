#pragma once
#include "dwgui/dwfswrapper.h"
#include "dwapp/dwstream.h"
#include "dwapp/dwfiletime.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwFsWrapperEx : public DwFsWrapper
{
public:
    /// open a file
    static DwFsWrapper::Handle openFile(const DwString& path, DwStream::AccessMode accessMode, DwStream::AccessPattern accessPattern, DWORD flagsAndAttributes = 0);
    /// close a file
    static void closeFile(DwFsWrapper::Handle h);
    /// write to a file
    static void write(DwFsWrapper::Handle h, const void* buf, DwStream::Size numBytes);
    /// read from a file
    static DwStream::Size read(DwFsWrapper::Handle h, void* buf, DwStream::Size numBytes);
    /// seek in a file
    static void seek(DwFsWrapper::Handle h, DwStream::Offset offset, DwStream::SeekOrigin orig);
    /// get position in file
    static DwStream::Position tell(DwFsWrapper::Handle h);
    /// flush a file
    static void flush(DwFsWrapper::Handle h);
    /// return true if at end-of-file
    static bool eof(DwFsWrapper::Handle h);
    /// get size of a file in bytes
    static DwStream::Size getFileSize(DwFsWrapper::Handle h);
    /// set read-only status of a file
    static void setReadOnly(const DwString& path, bool readOnly);
    /// get read-only status of a file
    static bool isReadOnly(const DwString& path);
    /// delete a file
    static bool deleteFile(const DwString& path);
    /// delete an empty directory
    static bool deleteDirectory(const DwString& path);
    /// return true if a file exists
    static bool fileExists(const DwString& path);
    /// return true if a directory exists
    static bool directoryExists(const DwString& path);
    /// set the write-access time stamp of a file
    static void setFileWriteTime(const DwString& path, DwFileTime fileTime);
    /// get the last write-access time stamp of a file
    static DwFileTime getFileWriteTime(const DwString& path);
    /// create a directory
    static bool createDirectory(const DwString& path);
private:
};// end of DwFsWrapperEx