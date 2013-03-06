#pragma once

#include "dwapp/dwstream.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwMemoryStream : public DwStream
{
    DW_DECLARE_CLASS(DwMemoryStream);
public:
    /// constructor
    DwMemoryStream();
    /// destructor
    virtual ~DwMemoryStream();
    /// memory streams support reading
    virtual bool canRead() const;
    /// memory streams support writing
    virtual bool canWrite() const;
    /// memory streams support seeking
    virtual bool canSeek() const;
    /// memory streams are mappable
    virtual bool canBeMapped() const;
    /// set new size of the stream in bytes
    virtual void setSize(Size s);
    /// get the size of the stream in bytes
    virtual Size size() const;
    /// get the current position of the read/write cursor
    virtual Position position() const;
    /// open the stream
    virtual bool open();
    /// close the stream
    virtual void close();
    /// directly write to the stream
    virtual void write(const void* ptr, Size numBytes);
    /// directly read from the stream
    virtual Size read(void* ptr, Size numBytes);
    /// seek in stream
    virtual void seek(Offset offset, SeekOrigin origin);
    /// return true if end-of-stream reached
    virtual bool eof() const;
    /// map for direct memory-access
    virtual void* map();
    /// unmap a mapped stream
    virtual void unmap();
    /// get a direct "raw" pointer to the data
    void* rawPointer() const;

private:
    /// re-allocate the memory buffer
    void realloc(Size s);
    /// return true if there's enough space for n more bytes
    bool hasRoom(Size numBytes) const;
    /// make room for at least n more bytes
    void makeRoom(Size numBytes);

    static const Size InitialSize = 256;
    Size m_capacity;
    Size m_size;
    Position m_position;
    unsigned char* m_buffer;
};// end of DwMemoryStream
