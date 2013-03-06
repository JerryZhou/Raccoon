#include "stable.h"
#include "dwsocket.h"

DW_IMPLEMENT_CLASS(DwSocket, 'SOET', DwRefCounted);

bool DwSocket::NetworkInitialized = false;

#if DW_ENABLE_HTTP_DEBUG_TOOLKIT
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "iphlpapi.lib")
#endif

//------------------------------------------------------------------------------
/**
*/
DwSocket::DwSocket() :
    m_error(ErrorNone),
    m_sock(0),
    m_isBlocking(true),
    m_isBound(false)
{
    DW_ASSERT(NetworkInitialized);
}

//------------------------------------------------------------------------------
/**
*/
DwSocket::~DwSocket()
{
    if (this->isOpen())
    {
        this->close();
    }
}

//------------------------------------------------------------------------------
/**
    This is a one-time init for the Windows Sockets system. The method
    is called from SysFunc::Setup() once at startup before any threads
    are launched.
*/
void DwSocket::initNetwork()
{
    DW_ASSERT(!DwSocket::NetworkInitialized);

    // now proceed as usual
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (0 != result)
    {
        DW_NET_LOG("WSAStartup() failed with '%s'!", DwSocket::WSAErrorToString(result).constString());
    }
    DwSocket::NetworkInitialized = true;
}   

//------------------------------------------------------------------------------
/**
*/
bool DwSocket::open(Protocol protocol)
{
    DW_ASSERT(!this->isOpen());

    this->clearError();

    int sockType;
    int protType;
    switch (protocol)
    {
        case TCP:    
            sockType = SOCK_STREAM; 
            protType = IPPROTO_TCP;
            break;
        case UDP:  
            sockType = SOCK_DGRAM; 
            protType = IPPROTO_UDP;
            break;
        default:
            // can't happen.
            DW_NET_LOG("Invalid socket type!");
            sockType = SOCK_STREAM; 
            protType = IPPROTO_TCP;
            break;
    }
    this->m_sock = socket(AF_INET, sockType, protType);
    if (INVALID_SOCKET == this->m_sock)
    {
        this->m_sock = 0;
        this->setToLastWSAError();
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
/**
    Open the socket object with an existing initialized system socket.
    This is a private method and only called by Accept() on the
    new socket created by the accept() function.
*/
void DwSocket::openWithExistingSocket(SOCKET s)
{
    DW_ASSERT(INVALID_SOCKET != s);
    DW_ASSERT(!this->isOpen());
    this->m_sock = s;
}

//------------------------------------------------------------------------------
/**
*/
void DwSocket::close()
{
    DW_ASSERT(this->isOpen());
    this->clearError();

    int res = 0;
    if (this->isConnected())
    {
        res = shutdown(this->m_sock, SD_BOTH);
        if (SOCKET_ERROR == res)
        {
            // note: the shutdown function may return NotConnected, this
            // is not really an error
            this->setToLastWSAError();
            if (ErrorNotConnected != this->m_error)
            {
                DW_NET_LOG("DwSocket::Close(): shutdown() failed with '%s'.\n", this->errorString().constString());
            }
        }
    }
    res = closesocket(this->m_sock);
    if (SOCKET_ERROR == res)
    {
        this->setToLastWSAError();
        DW_NET_LOG("DwSocket::Close(): closesocket() failed with '%s'.\n", this->errorString().constString());
    }
    this->m_sock = 0;
}

//------------------------------------------------------------------------------
/**
    Set a boolean option on the socket. This is a private helper
    function.
*/
void
DwSocket::setBoolOption(int optName, bool val)
{
    DW_ASSERT(this->isOpen());
    this->clearError();
    int level = SOL_SOCKET;
    if (optName == TCP_NODELAY)
    {
        level = IPPROTO_TCP;
    }
    int optVal = val ? 1 : 0;
    int res = setsockopt(this->m_sock, level, optName, (const char*) &optVal, sizeof(optVal));
    if (SOCKET_ERROR == res)
    {
        this->setToLastWSAError();
        DW_NET_LOG("DwSocket::SetBoolOption(): setsockopt() failed with '%s'.\n", this->errorString().constString());
    }
}

//------------------------------------------------------------------------------
/**
    Get a boolean option on the socket. This is a private helper
    function.
*/
bool DwSocket::boolOption(int optName)
{
    DW_ASSERT(this->isOpen());
    this->clearError();
    int level = SOL_SOCKET;
    if (optName == TCP_NODELAY)
    {
        level = IPPROTO_TCP;
    }
    int optVal = 0;
    int optValSize = sizeof(optVal);
    int res = getsockopt(this->m_sock, level, optName, (char*) &optVal, &optValSize);
    if (SOCKET_ERROR == res)
    {
        this->setToLastWSAError();
        DW_NET_LOG("DwSocket::GetBoolOption(): getsockopt() failed with '%s'.\n", this->errorString().constString());
    }
    DW_ASSERT(sizeof(optVal) == optValSize);
    return (0 != optVal);
}

//------------------------------------------------------------------------------
/**
    Set an int socket option. This is a private helper function.
*/
void
DwSocket::setIntOption(int optName, int val)
{
    DW_ASSERT(this->isOpen());
    this->clearError();
    int res = setsockopt(this->m_sock, SOL_SOCKET, optName, (const char*) &val, sizeof(val));
    if (SOCKET_ERROR == res)
    {
        this->setToLastWSAError();
        DW_NET_LOG("DwSocket::SetIntOption(): setsockopt() failed with '%s'.\n", this->errorString().constString());
    }
}

//------------------------------------------------------------------------------
/**
    Get an int socket option. This is a private helper function.
*/
int
DwSocket::intOption(int optName)
{
    DW_ASSERT(this->isOpen());
    this->clearError();
    int optVal = 0;
    int optValSize = sizeof(optVal);
    int res = getsockopt(this->m_sock, SOL_SOCKET, optName, (char*) &optVal, &optValSize);
    if (SOCKET_ERROR == res)
    {
        this->setToLastWSAError();
        DW_NET_LOG("DwSocket::GetIntOption(): getsockopt() failed with '%s'.\n", this->errorString().constString());
    }
    DW_ASSERT(sizeof(optVal) == optValSize);
    return optVal;
}

//------------------------------------------------------------------------------
/**
    Set the socket to blocking mode.
*/
void
DwSocket::setBlocking(bool b)
{
    DW_ASSERT(this->isOpen());
    this->clearError();
    u_long arg = b ? 0 : 1;
    int res = ioctlsocket(this->m_sock, FIONBIO, &arg);
    if (SOCKET_ERROR == res)
    {
        this->setToLastWSAError();
        DW_NET_LOG("DwSocket::SetBlocking(): ioctlsocket() failed with '%s'.\n", this->errorString().constString());
    }
    this->m_isBlocking = b;
}

//------------------------------------------------------------------------------
/**
    Bind the socket to its ip address set with SetAddress() and
    SetPort(). After binding the socket to an address, call
    the Listen() method to wait for incoming connections. This method
    only makes sense for server sockets.
*/
bool
DwSocket::bind()
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(!this->isBound());
    this->clearError();
    const sockaddr_in& sockAddr = this->m_addr.sockAddr();
    int res = ::bind(this->m_sock, (const sockaddr*) &sockAddr, sizeof(sockAddr));
    if (SOCKET_ERROR == res)
    {
        this->setToLastWSAError();
        DW_NET_LOG("DwSocket::Bind(): bind() failed with '%s'!\n", this->errorString().constString());
        return false;
    }
    this->m_isBound = true;
    return true;
}

//------------------------------------------------------------------------------
/**
    Wait for incoming connections to a server socket. Call this 
    method on server side after binding the socket to its address.
*/
bool
DwSocket::listen()
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(this->isBound());
    this->clearError();
    int res = ::listen(this->m_sock, SOMAXCONN);
    if (SOCKET_ERROR == res)
    {
        this->setToLastWSAError();
        DW_NET_LOG("DwSocket::Listen(): listen() failed with '%s'!\n", this->errorString().constString());
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
/**
    Accept an incoming connection to a server socket. This will spawn
    a new socket for the connection which will be returned in the provided
    pointer reference. The address of the returned socket will be set to
    the address of the "connecting entity".
*/
bool DwSocket::accept(DwPtr<DwSocket>& outSocket)
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(this->isBound());

    this->clearError();
    outSocket = 0;
    sockaddr_in sockAddr;
    int sockAddrSize = sizeof(sockAddr);
    SOCKET newSocket = ::accept(this->m_sock, (sockaddr*) &sockAddr, &sockAddrSize);
    if (INVALID_SOCKET == newSocket)
    {
        this->setToLastWSAError();
        DW_NET_LOG("DwSocket::Accept(): accept() failed with '%s'!\n", this->errorString().constString());
        return false;
    }
    outSocket = DwSocket::create();
    outSocket->setAddress(DwIpAddress(sockAddr));
    outSocket->openWithExistingSocket(newSocket);
    outSocket->setBlocking(this->m_isBlocking);
    
    return true;
}

//------------------------------------------------------------------------------
/**
    Connect to a server socket. This method is called by a client socket
    to connect to a server socket identified by the socket object's address.
    A non-blocking socket will return immediately with WouldBlock, since the
    connection cannot be established immediately. In this case, just continue
    to call Connect() until the method returns Success, or alternative, check
    the IsConnected() method, which will also return true once the connection
    has been establish.
*/
DwSocket::Result DwSocket::connect()
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(!this->isBound());

    this->clearError();
    const sockaddr_in& sockAddr = this->m_addr.sockAddr();
    int res = ::connect(this->m_sock, (const sockaddr*) &sockAddr, sizeof(sockAddr));
    if (SOCKET_ERROR == res)
    {
        // special handling for non-blocking sockets
        int wsaError = WSAGetLastError();
        if (!this->blocking())
        {
            if (WSAEWOULDBLOCK == wsaError)
            {
                return WouldBlock;
            }
            else if (WSAEALREADY == wsaError)
            {
                // connection is underway but not finished yet
                return WouldBlock;
            }
            else if (WSAEISCONN == wsaError)
            {
                // the connection is established
                return Success;
            }
            // fallthrough: a normal error
        }
        this->setWSAError(wsaError);
        DW_NET_LOG("DwSocket::Connect(): connect() failed with '%s'!\n", this->errorString().constString());
        return Error;
    }
    return Success;
}

//------------------------------------------------------------------------------
/**
    This tests if the socket is actually connected by doing a select()
    on the socket to probe for writability. So the IsConnected() method
    basically checks whether data can be sent through the socket.
*/
bool DwSocket::isConnected()
{
    DW_ASSERT(this->isOpen());
    fd_set writeSet = { 1, { this->m_sock } };
    TIMEVAL timeVal = { 0, 0 };
    int res = ::select(0, 0, &writeSet, 0, &timeVal);
    if (SOCKET_ERROR == res)
    {
        this->setToLastWSAError();
        DW_NET_LOG("DwSocket::IsConnected(): select() failed with '%s'!\n", this->errorString().constString());
        return false;
    }
    else if (0 == res)
    {
        return false;
    }
    else
    {
        return true;
    }
}

//------------------------------------------------------------------------------
/**
    Send raw data into the socket. Note that depending on the buffer size
    of the underlying socket implementation and other sockets, the method
    may not be able to send all provided data. In this case, the returned 
    content of bytesSent will be less then numBytes, even though the
    return value will be Success. It is up to the caller to handle the
    extra data which hasn't been sent with the current call.
*/
DwSocket::Result DwSocket::send(const void* buf, size_t numBytes, size_t& bytesSent)
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(0 != buf);
    this->clearError();
    bytesSent = 0;
    int res = ::send(this->m_sock, (const char*) buf, numBytes, 0);
    if (SOCKET_ERROR == res)
    {
        if (WSAEWOULDBLOCK == res)
        {
            return WouldBlock;
        }
        else
        {
            this->setToLastWSAError();
            DW_NET_LOG("DwSocket::Send(): send() failed with '%s'\n", this->errorString().constString());
            return Error;
        }
    }
    else
    {
        bytesSent = res;
        return Success;
    }
}

//------------------------------------------------------------------------------
/**
    This method checks if the socket has received data available. Use
    this method in a loop with Recv() to get all data waiting at the
    socket. This method will never block.
*/
bool DwSocket::hasRecvData()
{
    DW_ASSERT(this->isOpen());
    fd_set readSet = { 1, { this->m_sock } };
    TIMEVAL timeVal = { 0, 0 };
    int res = ::select(0, &readSet, 0, 0, &timeVal);
    if (SOCKET_ERROR == res)
    {
        this->setToLastWSAError();
        DW_NET_LOG("DwSocket::HasRecvData(): select() failed with '%s'!\n", this->errorString().constString());
        return false;
    }
    else if (0 == res)
    {
        return false;
    }
    else
    {
        return true;
    }
}

//------------------------------------------------------------------------------
/**
    Receive raw data from a socket and write the received data into the
    provided buffer. On a blocking socket this method will block until
    data arrives at the socket. A non-blocking socket would immediately return in
    this case with a WouldBlock result. When valid data has been received
    the method will return with a Success result and the bytesReceived argument
    will contain the number of received bytes. It is not guaranteed that a single
    receive will return all data waiting on the socket. To make sure that the
    socket is really empty, call Recv() in a loop until HasRecvData()
    returns false.
    When the socket has been gracefully closed by the other side, the method will 
    return with a Closed return value. Everything else will return with an Error 
    return code. Call GetErrorCode() or GetErrorDwString() to find out more in this case.
*/
DwSocket::Result DwSocket::recv(void* buf, size_t bufSize, size_t& bytesReceived)
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(0 != buf);
    this->clearError();
    bytesReceived = 0;
    int res = ::recv(this->m_sock, (char*) buf, bufSize, 0);
    if (0 == res)
    {
        // connection has been gracefully closed
        return Closed;
    }
    else if (SOCKET_ERROR == res)
    {
        // catch special error conditions
        int wsaError = WSAGetLastError();
        if (WSAEMSGSIZE == wsaError)
        {
            // more data is pending
            bytesReceived = bufSize;    // FIXME: is this correct?
            return Success;
        }
        if (!this->m_isBlocking)
        {
            // socket is non-blocking and no data is available
            if (WSAEWOULDBLOCK == wsaError)
            {
                return WouldBlock;
            }
        }

        // fallthrough: a real error
        this->setToLastWSAError();
        DW_NET_LOG("DwSocket::Recv(): recv() failed with '%s'\n", this->errorString().constString());
        return Error;
    }
    else
    {
        bytesReceived = res;
        return Success;
    }
}

//------------------------------------------------------------------------------
/**
    FIXME: this is the send method for connectionless sockets using the 
    UDP protocol.
*/
DwSocket::Result DwSocket::sendTo(const void* /*buf*/, size_t /*numBytes*/, uint32_t /*addr*/, uint16_t /*port*/, size_t& /*bytesSent*/)
{
    DW_NET_LOG("DwSocket::SendTo(): IMPLEMENT ME!");
    return Error;
}

//------------------------------------------------------------------------------
/**
    FIXME: this is the recv method for connectionless socket using the UDP
    protocol.
*/
DwSocket::Result
DwSocket::recvFrom(void* /*buf*/, size_t /*bufSize*/, uint32_t /*addr*/, uint16_t /*port*/, size_t& /*bytesReceived*/)
{
    DW_NET_LOG("DwSocket::RecvFrom(): IMPLEMENT ME!");
    return Error;
}

//------------------------------------------------------------------------------
/**
    Sets the internal error code to NoError.
*/
void
DwSocket::clearError()
{
    this->m_error = ErrorNone;
}

//------------------------------------------------------------------------------
/**
    Sets the internal error code to WSAGetLastError().
*/
void
DwSocket::setToLastWSAError()
{
    this->m_error = WSAErrorToErrorCode(WSAGetLastError());
}

//------------------------------------------------------------------------------
/**
    Sets the provided WSA error as error code.
*/
void
DwSocket::setWSAError(int wsaError)
{
    this->m_error = WSAErrorToErrorCode(wsaError);
}

//------------------------------------------------------------------------------
/**
    This method converts an Windows Socket error code into a
    portable error code used by DwSocket.
*/
DwSocket::ErrorCode DwSocket::WSAErrorToErrorCode(int wsaErrorCode)
{
    switch (wsaErrorCode)
    {
        case WSAEINTR:              return ErrorInterrupted; break;            
        case WSAEACCES:             return ErrorPermissionDenied; break;       
        case WSAEFAULT:             return ErrorBadAddress; break;             
        case WSAEINVAL:             return ErrorInvalidArgument; break;        
        case WSAEMFILE:             return ErrorTooManyOpenFiles; break;       
        case WSAEWOULDBLOCK:        return ErrorWouldBlock; break;             
        case WSAEINPROGRESS:        return ErrorInProgress; break;             
        case WSAEALREADY:           return ErrorAlreadyInProgress; break;      
        case WSAENOTSOCK:           return ErrorNotASocket; break;             
        case WSAEDESTADDRREQ:       return ErrorDestAddrRequired; break;       
        case WSAEMSGSIZE:           return ErrorMsgTooLong; break;             
        case WSAEPROTOTYPE:         return ErrorInvalidProtocol; break;        
        case WSAENOPROTOOPT:        return ErrorBadProtocolOption; break;      
        case WSAEPROTONOSUPPORT:    return ErrorProtocolNotSupported; break;   
        case WSAESOCKTNOSUPPORT:    return ErrorSocketTypeNotSupported; break; 
        case WSAEOPNOTSUPP:         return ErrorOperationNotSupported; break;  
        case WSAEPFNOSUPPORT:       return ErrorProtFamilyNotSupported; break; 
        case WSAEAFNOSUPPORT:       return ErrorAddrFamilyNotSupported; break; 
        case WSAEADDRINUSE:         return ErrorAddrInUse; break;              
        case WSAEADDRNOTAVAIL:      return ErrorAddrNotAvailable; break;       
        case WSAENETDOWN:           return ErrorNetDown; break;                
        case WSAENETUNREACH:        return ErrorNetUnreachable; break;         
        case WSAENETRESET:          return ErrorNetReset; break;               
        case WSAECONNABORTED:       return ErrorConnectionAborted; break;      
        case WSAECONNRESET:         return ErrorConnectionReset; break;        
        case WSAENOBUFS:            return ErrorNoBufferSpace; break;          
        case WSAEISCONN:            return ErrorIsConnected; break;            
        case WSAENOTCONN:           return ErrorNotConnected; break;           
        case WSAESHUTDOWN:          return ErrorIsShutdown; break;             
        case WSAETIMEDOUT:          return ErrorIsTimedOut; break;             
        case WSAECONNREFUSED:       return ErrorConnectionRefused; break;      
        case WSAEHOSTDOWN:          return ErrorHostDown; break;               
        case WSAEHOSTUNREACH:       return ErrorHostUnreachable; break;        
        case WSAEPROCLIM:           return ErrorTooManyProcesses; break;       
        case WSASYSNOTREADY:        return ErrorSystemNotReady; break;         
        case WSAVERNOTSUPPORTED:    return ErrorVersionNotSupported; break;    
        case WSANOTINITIALISED:     return ErrorNotInitialized; break;         
        case WSAEDISCON:            return ErrorDisconnecting; break;          
        case WSATYPE_NOT_FOUND:     return ErrorTypeNotFound; break;           
        case WSAHOST_NOT_FOUND:     return ErrorHostNotFound; break;           
        case WSATRY_AGAIN:          return ErrorTryAgain; break;               
        case WSANO_RECOVERY:        return ErrorNoRecovery; break;             
        case WSANO_DATA:            return ErrorNoData; break;                 
        default:                    
            return ErrorUnknown; 
            break;
    }
}

//------------------------------------------------------------------------------
/**
    Convert an error code to a human readable error message.
*/
DwString DwSocket::ErrorAsString(ErrorCode err)
{
    switch (err)
    {
        case ErrorNone:                      return "No error.";
        case ErrorUnknown:                   return "Unknown error (not mapped by DwSocket class).";
        case ErrorInterrupted:               return "Interrupted function call.";
        case ErrorPermissionDenied:          return "Permission denied.";
        case ErrorBadAddress:                return "Bad address.";
        case ErrorInvalidArgument:           return "Invalid argument.";
        case ErrorTooManyOpenFiles:          return "Too many open files (sockets).";
        case ErrorWouldBlock:                return "Operation would block.";
        case ErrorInProgress:                return "Operation now in progress.";
        case ErrorAlreadyInProgress:         return "Operation already in progress.";
        case ErrorNotASocket:                return "Socket operation on non-socket.";
        case ErrorDestAddrRequired:          return "Destination address required";
        case ErrorMsgTooLong:                return "Message too long.";
        case ErrorInvalidProtocol:           return "Protocol wrong type for socket.";
        case ErrorBadProtocolOption:         return "Bad protocal option.";
        case ErrorProtocolNotSupported:      return "Protocol not supported.";
        case ErrorSocketTypeNotSupported:    return "Socket type not supported.";
        case ErrorOperationNotSupported:     return "Operation not supported.";
        case ErrorProtFamilyNotSupported:    return "Protocol family not supported.";
        case ErrorAddrFamilyNotSupported:    return "Address family not supported by protocol family.";
        case ErrorAddrInUse:                 return "Address already in use.";
        case ErrorAddrNotAvailable:          return "Cannot assign requested address.";
        case ErrorNetDown:                   return "Network is down.";
        case ErrorNetUnreachable:            return "Network is unreachable.";
        case ErrorNetReset:                  return "Network dropped connection on reset.";
        case ErrorConnectionAborted:         return "Software caused connection abort.";
        case ErrorConnectionReset:           return "Connection reset by peer.";
        case ErrorNoBufferSpace:             return "No buffer space available.";
        case ErrorIsConnected:               return "Socket is already connected.";
        case ErrorNotConnected:              return "Socket is not connected.";
        case ErrorIsShutdown:                return "Cannot send after socket shutdown.";
        case ErrorIsTimedOut:                return "Connection timed out.";
        case ErrorConnectionRefused:         return "Connection refused.";
        case ErrorHostDown:                  return "Host is down.";
        case ErrorHostUnreachable:           return "No route to host.";
        case ErrorTooManyProcesses:          return "Too many processes.";
        case ErrorSystemNotReady:            return "Network subsystem is unavailable.";
        case ErrorVersionNotSupported:       return "Winsock.dll version out of range.";
        case ErrorNotInitialized:            return "Successful WSAStartup not yet performed.";
        case ErrorDisconnecting:             return "Graceful shutdown in progress.";
        case ErrorTypeNotFound:              return "Class type not found.";
        case ErrorHostNotFound:              return "Host not found.";
        case ErrorTryAgain:                  return "Nonauthoritative host not found.";
        case ErrorNoRecovery:                return "This is a nonrecoverable error.";
        case ErrorNoData:                    return "Valid name, no data record of requested type.";
        default:
            DW_NET_LOG("DwSocket::ErrorAsDwString(): unhandled error code!");
            return "";
    }
}

//------------------------------------------------------------------------------
/**
*/
DwString DwSocket::WSAErrorToString(int wsaError)
{
    return ErrorAsString(WSAErrorToErrorCode(wsaError));
}

//------------------------------------------------------------------------------
/**
*/
DwString DwSocket::errorString() const
{
    return ErrorAsString(this->m_error);
}

//------------------------------------------------------------------------------
/**
*/
bool DwSocket::isNetworkInitialized()
{
    return NetworkInitialized;
}