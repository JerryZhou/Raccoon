#pragma once
#include "dwapp/dwsocket.h"
#include "dwapp/dwstream.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwTcpClientConnection : public DwRefCounted
{
    DW_DECLARE_CLASS(DwTcpClientConnection);
public:
    /// constructor
    DwTcpClientConnection();
    /// destructor
    virtual ~DwTcpClientConnection();
    /// connect using provided socket
    virtual bool connect(const DwPtr<DwSocket>& s);
    /// get the connection status
    bool isConnected() const;
    /// shutdown the connection
    virtual void shutdown();
    /// get the client's ip address
    const DwIpAddress& clientAddress() const;
    /// send accumulated content of send stream to server
    virtual DwSocket::Result send();
    /// directly send a stream to the server, often prevents a memory copy
    virtual DwSocket::Result send(const DwPtr<DwStream>& stream);
    /// access to send stream
    virtual const DwPtr<DwStream>& sendStream();
    /// receive data from server into recv stream
    virtual DwSocket::Result recv();
    /// access to recv stream
    virtual const DwPtr<DwStream>& recvStream();

protected:
    DwPtr<DwSocket> m_socket;
    DwPtr<DwStream> m_sendStream;
    DwPtr<DwStream> m_recvStream;
};// end of DwTcpClientConnection