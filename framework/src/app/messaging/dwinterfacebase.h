#pragma once

#include "dwapp/dwasyncport.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwInterfaceBase : public DwAsyncPort
{
    DW_DECLARE_CLASS(DwInterfaceBase);
public:
    /// constructor
    DwInterfaceBase();
    /// destructor
    virtual ~DwInterfaceBase();
    
    /// attach a handler to the port (call before open!)
    virtual void attachHandler(const DwPtr<DwHandler>& h);
    /// open the async port
    virtual void open();

    /// get the company name
    const DwString& companyName() const;
    /// get the application name
    const DwString& appName() const;
    /// get the root directory
    const DwString& rootDirectory() const;
    
private:
    /// set the company name
    void setCompanyName(const DwString& c);
    /// set the application name
    void setAppName(const DwString& a);
    /// set the root directory (default is home:)
    void setRootDirectory(const DwString& dir);

    DwString m_rootDirectory;
    DwString m_companyName;
    DwString m_appName;
};// end of DwInterfaceBase

//------------------------------------------------------------------------------
/**
*/
inline void
DwInterfaceBase::setCompanyName(const DwString& n)
{
    this->m_companyName = n;
}

//------------------------------------------------------------------------------
/**
*/
inline const DwString&
DwInterfaceBase::companyName() const
{
    return this->m_companyName;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DwInterfaceBase::setAppName(const DwString& n)
{
    this->m_appName = n;
}

//------------------------------------------------------------------------------
/**
*/
inline const DwString&
DwInterfaceBase::appName() const
{
    return this->m_appName;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DwInterfaceBase::setRootDirectory(const DwString& d)
{
    this->m_rootDirectory = d;
}

//------------------------------------------------------------------------------
/**
*/
inline const DwString&
DwInterfaceBase::rootDirectory() const
{
    return this->m_rootDirectory;
}
    