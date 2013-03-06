#include "stable.h"

#include "dwiodevice.h"

DwIODevice::DwIODevice()
{
}

DwIODevice::DwIODevice( DwObject *parent )
    : DwObject(parent)
{

}

DwIODevice::~DwIODevice()
{

}

bool DwIODevice::atEnd() const
{
    return false;
}

bool DwIODevice::open( OpenMode mode )
{
    m_mode = mode;

    return true;
}

void DwIODevice::close()
{
    m_mode = NotOpen;
}

bool DwIODevice::readByte( char *ch )
{
    return readData(ch, 1) == 1;
}

int64_t DwIODevice::read( char *data, int64_t maxSize )
{
    return readData(data, maxSize);
}

DwByteArray DwIODevice::read( int64_t maxSize )
{
    DW_ASSERT(maxSize <= INT_MAX);

    DwByteArray data;
    data.resize((int)maxSize);
    readData(data.data(), maxSize);
    return data;
}

DwByteArray DwIODevice::readAll()
{
    int64_t size = bytesAvailable();
    DW_ASSERT(size <= INT_MAX);

    DwByteArray data;
    data.resize((int)size);

    readData(data.data(), size);
    return data;
}

bool DwIODevice::writeByte(char ch)
{
    return writeData(&ch, 1) == 1;
}

int64_t DwIODevice::write( const char *data, int64_t size )
{
    return writeData(data, size);
}

int64_t DwIODevice::write( const DwByteArray &data )
{
    return writeData(data.constData(), data.size());
}

int64_t DwIODevice::pos() const
{
    return 0;
}

bool DwIODevice::seek( int64_t pos )
{
    DW_ASSERT(false);
    DW_UNUSED(pos);
    return true;
}

int64_t DwIODevice::size() const
{
    return 0;
}

int64_t DwIODevice::bytesAvailable() const
{
    return 0;
}

DwIODevice::OpenMode DwIODevice::openMode() const
{
    return m_mode;
}

bool DwIODevice::isOpen() const
{
    return openMode() != NotOpen;
}

bool DwIODevice::isReadable() const
{
    return (openMode() & ReadOnly) != 0;
}

bool DwIODevice::isWritable() const
{
    return (openMode() & WriteOnly) != 0;
}
