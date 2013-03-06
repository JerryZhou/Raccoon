#pragma once

#include "dwcore/dwiodevice.h"

class DwBufferPrivate;

class DW_CORE_EXPORT DwBuffer : public DwIODevice
{
    DW_DISABLE_COPY(DwBuffer)

public:
    explicit DwBuffer(DwObject *parent = NULL);
    DwBuffer(DwByteArray *buf, DwObject *parent = NULL);
    ~DwBuffer();

    DwByteArray &buffer();
    const DwByteArray &buffer() const;
    void setBuffer(DwByteArray *a);

    const DwByteArray &data() const;
    void setData(const DwByteArray &data);
    void setData(const char *data, int len);

    bool canReadLine() const;
    virtual bool open(DwIODevice::OpenMode flags);
    virtual void close();
    virtual int64_t size() const;
    virtual int64_t pos() const;
    virtual bool seek(int64_t pos);
    virtual bool atEnd() const;
    virtual int64_t bytesAvailable () const;

protected:
    virtual int64_t readData(char *data, int64_t maxSize);
    virtual int64_t writeData(const char *data, int64_t maxSize);

private:
    DwBufferPrivate	*d_ptr;
};

inline void DwBuffer::setData(const char *data, int len)
{
    setData(DwByteArray(data, len));
}