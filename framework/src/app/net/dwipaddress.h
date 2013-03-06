#pragma once
#include "dwgui/dwurl.h"
#include "dwgui/dwringlogcase.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwIpAddress
{
public:
    /// default constructor
    DwIpAddress();
    /// copy constructor
    DwIpAddress(const DwIpAddress& rhs);
    /// construct from URI
    DwIpAddress(const DwUrl& uri);
    /// construct from host name and port number
    DwIpAddress(const DwString& hostName, uint16_t portNumber);
    
    /// equality operator
    bool operator==(const DwIpAddress& rhs) const;
    /// less-then operator
    bool operator<(const DwIpAddress& rhs) const;
    /// greater-then operator
    bool operator>(const DwIpAddress& rhs) const;
    
    /// extract host name and port number from URI
    void extractFromUri(const DwUrl& uri);
    /// set host name
    void setHostName(const DwString& hostName);
    /// get host name
    const DwString& hostName() const;
    /// set port number
    void setPort(uint16_t port);
    /// get port number
    uint16_t port() const;
    /// get the ip address resulting from the host name as string
    const DwString& hostAddr() const;

private:
    friend class DwSocket;

    /// construct from sockaddr_in struct
    DwIpAddress(const sockaddr_in& addr);
    /// set sockaddr_in directly
    void setSockAddr(const sockaddr_in& addr);
    /// get sockaddr_in field
    const sockaddr_in& sockAddr() const;
    /// perform address resolution, understands special host names
    static bool hostByName(const DwString& hostName, in_addr& outAddr);
    /// return true if an address is an internet address (not class A,B,C)
    static bool isInetAddr(const in_addr* addr);

    DwString m_hostName;
    DwString m_addrAsString;
    sockaddr_in m_addr;
};// end of DwIpAddress

//------------------------------------------------------------------------------
/**
*/
inline bool DwIpAddress::operator==(const DwIpAddress& rhs) const
{
    return ((this->m_addr.sin_addr.S_un.S_addr == rhs.m_addr.sin_addr.S_un.S_addr) &&
        (this->m_addr.sin_port == rhs.m_addr.sin_port));
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwIpAddress::operator<(const DwIpAddress& rhs) const
{
    if (this->m_addr.sin_addr.S_un.S_addr == rhs.m_addr.sin_addr.S_un.S_addr)
    {
        return this->m_addr.sin_port < rhs.m_addr.sin_port;
    }
    else
    {
        return this->m_addr.sin_addr.S_un.S_addr < rhs.m_addr.sin_addr.S_un.S_addr;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwIpAddress::operator>(const DwIpAddress& rhs) const
{
    if (this->m_addr.sin_addr.S_un.S_addr == rhs.m_addr.sin_addr.S_un.S_addr)
    {
        return this->m_addr.sin_port > rhs.m_addr.sin_port;
    }
    else
    {
        return this->m_addr.sin_addr.S_un.S_addr > rhs.m_addr.sin_addr.S_un.S_addr;
    }
}