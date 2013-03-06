
#include "stable.h"
#include "dwmemorystream.h"

DW_IMPLEMENT_CLASS(DwMemoryStream, 'MSTR', DwStream);

//------------------------------------------------------------------------------
/**
*/
DwMemoryStream::DwMemoryStream() :
    m_capacity(0),
    m_size(0),
    m_position(0),
    m_buffer(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
DwMemoryStream::~DwMemoryStream()
{
    // close the stream if still open
    if (this->isOpen())
    {
        this->close();
    }

    // release memory buffer if allocated
    if (0 != this->m_buffer)
    {
        DwMemoryFree(DW_SCRATCH_HEAP, this->m_buffer);
        this->m_buffer = 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool DwMemoryStream::canRead() const
{
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwMemoryStream::canWrite() const
{
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwMemoryStream::canSeek() const
{
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwMemoryStream::canBeMapped() const
{
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void DwMemoryStream::setSize(Size s)
{
    if (s > this->m_capacity)
    {
        this->realloc(s);
    }
    this->m_size = s;
}

//------------------------------------------------------------------------------
/**
*/
DwStream::Size DwMemoryStream::size() const
{
    return this->m_size;
}

//------------------------------------------------------------------------------
/**
*/
DwStream::Position DwMemoryStream::position() const
{
    return this->m_position;
}

//------------------------------------------------------------------------------
/**
    Open the stream for reading or writing. The stream may already contain
    data if it has been opened/closed before. 
*/
bool DwMemoryStream::open()
{
    DW_ASSERT(!this->isOpen());
    
    // nothing to do here, allocation happens in the first Write() call
    // if necessary, all we do is reset the read/write position to the
    // beginning of the stream
    if (DwStream::open())
    {
        if (WriteAccess == this->m_accessMode)
        {
            this->m_position = 0;
            this->m_size = 0;
        }
        else if (AppendAccess == this->m_accessMode)
        {
            this->m_position = this->m_size;
        }
        else
        {
            this->m_position = 0;
        }
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
    Close the stream. The contents of the stream will remain intact until
    destruction of the object, so that the same data may be accessed 
    or modified during a later session. 
*/
void DwMemoryStream::close()
{
    DW_ASSERT(this->isOpen());
    if (this->isMapped())
    {
        this->unmap();
    }
    DwStream::close();
}

//------------------------------------------------------------------------------
/**
*/
void DwMemoryStream::write(const void* ptr, Size numBytes)
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(!this->isMapped()); 
    DW_ASSERT((WriteAccess == this->m_accessMode) || (AppendAccess == this->m_accessMode) || (ReadWriteAccess == this->m_accessMode));
    DW_ASSERT((this->m_position >= 0) && (this->m_position <= this->m_size));

    // if not enough room, allocate more memory
    if (!this->hasRoom(numBytes))
    {
        this->makeRoom(numBytes);
    }

    // write data to stream
    DW_ASSERT((this->m_position + numBytes) <= this->m_capacity);
    DwMemoryCopy(this->m_buffer + this->m_position, ptr, numBytes);
    this->m_position += numBytes;
    if (this->m_position > this->m_size)
    {
        this->m_size = this->m_position;
    }
}

//------------------------------------------------------------------------------
/**
*/
DwStream::Size DwMemoryStream::read(void* ptr, Size numBytes)
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(!this->isMapped()); 
    DW_ASSERT((ReadAccess == this->m_accessMode) || (ReadWriteAccess == this->m_accessMode));
    DW_ASSERT((this->m_position >= 0) && (this->m_position <= this->m_size));

    // check if end-of-stream is near
    Size readBytes = numBytes <= this->m_size - this->m_position ? numBytes : this->m_size - this->m_position;
    DW_ASSERT((this->m_position + readBytes) <= this->m_size);
    if (readBytes > 0)
    {
        DwMemoryCopy(ptr, this->m_buffer + this->m_position, readBytes);
        this->m_position += readBytes;
    }
    return readBytes;
}

//------------------------------------------------------------------------------
/**
*/
void DwMemoryStream::seek(Offset offset, SeekOrigin origin)
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(!this->isMapped()); 
    DW_ASSERT((this->m_position >= 0) && (this->m_position <= this->m_size));
    switch (origin)
    {
        case Begin:
            this->m_position = offset;
            break;
        case Current:
            this->m_position += offset;
            break;
        case End:
            this->m_position = this->m_size + offset;
            break;
    }

    // make sure read/write position doesn't become invalid
    this->m_position = dwClamp(this->m_position, 0, this->m_size);
}

//------------------------------------------------------------------------------
/**
*/
bool DwMemoryStream::eof() const
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(!this->isMapped());
    DW_ASSERT((this->m_position >= 0) && (this->m_position <= this->m_size));
    return (this->m_position == this->m_size);
}

//------------------------------------------------------------------------------
/**
*/
bool DwMemoryStream::hasRoom(Size numBytes) const
{
    return ((this->m_position + numBytes) <= this->m_capacity);
}

//------------------------------------------------------------------------------
/**
    This (re-)allocates the memory buffer to a new size. If the new size
    is smaller then the existing size, the buffer contents will be clipped.
*/
void DwMemoryStream::realloc(Size newCapacity)
{
    unsigned char* newBuffer = (unsigned char*) DwMemoryAlloc(DW_SCRATCH_HEAP, newCapacity);
    unsigned char* endOfNewBuffer = newBuffer + newCapacity;
    DW_UNUSED(endOfNewBuffer);
    DW_ASSERT(0 != newBuffer);
    int newSize = newCapacity < this->m_size ? newCapacity : this->m_size;
    if (0 != this->m_buffer)
    {
        DW_ASSERT((newBuffer + newSize) < endOfNewBuffer);
        DwMemoryCopy(newBuffer, this->m_buffer, newSize);
        DwMemoryFree(DW_SCRATCH_HEAP, this->m_buffer);
    }
    this->m_buffer = newBuffer;
    this->m_size = newSize;
    this->m_capacity = newCapacity;
    if (this->m_position > this->m_size)
    {
        this->m_position = this->m_size;
    }
}

//------------------------------------------------------------------------------
/**
    This method makes room for at least N more bytes. The actually allocated
    memory buffer will be greater then that. This operation involves a copy
    of existing data.
*/
void DwMemoryStream::makeRoom(Size numBytes)
{
    DW_ASSERT(numBytes > 0);
    DW_ASSERT((this->m_size + numBytes) > this->m_capacity);

    // compute new capacity
    Size oneDotFiveCurrentSize = this->m_capacity + (this->m_capacity >> 1);
    Size newCapacity = this->m_size + numBytes;
    if (oneDotFiveCurrentSize > newCapacity)
    {
        newCapacity = oneDotFiveCurrentSize;
    }
    if (16 > newCapacity)
    {
        newCapacity = 16;
    }
    DW_ASSERT(newCapacity > this->m_capacity);

    // (re-)allocate memory buffer
    this->realloc(newCapacity);
}

//------------------------------------------------------------------------------
/**
    Map the stream for direct memory access. This is much faster then 
    reading/writing, but less flexible. A mapped stream cannot grow, instead
    the allowed memory range is determined by GetSize(). The read/writer must 
    take special care to not read or write past the memory buffer boundaries!
*/
void* DwMemoryStream::map()
{
    DW_ASSERT(this->isOpen());
    DwStream::map();
    DW_ASSERT(this->size() > 0);
    return this->m_buffer;
}

//------------------------------------------------------------------------------
/**
    Unmap a memory-mapped stream.
*/
void DwMemoryStream::unmap()
{
    DW_ASSERT(this->isOpen());
    DwStream::unmap();
}

//------------------------------------------------------------------------------
/**
    Get a direct pointer to the raw data. This is a convenience method
    and only works for memory streams.
    NOTE: writing new data to the stream may/will result in an invalid
    pointer, don't keep the returned pointer around between writes!
*/
void* DwMemoryStream::rawPointer() const
{
    DW_ASSERT(0 != this->m_buffer);
    return this->m_buffer;
}
