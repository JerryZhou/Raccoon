#pragma once

#include "dwapp/dwstream.h"
#include "dwapp/dwfiletime.h"
#include "dwapp/dwfswrapperex.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwFileStream : public DwStream
{
    DW_DECLARE_CLASS(DwFileStream);
public:
    /// constructor
    DwFileStream();
    /// destructor
    virtual ~DwFileStream();
    /// supports reading?
    virtual bool canRead() const;
    /// supports writing?
    virtual bool canWrite() const;
    /// supports seeking?
    virtual bool canSeek() const;
    /// supports memory mapping (read-only)?
    virtual bool canBeMapped() const;
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
    /// flush unsaved data
    virtual void flush();
    /// return true if end-of-stream reached
    virtual bool eof() const;
    /// map stream to memory
    virtual void* map();
    /// unmap stream
    virtual void unmap();

private:
    DwFsWrapper::Handle m_handle;
    void* m_mappedContent;
};// end of DwFileStream