#include "stable.h"
#include "dwmainwindow.h"
#include "dwgui/dwtitlebar.h"

DW_IMPLEMENT_CLASS(DwMainWindow, 'MWOW', DwFrameWindow);

//------------------------------------------------------------------------------
/**
*/
DwMainWindow::DwMainWindow( DwObject *parent )
    : DwFrameWindow(parent)
{
    m_titleBar = new DwTitleBar(this);

    //accessible
    setAccessibleName(tr("raccoon main window"));
    setAccessibleDescription(tr("raccoon main window"));
    setWhatsThis(tr("the raccoon engine's main window"));
}

//------------------------------------------------------------------------------
/**
*/
DwMainWindow::~DwMainWindow()
{
    ;
}
