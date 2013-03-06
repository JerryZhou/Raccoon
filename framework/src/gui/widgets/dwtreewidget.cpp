#include "stable.h"
#include "dwtreewidget.h"

DW_IMPLEMENT_CLASS(DwTreeWidget, 'DTWT', DwScrollArea);

//------------------------------------------------------------------------------
/**
*/
DwTreeWidget::DwTreeWidget( DwGraphicsItem *parent /*= NULL*/ )
: DwScrollArea(parent)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwTreeWidget::~DwTreeWidget()
{

}
