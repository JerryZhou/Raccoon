#pragma once
#include "dwgui/dwresourcelib.h"
#include "dwgraphics/dwimage.h"
#include "dwgui/dwurl.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwResourceInterface
{
public:
    // will be effective lookup image
    static DwImage acquireImage(const DwUrl &url);
    // will be effective lookup bytearray
    static DwByteArray acquireData(const DwUrl &url);
    // will be effective find exits
    static bool hasExist(const DwUrl &url);
    // will be deal with async 
    static DwImage acquireImage(const DwUrl &url, DwRttiObject *obj);
    // will be deal with async
    static DwByteArray acquireData(const DwUrl &url, DwRttiObject *obj, bool cached = true);
};// end of DwResourceInterface

#ifdef BUILD_DW_FRAMEWORK_STATIC_BUILD
#define DW_DECLARE_DLLRESOURCE(project, module) \
    extern int initDwResourceHandler_##project##_##module##()
#define DW_MOUNTED_DLLRESOURCE(project, module) \
    initDwResourceHandler_##project##_##module##()
#else
#define DW_DECLARE_DLLRESOURCE(project, module)
#define DW_MOUNTED_DLLRESOURCE(project, module)
#endif

#define DW_PIXMAP(url) DwResourceInterface::acquireImage(url)
#define DW_PIXMAP_ASYNC(url, obj) DwResourceInterface::acquireImage(url, obj)

#define DW_DATA(url) DwResourceInterface::acquireData(url)
#define DW_DATA_ASYNC(url, obj, cached) DwResourceInterface::acquireData(url, obj, cached)

