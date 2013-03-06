
#include "stable.h"
#include "dwfilestream.h"


DW_IMPLEMENT_CLASS(DwFileStream, 'FSTR', DwStream);


//------------------------------------------------------------------------------
/**
*/
DwFileStream::DwFileStream() :
    m_handle(0),
    m_mappedContent(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
DwFileStream::~DwFileStream()
{
    if (this->isOpen())
    {
        this->close();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
DwFileStream::canRead() const
{
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
DwFileStream::canWrite() const
{
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
DwFileStream::canSeek() const
{
    return true;
}

//------------------------------------------------------------------------------
/**
    FileStreams support mapping (only read access for now).
*/
bool
DwFileStream::canBeMapped() const
{
    return true;
}

//------------------------------------------------------------------------------
/**
*/
DwStream::Size
DwFileStream::size() const
{
    DW_ASSERT(0 != this->m_handle);
    return DwFsWrapperEx::getFileSize(this->m_handle);
}

//------------------------------------------------------------------------------
/**
*/
DwStream::Position
DwFileStream::position() const
{
    DW_ASSERT(0 != this->m_handle);
    return DwFsWrapperEx::tell(this->m_handle);
}

//------------------------------------------------------------------------------
/**
*/
bool
DwFileStream::open()
{
    DW_ASSERT(!this->isOpen());
    DW_ASSERT(0 == this->m_handle);
    DW_ASSERT(this->m_uri.scheme() == "file");
    DW_ASSERT(!this->m_uri.localPath().isEmpty());
    if (DwStream::open())
    {
        this->m_handle = DwFsWrapperEx::openFile(this->m_uri.hostAndLocalPath(), this->m_accessMode, this->m_accessPattern);
        if (0 != this->m_handle)
        {
            return true;
        }
    }

    // fallthrough: failure
    DwStream::close();
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
DwFileStream::close()
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(0 != this->m_handle);
    if (this->isMapped())
    {
        this->unmap();
    }
    DwFsWrapperEx::closeFile(this->m_handle);
    this->m_handle = 0;
    DwStream::close();
}

//------------------------------------------------------------------------------
/**
*/
void
DwFileStream::write(const void* ptr, Size numBytes)
{
    DW_ASSERT(!this->isMapped());
    if (numBytes > 0)
    {
        DW_ASSERT(this->isOpen());
        DW_ASSERT(0 != this->m_handle);
        DW_ASSERT(0 != ptr);
        DwFsWrapperEx::write(this->m_handle, ptr, numBytes);
    }
}

//------------------------------------------------------------------------------
/**
*/
DwStream::Size
DwFileStream::read(void* ptr, Size numBytes)
{
    DW_ASSERT(!this->isMapped());
    DW_ASSERT(this->isOpen());
    DW_ASSERT(0 != this->m_handle);
    DW_ASSERT(0 != ptr);
    DW_ASSERT(numBytes > 0);
    return DwFsWrapperEx::read(this->m_handle, ptr, numBytes);
}

//------------------------------------------------------------------------------
/**
*/
void
DwFileStream::seek(Offset offset, SeekOrigin origin)
{
    DW_ASSERT(!this->isMapped());
    DW_ASSERT(this->isOpen());
    DW_ASSERT(0 != this->m_handle);
    DwFsWrapperEx::seek(this->m_handle, offset, origin);
}

//------------------------------------------------------------------------------
/**
*/
void
DwFileStream::flush()
{
    DW_ASSERT(!this->isMapped());
    DW_ASSERT(this->isOpen());
    DW_ASSERT(0 != this->m_handle);
    DwFsWrapperEx::flush(this->m_handle);
}

//------------------------------------------------------------------------------
/**
*/
bool
DwFileStream::eof() const
{
    DW_ASSERT(!this->isMapped());
    DW_ASSERT(this->isOpen());
    DW_ASSERT(0 != this->m_handle);
    return DwFsWrapperEx::eof(this->m_handle);
}

//------------------------------------------------------------------------------
/**
*/
void*
DwFileStream::map()
{
    DW_ASSERT(0 == this->m_mappedContent);
    
    Size size = this->size();
    DW_ASSERT(size > 0);
    this->m_mappedContent = DwMemoryAlloc(DW_SCRATCH_HEAP, size);
    this->seek(0, Begin);
    Size readSize = this->read(this->m_mappedContent, size);
    DW_UNUSED(readSize);
    DW_ASSERT(readSize == size);
    DwStream::map();
    return this->m_mappedContent;
}

//------------------------------------------------------------------------------
/**
*/
void
DwFileStream::unmap()
{
    DW_ASSERT(0 != this->m_mappedContent);
    DwStream::unmap();
    DwMemoryFree(DW_SCRATCH_HEAP, this->m_mappedContent);
    this->m_mappedContent = 0;
}
