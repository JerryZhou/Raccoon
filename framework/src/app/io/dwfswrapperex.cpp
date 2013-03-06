#include "stable.h"
#include "dwfswrapperex.h"


//------------------------------------------------------------------------------
/**
*/
DwFsWrapperEx::Handle
DwFsWrapperEx::openFile(const DwString& path, DwStream::AccessMode accessMode, DwStream::AccessPattern accessPattern, DWORD flagsAndAttributes)
{
    DWORD access = 0;
    DWORD disposition = 0;
    DWORD shareMode = 0;
    switch (accessMode)
    {
        case DwStream::ReadAccess:
            access = GENERIC_READ;            
            disposition = OPEN_EXISTING;
            shareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
            break;

        case DwStream::WriteAccess:
            access = GENERIC_WRITE;
            disposition = CREATE_ALWAYS;
            shareMode = FILE_SHARE_READ;
            break;

        case DwStream::ReadWriteAccess:
        case DwStream::AppendAccess:
            access = GENERIC_READ | GENERIC_WRITE;
            disposition = OPEN_ALWAYS;
            shareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
            break;
    }
    switch (accessPattern)
    {
        case DwStream::Random:
            flagsAndAttributes |= FILE_FLAG_RANDOM_ACCESS;
            break;

        case DwStream::Sequential:
            flagsAndAttributes |= FILE_FLAG_SEQUENTIAL_SCAN;
            break;
    }

    // open/create the file
    Handle handle = CreateFileW((LPCWSTR)path.constString(),       // lpFileName
        access,                  // dwDesiredAccess
        shareMode,               // dwShareMode
        0,                       // lpSecurityAttributes
        disposition,             // dwCreationDisposition,
        flagsAndAttributes,      // dwFlagsAndAttributes
        NULL);                   // hTemplateFile
    if (handle != INVALID_HANDLE_VALUE)
    {
        // in append mode, we need to seek to the end of the file
        if (DwStream::AppendAccess == accessMode)
        {
            SetFilePointer(handle, 0, NULL, FILE_END);
        }
        return handle;
    }
    else
    {
        return 0;
    }
}

//------------------------------------------------------------------------------
/**
    Closes a file opened by DwFsWrapperEx::OpenFile().
*/
void
DwFsWrapperEx::closeFile(Handle handle)
{
    DW_ASSERT(0 != handle);
    CloseHandle(handle);
}

//------------------------------------------------------------------------------
/**
    Write data to a file.
*/
void
DwFsWrapperEx::write(Handle handle, const void* buf, DwStream::Size numBytes)
{
    DW_ASSERT(0 != handle);
    DW_ASSERT(buf != 0);
    DW_ASSERT(numBytes > 0);
    DWORD bytesWritten;
    BOOL result = WriteFile(handle, buf, numBytes, &bytesWritten, NULL);
    if ((0 == result) || ((DWORD)numBytes != bytesWritten))
    {
        DW_ASSERT( false && "DwFsWrapperEx: WriteFile() failed!");
    }
}

//------------------------------------------------------------------------------
/**
    Read data from a file, returns number of bytes read.
*/
DwStream::Size
DwFsWrapperEx::read(Handle handle, void* buf, DwStream::Size numBytes)
{
    DW_ASSERT(0 != handle);
    DW_ASSERT(buf != 0);
    DW_ASSERT(numBytes > 0);
    DWORD bytesRead;
    BOOL result = ReadFile(handle, buf, numBytes, &bytesRead, NULL);
    if (0 == result)
    {
        DW_ASSERT( false && "DwFsWrapperEx: ReadFile() failed!");
    }
    return bytesRead;
}

//------------------------------------------------------------------------------
/**
    Seek in a file.
*/
void
DwFsWrapperEx::seek(Handle handle, DwStream::Offset offset, DwStream::SeekOrigin orig)
{
    DW_ASSERT(0 != handle);
    DWORD moveMethod;
    switch (orig)
    {
        case DwStream::Begin:
            moveMethod = FILE_BEGIN;
            break;
        case DwStream::Current:
            moveMethod = FILE_CURRENT;
            break;
        case DwStream::End:
            moveMethod = FILE_END;
            break;
        default:
            // can't happen
            moveMethod = FILE_BEGIN;
            break;
    }
    SetFilePointer(handle, offset, NULL, moveMethod);
}

//------------------------------------------------------------------------------
/**
    Get current position in file.
*/
DwStream::Position
DwFsWrapperEx::tell(Handle handle)
{
    DW_ASSERT(0 != handle);
    return SetFilePointer(handle, 0, NULL, FILE_CURRENT);
}

//------------------------------------------------------------------------------
/**
    Flush unwritten data to file.
*/
void
DwFsWrapperEx::flush(Handle handle)
{
    DW_ASSERT(0 != handle);
    FlushFileBuffers(handle);
}

//------------------------------------------------------------------------------
/**
    Returns true if current position is at end of file.
*/
bool
DwFsWrapperEx::eof(Handle handle)
{
    DW_ASSERT(0 != handle);
    DWORD fpos = SetFilePointer(handle, 0, NULL, FILE_CURRENT);
    DWORD size = ::GetFileSize(handle, NULL);

    // NOTE: THE '>=' IS NOT A BUG!!!
    return fpos >= size;
}

//------------------------------------------------------------------------------
/**
    Returns the size of a file in bytes.
*/
DwStream::Size
DwFsWrapperEx::getFileSize(Handle handle)
{
    DW_ASSERT(0 != handle);
    return ::GetFileSize(handle, NULL);
}

//------------------------------------------------------------------------------
/**
    Set the read-only status of a file. This method does nothing on the
    Xbox360.
*/
void
DwFsWrapperEx::setReadOnly(const DwString& path, bool readOnly)
{
    DW_ASSERT(!path.isEmpty());
    DWORD fileAttrs = GetFileAttributesW((LPCWSTR)path.constString());
    if (readOnly)
    {
        fileAttrs |= FILE_ATTRIBUTE_READONLY;
    }
    else
    {
        fileAttrs &= ~FILE_ATTRIBUTE_READONLY;
    }
    SetFileAttributesW(path.constString(), fileAttrs);
}

//------------------------------------------------------------------------------
/**
    Get the read-only status of a file. This method always returns true
    on the Xbox360.
*/
bool
DwFsWrapperEx::isReadOnly(const DwString& path)
{
    DW_ASSERT(!path.isEmpty());
    DWORD fileAttrs = GetFileAttributesW((LPCWSTR)path.constString());
    return (fileAttrs & FILE_ATTRIBUTE_READONLY);
}

//------------------------------------------------------------------------------
/**
    Deletes a file. Returns true if the operation was successful. The delete
    will fail if the fail doesn't exist or the file is read-only.
*/
bool
DwFsWrapperEx::deleteFile(const DwString& path)
{
    DW_ASSERT(!path.isEmpty());
    return (0 != ::DeleteFileW((LPCWSTR)path.constString()));
}

//------------------------------------------------------------------------------
/**
    Delete an empty directory. Returns true if the operation was successful.
*/
bool
DwFsWrapperEx::deleteDirectory(const DwString& path)
{
    DW_ASSERT(!path.isEmpty());
    return (0 != ::RemoveDirectoryW((LPCWSTR)path.constString()));
}

//------------------------------------------------------------------------------
/**
    Return true if a file exists.
*/
bool
DwFsWrapperEx::fileExists(const DwString& path)
{
    DW_ASSERT(!path.isEmpty());
    DWORD fileAttrs = GetFileAttributesW((LPCWSTR)path.constString());
    if ((-1 != fileAttrs) && (0 == (FILE_ATTRIBUTE_DIRECTORY & fileAttrs)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
    Return true if a directory exists.
*/
bool
DwFsWrapperEx::directoryExists(const DwString& path)
{
    DW_ASSERT(!path.isEmpty());
    DWORD fileAttrs = GetFileAttributesW((LPCWSTR)path.constString());
    if ((-1 != fileAttrs) && (0 != (FILE_ATTRIBUTE_DIRECTORY & fileAttrs)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
    Set the write-access time stamp of a file.
*/
void
DwFsWrapperEx::setFileWriteTime(const DwString& path, DwFileTime fileTime)
{
    DW_ASSERT(!path.isEmpty());
    Handle h = DwFsWrapperEx::openFile(path, DwStream::ReadWriteAccess, DwStream::Sequential);
    if (0 != h)
    {
        SetFileTime(h, NULL, NULL, &fileTime.time);
        DwFsWrapperEx::closeFile(h);
    }
    else
    {
        DW_ASSERT( false && "DwFsWrapperEx::SetFileWriteTime(): failed to open file !");
    }
}

//------------------------------------------------------------------------------
/**
    Return the last write-access time to a file.
*/
DwFileTime
DwFsWrapperEx::getFileWriteTime(const DwString& path)
{
    DW_ASSERT(!path.isEmpty());
    DwFileTime fileTime;
    Handle h = DwFsWrapperEx::openFile(path, DwStream::ReadAccess, DwStream::Sequential);
    if (0 != h)
    {
        GetFileTime(h, NULL, NULL, &fileTime.time);
        DwFsWrapperEx::closeFile(h);
    }
    else
    {
        ;
    }
    return fileTime;
}

//------------------------------------------------------------------------------
/**
    Creates a new directory.
*/
bool
DwFsWrapperEx::createDirectory(const DwString& path)
{
    DW_ASSERT(!path.isEmpty());
    return (0 != ::CreateDirectoryW((LPCWSTR)path.constString(), NULL));
}