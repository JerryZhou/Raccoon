#include "stable.h"
#include "dwcursor.h"
#include "dwcore/dwglobalstatic.h"

DW_IMPLEMENT_CLASS(DwCursor, 'CUOR', DwRttiObject);

//------------------------------------------------------------------------------
/**
*/
static HCURSOR _dw_CursorShape2WinHCursor(Dw::CursorShape cshape)
{
    DW_ASSERT(cshape >= Dw::ArrowCursor && cshape < Dw::LastCursor);
    wchar_t *sh = 0;
    switch (cshape)                          // map to windows cursor
    {
    case Dw::ArrowCursor:
        sh = IDC_ARROW;
        break;
    case Dw::UpArrowCursor:
        sh = IDC_UPARROW;
        break;
    case Dw::CrossCursor:
        sh = IDC_CROSS;
        break;
    case Dw::WaitCursor:
        sh = IDC_WAIT;
        break;
    case Dw::IBeamCursor:
        sh = IDC_IBEAM;
        break;
    case Dw::SizeVerCursor:
        sh = IDC_SIZENS;
        break;
    case Dw::SizeHorCursor:
        sh = IDC_SIZEWE;
        break;
    case Dw::SizeBDiagCursor:
        sh = IDC_SIZENESW;
        break;
    case Dw::SizeFDiagCursor:
        sh = IDC_SIZENWSE;
        break;
    case Dw::SizeAllCursor:
        sh = IDC_SIZEALL;
        break;
    case Dw::ForbiddenCursor:
        sh = IDC_NO;
        break;
    case Dw::WhatsThisCursor:
        sh = IDC_HELP;
        break;
    case Dw::BusyCursor:
        sh = IDC_APPSTARTING;
        break;
    case Dw::PointingHandCursor:
        sh = IDC_HAND;
        break;
    default:
        sh = IDC_ARROW;
    }

    HCURSOR hcurs = 0;
    if (sh)
    {
        hcurs = (HCURSOR)LoadImage(0, sh, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
    }
    return hcurs;
}

static DwString _dw_CursorShape2String(Dw::CursorShape cshape)
{
    switch (cshape)
    {
        // map to windows cursor
    case Dw::ArrowCursor:
        return DwString(L"ArrowCursor");
        break;
    case Dw::UpArrowCursor:
        return DwString(L"UpArrowCursor");
        break;
    case Dw::CrossCursor:
        return DwString(L"CrossCursor");
        break;
    case Dw::WaitCursor:
        return DwString(L"WaitCursor");
        break;
    case Dw::IBeamCursor:
        return DwString(L"IBeamCursor");
        break;
    case Dw::SizeVerCursor:
        return DwString(L"SizeVerCursor");
        break;
    case Dw::SizeHorCursor:
        return DwString(L"SizeHorCursor");
        break;
    case Dw::SizeBDiagCursor:
        return DwString(L"ArrowCursor");
        break;
    case Dw::SizeFDiagCursor:
        return DwString(L"SizeFDiagCursor");
        break;
    case Dw::SizeAllCursor:
        return DwString(L"ArrowCursor");
        break;
    case Dw::ForbiddenCursor:
        return DwString(L"ForbiddenCursor");
        break;
    case Dw::WhatsThisCursor:
        return DwString(L"WhatsThisCursor");
        break;
    case Dw::BusyCursor:
        return DwString(L"BusyCursor");
        break;
    case Dw::PointingHandCursor:
        return DwString(L"PointingHandCursor");
        break;
    default:
        return DwString(L"IDC_ARROW");
    }
}

//------------------------------------------------------------------------------
class DwCursorData : public DwSharedData
{
public:
    DwCursorData()
        : m_hcursor (NULL)
        , m_shape(Dw::ArrowCursor)
    {
        alloc();
    }

    DwCursorData(DwImage pixmap)
        : m_hcursor (NULL)
        , m_shape (Dw::BitmapCursor)
    {
        m_pixmap = pixmap;
        alloc();
    }

    DwCursorData(Dw::CursorShape shape)
        : m_hcursor(NULL)
        , m_shape(shape)
    {
        alloc();
    }

    virtual ~DwCursorData()
    {
        free();
    }

    void alloc()
    {
        if (m_hcursor) return;

        if (m_shape ==  Dw::BitmapCursor)
        {
            ;//TODO
        }
        else if (m_shape == Dw::CustomCursor)
        {
            ;//TODO
        }
        else
        {
            DW_ASSERT(m_shape >= Dw::ArrowCursor && m_shape < Dw::LastCursor);
            m_hcursor = _dw_CursorShape2WinHCursor(m_shape);
        }
    }

    void free()
    {
        if (m_hcursor)
        {
            ::DestroyCursor(m_hcursor);
            m_hcursor = NULL;
        }
    }

    Dw::CursorShape m_shape;
    DwImage m_pixmap;
    DwPoint m_hotSpot;  // TODO: xx
    HCURSOR m_hcursor;
};

//------------------------------------------------------------------------------
class DwCursorLib
{
public:
    DwCursorLib()
        : m_curCursorData(NULL)
        , m_initCursorData(NULL)
    {
        m_initCursorData = allocCursorData(Dw::ArrowCursor);
        m_initCursorData->ref.ref();

        m_curCursorData = m_initCursorData;
        m_curCursorData->ref.ref();
    }

    ~DwCursorLib()
    {
        // clear cursor stack
        if (m_cursors.size() > 0)
        {
            for (int i=0; i<m_cursors.size(); ++i)
            {
                freeCursorData(m_cursors.at(i));
            }
            m_cursors.clear();

            // set back cursor to arrow when app quit
            setCursor(m_initCursorData);
        }

        // clear the first stack
        freeCursorData(m_initCursorData);

        // clear shape cursor lib
        if (m_shape2Datas.count() > 0)
        {
            DwMap<Dw::CursorShape , DwCursorData* >::iterator ite = m_shape2Datas.begin();
            while (ite != m_shape2Datas.end())
            {
                freeCursorData(ite->second);
                ++ite;
            }
            m_shape2Datas.clear();
        }

        // clear pixmap cursor lib
        if (m_pixmap2Datas.count() > 0)
        {
            DwMap<DwImage, DwCursorData* >::iterator ite = m_pixmap2Datas.begin();
            while (ite != m_pixmap2Datas.end())
            {
                freeCursorData(ite->second);
                ++ite;
            }
            m_pixmap2Datas.clear();
        }
    }

    // the lib will own the cursor data
    DwCursorData* allocCursorData(Dw::CursorShape shape, const DwImage &bitmap = DwImage())
    {
        DW_UNUSED(bitmap);
        if (shape == Dw::BitmapCursor)
        {
            return NULL; // TODO:
        }
        else if (shape == Dw::CustomCursor)
        {
            return NULL; // TODO:
        }
        else
        {
            DwMap<Dw::CursorShape , DwCursorData* >::iterator ite = m_shape2Datas.find(shape);
            if (ite != m_shape2Datas.end())
            {
                return ite->second;
            }
            else
            {
                DwCursorData* d = new DwCursorData(shape);
                d->ref.ref();
                m_shape2Datas.insert(shape, d);
                return d;
            }
        }
    }

    void freeCursorData(DwCursorData* d)
    {
        if(!d) return;
        if(d && !d->ref.deref())
        {
            delete d;
        }
    }

    void setCursor(const DwCursor &cursor)
    {
        setCursor(cursor.d);
    }

    void setCursor(DwCursorData* cursorData)
    {
        // if some else change the cursor outside
        HCURSOR hcurs = ::GetCursor();
        if (cursorData == m_curCursorData
                && ( !m_curCursorData || hcurs == m_curCursorData->m_hcursor )) return;

        if (m_curCursorData)
        {
            freeCursorData(m_curCursorData);
        }
        m_curCursorData = cursorData;
        if (m_curCursorData)
        {
            m_curCursorData->ref.ref();
            if (m_curCursorData->m_hcursor && hcurs != m_curCursorData->m_hcursor)
            {
                ::SetCursor(m_curCursorData->m_hcursor);
            }
        }
    }

    void saveCursor()
    {
        if (m_curCursorData)
        {
            m_curCursorData->ref.ref();
            m_cursors.append(m_curCursorData);
        }
    }

    void restoreCursor()
    {
        DW_ASSERT(m_cursors.size() > 0);
        if (m_cursors.size() > 0)
        {
            DwCursorData *lastCursor = m_cursors.back();
            m_cursors.pop_back();
            setCursor(lastCursor);
            freeCursorData(lastCursor);
        }
    }

    DwPoint pos()
    {
        tagPOINT p;
        ::GetCursorPos(&p);
        return DwPoint(p.x, p.y);
    }

    void setPos(DwPoint p)
    {
        ::SetCursorPos(p.x(), p.y());
    }

    //all of cursor data pointer will own cursordata ref
    DwCursorData *m_curCursorData;
    DwCursorData *m_initCursorData;
    DwMap<Dw::CursorShape , DwCursorData* > m_shape2Datas;
    DwMap<DwImage, DwCursorData* > m_pixmap2Datas;
    DwVector<DwCursorData*> m_cursors;
protected:
};
DW_GLOBAL_STATIC(DwCursorLib, _dw_cursorLib);

//------------------------------------------------------------------------------
/**
*/
DwCursor::DwCursor()
    : d (NULL)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwCursor::DwCursor(Dw::CursorShape shape)
    : d (NULL)
{
    d = _dw_cursorLib()->allocCursorData(shape);
    d->ref.ref();
}

//------------------------------------------------------------------------------
/**
*/
DwCursor::DwCursor(DwImage image)
    : d(NULL)
{
    d = _dw_cursorLib()->allocCursorData(Dw::BitmapCursor, image);
    d->ref.ref();
}

//------------------------------------------------------------------------------
/**
*/
DwCursor::DwCursor(const DwCursor &other)
    : d(NULL)
{
    *this = other;
}

//------------------------------------------------------------------------------
/**
*/
DwCursor& DwCursor::operator = (const DwCursor &other)
{
    if(d != other.d)
    {
        if(other.d)
        {
            other.d->ref.ref();
        }
        DwCursorData *old = d;
        d = other.d;
        _dw_cursorLib()->freeCursorData(old);
    }
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
DwCursor::~DwCursor()
{
    _dw_cursorLib()->freeCursorData(d);
}

//------------------------------------------------------------------------------
/**
*/
Dw::CursorShape DwCursor::shape() const
{
    return d->m_shape;
}

//------------------------------------------------------------------------------
/**
*/
DwImage DwCursor::pixmap() const
{
    return d->m_pixmap;
}

//------------------------------------------------------------------------------
/**
*/
DwPoint DwCursor::hotSpot() const
{
    return d->m_hotSpot;
}

//------------------------------------------------------------------------------
/**
*/
void DwCursor::setHotSpot(DwPoint p)
{
    d->m_hotSpot = p;
}

//------------------------------------------------------------------------------
/**
*/
void DwCursor::save()
{
    _dw_cursorLib()->saveCursor();
}

//------------------------------------------------------------------------------
/**
*/
void DwCursor::restore()
{
    _dw_cursorLib()->restoreCursor();
}

//------------------------------------------------------------------------------
/**
*/
void DwCursor::setCursor(const DwCursor &cursor)
{
    _dw_cursorLib()->setCursor(cursor);
}

//------------------------------------------------------------------------------
/**
*/
void DwCursor::setCursor(Dw::CursorShape shape)
{
    _dw_cursorLib()->setCursor(DwCursor(shape));
}

//------------------------------------------------------------------------------
/**
*/
DwPoint DwCursor::pos()
{
    return _dw_cursorLib()->pos();
}

//------------------------------------------------------------------------------
/**
*/
void DwCursor::setPos(const DwPoint &p)
{
    _dw_cursorLib()->setPos(p);
}
