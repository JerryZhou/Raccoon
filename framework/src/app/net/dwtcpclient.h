#pragma once
#include "dwapp/dwsocket.h"
#include "dwapp/dwstream.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwTcpClient : public DwRefCounted
{
    DW_DECLARE_CLASS(DwTcpClient);
public:
    enum Result
    {
        Error,
        Success,
        Connecting,
    };
    /// constructor
    DwTcpClient();
    /// destructor
    virtual ~DwTcpClient();
    /// enable/disable blocking behaviour
    void setBlocking(bool b);
    /// get blocking behaviour
    bool isBlocking() const;
    /// set the server address to connect to
    void setServerAddress(const DwIpAddress& addr);
    /// get the server address
    const DwIpAddress& serverAddress() const;
    /// establish a connection with the server
    Result connect();
    /// disconnect from the server
    void disconnect();
    /// return true if currently connected
    bool isConnected();
    /// send accumulated content of send stream to server
    bool send();
    /// access to send stream
    const DwPtr<DwStream>& sendStream();
    /// receive data from server into recv stream
    bool recv();
    /// access to recv stream
    const DwPtr<DwStream>& recvStream();

protected:
    DwPtr<DwSocket> m_socket;
    DwIpAddress m_serverAddr;
    bool m_blocking;
    DwPtr<DwStream> m_sendStream;
    DwPtr<DwStream> m_recvStream;
    bool m_inConnectionState;
};// end of DwTcpClient

//------------------------------------------------------------------------------
/**
*/
inline void DwTcpClient::setBlocking(bool b)
{
    this->m_blocking = b;
    if (this->m_socket)
    {
        this->m_socket->setBlocking(b);
    }
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwTcpClient::isBlocking() const
{
    return this->m_blocking;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwTcpClient::setServerAddress(const DwIpAddress& addr)
{
    this->m_serverAddr = addr;
}

//------------------------------------------------------------------------------
/**
*/
inline const DwIpAddress& DwTcpClient::serverAddress() const
{
    return this->m_serverAddr;
}
