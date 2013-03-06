
#pragma once

template <typename T>
class DwTypeInfo
{
public:
    enum
    {
        isPointer = false,
        isComplex = true,
        isStatic = false,		//QTypeInfo中的实现为 isStatic = true，为效率问题，不采纳
        isLarge = (sizeof(T) > sizeof(void*)),
        isDummy = false
    };
};

template <typename T>
class DwTypeInfo<T*>
{
public:
    enum
    {
        isPointer = true,
        isComplex = false,
        isStatic = false,
        isLarge = false,
        isDummy = false
    };
};

//可使用以下格式对指定类型进行特化：
//DW_DECLARE_TYPEINFO(type, flags);
//其中type为数据类型名，flags为以下TYPEINFO flags的异或组合值

enum
{
    DW_COMPLEX_TYPE = 0,
    DW_PRIMITIVE_TYPE = 0x1,
    DW_STATIC_TYPE = 0,
    DW_MOVABLE_TYPE = 0x2,
    DW_DUMMY_TYPE = 0x4
};

#define DW_DECLARE_TYPEINFO_BODY(TYPE, FLAGS) \
class DwTypeInfo<TYPE > \
{ \
public: \
    enum { \
        isComplex = (((FLAGS) & DW_PRIMITIVE_TYPE) == 0), \
        isStatic = (((FLAGS) & (DW_MOVABLE_TYPE | DW_PRIMITIVE_TYPE)) == 0), \
        isLarge = (sizeof(TYPE)>sizeof(void*)), \
        isPointer = false, \
        isDummy = (((FLAGS) & DW_DUMMY_TYPE) != 0) \
    }; \
    static inline const char *name() { return #TYPE; } \
}

#define DW_DECLARE_TYPEINFO(TYPE, FLAGS) \
template<> \
DW_DECLARE_TYPEINFO_BODY(TYPE, FLAGS)


//对基本数据类型的模板特化

DW_DECLARE_TYPEINFO(bool, DW_PRIMITIVE_TYPE);
DW_DECLARE_TYPEINFO(char, DW_PRIMITIVE_TYPE);
DW_DECLARE_TYPEINFO(signed char, DW_PRIMITIVE_TYPE);
//DW_DECLARE_TYPEINFO(unsigned char, DW_PRIMITIVE_TYPE);		//uint8_t
//DW_DECLARE_TYPEINFO(short, DW_PRIMITIVE_TYPE);				//int16_t
//DW_DECLARE_TYPEINFO(unsigned short, DW_PRIMITIVE_TYPE);		//uint16_t
//DW_DECLARE_TYPEINFO(int, DW_PRIMITIVE_TYPE);					//int32_t
//DW_DECLARE_TYPEINFO(unsigned int, DW_PRIMITIVE_TYPE);			//uint32_t
DW_DECLARE_TYPEINFO(long, DW_PRIMITIVE_TYPE);
DW_DECLARE_TYPEINFO(unsigned long, DW_PRIMITIVE_TYPE);
//DW_DECLARE_TYPEINFO(__int64, DW_PRIMITIVE_TYPE);				//int64_t
//DW_DECLARE_TYPEINFO(unsigned __int64, DW_PRIMITIVE_TYPE);		//uint64_t
DW_DECLARE_TYPEINFO(float, DW_PRIMITIVE_TYPE);
//DW_DECLARE_TYPEINFO(double, DW_PRIMITIVE_TYPE);				//DwReal
DW_DECLARE_TYPEINFO(long double, DW_PRIMITIVE_TYPE);

//DW_TYPE
DW_DECLARE_TYPEINFO(uint8_t, DW_PRIMITIVE_TYPE);
DW_DECLARE_TYPEINFO(uint16_t, DW_PRIMITIVE_TYPE);
DW_DECLARE_TYPEINFO(uint32_t, DW_PRIMITIVE_TYPE);
DW_DECLARE_TYPEINFO(uint64_t, DW_PRIMITIVE_TYPE);
DW_DECLARE_TYPEINFO(int16_t, DW_PRIMITIVE_TYPE);
DW_DECLARE_TYPEINFO(int32_t, DW_PRIMITIVE_TYPE);
DW_DECLARE_TYPEINFO(int64_t, DW_PRIMITIVE_TYPE);
DW_DECLARE_TYPEINFO(DwReal, DW_PRIMITIVE_TYPE);