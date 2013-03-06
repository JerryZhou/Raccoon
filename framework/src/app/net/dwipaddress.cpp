
#include "stable.h"
#include "dwipaddress.h"
#include "dwapp/dwsocket.h"
#include "dwapp/dwutil.h"

#define MEM_CLEAR(p, len) ::memset(p, 0, len)

//------------------------------------------------------------------------------
/**
*/
DwIpAddress::DwIpAddress()
{
    if (!DwSocket::NetworkInitialized)
    {
        DwSocket::initNetwork();
    }
    MEM_CLEAR(&this->m_addr, sizeof(this->m_addr));
    this->m_addr.sin_family = AF_INET;
}

//------------------------------------------------------------------------------
/**
*/
DwIpAddress::DwIpAddress(const DwIpAddress& rhs) :
    m_hostName(rhs.m_hostName),
    m_addrAsString(rhs.m_addrAsString),
    m_addr(rhs.m_addr)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
DwIpAddress::DwIpAddress(const DwString& hostName_, uint16_t portNumber_)
{
    MEM_CLEAR(&this->m_addr, sizeof(this->m_addr));
    this->m_addr.sin_family = AF_INET;
    this->setHostName(hostName_);
    this->setPort(portNumber_);
}

//------------------------------------------------------------------------------
/**
*/
DwIpAddress::DwIpAddress(const DwUrl& uri)
{
    MEM_CLEAR(&this->m_addr, sizeof(this->m_addr));
    this->m_addr.sin_family = AF_INET;
    this->extractFromUri(uri);
}

//------------------------------------------------------------------------------
/**
*/
DwIpAddress::DwIpAddress(const sockaddr_in& sa)
{
    MEM_CLEAR(&this->m_addr, sizeof(this->m_addr));
    this->m_addr.sin_family = AF_INET;
    this->setSockAddr(sa);
}

//------------------------------------------------------------------------------
/**
    Set the address directly from a sockaddr_in struct. This will
    set the host name to the string representation of the 
    host address.
*/
void
DwIpAddress::setSockAddr(const sockaddr_in& sa)
{
    this->m_addr = sa;

    DwStringUtil::stringFormat(this->m_hostName, "%d.%d.%d.%d",
        sa.sin_addr.S_un.S_un_b.s_b1,
        sa.sin_addr.S_un.S_un_b.s_b2,
        sa.sin_addr.S_un.S_un_b.s_b3,
        sa.sin_addr.S_un.S_un_b.s_b4);
    this->m_addrAsString = this->m_hostName;
}

//------------------------------------------------------------------------------
/**
    Get the sockaddr_in struct, which has either been set directly
    with SetSockAddr() or indirectly through host name, port number
    or from an URI.
*/
const sockaddr_in& DwIpAddress::sockAddr() const
{
    return this->m_addr;
}

//------------------------------------------------------------------------------
/**
    Extract the host name and optionally the port number from the provided
    URI. If no port number is set in the URI, the current port number
    will be left as is. If the host name is empty, it will be set to
    "localhost".
*/
void DwIpAddress::extractFromUri(const DwUrl& uri)
{
    if (!uri.host().isEmpty())
    {
        this->setHostName(uri.host());
    }
    else
    {
        this->setHostName("localhost");
    }
    if (!uri.port().isEmpty())
    {
        this->setPort((uint16_t)uri.port().toInt32());
    }
}

//------------------------------------------------------------------------------
/**
    Set the port number. Will be translated to network byte order internally.
*/
void DwIpAddress::setPort(uint16_t port)
{
    this->m_addr.sin_port = htons(port);
}

//------------------------------------------------------------------------------
/**
    Get the port number in host byte order.
*/
uint16_t DwIpAddress::port() const
{
    return ntohs(this->m_addr.sin_port);
}

//------------------------------------------------------------------------------
/**
    Set the host name, and immediately convert it to an ip address. This 
    accepts the special hostnames "any", "broadcast", "localhost", "self" 
    and "inetself". The result ip address can be returned in string form
    with the method GetAddrAsString().
*/
void DwIpAddress::setHostName(const DwString& n)
{
    DW_ASSERT(!n.isEmpty());
    this->m_hostName = n;
    this->hostByName(n, this->m_addr.sin_addr);
    DwStringUtil::stringFormat(this->m_addrAsString, "%d.%d.%d.%d",
        this->m_addr.sin_addr.S_un.S_un_b.s_b1,
        this->m_addr.sin_addr.S_un.S_un_b.s_b2,
        this->m_addr.sin_addr.S_un.S_un_b.s_b3,
        this->m_addr.sin_addr.S_un.S_un_b.s_b4);
}

//------------------------------------------------------------------------------
/**
    Get the host name.
*/
const DwString& DwIpAddress::hostName() const
{
    return this->m_hostName;
}

//------------------------------------------------------------------------------
/**
    Return the in address as string.
*/
const DwString& DwIpAddress::hostAddr() const
{
    return this->m_addrAsString;
}

//------------------------------------------------------------------------------
/**
    This resolves a host name into a IPv4 ip address. The ip address is
    returned in network byte order in the hostAddress argument. The return value
    indicates whether the operation was successful. The following special hostnames 
    can be defined:

    - "any"         resolves to INADDR_ANY (0.0.0.0)
    - "broadcast"   resolves to INADDR_BROADCAST (255.255.255.255)
    - "localhost"   resolves to 127.0.0.1
    - "self"        (NOT IMPLEMENTED ON XBOX360) resolves to the first address of this host
    - "inetself"    (NOT IMPLEMENTED ON XBOX360) resolves to the first address which is not a LAN address

    An empty host name is invalid. A hostname can also be an address string
    of the form xxx.yyy.zzz.www.

    NOTE: resolving host names and host addresses is not supported
    on the Xbox360, this basically means that an Xbox360 devkit can function
    as a server, but not as a client (this is fine for most debugging purposes).
*/
bool DwIpAddress::hostByName(const DwString& hostName_, in_addr& outAddr)
{
    DW_ASSERT(!hostName_.isEmpty());
    outAddr.S_un.S_addr = 0;

    if (DwString("any") == hostName_)
    {
        outAddr.S_un.S_addr = htonl(INADDR_ANY);
        return true;
    }
    else if (DwString("broadcast") == hostName_)
    {
        outAddr.S_un.S_addr = htonl(INADDR_BROADCAST);
        return true;
    }
    else if ((DwString("self") == hostName_) || (DwString("inetself") == hostName_))
    {
        // get the machine's host name
        char localHostName[512];
        int err = gethostname(localHostName, sizeof(localHostName));
        if (SOCKET_ERROR == err)
        {
            return false;
        }

        // resolve own host name
        struct hostent* he = gethostbyname(localHostName);
        if (0 == he)
        {
            // could not resolve own host name
            return false;
        }

        // initialize with the default address 
        const in_addr* inAddr = (const in_addr *) he->h_addr;
        if (hostName_ == DwString("inetself"))
        {
            // if internet address requested, scan list of ip addresses
            // for a non-Class A,B or C network address
            int i;
            for (i = 0; (0 != he->h_addr_list[i]); i++)
            {
                if (isInetAddr((const in_addr *)he->h_addr_list[i]))
                {
                    inAddr = (in_addr *)he->h_addr_list[i];
                    break;
                }
            }
        }
        outAddr = *inAddr;
        return true;
    }
    else if (DwStringUtil::checkValidCharSet(hostName_, DwString(".0123456789")))
    {
        // a numeric address...
        outAddr.S_un.S_addr = inet_addr(hostName_.toCap().constData());
        return true;
    }
    else
    {
        // the default case: do a DNS name lookup
        struct hostent* he = gethostbyname(hostName_.toCap().constData());
        if (0 == he)
        {
            // could not resolve host name!
            return false;
        }
        outAddr = *((in_addr*)he->h_addr);
        return true;
    }
}

//------------------------------------------------------------------------------
/**
    This method checks if the provided address is an "internet" address,
    not a LAN address (not a class A, B or C network address).
*/
bool DwIpAddress::isInetAddr(const in_addr* addr)
{
    // generate address string from addr
    DwString addrString;
    DwStringUtil::stringFormat(addrString, "%d.%d.%d.%d", 
        addr->S_un.S_un_b.s_b1, 
        addr->S_un.S_un_b.s_b2, 
        addr->S_un.S_un_b.s_b3, 
        addr->S_un.S_un_b.s_b4); 

    // tokenize string into its members
    DwVector<DwString> tokens = DwStringUtil::tokenize(addrString, ".");
    DW_ASSERT(tokens.size() == 4);
    int b1 = tokens[0].toInt32();
    int b2 = tokens[1].toInt32();
    int b3 = tokens[2].toInt32();
    if ((b1 == 10) && (b2 >= 0) && (b2 <= 254))
    {
        // Class A net
        return false;
    }
    else if ((b1 == 172) && (b2 >= 16) && (b2 <= 31))
    {
        // Class B net
        return false;
    }
    else if ((b1 == 192) && (b2 == 168) && (b3 >= 0) && (b3 <= 254))
    {
        // Class C net
        return false;
    }
    else if (b1 < 224)
    {
        // unknown other local net type
        return false;
    }
    // an internet address
    return true;
}
