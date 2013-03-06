#pragma once

#include <dwcore/dwobject.h>
#include <dwcore/dwbytearray.h>

class DW_CORE_EXPORT DwIODevice : public DwObject
{
    DW_DISABLE_COPY(DwIODevice)

public:
    enum OpenMode
    {
        NotOpen = 0x00,
        ReadOnly = 0x01,
        WriteOnly = 0x02,
        ReadWrite = ReadOnly | WriteOnly,
        Append = 0x04,
        Truncate = 0x08
    };
    DwIODevice();
    explicit DwIODevice(DwObject *parent);

    ~DwIODevice();

    virtual bool atEnd() const;

    virtual bool open(OpenMode mode);
    virtual void close();

    virtual int64_t bytesAvailable () const;

    bool readByte(char *ch);
    int64_t read(char *data, int64_t maxSize);
    DwByteArray read(int64_t maxSize);
    DwByteArray readAll();

    bool writeByte(char ch);
    int64_t write(const char *data, int64_t size);
    int64_t write(const DwByteArray &data);

    virtual int64_t pos() const;
    virtual bool seek(int64_t pos);
    virtual int64_t size() const;

    DwIODevice::OpenMode openMode() const;
    bool isOpen() const;
    bool isReadable() const;
    bool isWritable() const;

protected:
    virtual int64_t readData(char *data, int64_t maxSize) = 0;
    virtual int64_t writeData(const char *data, int64_t maxSize) = 0;

private:
    OpenMode m_mode;
};
