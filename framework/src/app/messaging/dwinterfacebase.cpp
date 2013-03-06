
#include "stable.h"
#include "dwinterfacebase.h"
#include "dwinterfacehandlerbase.h"


DW_IMPLEMENT_CLASS(DwInterfaceBase, 'INBS', DwAsyncPort);

//------------------------------------------------------------------------------
/**
*/
DwInterfaceBase::DwInterfaceBase() :
    m_rootDirectory("home:")
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
DwInterfaceBase::~DwInterfaceBase()        
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
DwInterfaceBase::open()
{
    DW_ASSERT(!this->isOpen());
    
    // set core server attributes
    this->setCompanyName("Duowan");
    this->setAppName("Raccoon");
    this->setRootDirectory("home:");

    DwAsyncPort::open();
}

//------------------------------------------------------------------------------
/**
*/
void
DwInterfaceBase::attachHandler(const DwPtr<DwHandler>& handler)
{
    // set application attributes on handler
    const DwPtr<DwInterfaceHandlerBase>& interfaceHandler = handler.downcast<DwInterfaceHandlerBase>();
    interfaceHandler->setCompanyName(this->m_companyName);
    interfaceHandler->setAppName(this->m_appName);

    // call parent class
    DwAsyncPort::attachHandler(handler);
}