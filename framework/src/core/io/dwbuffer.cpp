#include "stable.h"

#include "dwbuffer.h"


class DwBufferPrivate
{
public:
    DwBufferPrivate();
    ~DwBufferPrivate();

    int64_t peek(char *data, int64_t maxSize);
    DwByteArray peek(int64_t maxSize);

    DwByteArray	*m_buf;
    DwByteArray	m_defaultBuf;
    int64_t		m_ioIndex;
};

DwBufferPrivate::DwBufferPrivate()
    : m_buf(NULL)
    , m_ioIndex(0)
{
}

DwBufferPrivate::~DwBufferPrivate()
{
    DW_SAFE_DELETE(m_buf);
}

//////////////////////////////////////////////////////////////////////////

DwBuffer::DwBuffer( DwObject *parent /*= NULL*/ )
    : DwIODevice(parent)
    , d_ptr(new DwBufferPrivate)
{
    d_ptr->m_buf = &d_ptr->m_defaultBuf;
}

DwBuffer::DwBuffer( DwByteArray *buf, DwObject *parent /*= NULL*/ )
    : DwIODevice(parent)
    , d_ptr(new DwBufferPrivate)
{
    d_ptr->m_buf = buf ? buf : &d_ptr->m_defaultBuf;
    d_ptr->m_defaultBuf.clear();
}

DwBuffer::~DwBuffer()
{
    DW_SAFE_DELETE(d_ptr);
}

DwByteArray & DwBuffer::buffer()
{
    return *d_ptr->m_buf;
}

const DwByteArray & DwBuffer::buffer() const
{
    return *d_ptr->m_buf;
}

void DwBuffer::setBuffer( DwByteArray *a )
{
    d_ptr->m_buf = a ? a : &d_ptr->m_defaultBuf;
    d_ptr->m_defaultBuf.clear();
    d_ptr->m_ioIndex = 0;
}

const DwByteArray & DwBuffer::data() const
{
    return *d_ptr->m_buf;
}

void DwBuffer::setData( const DwByteArray &data )
{
    if (isOpen())
    {
        return;
    }
    *d_ptr->m_buf = data;
    d_ptr->m_ioIndex = 0;
}

bool DwBuffer::canReadLine() const
{
    if (isOpen())
    {
        return d_ptr->m_buf->indexOf('\n', int(pos())) != -1;
    }
    return false;
}

bool DwBuffer::open( DwIODevice::OpenMode flags )
{
    if ((flags & Append) == Append)
    {
        flags = (DwIODevice::OpenMode)(flags | WriteOnly);
    }

    if (!DwIODevice::open(flags))
    {
        return false;
    }

    if (!(isReadable() || isWritable()))
    {
        return false;
    }

    if ((flags & DwIODevice::Truncate) == DwIODevice::Truncate)
    {
        d_ptr->m_buf->resize(0);
    }
    if ((flags & DwIODevice::Append) == DwIODevice::Append) // append to end of buffer
    {
        seek(d_ptr->m_buf->size());
    }
    else
    {
        seek(0);
    }

    return true;
}

void DwBuffer::close()
{
    DwIODevice::close();
}

int64_t DwBuffer::size() const
{
    return int64_t(d_ptr->m_buf->size());
}

int64_t DwBuffer::pos() const
{
    return d_ptr->m_ioIndex;
}

bool DwBuffer::seek( int64_t pos )
{
    if (!isOpen())
    {
        return false;
    }
    int64_t totalSize = d_ptr->m_buf->size();
    if (pos > totalSize && isWritable())
    {
        if (seek(totalSize))
        {
            int64_t gapSize = pos - totalSize;
            if (write(DwByteArray('\0', (int)gapSize)) != gapSize)
            {
                return false;//Unable to fill gap
            }
        }
        else
        {
            return false;
        }
    }
    else if (pos > totalSize || pos < 0)
    {
        return false;
    }
    d_ptr->m_ioIndex = int(pos);
    return true;
}

bool DwBuffer::atEnd() const
{
    return !isOpen() || (d_ptr->m_buf->isEmpty() || bytesAvailable() == 0 );
}

int64_t DwBuffer::readData( char *data, int64_t maxSize )
{
    if (isReadable())
    {
        int64_t validLen = size() - pos();
        int64_t realLen =  (validLen > maxSize ? maxSize : validLen);
        strncpy(data, d_ptr->m_buf->data() + pos(), (size_t)realLen);
        data[realLen] = '\0';
        seek(pos() + realLen);
        return realLen;
    }
    return -1;
}

int64_t DwBuffer::writeData( const char *data, int64_t maxSize )
{
    if (isWritable())
    {
        d_ptr->m_buf->insert((int)pos(), data, (int)maxSize);
        seek(pos() + maxSize);
        return maxSize;
    }
    return -1;
}

int64_t DwBuffer::bytesAvailable() const
{
    return dwMax(size() - pos(), (int64_t)0);
}
