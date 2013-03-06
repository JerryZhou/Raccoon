#pragma once

#include "dwcore/dwpoint.h"
#include "dwgraphics/dwimage.h"
#include "dwgui/dwrttiobject.h"

//------------------------------------------------------------------------------
class DwCursorData;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwCursor : public DwRttiObject
{
    DW_DECLARE_CLASS(DwCursor);
public:
    DwCursor();
    explicit DwCursor(Dw::CursorShape );
    explicit DwCursor(DwImage );
    DwCursor(const DwCursor &other);
    DwCursor& operator = (const DwCursor &other);
    virtual ~DwCursor();

    Dw::CursorShape shape() const;
    DwImage pixmap() const;
    DwPoint hotSpot() const;
    void setHotSpot(DwPoint p);

    static void save();
    static void restore();
    static void setCursor(const DwCursor &);
    static void setCursor(Dw::CursorShape );
    static DwPoint pos() ;
    static void setPos(const DwPoint &p);

protected:
    friend class DwCursorLib;

    DwCursorData *d;
};// end of DwCursor