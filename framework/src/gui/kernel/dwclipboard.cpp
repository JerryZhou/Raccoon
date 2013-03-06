#include "stable.h"
#include "dwclipboard.h"
#include "dwclipboard_p.h"
#include "dwgui/dweventsignaldispatch.h"

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwClipBoard, 'DCBD', DwRttiObject);

//------------------------------------------------------------------------------
/**
*/
DwClipBoard::DwClipBoard()
{
    _dw_ClipBoardData();
}

//------------------------------------------------------------------------------
/**
*/
DwClipBoard::~DwClipBoard()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
bool DwClipBoard::supportsSelection()
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwClipBoard::supportsFindBuffer()
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwClipBoard::ownsSelection()
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwClipBoard::ownsClipboard()
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwClipBoard::ownsFindBuffer()
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void DwClipBoard::clear(Mode mode/* = Clipboard*/)
{
    if (mode == Clipboard)
    {
        _dw_ClipBoardData()->Clear();
    }
}

//------------------------------------------------------------------------------
/**
    TODO: xx
*/
DwString DwClipBoard::text(Mode mode/* = Clipboard*/)
{
    DwString txt;
    if (mode == Clipboard)
    {
        _dw_ClipBoardData()->GetText(txt);
    }

    return txt;
}

//------------------------------------------------------------------------------
/**
    TODO: xx
*/
void DwClipBoard::setText(const DwString &txt, Mode mode/* = Clipboard*/)
{
    if (mode == Clipboard)
    {
        _dw_ClipBoardData()->SetText(txt);
    }
}

//------------------------------------------------------------------------------
/**
        TODO: xx
*/
const DwMimeData *DwClipBoard::mimeData(Mode mode/* = Clipboard */)
{
    DW_UNUSED(mode);
    return NULL;
}

//------------------------------------------------------------------------------
/**
    TODO: xx
*/
void DwClipBoard::setMimeData(DwMimeData *data, Mode mode/* = Clipboard*/)
{
    DW_UNUSED(data);
    DW_UNUSED(mode);
}

//------------------------------------------------------------------------------
/**
    TODO: xx
*/
DwImage DwClipBoard::image(Mode mode/* = Clipboard*/)
{
    DW_UNUSED(mode);
    return DwImage();
}

//------------------------------------------------------------------------------
/**
    TODO: xx
*/
void DwClipBoard::setImage(const DwImage &img, Mode mode/*  = Clipboard*/)
{
    DW_UNUSED(img);
    DW_UNUSED(mode);
}

//------------------------------------------------------------------------------
/**
    TODO: xx
*/
DwEventSignalDispatch *DwClipBoard::signalDispatch()
{
    return &(_dw_ClipBoardData()->signalDisptach);
}