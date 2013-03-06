#include "dwcore/dwglobalstatic.h"
// NB! THE RTTI ARE NOT THREAD SAFE
//------------------------------------------------------------------------------
/**
    Declaration macro. Put this into the class declaration.
*/
#define DW_DECLARE_CLASS(type) \
public: \
    void* operator new(size_t s) \
    { \
		DW_UNUSED(s); \
		DW_ASSERT(RTTI()->instanceSize() == s && "should declare rtti in the class"); \
        return RTTI()->allocInstanceMemory(); \
    }; \
    void operator delete(void* p) \
    { \
        RTTI()->freeInstanceMemory(p); \
    }; \
    static DwRtti* RTTI(); \
    static DwRttiObject* factoryCreator(); \
    static type* create(); \
    static bool registerWithFactory(); \
    virtual DwRtti* rtti() const; \
private:

#define DW_DECLARE_ABSTRACT_CLASS(class_name) \
public: \
    static DwRtti* RTTI(); \
    virtual DwRtti* rtti() const; \
private:

//------------------------------------------------------------------------------
/**
    Register a class with the factory. This is only necessary for classes
    which can create objects by name or fourcc.
*/
#define DW_REGISTER_CLASS(type) \
    static const bool type##_registered = type::registerWithFactory(); \
 
//------------------------------------------------------------------------------
/**
    Implementation macro for default memory pool sizes. Put this into the source file.
*/
#define DW_IMPLEMENT_CLASS(type, fourcc, baseType) \
	DwRtti* type::RTTI() \
	{\
		static DwRtti* RTTI = NULL;\
		if(!RTTI) RTTI = new DwRtti(#type, fourcc, type::factoryCreator, baseType::RTTI(), sizeof(type));\
		return RTTI; \
	}\
	DwRtti* type::rtti() const { return type::RTTI(); } \
    DwRttiObject* type::factoryCreator() { return type::create(); } \
    type* type::create() \
    { \
        return new(type); \
    }\
    bool type::registerWithFactory() \
    { \
        if (!dwFactory()->classExists(#type)) \
        { \
            dwFactory()->registerClass(type::RTTI(), #type, fourcc); \
        } \
        return true; \
    }


#define DW_IMPLEMENT_ABSTRACT_CLASS(type, fourcc, baseType) \
	DwRtti* type::RTTI() \
	{\
		static DwRtti* RTTI = NULL;\
		if(!RTTI) RTTI = new DwRtti(#type, fourcc, 0, baseType::RTTI(), sizeof(type));\
		return RTTI; \
	}\
    DwRtti* type::rtti() const { return type::RTTI(); }

//------------------------------------------------------------------------------
/**
    Type implementation of topmost type in inheritance hierarchy (source file).
*/
#define DW_IMPLEMENT_ROOT_CLASS(type, fourcc) \
	DwRtti* type::RTTI() \
	{\
		static DwRtti* RTTI = NULL;\
		if(!RTTI) RTTI = new DwRtti(#type, fourcc, type::factoryCreator, 0, sizeof(type));\
		return RTTI; \
	}\
    DwRtti* type::rtti() const { return type::RTTI(); } \
    DwRttiObject* type::factoryCreator() { return type::create(); } \
    type* type::create() \
    { \
        return new(type); \
    }\
    bool type::registerWithFactory() \
    { \
        if (!dwFactory()->classExists(#type)) \
        { \
            dwFactory()->registerClass(type::RTTI(), #type, fourcc); \
        } \
        return true; \
    }
//------------------------------------------------------------------------------

#define DW_ONCE_ASSERT(exp) \
{\
    static volatile bool _assert_once_ = true;\
    if(!(exp) && _assert_once_)\
    {\
        DW_ASSERT(false);\
        _assert_once_ = false;\
    }\
}

template< bool expr> struct CompileTimeChecker;
template<> struct CompileTimeChecker< true> {};
#define DW_STATIC_ASSERT(expr) \
{\
    (void)sizeof(CompileTimeChecker< (expr) != 0>);\
}