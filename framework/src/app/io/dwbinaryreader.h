#pragma once

#include "dwapp/dwstreamreader.h"
#include "dwapp/dwbyteorder.h"

//------------------------------------------------------------------------------
class DwBinaryReader : public DwStreamReader
{
    DW_DECLARE_CLASS(DwBinaryReader);
public:        
    /// constructor
    DwBinaryReader();
    /// destructor
    virtual ~DwBinaryReader();
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
    /// read an 8-bit char from the stream
    char readChar();
    /// read an 8-bit unsigned character from the stream
    unsigned char readUChar();
    /// read a 16-bit short from the stream
    short readShort();
    /// read a 16-bit unsigned short from the stream
    unsigned short readUShort();
    /// read a 32-bit int from the stream
    int readInt();
    /// read a 32-bit unsigned int from the stream
    unsigned int readUInt();
    /// read a float value from the stream
    float readFloat();
    /// read a compressed float value from the stream, lossy and needed to be in the range of -1.0 and +1.0
    float readFloatFromNormalizedUByte2();
    /// read a compressed float value from the stream, lossy and needed to be in the range of 0.0 and +1.0
    float readFloatFromUnsignedNormalizedUByte2();
    /// read a double value from the stream
    double readDouble();
    /// read a bool value from the stream
    bool readBool();
    /// read a string from the stream
    DwString readString();
    /// read raw data
    void readRawData(void* ptr, size_t numBytes);

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
inline void
DwBinaryReader::setMemoryMappingEnabled(bool b)
{
    this->m_enableMapping = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
DwBinaryReader::isMemoryMappingEnabled() const
{
    return this->m_enableMapping;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DwBinaryReader::setStreamByteOrder(DwByteOrder::Type order)
{
    this->m_byteOrder.setFromByteOrder(order);
}

//------------------------------------------------------------------------------
/**
*/
inline DwByteOrder::Type
DwBinaryReader::streamByteOrder() const
{
    return this->m_byteOrder.getFromByteOrder();
}
