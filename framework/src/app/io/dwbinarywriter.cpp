
#include "stable.h"
#include "dwbinarywriter.h"

DW_IMPLEMENT_CLASS(DwBinaryWriter, 'BINW', DwStreamWriter);
    
//------------------------------------------------------------------------------
/**
*/
DwBinaryWriter::DwBinaryWriter() :
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
DwBinaryWriter::~DwBinaryWriter()
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
DwBinaryWriter::open()
{
    if (DwStreamWriter::open())
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
DwBinaryWriter::close()
{
    DwStreamWriter::close();
    this->m_isMapped = false;
    this->m_mapCursor = 0;
    this->m_mapEnd = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
DwBinaryWriter::writeChar(char c)
{
    if (this->m_isMapped)
    {
        DW_ASSERT(this->m_mapCursor < this->m_mapEnd);
        *this->m_mapCursor++ = c;
    }
    else
    {
        this->m_stream->write(&c, sizeof(c));
    }
}

//------------------------------------------------------------------------------
/**
*/
void
DwBinaryWriter::writeUChar(unsigned char c)
{
    if (this->m_isMapped)
    {
        DW_ASSERT(this->m_mapCursor < this->m_mapEnd);
        *this->m_mapCursor++ = c;
    }
    else
    {
        this->m_stream->write(&c, sizeof(c));
    }
}

//------------------------------------------------------------------------------
/**
*/
void
DwBinaryWriter::writeShort(short s)
{
    s = this->m_byteOrder.convert<short>(s);
    if (this->m_isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        DW_ASSERT((this->m_mapCursor + sizeof(s)) <= this->m_mapEnd);
        DwMemoryCopyFt(&s, this->m_mapCursor, sizeof(s));
        this->m_mapCursor += sizeof(s);
    }
    else
    {
        this->m_stream->write(&s, sizeof(s));
    }
}

//------------------------------------------------------------------------------
/**
*/
void
DwBinaryWriter::writeUShort(unsigned short s)
{
    s = this->m_byteOrder.convert<unsigned short>(s);
    if (this->m_isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        DW_ASSERT((this->m_mapCursor + sizeof(s)) <= this->m_mapEnd);
        DwMemoryCopyFt(&s, this->m_mapCursor, sizeof(s));
        this->m_mapCursor += sizeof(s);
    }
    else
    {
        this->m_stream->write(&s, sizeof(s));
    }
}

//------------------------------------------------------------------------------
/**
*/
void
DwBinaryWriter::writeInt(int i)
{
    i = this->m_byteOrder.convert<int>(i);
    if (this->m_isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        DW_ASSERT((this->m_mapCursor + sizeof(i)) <= this->m_mapEnd);
        DwMemoryCopyFt(&i, this->m_mapCursor, sizeof(i));
        this->m_mapCursor += sizeof(i);
    }
    else
    {
        this->m_stream->write(&i, sizeof(i));
    }
}

//------------------------------------------------------------------------------
/**
*/
void
DwBinaryWriter::writeUInt(unsigned int i)
{
    i = this->m_byteOrder.convert<unsigned int>(i);
    if (this->m_isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        DW_ASSERT((this->m_mapCursor + sizeof(i)) <= this->m_mapEnd);
        DwMemoryCopyFt(&i, this->m_mapCursor, sizeof(i));
        this->m_mapCursor += sizeof(i);
    }
    else
    {
        this->m_stream->write(&i, sizeof(i));
    }
}

//------------------------------------------------------------------------------
/**
*/
void
DwBinaryWriter::writeFloat(float f)
{
    f = this->m_byteOrder.convert<float>(f);
    if (this->m_isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        DW_ASSERT((this->m_mapCursor + sizeof(f)) <= this->m_mapEnd);
        DwMemoryCopyFt(&f, this->m_mapCursor, sizeof(f));
        this->m_mapCursor += sizeof(f);
    }
    else
    {
        this->m_stream->write(&f, sizeof(f));
    }
}

//------------------------------------------------------------------------------
/**
*/
void
DwBinaryWriter::writeFloatAsNormalizedUByte2(float f)
{
    DW_ASSERT(f >= -1.0f && f <= 1.0f);
    unsigned short compressedVal = (unsigned short)((f + 1.0f) * 32767.5f);   
    this->writeUShort(compressedVal);
}
          
//------------------------------------------------------------------------------
/**
*/
void 
DwBinaryWriter::writeFloatAsUnsignedNormalizedUByte2(float f)
{
    DW_ASSERT(f >= 0.0f && f <= 1.0f);
    unsigned short compressedVal = (unsigned short)(f * 65535.0f);   
    this->writeUShort(compressedVal);
}

//------------------------------------------------------------------------------
/**
*/
void
DwBinaryWriter::writeDouble(double d)
{
    d = this->m_byteOrder.convert<double>(d);
    if (this->m_isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        DW_ASSERT((this->m_mapCursor + sizeof(d)) <= this->m_mapEnd);
        DwMemoryCopyFt(&d, this->m_mapCursor, sizeof(d));
        this->m_mapCursor += sizeof(d);
    }
    else
    {
        this->m_stream->write(&d, sizeof(d));
    }
}

//------------------------------------------------------------------------------
/**
*/
void
DwBinaryWriter::writeBool(bool b)
{
    if (this->m_isMapped)
    {
        // note: the memory copy is necessary to circumvent alignment problem on some CPUs
        DW_ASSERT((this->m_mapCursor + sizeof(b)) <= this->m_mapEnd);
        DwMemoryCopyFt(&b, this->m_mapCursor, sizeof(b));
        this->m_mapCursor += sizeof(b);
    }
    else
    {
        this->m_stream->write(&b, sizeof(b));
    }
}

//------------------------------------------------------------------------------
/**
    NOTE: for strings, first the length will be written into a
    32-bit int, then the string contents without the 0-terminator.
*/
void
DwBinaryWriter::writeString(const DwString& s)
{
    enum { KCharBytes = sizeof(wchar_t) };
    const int numBytes = s.length()*KCharBytes;
    DW_ASSERT(numBytes < (1<<16));
    this->writeUShort(unsigned short(s.length()));
    if (s.length() > 0)
    {
        if (this->m_isMapped)
        {
            DW_ASSERT((this->m_mapCursor + numBytes) <= this->m_mapEnd);
            DwMemoryCopyFt((void*)s.constString(), this->m_mapCursor, numBytes);
            this->m_mapCursor += numBytes;
        }
        else
        {
            this->m_stream->write((void*)s.constString(), numBytes);
        }
    }
}
    
//------------------------------------------------------------------------------
/**
*/
void
DwBinaryWriter::writeRawData(const void* ptr, size_t numBytes)
{
    DW_ASSERT((ptr != 0) && (numBytes > 0));
    if (this->m_isMapped)
    {
        DW_ASSERT((this->m_mapCursor + numBytes) <= this->m_mapEnd);
        DwMemoryCopyFt(ptr, this->m_mapCursor, numBytes);
        this->m_mapCursor += numBytes;
    }
    else
    {
        this->m_stream->write(ptr, numBytes);
    }
}
