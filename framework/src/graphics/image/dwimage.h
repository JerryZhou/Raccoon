#pragma once

#include <dwcore/dwstring.h>
#include <dwcore/dwsize.h>
#include <dwcore/dwrect.h>
#include "dwgraphics/dwcolor.h"
#include "dwcore/dwlinkedlist.h"

//------------------------------------------------------------------------------
class DwImageData;
class DwCachedCanvas;

//------------------------------------------------------------------------------
class DW_GRAPHICS_EXPORT DwImage
{
public:
    DwImage() : d(NULL) { }
    DwImage(const DwImage &other);
    explicit DwImage(const DwString &fileName);
    explicit DwImage(DwCachedCanvas *canvas);
    explicit DwImage(DwAtomicInt key);
    DwImage(const DwString &key, const DwByteArray &data);
    ~DwImage();

    DwSize size() const;

    DwImage copy(int x, int y, int width, int height) const;

    DwImage &operator=(const DwImage &other);

    DwColor pixel(int x, int y)const;

    bool isNull() const ;

    int uniqueKey() const;

    DwString path() const;

    int frameCount() const;

    int curActiveFrame() const;

    void setActiveFrame(int index) ;

    int nextInterval()const;

    int refCnt()const;

    bool loadFromData(const DwByteArray &data);

    bool operator == (const DwImage& other);
    bool operator != (const DwImage& other);

    // NB! MUST CALL BEFORE USE ANY IMAGE
    static void initImageLib();
    // MUST CALL BEFFORE THE GRAPHICS DESTROY
    static void unInitImageLib();
    // HANDLE RELEASE THE UN USED IMAGE
    static void shrinkCaches(int n = -1);
    // LOOK UP THE CACHED DATA
    static bool hasCachedOf(const DwString &key);
    // RETURN ALL THE IMAGES
    static DwLinkedList<DwImage> allImages();

    // deal with the system hbitmap, need delete object self
    HBITMAP toWinHBITMAP(const DwColor& format) const;
    // deal with the system hbitmap, need delete object self
    HICON toWinHICON() const;
    // deal with the system hbitmap, need delete object self
    HICON toWinHICON(const DwRect& src) const;

protected:
    explicit DwImage(DwImageData *imageData);
    void *nativeImage() const;

    friend class DwCanvas;
    friend class DwImageDataLib;
private:
    DwImageData *d;
};
