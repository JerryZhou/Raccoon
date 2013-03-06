#include "stable.h"

#include <dwcore/dwlinkedlist.h>
#include <dwcore/dwglobalstatic.h>
#include <dwcore/dwdebug.h>

#include "dwresourcefileengine.h"
#include "dwfileengine.h"

class DwAbstractFileEngineHandlerList : public DwLinkedList<DwAbstractFileEngineHandler*>
{
public:
    DwAbstractFileEngine *create(const DwString &fileName) const
    {
        for(const_iterator it = begin(); it != end(); ++it)
        {
            if(DwAbstractFileEngine *ret = (*it)->create(fileName))
            {
                return ret;
            }
        }

        dwDebug() << "no file engine for file " << fileName;
        return NULL;
    }
};
DW_GLOBAL_STATIC(DwAbstractFileEngineHandlerList, __dwFileEngineHandlers);

class DwDefaultFileEngineHandler : public DwAbstractFileEngineHandler
{
    virtual DwAbstractFileEngine *create(const DwString &) const
    {
        return new DwFileEngine();
    }
};

class DwResourceFileEngineHandler : public DwAbstractFileEngineHandler
{
    virtual DwAbstractFileEngine *create(const DwString &fileName) const
    {
        if(!fileName.startsWith(":"))
        {
            return NULL;
        }
        return new DwResourceFileEngine();
    }
};
static DwDefaultFileEngineHandler __dwDefaultFileEngineHandler;
static DwResourceFileEngineHandler __dwResourceFileEngineHandler;

DwFileEngine::DwFileEngine()
    : m_fileHandle(INVALID_HANDLE_VALUE)
{

}

bool DwFileEngine::exist( const DwString &fileName )
{
    DWORD ret = GetFileAttributes( fileName.constString() );

    if (ret == INVALID_FILE_ATTRIBUTES)
    {
        return false;
    }

    return true;
}

bool DwFileEngine::remove( const DwString &fileName )
{
    return ::DeleteFile(fileName.constString()) ? true : false;
}

bool DwFileEngine::open( DwIODevice::OpenMode mode )
{
    DW_ASSERT(m_fileHandle == INVALID_HANDLE_VALUE);

    // All files are opened in share mode (both read and write).
    DWORD shareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;

    int accessRights = 0;
    if (mode & DwIODevice::ReadOnly)
    {
        accessRights |= GENERIC_READ;
    }
    if (mode & DwIODevice::WriteOnly)
    {
        accessRights |= GENERIC_WRITE;
    }

    SECURITY_ATTRIBUTES securityAtts = { sizeof(SECURITY_ATTRIBUTES), NULL, FALSE };

    // WriteOnly can create files, ReadOnly cannot.
    DWORD creationDisp = (mode & DwIODevice::WriteOnly) ? OPEN_ALWAYS : OPEN_EXISTING;

    if(mode & DwIODevice::Truncate)
    {
        creationDisp = CREATE_ALWAYS;
    }

    // Create the file handle.
    m_fileHandle = CreateFile(fileName().constString(),
                              accessRights,
                              shareMode,
                              &securityAtts,
                              creationDisp,
                              FILE_ATTRIBUTE_NORMAL,
                              NULL);

    if(m_fileHandle == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    if(mode & DwIODevice::Append)
    {
        seekToEnd();
    }

    return true;
}

bool DwFileEngine::seek( int64_t pos )
{
    DW_ASSERT(m_fileHandle != INVALID_HANDLE_VALUE);

    DWORD seekTo = SetFilePointer(m_fileHandle, dwInt64Low(pos), 0, FILE_BEGIN);
    return seekTo == pos;
}

int64_t DwFileEngine::pos() const
{
    DW_ASSERT(m_fileHandle != INVALID_HANDLE_VALUE);

    LARGE_INTEGER currentFilePos;
    LARGE_INTEGER offset;
    offset.QuadPart = 0;
    if (!::SetFilePointerEx(m_fileHandle, offset, &currentFilePos, FILE_CURRENT))
    {
        DW_ASSERT(false);
        return 0;
    }

    return int64_t(currentFilePos.QuadPart);
}

int64_t DwFileEngine::size() const
{
    DW_ASSERT(m_fileHandle != INVALID_HANDLE_VALUE);

    WIN32_FILE_ATTRIBUTE_DATA fileInfo = {0};
    BOOL ret = GetFileAttributesEx(
                   fileName().constString(),
                   GetFileExInfoStandard,
                   (void*)&fileInfo);

    if (!ret)
    {
        return -1;
    }

    return dwInt64(fileInfo.nFileSizeHigh, fileInfo.nFileSizeLow);
}

int64_t DwFileEngine::read( char *data, int64_t size )
{
    DW_ASSERT(m_fileHandle != INVALID_HANDLE_VALUE);

    DWORD numberOfRead = 0;
    ::ReadFile(m_fileHandle, data, DWORD(size), &numberOfRead, NULL);
    return numberOfRead;
}

int64_t DwFileEngine::write( const char *data, int64_t size )
{
    DW_ASSERT(m_fileHandle != INVALID_HANDLE_VALUE);

    DWORD numberOfWritten = 0;
    ::WriteFile(m_fileHandle, data, DWORD(size), &numberOfWritten, NULL);
    return numberOfWritten;
}

void DwFileEngine::close()
{
    DW_ASSERT(m_fileHandle != INVALID_HANDLE_VALUE);

    if(m_fileHandle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_fileHandle);
        m_fileHandle = INVALID_HANDLE_VALUE;
    }
}

bool DwFileEngine::seekToEnd()
{
    DW_ASSERT(m_fileHandle != INVALID_HANDLE_VALUE);

    DWORD ret = ::SetFilePointer(m_fileHandle, 0, 0, FILE_END);
    return ret != INVALID_SET_FILE_POINTER;
}

bool DwFileEngine::atEnd() const
{
    return size() == pos();
}

DwAbstractFileEngineHandler::DwAbstractFileEngineHandler()
{
    __dwFileEngineHandlers()->prepend(this);
}

DwAbstractFileEngineHandler::~DwAbstractFileEngineHandler()
{
    __dwFileEngineHandlers()->removeOne(this);
}

DwAbstractFileEngine * DwAbstractFileEngine::create( const DwString &fileName )
{
    return __dwFileEngineHandlers()->create(fileName);
}
