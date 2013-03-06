//------------------------------------------------------------------------------
//  stream.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stable.h"
#include "dwstream.h"

DW_IMPLEMENT_CLASS(DwStream, 'STRM', DwRefCounted);

//------------------------------------------------------------------------------
/**
*/
DwStream::DwStream() :
    m_accessMode(ReadAccess),
    m_accessPattern(Sequential),
    m_isOpen(false),
    m_isMapped(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
DwStream::~DwStream()
{
    DW_ASSERT(!this->isOpen());
    DW_ASSERT(!this->isMapped());
}

//------------------------------------------------------------------------------
/**
    Set the URI of the stream as string. The URI identifies the source
    resource of the stream.
*/
void DwStream::setURI(const DwUrl& u)
{
    DW_ASSERT(!this->isOpen());
    this->m_uri = u;
}

//------------------------------------------------------------------------------
/**
    Get the URI of the stream as string.
*/
const DwUrl& DwStream::getURI() const
{
    return this->m_uri;
}

//------------------------------------------------------------------------------
/**
    This method must return true if the derived stream class supports reading.
*/
bool DwStream::canRead() const
{
    return false;
}

//------------------------------------------------------------------------------
/**
    This method must return true if the derived stream class supports writing.
*/
bool DwStream::canWrite() const
{
    return false;
}

//------------------------------------------------------------------------------
/**
    This method must return true if the derived stream supports seeking.
*/
bool DwStream::canSeek() const
{
    return false;
}

//------------------------------------------------------------------------------
/**
    This method must return true if the stream supports direct memory
    access through the Map()/Unmap() methods.
*/
bool DwStream::canBeMapped() const
{
    return false;
}

//------------------------------------------------------------------------------
/**
    This sets a new size for the stream. Not all streams support this method.
    If the new size if smaller then the existing size, the contents will
    be clipped.
*/
void DwStream::setSize(Size /*s*/)
{
    DW_ASSERT(!this->isOpen());
}

//------------------------------------------------------------------------------
/**
    This method returns the size of the stream in bytes.
*/
DwStream::Size DwStream::size() const
{
    return 0;
}

//------------------------------------------------------------------------------
/**
    This method returns the current position of the read/write cursor.
*/
DwStream::Position DwStream::position() const
{
    return 0;
}

//------------------------------------------------------------------------------
/**
    This method sets the intended access mode of the stream. The actual
    behaviour depends on the implementation of the derived class. The
    default is ReadWrite.
*/
void DwStream::setAccessMode(AccessMode m)
{
    DW_ASSERT(!this->isOpen());
    this->m_accessMode = m;
}

//------------------------------------------------------------------------------
/**
    Get the access mode of the stream.
*/
DwStream::AccessMode DwStream::accessMode() const
{
    return this->m_accessMode;
}

//------------------------------------------------------------------------------
/**
    Set the prefered access pattern of the stream. This can be Random or
    Sequential. This is an optional flag to improve performance with
    some stream implementations. The default is sequential. The pattern
    cannot be changed while the stream is open.
*/
void DwStream::setAccessPattern(AccessPattern p)
{
    DW_ASSERT(!this->isOpen());
    this->m_accessPattern = p;
}

//------------------------------------------------------------------------------
/**
    Get the currently set prefered access pattern of the stream.
*/
DwStream::AccessPattern DwStream::accessPattern() const
{
    return this->m_accessPattern;
}

//------------------------------------------------------------------------------
/**
*/
void DwStream::setMediaType(const DwMediaType& t)
{
    this->m_mediaType = t;
}

//------------------------------------------------------------------------------
/**
*/
const DwMediaType& DwStream::mediaType() const
{
    return this->m_mediaType;
}

//------------------------------------------------------------------------------
/**
    Open the stream. Only one thread may open a stream at any time.
    Returns true if succeeded.
*/
bool DwStream::open()
{
    DW_ASSERT(!this->isOpen());
    this->m_isOpen = true;
    return true;
}

//------------------------------------------------------------------------------
/**
    Closes the stream.
*/
void DwStream::close()
{
    DW_ASSERT(this->isOpen());
    this->m_isOpen = false;
}

//------------------------------------------------------------------------------
/**
    Return true if the stream is currently open.
*/
bool DwStream::isOpen() const
{
    return this->m_isOpen;
}

//------------------------------------------------------------------------------
/**
    Write raw data to the stream. For more convenient writing, attach the
    stream to an DwStreamWriter object. This method is only valid if the 
    stream class returns true in CanWrite().
*/
void DwStream::write(const void* /*ptr*/, Size /*numBytes*/)
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(!this->m_isMapped);
}

//------------------------------------------------------------------------------
/**
    Read raw data from the stream. For more convenient reading, attach
    the stream to an DwStreamReader object. The method returns the number
    of bytes actually read. This method is only valid if the stream
    class returns true in CanRead(). Returns the number of bytes actually
    read from the stream, this may be less then numBytes, or 0 if end-of-stream
    is reached.
*/
DwStream::Size DwStream::read(void* /*ptr*/, Size /*numBytes*/)
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(!this->m_isMapped);
    return 0;
}

//------------------------------------------------------------------------------
/**
    Move the read/write cursor to a new position, returns the new position
    in the stream. This method is only supported if the stream class
    returns true in CanSeek().
*/
void DwStream::seek(Offset /*offset*/, SeekOrigin /*origin*/)
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(!this->m_isMapped);
}

//------------------------------------------------------------------------------
/**
    Flush any unsaved data. Note that unsaved data will also be flushed
    automatically when the stream is closed.
*/
void DwStream::flush()
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(!this->m_isMapped);
}

//------------------------------------------------------------------------------
/**
    Return true if the read/write cursor is at the end of the stream.
*/
bool DwStream::eof() const
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(!this->m_isMapped);
    return true;
}

//------------------------------------------------------------------------------
/**
    If the stream provides memory mapping, this method will return a pointer
    to the beginning of the stream data in memory. The application is 
    free to read and write to the stream through direct memory access. Special
    care must be taken to not read or write past the end of the mapped data
    (indicated by GetSize()). The normal Read()/Write() method are not
    valid while the stream is mapped, also the read/write cursor position
    will not be updated.
*/
void* DwStream::map()
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(this->canBeMapped());
    DW_ASSERT(!this->m_isMapped);
    this->m_isMapped = true;
    return 0;
}

//------------------------------------------------------------------------------
/**
    This will unmap a memory-mapped stream.
*/
void DwStream::unmap()
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(this->canBeMapped());
    DW_ASSERT(this->m_isMapped);
    this->m_isMapped = false;
}

//------------------------------------------------------------------------------
/**
    Returns true if the stream is currently mapped.
*/
bool DwStream::isMapped() const
{
    return this->m_isMapped;
}
