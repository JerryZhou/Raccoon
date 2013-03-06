#pragma once

#include "dwapp/dwstreamwriter.h"
#include "dwapp/dwbyteorder.h"

//------------------------------------------------------------------------------
class DwBinaryWriter : public DwStreamWriter
{
    DW_DECLARE_CLASS(DwBinaryWriter);
public:
    /// constructor
    DwBinaryWriter();
    /// destructor
    virtual ~DwBinaryWriter();
    /// call before Open() to enable memory mapping (if stream supports mapping)
    void setMemoryMappingEnabled(bool b);
    /// return true if memory mapping is enabled
    bool isMemoryMappingEnabled() const;
    /// set the stream byte order (default is host byte order)
    void setStreamByteOrder(DwByteOrder::Type byteOrder);
    /// get the stream byte order
    DwByteOrder::Type streamByteOrder() const;
    /// begin reading from the stream
    virtual bool open();
    /// end reading from the stream
    virtual void close();
    /// write an 8-bit char to the stream
    void writeChar(char c);
    /// write an 8-bit unsigned char to the stream
    void writeUChar(unsigned char c);
    /// write an 16-bit short to the stream
    void writeShort(short s);
    /// write an 16-bit unsigned short to the stream
    void writeUShort(unsigned short s);
    /// write an 32-bit int to the stream
    void writeInt(int i);
    /// write an 32-bit unsigned int to the stream
    void writeUInt(unsigned int i);
    /// write a float value to the stream    
    void writeFloat(float f);
    /// write a compressed float value to the stream, lossy and needed to be in the range of -1.0 and +1.0
    void writeFloatAsNormalizedUByte2(float f);
    /// write a compressed float value to the stream, lossy and needed to be in the range of 0.0 and +1.0
    void writeFloatAsUnsignedNormalizedUByte2(float f);
    /// write a double value to the stream
    void writeDouble(double d);
    /// write a boolean value to the stream
    void writeBool(bool b);
    /// write a string to the stream
    void writeString(const DwString& s);
    /// write raw data
    void writeRawData(const void* ptr, size_t numBytes);

public:
    bool m_enableMapping;
    bool m_isMapped;
    DwByteOrder m_byteOrder;
    unsigned char* m_mapCursor;
    unsigned char* m_mapEnd;
};

//------------------------------------------------------------------------------
/**
*/
inline
void
DwBinaryWriter::setMemoryMappingEnabled(bool b)
{
    this->m_enableMapping = b;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
DwBinaryWriter::isMemoryMappingEnabled() const
{
    return this->m_enableMapping;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DwBinaryWriter::setStreamByteOrder(DwByteOrder::Type order)
{
    this->m_byteOrder.setToByteOrder(order);
}

//------------------------------------------------------------------------------
/**
*/
inline DwByteOrder::Type
DwBinaryWriter::streamByteOrder() const
{
    return this->m_byteOrder.getToByteOrder();
}