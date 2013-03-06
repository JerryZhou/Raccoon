#pragma once

#include "dwapp/dwrefcounted.h"
#include "dwapp/dwinterlocked.h"
#include "dwapp/dwptr.h"
#include "dwapp/dwipaddress.h"


//------------------------------------------------------------------------------
class DW_APP_EXPORT DwSocket : public DwRefCounted
{
    DW_DECLARE_CLASS(DwSocket);
public:
    /// protocol types
    enum Protocol
    {
        TCP,            // a reliable TCP connection
        UDP,            // unreliable UDP connection
    };

    // return values
    enum Result
    {
        Error = 0,      // an error occured, call GetErrorCode() or GetErrorString() for more info
        Success,        // everything ok
        WouldBlock,     // operation on non-blocking socket is not complete
        Closed,         // connection has been gracefully closed
    };

    /// error codes
    enum ErrorCode
    {
        ErrorNone,                   // no error occured
        ErrorUnknown,                // an "unknown" error occured     
        ErrorInterrupted,            // WSAEINTR
        ErrorPermissionDenied,       // WSAEACCESS
        ErrorBadAddress,             // WSAEFAULT
        ErrorInvalidArgument,        // WSAEINVAL
        ErrorTooManyOpenFiles,       // WSAEMFILE
        ErrorWouldBlock,             // WSAEWOULDBLOCK
        ErrorInProgress,             // WSAEINPROGRESS
        ErrorAlreadyInProgress,      // WSAEALREADY
        ErrorNotASocket,             // WSAENOTSOCK
        ErrorDestAddrRequired,       // WSAEDESTADDRREQ
        ErrorMsgTooLong,             // WSAEMSGSIZE
        ErrorInvalidProtocol,        // WSAEPROTOTYPE
        ErrorBadProtocolOption,      // WSAENOPROTOOPT
        ErrorProtocolNotSupported,   // WSEAPROTONOSUPPORT
        ErrorSocketTypeNotSupported, // WSAESOCKTNOSUPPORT
        ErrorOperationNotSupported,  // WSAEOPNOTSUPP
        ErrorProtFamilyNotSupported, // WSAEPFNOSUPPORT
        ErrorAddrFamilyNotSupported, // WSAEAFNOSUPPORT
        ErrorAddrInUse,              // WSAEADDRINUSE
        ErrorAddrNotAvailable,       // WSAEADDRNOTAVAIL
        ErrorNetDown,                // WSAENETDOWN
        ErrorNetUnreachable,         // WSAENETUNREACH
        ErrorNetReset,               // WSAENETRESET
        ErrorConnectionAborted,      // WSAECONNABORTED
        ErrorConnectionReset,        // WSAECONNRESET
        ErrorNoBufferSpace,          // WSAENOBUFS
        ErrorIsConnected,            // WSAEISCONN
        ErrorNotConnected,           // WSAENOTCONNECTED
        ErrorIsShutdown,             // WSAESHUTDOWN
        ErrorIsTimedOut,             // WSAETIMEDOUT
        ErrorConnectionRefused,      // WSAECONNREFUSED
        ErrorHostDown,               // WSAEHOSTDOWN
        ErrorHostUnreachable,        // WSAEHOSTUNREACH
        ErrorTooManyProcesses,       // WSAEPROCLIM
        ErrorSystemNotReady,         // WSAESYSNOTREADY
        ErrorVersionNotSupported,    // WSAEVERNOTSUPPORTED
        ErrorNotInitialized,         // WSAENOTINITIALISED
        ErrorDisconnecting,          // WSAEDISCONN
        ErrorTypeNotFound,           // WSAETYPE_NOT_FOUND
        ErrorHostNotFound,           // WSAEHOST_NOT_FOUND
        ErrorTryAgain,               // WSAETRY_AGAIN
        ErrorNoRecovery,             // WSAENO_RECOVERY
        ErrorNoData,                 // WSAENO_DATA
    };

    /// constructor
    DwSocket();
    /// destructor
    virtual ~DwSocket();
    /// static initializer method (called by SysFunc::Setup())
    static void initNetwork();
    /// is network initialized
    static bool isNetworkInitialized();

    /// open the socket
    bool open(Protocol p);
    /// close the socket
    void close();
    /// return true if the socket is open
    bool isOpen() const;
    /// get the last error code
    ErrorCode errorCode() const;
    /// get the last error string
    DwString errorString() const;

    /// set internet address of socket
    void setAddress(const DwIpAddress& a);
    /// get internet address of socket
    const DwIpAddress& address() const;
    
    /// set the broadcast flag (SO_BROADCAST)
    void setBroadcast(bool b);
    /// get the broadcast flag
    bool broadcast();
    /// set the keepalive flag (SO_KEEPALIVE)
    void setKeepAlive(bool b);
    /// get the keepalive flag
    bool keepAlive();
    /// set reuseaddr flag (SO_REUSEADDR)
    void setReUseAddr(bool b);
    /// get reuseaddr flag
    bool reUseAddr();
    /// set nodelay flag (TCP_NODELAY)
    void setNoDelay(bool b);
    /// get nodelay flag
    bool noDelay();
    /// set receive buffer size
    void setRecvBufSize(size_t s);
    /// get receive buffer size
    size_t recvBufSize();
    /// set send buffer size
    void setSendBufSize(size_t s);
    /// get send buffer size
    size_t sendBufSize();
    /// set blocking mode (FIONBIO)
    void setBlocking(bool b);
    /// get blocking mode
    bool blocking() const;
    /// get the maximum message size that can be sent atomically
    size_t maxMsgSize();

    /// bind socket to ip address
    bool bind();
    /// return true if the socket is bound to an address
    bool isBound() const;
    /// listen for incoming connections (for server sockets)
    bool listen();
    /// accept incoming connection, return a new socket (for server sockets)
    bool accept(DwPtr<DwSocket>& outSocket);
    /// connect to the sockets address (for client sockets)
    Result connect();
    /// test if the socket is currently connected
    bool isConnected();
    /// send raw data into the socket
    Result send(const void* buf, size_t numBytes, size_t& bytesSent);
    /// return true if recv data is available at the socket
    bool hasRecvData();
    /// receive raw data from the socket
    Result recv(void* buf, size_t bufSize, size_t& bytesReceived);
    /// send raw data to address for connectionless sockets
    Result sendTo(const void* buf, size_t numBytes, uint32_t addr, uint16_t port, size_t& bytesSent);
    /// receive raw data from address for connectionless sockets
    Result recvFrom(void* buf, size_t bufSize, uint32_t addr, uint16_t port, size_t& bytesReceived);

private:
    friend class DwIpAddress;

    /// open with an existing socket (called by Accept())
    void openWithExistingSocket(SOCKET s);
    /// clear the last error code
    void clearError();
    /// set error code to WSAGetLastError();
    void setToLastWSAError();
    /// set WSA error code
    void setWSAError(int wsaError);
    /// convert a WSA error code to internal error code
    static ErrorCode WSAErrorToErrorCode(int wsaError);
    /// convert error code to human readable string
    static DwString ErrorAsString(ErrorCode err);
    /// convert WSA error code directly to string
    static DwString WSAErrorToString(int wsaError);
    /// set a bool socket option
    void setBoolOption(int optName, bool val);
    /// get a bool socket option
    bool boolOption(int optName);
    /// set an integer socket option
    void setIntOption(int optName, int val);
    /// get an integer socket option
    int intOption(int optName);

    static bool NetworkInitialized;
    ErrorCode m_error;
    SOCKET m_sock;
    DwIpAddress m_addr;
    bool m_isBlocking;
    bool m_isBound;
};// end of DwSocket

//------------------------------------------------------------------------------
/**
*/
inline bool DwSocket::isOpen() const
{
    return (0 != this->m_sock);
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwSocket::isBound() const
{
    return this->m_isBound;
}

//------------------------------------------------------------------------------
/**
    Set internet address of socket.
*/
inline void DwSocket::setAddress(const DwIpAddress& a)
{
    this->m_addr = a;
}

//------------------------------------------------------------------------------
/**
    Get internet address of socket.
*/
inline const DwIpAddress& DwSocket::address() const
{
    return this->m_addr;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwSocket::setBroadcast(bool b)
{
    this->setBoolOption(SO_BROADCAST, b);
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwSocket::broadcast()
{
    return boolOption(SO_BROADCAST);
}

//------------------------------------------------------------------------------
/**
*/
inline void DwSocket::setKeepAlive(bool b)
{
    setBoolOption(SO_KEEPALIVE, b);
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwSocket::keepAlive()
{
    return this->boolOption(SO_KEEPALIVE);
}

//------------------------------------------------------------------------------
/**
*/
inline void DwSocket::setReUseAddr(bool b)
{
    this->setBoolOption(SO_REUSEADDR, b);
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwSocket::reUseAddr()
{
    return this->boolOption(SO_REUSEADDR);
}

//------------------------------------------------------------------------------
/**
*/
inline void DwSocket::setNoDelay(bool b)
{
    this->setBoolOption(TCP_NODELAY, b);
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwSocket::noDelay()
{
    return this->boolOption(TCP_NODELAY);
}

//------------------------------------------------------------------------------
/**
*/
inline void DwSocket::setRecvBufSize(size_t s)
{
    this->setIntOption(SO_RCVBUF, s);
}

//------------------------------------------------------------------------------
/**
*/
inline size_t DwSocket::recvBufSize()
{
    return this->intOption(SO_RCVBUF);
}

//------------------------------------------------------------------------------
/**
*/
inline void DwSocket::setSendBufSize(size_t s)
{
    this->setIntOption(SO_SNDBUF, s);
}

//------------------------------------------------------------------------------
/**
*/
inline size_t DwSocket::sendBufSize()
{
    return this->intOption(SO_SNDBUF);
}

//------------------------------------------------------------------------------
/**
*/
inline size_t DwSocket::maxMsgSize()
{
    return 8192;
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwSocket::blocking() const
{
    return this->m_isBlocking;
}
