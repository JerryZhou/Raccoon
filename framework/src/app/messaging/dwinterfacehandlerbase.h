#pragma once

#include "dwapp/dwhandler.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwInterfaceHandlerBase : public DwHandler
{
    DW_DECLARE_CLASS(DwInterfaceHandlerBase);
public:
    /// constructor
    DwInterfaceHandlerBase();
    /// set the company name
    void setCompanyName(const DwString& c);
    /// get the company name
    const DwString& companyName() const;
    /// set the application name
    void setAppName(const DwString& a);
    /// get the application name
    const DwString& appName() const;
    /// optional "per-frame" DoWork method for continuous handlers
    virtual void doWork();

protected:
    DwString m_companyName;
    DwString m_appName;
};// end of DwInterfaceHandlerBase

//------------------------------------------------------------------------------
/**
*/
inline void
DwInterfaceHandlerBase::setCompanyName(const DwString& n)
{
    this->m_companyName = n;
}

//------------------------------------------------------------------------------
/**
*/
inline const DwString&
DwInterfaceHandlerBase::companyName() const
{
    return this->m_companyName;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DwInterfaceHandlerBase::setAppName(const DwString& n)
{
    this->m_appName = n;
}

//------------------------------------------------------------------------------
/**
*/
inline const DwString&
DwInterfaceHandlerBase::appName() const
{
    return this->m_appName;
}
