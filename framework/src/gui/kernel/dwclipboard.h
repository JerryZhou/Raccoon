#pragma once
#include "dwgui/dwmimedata.h"
#include "dwgui/dweventsignaldispatch.h"

//------------------------------------------------------------------------------
class DwClipBoardData;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwClipBoard : public DwRttiObject
{
    DW_DECLARE_CLASS(DwClipBoard);
public:
    enum Mode { Clipboard, Selection, FindBuffer, LastMode = FindBuffer };

    DwClipBoard();
    virtual ~DwClipBoard();

    static bool supportsSelection() ;
    static bool supportsFindBuffer() ;

    static bool ownsSelection() ;
    static bool ownsClipboard() ;
    static bool ownsFindBuffer() ;

    static void clear(Mode mode = Clipboard);

    static DwString text(Mode mode = Clipboard);
    static void setText(const DwString &, Mode mode = Clipboard);

    static const DwMimeData *mimeData(Mode mode = Clipboard ) ;
    static void setMimeData(DwMimeData *data, Mode mode = Clipboard);

    static DwImage image(Mode mode = Clipboard) ;
    static void setImage(const DwImage &, Mode mode  = Clipboard);

    static DwEventSignalDispatch *signalDispatch();
protected:
};// end of DwClipBoard