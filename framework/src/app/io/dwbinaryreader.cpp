
#include "stable.h"
#include "dwbinaryreader.h"

DW_IMPLEMENT_CLASS(DwBinaryReader, 'BINR', DwStreamReader);

//------------------------------------------------------------------------------
/**
*/
DwBinaryReader::DwBinaryReader() :
    m_enableMapping(false),
    m_isMapped(false),
    m_mapCursor(0),
    m_mapEnd(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
DwBinaryReader::~DwBinaryReader()
{
    if (this->isOpen())
    {
        this->close();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
DwBinaryReader::open()
{
    if (DwStreamReader::open())
    {
        if (this->m_enableMapping && this->m_stream->canBeMapped())
        {
            this->m_isMapped = true;
            this->m_mapCursor = (unsigned char*) this->m_stream->map();
            this->m_mapEnd = this->m_mapCursor + this->m_stream->size();
        }
        else
        {
            this->m_isMapped = false;
            this->m_mapCursor = 0;
            this->m_mapEnd = 0;
        }
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
DwBinaryReader::close()
{
    DwStreamReader::close();
    this->m_isMapped = false;
    this->m_mapCursor = 0;
    this->m_mapEnd = 0;
}

//------------------------------------------------------------------------------
/**
*/
char
DwBinaryReader::readChar()
{
    if (this->m_isMapped)
    {
        DW_ASSERT(this->m_mapCursor < this->m_mapEnd);
        return *this->m_mapCursor++;
    }
    else
    {
        char c;
        this->m_stream->read(&c, sizeof(c));
        return c;
    }
}

//------------------------------------------------------------------------------
/**
*/
unsigned char
DwBinaryReader::readUChar()
{
    if (this->m_isMapped)
    {
        DW_ASSERT(this->m_mapCursor < this->m_mapEnd);
        return *this->m_mapCursor++;
    }
    else
    {
        unsigned char c;
        this->m_stream->read(&c, sizeof(c));
        return c;
    }
}

//------------------------------------------------------------------------------
/**
*/
short
DwBinaryReader::readShort()
{
    short val(0);
    if (this->m_isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        DW_ASSERT((this->m_mapCursor + sizeof(short)) <= this->m_mapEnd);        
        DwMemoryCopy(this->m_mapCursor, &val, sizeof(val));
        this->m_mapCursor += sizeof(val);
    }
    else
    {
        this->m_stream->read(&val, sizeof(val));
    }
    return this->m_byteOrder.convert<short>(val);
}

//------------------------------------------------------------------------------
/**
*/ 
unsigned short
DwBinaryReader::readUShort()
{
    unsigned short val(0);
    if (this->m_isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        DW_ASSERT((this->m_mapCursor + sizeof(unsigned short)) <= this->m_mapEnd);
        DwMemoryCopy(this->m_mapCursor, &val, sizeof(val));
        this->m_mapCursor += sizeof(val);
    }
    else
    {
        this->m_stream->read(&val, sizeof(val));
    }
    return this->m_byteOrder.convert<unsigned short>(val);
}

//------------------------------------------------------------------------------
/**
*/
int
DwBinaryReader::readInt()
{
    int val(0);
    if (this->m_isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        DW_ASSERT((this->m_mapCursor + sizeof(int)) <= this->m_mapEnd);
        DwMemoryCopy(this->m_mapCursor, &val, sizeof(val));
        this->m_mapCursor += sizeof(val);
    }
    else
    {
        this->m_stream->read(&val, sizeof(val));
    }
    return this->m_byteOrder.convert<int>(val);
}

//------------------------------------------------------------------------------
/**
*/
unsigned int
DwBinaryReader::readUInt()
{
    unsigned int val(0);
    if (this->m_isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        DW_ASSERT((this->m_mapCursor + sizeof(unsigned int)) <= this->m_mapEnd);
        DwMemoryCopy(this->m_mapCursor, &val, sizeof(val));
        this->m_mapCursor += sizeof(val);
    }
    else
    {
        this->m_stream->read(&val, sizeof(val));
    }
    return this->m_byteOrder.convert<unsigned int>(val);
}

//------------------------------------------------------------------------------
/**
*/
float
DwBinaryReader::readFloat()
{
    float val(0);
    if (this->m_isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        DW_ASSERT((this->m_mapCursor + sizeof(float)) <= this->m_mapEnd);
        DwMemoryCopy(this->m_mapCursor, &val, sizeof(val));
        this->m_mapCursor += sizeof(val);
    }
    else
    {
        this->m_stream->read(&val, sizeof(val));
    }
    return this->m_byteOrder.convert<float>(val);
}

//------------------------------------------------------------------------------
/**
*/
float
DwBinaryReader::readFloatFromNormalizedUByte2()
{
    unsigned short compressedVal = this->readUShort();
    float result = ((float)compressedVal / 32767.5f) + 1.0f;
    return result;
}

//------------------------------------------------------------------------------
/**
*/
float
DwBinaryReader::readFloatFromUnsignedNormalizedUByte2()
{
    unsigned short compressedVal = this->readUShort();
    float result = (float)compressedVal / 65535.0f;
    return result;
}

//------------------------------------------------------------------------------
/**
*/
double
DwBinaryReader::readDouble()
{
    double val(0);
    if (this->m_isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        DW_ASSERT((this->m_mapCursor + sizeof(double)) <= this->m_mapEnd);
        DwMemoryCopy(this->m_mapCursor, &val, sizeof(val));
        this->m_mapCursor += sizeof(val);
    }
    else
    {
        this->m_stream->read(&val, sizeof(val));
    }
    return this->m_byteOrder.convert<double>(val);
}

//------------------------------------------------------------------------------
/**
*/
bool
DwBinaryReader::readBool()
{
    bool val(0);
    if (this->m_isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        DW_ASSERT((this->m_mapCursor + sizeof(bool)) <= this->m_mapEnd);
        DwMemoryCopy(this->m_mapCursor, &val, sizeof(val));
        this->m_mapCursor += sizeof(val);
    }
    else
    {
        this->m_stream->read(&val, sizeof(val));        
    }
    return val;
}

//------------------------------------------------------------------------------
/**
*/
DwString
DwBinaryReader::readString()
{
    enum { KCharBytes = sizeof(wchar_t) };
    if (this->m_isMapped)
    {
        unsigned short length = this->readUShort();
        const int numBytes = length*KCharBytes;
        DW_ASSERT((this->m_mapCursor + numBytes) <= this->m_mapEnd);
        DwString str;
        if (length > 0)
        {
            str.resize(length + 1);
            wchar_t* buf = (wchar_t*) str.constString();
            DwMemoryCopy(this->m_mapCursor, (void*)buf, numBytes);
            this->m_mapCursor += numBytes;
            buf[length] = 0;
        }
        return str;    
    }
    else
    {
        unsigned short length = this->readUShort();
        DwString str;
        if (length > 0)
        {
            const int numBytes = length*KCharBytes;
            str.resize(length + 1);
            wchar_t* buf = (wchar_t*) str.constString();
            this->m_stream->read((void*)buf, numBytes);
            buf[length] = 0;
        }
        return str;
    }
}
    
//------------------------------------------------------------------------------
/**
*/ 
void
DwBinaryReader::readRawData(void* ptr, size_t numBytes)
{
    DW_ASSERT((ptr != 0) && (numBytes > 0));
    if (this->m_isMapped)
    {
        DW_ASSERT((this->m_mapCursor + numBytes) <= this->m_mapEnd);
        DwMemoryCopy(this->m_mapCursor, ptr, numBytes);
        this->m_mapCursor += numBytes;
    }
    else
    {
        this->m_stream->read(ptr, numBytes);
    }
}
