#include "stable.h"
#include "dwmainapplication.h"
#include "dwapp/dwhttpinterface.h"

DW_IMPLEMENT_CLASS(DwMainApplication, 'MAON', DwApplication);

DwPtr<DwHttpInterface> _httpInterface;

//------------------------------------------------------------------------------
/**
*/
DwMainApplication::DwMainApplication()
{
    _httpInterface = DwHttpInterface::create();
    _httpInterface->open();
}

//------------------------------------------------------------------------------
/**
*/
DwMainApplication::~DwMainApplication()
{
    _httpInterface->close();
    _httpInterface = NULL;
}
