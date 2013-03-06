#pragma once
//------------------------------------------------------------------------------
/**
    @class DwByteOrder
    
    Provides information and methods to help with byte ordering issues.

    The following byte orders are defined:

    - ByteOrder::LittleEndian:  Intel byte order
    - ByteOrder::BigEndian:     Motorola byte order
    - ByteOrder::Network        network byte order (always big endian)
    - ByteOrder::Host           hardwired to little or big endian depending on host cpu

    NOTE: on console-platforms it is strongly recommended to store binary data
    in console-byte-order during asset processing on the development PC,
    so that the console doesn't waste loading time with byte conversions!
*/

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwByteOrder
{
public:
    // byte orders
    enum Type
    {
        LittleEndian = 0,           // e.g. x86
        BigEndian,                  // e.g. PowerPC        
        Network = BigEndian,        // network byte order

        #if WIN32
        Host = LittleEndian,
        #else
        Host = BigEndian,
        #endif
    };

    /// default constructor
    DwByteOrder();
    /// constructor: set byte order conversion rule
    DwByteOrder(Type fromByteOrder, Type toByteOrder);
    /// set from-byte-order
    void setFromByteOrder(Type fromByteOrder);
    /// get from-byte-order
    Type getFromByteOrder() const;
    /// set to-byte-order
    void setToByteOrder(Type toByteOrder);
    /// get to-byte-order
    Type getToByteOrder() const;
    /// endian-convert in place
    template<class TYPE> void convertInPlace(TYPE& val) const;
    /// endian-convert by copy
    template<class TYPE> TYPE convert(TYPE val) const;
    /// endian-convert in place
    template<class TYPE> static void convertInPlace(Type fromByteOrder, Type toByteOrder, TYPE& val);
    /// endian-convert by copy
    template<class TYPE> static TYPE convert(Type fromByteOrder, Type toByteOrder, TYPE val);    

private:
    Type from;
    Type to;

    union PunFloatUL
    {
        float f;
        unsigned long u;
    };
    union PunDoubleULL
    {
        double d;
        unsigned long long u;
    };
};

//------------------------------------------------------------------------------
/**
*/
__forceinline
DwByteOrder::DwByteOrder() :
    from(Host),
    to(Host)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
__forceinline
DwByteOrder::DwByteOrder(DwByteOrder::Type fromByteOrder, DwByteOrder::Type toByteOrder) :
    from(fromByteOrder),
    to(toByteOrder)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
DwByteOrder::setFromByteOrder(Type fromByteOrder)
{
    this->from = fromByteOrder;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline DwByteOrder::Type
DwByteOrder::getFromByteOrder() const
{
    return this->from;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
DwByteOrder::setToByteOrder(Type toByteOrder)
{
    this->to = toByteOrder;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline DwByteOrder::Type
DwByteOrder::getToByteOrder() const
{
    return this->to;
}

//------------------------------------------------------------------------------
/**
*/
template<> __forceinline void
DwByteOrder::convertInPlace<short>(Type fromByteOrder, Type toByteOrder, short& val)
{
    if (fromByteOrder != toByteOrder)
    {
        unsigned short res = _byteswap_ushort((unsigned short)val);
        val = (short)res;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<> __forceinline void
DwByteOrder::convertInPlace<short>(short& val) const
{
    if (this->from != this->to)
    {
        unsigned short res = _byteswap_ushort((unsigned short)val);
        val = (short)res;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<> __forceinline short
DwByteOrder::convert<short>(Type fromByteOrder, Type toByteOrder, short val)
{
    if (fromByteOrder != toByteOrder)
    {
        return (short)_byteswap_ushort((unsigned short)val);        
    }
    else
    {
        return val;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<> __forceinline short
DwByteOrder::convert<short>(short val) const
{
    if (this->from != this->to)
    {
        return (short)_byteswap_ushort((unsigned short)val);        
    }
    else
    {
        return val;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<> __forceinline void
DwByteOrder::convertInPlace<unsigned short>(Type fromByteOrder, Type toByteOrder, unsigned short& val)
{
    if (fromByteOrder != toByteOrder)
    {
        val = _byteswap_ushort(val);
    }
}

//------------------------------------------------------------------------------
/**
*/
template<> __forceinline void
DwByteOrder::convertInPlace<unsigned short>(unsigned short& val) const
{
    if (this->from != this->to)
    {
        val = _byteswap_ushort(val);
    }
}

//------------------------------------------------------------------------------
/**
*/
template<> __forceinline unsigned short
DwByteOrder::convert<unsigned short>(Type fromByteOrder, Type toByteOrder, unsigned short val)
{
    if (fromByteOrder != toByteOrder)
    {
        return _byteswap_ushort(val);
    }
    else
    {
        return val;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<> __forceinline unsigned short
DwByteOrder::convert<unsigned short>(unsigned short val) const
{
    if (this->from != this->to)
    {
        return _byteswap_ushort(val);
    }
    else
    {
        return val;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<> __forceinline void
DwByteOrder::convertInPlace<int>(Type fromByteOrder, Type toByteOrder, int& val)
{
    if (fromByteOrder != toByteOrder)
    {
        unsigned int res = _byteswap_ulong((unsigned int)val);
        val = (int)res;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<> __forceinline void
DwByteOrder::convertInPlace<int>(int& val) const
{
    if (this->from != this->to)
    {
        unsigned int res = _byteswap_ulong((unsigned int)val);
        val = (int)res;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<> __forceinline int
DwByteOrder::convert<int>(Type fromByteOrder, Type toByteOrder, int val)
{
    if (fromByteOrder != toByteOrder)
    {
        return (int) _byteswap_ulong((unsigned int)val);
    }
    else
    {
        return val;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<> __forceinline int
DwByteOrder::convert<int>(int val) const
{
    if (this->from != this->to)
    {
        return (int) _byteswap_ulong((unsigned int)val);
    }
    else
    {
        return val;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<> __forceinline void
DwByteOrder::convertInPlace<unsigned int>(Type fromByteOrder, Type toByteOrder, unsigned int& val)
{
    if (fromByteOrder != toByteOrder)
    {
        val = _byteswap_ulong(val);
    }
}

//------------------------------------------------------------------------------
/**
*/
template<> __forceinline void
DwByteOrder::convertInPlace<unsigned int>(unsigned int& val) const
{
    if (this->from != this->to)
    {
        val = _byteswap_ulong(val);
    }
}

//------------------------------------------------------------------------------
/**
*/
template<> __forceinline unsigned int
DwByteOrder::convert<unsigned int>(Type fromByteOrder, Type toByteOrder, unsigned int val)
{
    if (fromByteOrder != toByteOrder)
    {
        return _byteswap_ulong(val);
    }
    else
    {
        return val;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<> __forceinline unsigned int
DwByteOrder::convert<unsigned int>(unsigned int val) const
{
    if (this->from != this->to)
    {
        return _byteswap_ulong(val);
    }
    else
    {
        return val;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<> __forceinline void
DwByteOrder::convertInPlace<float>(Type fromByteOrder, Type toByteOrder, float& val)
{
    if (fromByteOrder != toByteOrder)
    {
        PunFloatUL pun;
        pun.f = val;
        pun.u = _byteswap_ulong(pun.u);
        val = pun.f;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<> __forceinline void
DwByteOrder::convertInPlace<float>(float& val) const
{
    if (this->from != this->to)
    {
        PunFloatUL pun;
        pun.f = val;
        pun.u = _byteswap_ulong(pun.u);
        val = pun.f;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<> __forceinline float
DwByteOrder::convert<float>(Type fromByteOrder, Type toByteOrder, float val)
{
    if (fromByteOrder != toByteOrder)
    {
        PunFloatUL pun;
        pun.f = val;
        pun.u = _byteswap_ulong(pun.u);
        return pun.f;
    }
    else
    {
        return val;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<> __forceinline float
DwByteOrder::convert<float>(float val) const
{
    if (this->from != this->to)
    {
        PunFloatUL pun;
        pun.f = val;
        pun.u = _byteswap_ulong(pun.u);
        return pun.f;
    }
    else
    {
        return val;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<> __forceinline void
DwByteOrder::convertInPlace<double>(Type fromByteOrder, Type toByteOrder, double& val)
{
    if (fromByteOrder != toByteOrder)
    {
        PunDoubleULL pun;
        pun.d = val;
        pun.u = _byteswap_uint64(pun.u);
        val = pun.d;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<> __forceinline void
DwByteOrder::convertInPlace<double>(double& val) const
{
    if (this->from != this->to)
    {
        PunDoubleULL pun;
        pun.d = val;
        pun.u = _byteswap_uint64(pun.u);
        val = pun.d;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<> __forceinline double
DwByteOrder::convert<double>(Type fromByteOrder, Type toByteOrder, double val)
{
    if (fromByteOrder != toByteOrder)
    {
        PunDoubleULL pun;
        pun.d = val;
        pun.u = _byteswap_uint64(pun.u);
        return pun.d;
    }
    else
    {
        return val;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<> __forceinline double
DwByteOrder::convert<double>(double val) const
{
    if (this->from != this->to)
    {
        PunDoubleULL pun;
        pun.d = val;
        pun.u = _byteswap_uint64(pun.u);
        return pun.d;
    }
    else
    {
        return val;
    }
}