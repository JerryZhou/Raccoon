#include "stable.h"

#include "codecs/dwtextcodecs.h"
#include "dwstream.h"

const int KTextStreamBufferSize = 16 * 1024;

DwTextStream::DwTextStream()
    : m_device(NULL)
    , m_format(Unknown)
    , m_textCodec(NULL)
{
}

bool DwTextStream::atEnd() const
{
    if(m_buffer.length())
    {
        return false;
    }
    return m_device->atEnd();
}

DwString DwTextStream::readLine(int64_t maxlen)
{
    DwString line;
    wchar_t ch = 0;
    while((maxlen == 0 || line.length() < maxlen) && readChar(&ch))
    {
        if(ch == L'\n')
        {
            break;
        }
        if(ch != L'\r')
        {
            line += ch;
        }
    }

    return line;
}

DwTextStream::DwTextStream( DwIODevice *device )
    : m_device(device)
    , m_format(Unknown)
{
    DW_ASSERT(m_device->pos() == 0);
}

bool DwTextStream::readChar(wchar_t *ch)
{
    if(m_buffer.length() == 0)
    {
        fillReadBuffer(16384);

        if(m_buffer.length() == 0)
        {
            return false;
        }
    }

    *ch = m_buffer[0];
    m_buffer.remove(0, 1);
    return true;
}

void DwTextStream::fillReadBuffer(int64_t maxSize)
{
    maxSize;
    checkTextFormat();

    char buf[KTextStreamBufferSize];
    int64_t bytesRead = m_device->read(buf, KTextStreamBufferSize);
    if(bytesRead == 0)
    {
        return;
    }

    int bufSize = (int)bytesRead;
    switch (m_format)
    {
    case Ansi:
    {
        int remainBytes = 0;
        m_textCodec->toUnicode(buf, bufSize, &remainBytes);
        if(remainBytes)
        {
            m_device->seek(m_device->pos() - remainBytes);
        }
        m_buffer += DwString::fromAscii(buf, bufSize);
        break;
    }
    case Utf8:
    {
        int remainBytes = 0;
        m_buffer += m_textCodec->toUnicode(buf, bufSize, &remainBytes);
        if(remainBytes)
        {
            m_device->seek(m_device->pos() - remainBytes);
        }
        break;
    }
    case UnicoeLittleEndian:
    {
        m_buffer += DwString::fromWCharArray((const wchar_t*)buf, bufSize / 2);
        int remainBytes = bytesRead % 2;
        if(remainBytes)
        {
            m_device->seek(m_device->pos() - remainBytes);
        }
        break;
    }
    case UnicoeBigEndian:
    {
        break;
    }
    }
}

void DwTextStream::checkTextFormat()
{
    if(m_format != Unknown)
    {
        return;
    }

    int64_t size = m_device->size();
    if(size < 3)
    {
        m_format = Ansi;
    }

    DwByteArray bom = m_device->read(3);
    if(bom[0] == '\xEF' && bom[1] == '\xBB' && bom[2] == '\xBF')
    {
        m_format = Utf8;
        m_textCodec = new DwTextCodec(DwTextCodec::Utf8);
    }
    else if(bom[0] == '\xFF' && bom[1] == '\xFE')
    {
        m_format = UnicoeLittleEndian;
        m_device->seek(2);
    }
    else if(bom[0] == '\xFE' && bom[1] == '\xFF')
    {
        m_format = UnicoeBigEndian;
        m_device->seek(0);
    }
    else
    {
        m_format = Ansi;
        m_textCodec = new DwTextCodec(DwTextCodec::Locale);
        m_device->seek(0);
    }
}

DwDataStream::DwDataStream( DwIODevice *device )
{
    DW_UNUSED(device)
}

DwDataStream::DwDataStream()
{
}
