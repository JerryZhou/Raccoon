#pragma once

#include "dwapp/dwrefcounted.h"
#include "dwgui/dwurl.h"
#include "dwgui/dwmediatype.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwStream : public DwRefCounted
{
    DW_DECLARE_CLASS(DwStream);
public:
    /// typedefs
    typedef int Position;       // DON'T DEFINE AS UNSIGNED!
    typedef int Offset;         // DON'T DEFINE AS UNSIGNED!
    typedef int Size;           // DON'T DEFINE AS UNSIGNED!

    /// access modes
    enum AccessMode
    {
        ReadAccess,
        WriteAccess,
        AppendAccess,
        ReadWriteAccess,
    };

    /// access prefered pattern
    enum AccessPattern
    {
        Random,
        Sequential,
    };

    /// seek origins
    enum SeekOrigin
    {
        Begin,
        Current,
        End,
    };
    /// constructor
    DwStream();
    /// destructor
    virtual ~DwStream();
    /// set stream location as URI
    void setURI(const DwUrl& u);
    /// get stream URI
    const DwUrl& getURI() const;
    /// return true if the stream supports reading
    virtual bool canRead() const;
    /// return true if the stream supports writing
    virtual bool canWrite() const;
    /// return true if the stream supports seeking
    virtual bool canSeek() const;
    /// return true if the stream provides direct memory access
    virtual bool canBeMapped() const;
    /// set a new size for the stream
    virtual void setSize(Size s);
    /// get the size of the stream in bytes
    virtual Size size() const;
    /// get the current position of the read/write cursor
    virtual Position position() const;
    /// set the access mode of the stream (default is ReadAccess)
    void setAccessMode(AccessMode m);
    /// get the access mode of the stream
    AccessMode accessMode() const;
    /// set the prefered access pattern (default is Sequential)
    void setAccessPattern(AccessPattern p);
    /// get the prefered access pattern
    AccessPattern accessPattern() const;
    /// set optional media type of stream content
    void setMediaType(const DwMediaType& t);
    /// get optional media type
    const DwMediaType& mediaType() const;
    /// open the stream
    virtual bool open();
    /// close the stream
    virtual void close();
    /// return true if currently open
    bool isOpen() const;
    /// directly write to the stream
    virtual void write(const void* ptr, Size numBytes);
    /// directly read from the stream
    virtual Size read(void* ptr, Size numBytes);
    /// seek in stream
    virtual void seek(Offset offset, SeekOrigin origin);
    /// flush unsaved data
    virtual void flush();
    /// return true if end-of-stream reached
    virtual bool eof() const;
    /// map stream to memory
    virtual void* map();
    /// unmap stream
    virtual void unmap();
    /// return true if stream is currently mapped to memory
    bool isMapped() const;
        
protected:
    DwUrl m_uri;
    AccessMode m_accessMode;
    AccessPattern m_accessPattern;
    bool m_isOpen;
    bool m_isMapped;
    DwMediaType m_mediaType;
};// end of DwStream

#define DW_SCRATCH_HEAP 0

#define DwMemoryAlloc(h, size) \
    ::malloc(size)

#define DwMemoryFree(h, ptr) \
    ::free(ptr);\
    ptr = NULL

#define DwMemoryCopy(to, from, num) \
    ::memcpy(to, from, num)

#define DwMemoryCopyFt(from, to, num) \
    ::memcpy(to, from, num)