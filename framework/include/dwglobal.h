#pragma once

#include <assert.h>
#include <math.h>

#ifdef BUILD_DW_FRAMEWORK_STATIC_BUILD

#    define DW_CORE_EXPORT
#    define DW_COROUTINE_EXPORT
#    define DW_GRAPHICS_EXPORT
#    define DW_GUI_EXPORT
#    define DW_LUABIND_EXPORT
#    define EASTL_API
#    define DW_NETWORK_EXPORT
#    define DW_APP_EXPORT

#else

#  ifdef BUILD_DW_CORE
#    define DW_CORE_EXPORT __declspec( dllexport )
#  else
#    define DW_CORE_EXPORT __declspec( dllimport )
#  endif // BUILD_DW_CORE

#  ifdef BUILD_DW_COROUTINE
#    define DW_COROUTINE_EXPORT __declspec( dllexport )
#  else
#    define DW_COROUTINE_EXPORT __declspec( dllimport )
#  endif // BUILD_DW_COROUTINE

#  ifdef BUILD_DW_GRAPHICS
#    define DW_GRAPHICS_EXPORT __declspec( dllexport )
#  else
#    define DW_GRAPHICS_EXPORT __declspec( dllimport )
#  endif // BUILD_DW_GRAPHICS

#  ifdef BUILD_DW_GUI
#    define DW_GUI_EXPORT __declspec( dllexport )
#  else
#    define DW_GUI_EXPORT __declspec( dllimport )
#  endif // BUILD_DWT_GUI

#  ifdef BUILD_DW_LUABIND
#    define DW_LUABIND_EXPORT __declspec( dllexport )
#    define LUA_BUILD_AS_DLL
#    define LUA_LIB
#  else
#    define DW_LUABIND_EXPORT __declspec( dllimport )
#    define LUA_BUILD_AS_DLL
#  endif // BUILD_DW_LUABIND

#  ifdef BUILD_DW_NETWORK
#    define DW_NETWORK_EXPORT __declspec( dllexport )
#  else
#    define DW_NETWORK_EXPORT __declspec( dllimport )
#  endif// BUILD_DW_NETWORK

#  ifdef BUILD_DW_APP
#    define DW_APP_EXPORT __declspec( dllexport )
#  else
#    define DW_APP_EXPORT __declspec( dllimport )
#  endif

#endif

#define DW_SAFE_DELETE(p) { if(p) delete (p); p = NULL; }
#define DW_SAFE_DELETE_ARRAY(p) { if(p) delete[] (p); p = NULL; }
#define DW_SAFE_FREE(p) { if(p) free(p); p = NULL; }

#define DW_ASSERT(x) assert(x)
#if defined(DEBUG) || defined(_DEBUG)
#   define DW_VERIFY(x) DW_ASSERT(x)
#else
#   define DW_VERIFY(x) (x)
#endif
#define DW_UNUSED(x) (void)x;
#define DW_WARNING(x) (void)(x)

#define DW_DISABLE_COPY(Class) \
    Class(const Class &); \
    Class &operator=(const Class &);

#define DW_D(Class)	Class##Private * const d = reinterpret_cast<Class##Private *>(d_ptr)

#define dwInt64(h, l) (int64_t(h) << 32 | l)
#define dwInt64Hight(x) (int32_t(x >> 32))
#define dwInt64Low(x) (uint32_t(x & 0xFFFFFFFF))

//signed 64 bit constant
#define DW_INT64_C(c) c ## i64	
//unsigned 64 bit constant
#define DW_UINT64_C(c) c ## ui64

#ifndef DW_ENABLE_EXCEPTION
#undef _HAS_EXCEPTIONS
#define _HAS_EXCEPTIONS 0
#pragma warning(disable:4275)
#pragma warning(disable:4805)
#endif	//DW_ENABLE_EXCEPTION


