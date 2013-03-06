#pragma once

#include <dwcore/dwiodevice.h>

class DwTextCodec;
class DW_CORE_EXPORT DwTextStream
{
    DW_DISABLE_COPY(DwTextStream);

    enum TextFormat
    {
        Unknown,
        Ansi,
        Utf8,
        UnicoeLittleEndian,
        UnicoeBigEndian,
    };

public:
    DwTextStream();
    explicit DwTextStream(DwIODevice *device);

    bool atEnd() const;
    bool readChar(wchar_t *ch);
    DwString readLine(int64_t maxlen = 0);

private:
    void checkTextFormat();
    void fillReadBuffer(int64_t maxSize);

private:
    DwIODevice *m_device;
    DwString m_buffer;
    TextFormat m_format;
    DwTextCodec *m_textCodec;
};

class DW_CORE_EXPORT DwDataStream
{
    DW_DISABLE_COPY(DwDataStream)

public:
    DwDataStream();
    explicit DwDataStream(DwIODevice *device);

private:
    DwIODevice *m_device;
};