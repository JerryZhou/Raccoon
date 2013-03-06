#include "stable.h"

#include <dwcore/dwfile.h>
#include <dwcore/dwmap.h>
#include <dwcore/dwhash.h>
#include <dwcore/dwglobalstatic.h>
#include <dwgraphics/dwcanvas.h>
#include <dwgraphics/dwcachedcanvas.h>
#include <dwgraphics/dwninepatch.h>

#include "dwimage.h"

//enable premultiplied rgba image
const Gdiplus::PixelFormat KDefaultImageFormat = PixelFormat32bppPARGB;

//------------------------------------------------------------------------------
/**
*/
class DwImageData : public DwSharedData
{
public:
    DwImageData(Gdiplus::Bitmap *bitmap);
    DwImageData(const DwString &fileName);
    DwImageData(const DwByteArray &data);

    virtual ~DwImageData();

private:
    Gdiplus::Bitmap *loadImage(const DwString &fileName);
    Gdiplus::Bitmap *loadImage(const DwByteArray &data);

    bool loadFromData(const DwByteArray &data);

    void initFrames();

private:
    friend class DwImage;
    friend class DwImageDataLib;
    DwAtomicInt m_key;
    DwString m_path;
    Gdiplus::Bitmap *m_bitmap;
    Gdiplus::PropertyItem* m_pItem;
    int m_frameCount;
    int m_curActiveFrame;
};

//------------------------------------------------------------------------------
/**
*/
class DwImageDataLib
{
public:
    DwHash<DwString , DwImageData*> imageDatas;
    DwHash<DwAtomicInt , DwImageData*> key2ImageDatas;

    DwImageDataLib() {}

    ~DwImageDataLib()
    {
        freeAllTheImageData();
    }

    DwImageData* allocImageData(const DwString& path)
    {
        DwHash<DwString , DwImageData*>::iterator ite = imageDatas.find(path);
        if (ite != imageDatas.end())
        {
            return ite->second;
        }

        DwImageData* d = new DwImageData(path);
        d->ref.ref();
        d->m_path = path;
        imageDatas[path] = d;
        key2ImageDatas[d->m_key] = d;
        return d;
    }

    DwImageData *allocImageData(const DwByteArray &data, const DwString &path)
    {
        DwHash<DwString , DwImageData*>::iterator ite = imageDatas.find(path);
        if (ite != imageDatas.end())
        {
            return ite->second;
        }

        DwImageData* d = new DwImageData(data);
        d->ref.ref();
        d->m_path = path;
        imageDatas[path] = d;
        key2ImageDatas[d->m_key] = d;
        return d;
    }

    DwImageData* allocImageData(Gdiplus::Bitmap *bitmap)
    {
        DwImageData* d = new DwImageData(bitmap);
        d->ref.ref();
        // TODO : USE A COMPLEX KEY GENERATOR
        DwString path = DwString::number(d->m_key);
        d->m_path = path;
        imageDatas[path] = d;
        key2ImageDatas[d->m_key] = d;
        return d;
    }

    DwImageData* allocImageData(DwAtomicInt key)
    {
        DwHash<DwAtomicInt , DwImageData*>::iterator ite = key2ImageDatas.find(key);
        if (ite != key2ImageDatas.end())
        {
            return ite->second;
        }
        return NULL;
    }

    void freeImageData(DwImageData* d)
    {
        if(d && !d->ref.deref())
        {
            // ONLY IN THE freeAllTheImageData CAN REACH THIS
            delete d;
        }
    }

    void freeAllTheImageData()
    {
        DwHash<DwString , DwImageData*>::iterator ite = imageDatas.begin();
        while(ite != imageDatas.end())
        {
            freeImageData(ite->second);
            ++ite;
        };
        imageDatas.clear();
        key2ImageDatas.clear();
    }

    void shrinkCaches(int n = -1)
    {
        DwHash<DwString , DwImageData*>::iterator ite = imageDatas.begin();
        while(ite != imageDatas.end() && n != 0)
        {
            DwImageData* d = ite->second;

            if (d && d->ref == 1)
            {
                d->ref.deref();
                key2ImageDatas.remove(d->m_key);

                ite = imageDatas.remove(ite);

                delete d;
                --n;
            }
            else
            {
                ++ite;
            }
        };
    }

    bool hasCachedOf(const DwString &key)const
    {
        DwHash<DwString , DwImageData*>::const_iterator ite = imageDatas.find(key);
        if (ite != imageDatas.end())
        {
            return true;
        }
        return false;
    }

    DwLinkedList<DwImage> allImages()const
    {
        DwLinkedList<DwImage> imags;

        DwHash<DwString , DwImageData*>::const_iterator ite = imageDatas.begin();
        while(ite != imageDatas.end())
        {
            imags.append(DwImage(ite->second));
            ++ite;
        }

        return imags;
    }
};
DW_GLOBAL_STATIC(DwImageDataLib, _dw_imageDatalib);

//------------------------------------------------------------------------------
/**
*/
DwImage::DwImage(const DwImage &other)
    : d(NULL)
{
    *this = other;
}

//------------------------------------------------------------------------------
/**
*/
DwImage::DwImage(const DwString &fileName)
{
    d = _dw_imageDatalib()->allocImageData(fileName);
    DW_ASSERT(d);
    d->ref.ref();
}

//------------------------------------------------------------------------------
/**
*/
DwImage::DwImage(DwCachedCanvas *canvas)
{
    DW_ASSERT(canvas);
    d = _dw_imageDatalib()->allocImageData(canvas->toBitmap());
    DW_ASSERT(d);
    d->ref.ref();
}

//------------------------------------------------------------------------------
/**
*/
DwImage::DwImage(DwAtomicInt key)
{
    d = _dw_imageDatalib()->allocImageData(key);
    if (d)
    {
        d->ref.ref();
    }
}

//------------------------------------------------------------------------------
/**
*/
DwImage::DwImage(const DwString &key, const DwByteArray &data)
{
    d = _dw_imageDatalib()->allocImageData(data, key);
    if (d)
    {
        d->ref.ref();
    }
}

//------------------------------------------------------------------------------
/**
*/
DwImage::DwImage( DwImageData *imageData )
    : d(imageData)
{
    d->ref.ref();
}

//------------------------------------------------------------------------------
/**
*/
DwImage::~DwImage()
{
    _dw_imageDatalib()->freeImageData(d);
}

//------------------------------------------------------------------------------
/**
*/
DwImage & DwImage::operator=( const DwImage &other )
{
    if(d != other.d)
    {
        if(other.d)
        {
            other.d->ref.ref();
        }
        DwImageData *old = d;
        d = other.d;
        _dw_imageDatalib()->freeImageData(old);
    }
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
bool DwImage::isNull() const
{
    return !d || !d->m_bitmap;
}

//------------------------------------------------------------------------------
/**
    0 is invalid key
*/
int DwImage::uniqueKey() const
{
    if (d)
    {
        return d->m_key;
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
DwString DwImage::path() const
{
    if (d)
    {
        return d->m_path;
    }
    return DwString();
}

//------------------------------------------------------------------------------
/**
*/
int DwImage::frameCount() const
{
    if (d)
    {
        return d->m_frameCount;
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
int DwImage::curActiveFrame() const
{
    if (d)
    {
        return d->m_curActiveFrame;
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
void DwImage::setActiveFrame(int index)
{
    if (d && d->m_frameCount > 1)
    {
        index = index % d->m_frameCount;
        d->m_curActiveFrame = index;
        GUID Guid = Gdiplus::FrameDimensionTime;
        Gdiplus::Status ok = d->m_bitmap->SelectActiveFrame(&Guid, index);
        if (ok != Gdiplus::Ok)
        {
            dwDebug() << "DwImage::setActiveFrame" << ok;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
int DwImage::nextInterval()const
{
    if (d && d->m_frameCount > 1 && d->m_pItem)
    {
        return (int)( ((UINT*)d->m_pItem[0].value)[d->m_curActiveFrame] * 10 );
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
int DwImage::refCnt()const
{
    if (d)
    {
        return (int)(d->ref);
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
bool DwImage::loadFromData(const DwByteArray &data)
{
    if (d)
    {
        return d->loadFromData(data);
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void * DwImage::nativeImage() const
{
    return d->m_bitmap;
}

//------------------------------------------------------------------------------
/**
*/
DwSize DwImage::size() const
{
    if (d && d->m_bitmap)
    {
        return DwSize(d->m_bitmap->GetWidth(), d->m_bitmap->GetHeight());
    }
    else
    {
        return DwSize(0, 0);
    }

}

//------------------------------------------------------------------------------
/**
*/
DwImage DwImage::copy( int x, int y, int width, int height ) const
{
    Gdiplus::Bitmap *newBitmap = d->m_bitmap->Clone(Gdiplus::Rect(x, y, width, height), KDefaultImageFormat);
    DwImageData *newImageData = new DwImageData(newBitmap);
    return DwImage(newImageData);
}

//------------------------------------------------------------------------------
/**
*/
DwColor DwImage::pixel(int x, int y)const
{
    if (d && d->m_bitmap && x >=0 && y>=0 && x<(int)d->m_bitmap->GetWidth() && y<(int)d->m_bitmap->GetHeight())
    {
        Gdiplus::Color c;
        d->m_bitmap->GetPixel(x, y, &c);
        return DwColor(c.GetR(), c.GetG(), c.GetB(), c.GetA());
    }
    return DwColor();
}

//------------------------------------------------------------------------------
/**
*/
bool DwImage::operator == (const DwImage& other)
{
    return uniqueKey() == other.uniqueKey();
}

//------------------------------------------------------------------------------
/**
*/
bool DwImage::operator != (const DwImage& other)
{
    return !(*this == other);
}

//------------------------------------------------------------------------------
/**
*/
void DwImage::initImageLib()
{
    // do hot load;
}

//------------------------------------------------------------------------------
/**
*/
void DwImage::unInitImageLib()
{
    _dw_imageDatalib()->freeAllTheImageData();
}

//------------------------------------------------------------------------------
/**
*/
void DwImage::shrinkCaches(int n/* = -1*/)
{
    _dw_imageDatalib()->shrinkCaches(n);
}

//------------------------------------------------------------------------------
/**
*/
bool DwImage::hasCachedOf(const DwString &key)
{
    return _dw_imageDatalib()->hasCachedOf(key);
}

//------------------------------------------------------------------------------
/**
*/
DwLinkedList<DwImage> DwImage::allImages()
{
    return _dw_imageDatalib()->allImages();
}

//------------------------------------------------------------------------------
/**
*/
HBITMAP DwImage::toWinHBITMAP(const DwColor& format) const
{
    HBITMAP hbitmap = NULL;
    if(d && d->m_bitmap)
    {
        Gdiplus::Color c(format.rgba());
        d->m_bitmap->GetHBITMAP(c, &hbitmap);
    }
    return hbitmap;
}

//------------------------------------------------------------------------------
/**
*/
HICON DwImage::toWinHICON() const
{
    HICON icon = NULL;
    if (d && d->m_bitmap)
    {
        d->m_bitmap->GetHICON(&icon);
    }
    return icon;
}

//------------------------------------------------------------------------------
/**
*/
HICON DwImage::toWinHICON(const DwRect& src) const
{
    if (src.isNull() || src.isEmpty() || src.size() == size())
    {
        return toWinHICON();
    }

    HICON icon = NULL;
    if (d && d->m_bitmap)
    {
        Gdiplus::Bitmap *bitmap = d->m_bitmap->Clone(src.left(), src.top(), src.width(), src.height(), PixelFormat32bppARGB);
        if(bitmap)
        {
            bitmap->GetHICON(&icon);
        }
        DW_SAFE_DELETE(bitmap);
    }
    return icon;
}

//------------------------------------------------------------------------------
/**
*/
static int _dw_imageKey()
{
    static DwAtomicInt imageKey(0);
    imageKey.ref();
    return imageKey;
}

//------------------------------------------------------------------------------
/**
*/
DwImageData::DwImageData( const DwString &fileName )
    : m_bitmap(NULL)
    , m_pItem(NULL)
    , m_frameCount(0)
    , m_curActiveFrame(0)
{
    m_key = _dw_imageKey();
    m_bitmap = loadImage(fileName);

    initFrames();
}

//------------------------------------------------------------------------------
/**
*/
DwImageData::DwImageData(Gdiplus::Bitmap *bitmap)
    : m_bitmap(bitmap)
    , m_pItem(NULL)
    , m_frameCount(0)
    , m_curActiveFrame(0)
{
    m_key = _dw_imageKey();

    initFrames();
}

//------------------------------------------------------------------------------
/**
*/
DwImageData::DwImageData(const DwByteArray &data)
    : m_bitmap(NULL)
    , m_pItem(NULL)
    , m_frameCount(0)
    , m_curActiveFrame(0)
{
    m_key = _dw_imageKey();
    m_bitmap = loadImage(data);

    initFrames();
}

//------------------------------------------------------------------------------
/**
*/
DwImageData::~DwImageData()
{
    DW_SAFE_DELETE(m_bitmap);
    DW_SAFE_FREE(m_pItem);

    DwNinePatch::releaseImage(m_key);
}

//------------------------------------------------------------------------------
/**
*/
Gdiplus::Bitmap * DwImageData::loadImage( const DwString &fileName )
{
    if (!fileName.isEmpty())
    {
        Gdiplus::Bitmap *bmp = new Gdiplus::Bitmap(fileName.constString());
        // gdiplus 1.1
        /*bmp->ConvertFormat(
            KDefaultImageFormat,
            Gdiplus::DitherTypeNone,
            Gdiplus::PaletteTypeCustom,
            NULL,
            0);*/

        if ( bmp->GetWidth() > 0 && bmp->GetHeight() > 0)
        {
            return bmp;
        }
        DW_SAFE_DELETE(bmp);
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
Gdiplus::Bitmap * DwImageData::loadImage( const DwByteArray &data )
{
    Gdiplus::Bitmap *bmp = NULL;

    if (data.size() > 0)
    {
        HGLOBAL hg = ::GlobalAlloc(GMEM_FIXED, data.size());
        DW_ASSERT(hg);

        if(hg)
        {
            memcpy(hg, data.constData(), data.size());

            IStream *spStream = 0;
            HRESULT hr = ::CreateStreamOnHGlobal(hg, TRUE, &spStream);
            DW_ASSERT(spStream);

            if(hr == S_OK && spStream)
            {
                bmp = Gdiplus::Bitmap::FromStream(spStream);
                spStream->Release();

                if (bmp)
                {
                    // gdiplus 1.1
                    /*bmp->ConvertFormat(
                        KDefaultImageFormat,
                        Gdiplus::DitherTypeNone,
                        Gdiplus::PaletteTypeCustom,
                        NULL,
                        0);*/

                    if ( bmp->GetLastStatus() == Gdiplus::Ok)
                    {
                        return bmp;
                    }
                    DW_SAFE_DELETE(bmp);
                    DW_ASSERT(false && "error bitmap data");
                }
            }
            ::GlobalFree(hg);
        }
    }

    return bmp;
}

bool DwImageData::loadFromData(const DwByteArray &data)
{
    DW_SAFE_DELETE(m_bitmap);
    DW_SAFE_FREE(m_pItem);

    m_bitmap = loadImage(data);
    initFrames();
    return true;
}

void  DwImageData::initFrames()
{
    if (m_bitmap)
    {
        int dimensions = m_bitmap->GetFrameDimensionsCount();
        if (dimensions > 0)
        {
            GUID *dimensionIDs = new GUID[dimensions];
            Gdiplus::Status ok = m_bitmap->GetFrameDimensionsList(dimensionIDs, dimensions);
            if (ok != Gdiplus::Ok)
            {
                dwDebug() << "DwImage::initFrames" << ok;
            }

            // Only The First Dimensions
            m_frameCount = m_bitmap->GetFrameCount(&dimensionIDs[0]);
            DW_SAFE_DELETE_ARRAY(dimensionIDs);
            m_curActiveFrame = 0;

            if (m_frameCount > 1)
            {
                DW_SAFE_FREE(m_pItem);
                //PropertyTagFrameDelay is a pre-defined identifier
                //to present frame-delays by GDI+
                UINT TotalBuffer = m_bitmap->GetPropertyItemSize(PropertyTagFrameDelay);
                m_pItem = (Gdiplus::PropertyItem*)malloc(TotalBuffer);
                m_bitmap->GetPropertyItem(PropertyTagFrameDelay,TotalBuffer,m_pItem);
            }
        }
    }
    else
    {
        DW_SAFE_FREE(m_pItem);
        m_frameCount = 0;
        m_curActiveFrame = 0;
    }
}

/*** deprecated image load code, FYI ***

#if 0
    Gdiplus::Bitmap bmp(fileName.constString());
    int bmpWidth = bmp.GetWidth();
    int bmpHeight = bmp.GetHeight();

    Gdiplus::Rect rect(0, 0, bmpWidth, bmpHeight);
    Gdiplus::BitmapData bitmapData;

    bmp.LockBits(
                 &rect,
                 Gdiplus::ImageLockModeRead,
                 KDefaultImageFormat,
                 &bitmapData);

    m_bitmapBits = new BYTE[bmpWidth * 4 * bmpHeight];
    memcpy(m_bitmapBits, bitmapData.Scan0, bmpWidth * 4 * bmpHeight);
    bmp.UnlockBits(&bitmapData);
    m_bitmap = new Gdiplus::Bitmap(bmpWidth, bmpHeight, bmpWidth * 4, KDefaultImageFormat, m_bitmapBits);
#elif 0
    m_bitmap = new Gdiplus::Bitmap(fileName.constString());
    int bmpWidth = m_bitmap->GetWidth();
    int bmpHeight = m_bitmap->GetHeight();

    Gdiplus::Rect rect(0, 0, bmpWidth, bmpHeight);
    m_bitmap->LockBits(
                       &rect,
                       Gdiplus::ImageLockModeRead,
                       KDefaultImageFormat,
                       &m_bitmapData);

    m_bitmap->UnlockBits(&m_bitmapData);
#else
    m_bitmap = new Gdiplus::Bitmap(fileName.constString());
    m_bitmap->ConvertFormat(
                            KDefaultImageFormat,
                            Gdiplus::DitherTypeOrdered8x8,
                            Gdiplus::PaletteTypeFixedHalftone27,
                            0,
                            0);
#endif
*/
