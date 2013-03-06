#pragma once
#include "dwcore/dwrect.h"
#include "dwcore/dwmargins.h"
#include "dwcore/dwvector.h"
#include "dwgraphics/dwimage.h"

//------------------------------------------------------------------------------
class DW_GRAPHICS_EXPORT DwNinePatch
{
public:
    /// info struct
    struct NinePatchInfo 
    {
        DwRect srcRect;
        DwMargins margins;
    };
    typedef DwVector<NinePatchInfo> NinePatchInfos;

    static bool isNinePatchs(const DwImage&);
    static int numOfSubs(const DwImage &);
    static DwRect rectOfSub(const DwImage &, int idx);
    static DwMargins marginsOfSub(const DwImage &, int idx);
    static NinePatchInfo ninePatchInfo(const DwImage &, int idx);
    static NinePatchInfos ninePatchInfos(const DwImage &);
    static bool isValid(const DwImage &, int idx);

protected:
    friend class DwImage;
    friend class DwImageData;

    static void releaseImage(const DwImage &);
    static void releaseImage(int uniqueKey);
    static void setupImage(const DwImage &);
};// end of DW_GRAPHICS_EXPORT