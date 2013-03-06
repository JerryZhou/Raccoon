#include "stable.h"
#include "dwtcpclient.h"
#include "dwapp/dwmemorystream.h"

DW_IMPLEMENT_CLASS(DwTcpClient, 'STCL', DwRefCounted);

//------------------------------------------------------------------------------
/**
*/
DwTcpClient::DwTcpClient() :
    m_blocking(true),
    m_inConnectionState(false)
{
    // create send and receive streams
    this->m_sendStream = DwMemoryStream::create();
    this->m_recvStream = DwMemoryStream::create();
}

//------------------------------------------------------------------------------
/**
*/
DwTcpClient::~DwTcpClient()
{
    if (this->isConnected())
    {
        this->disconnect();
    }
}

//------------------------------------------------------------------------------
/**
    Establish a connection with the server. If the client is set to
    non-blocking at the time this method is called, it will return immediately
    with the result Connecting. To check if the connection is standing, just
    call Connect() again in intervals which will eventually return Success.
    On a blocking client, connect returns after a connection has been established,
    or with a time out when no connection could be established.
*/
DwTcpClient::Result DwTcpClient::connect()
{
    DW_ASSERT(!this->m_socket);
    DW_ASSERT(this->m_sendStream);
    DW_ASSERT(this->m_recvStream);

    // create a new socket and try to connect to server
    this->m_socket = DwSocket::create();
    if (this->m_socket->open(DwSocket::TCP))
    {
        this->m_socket->setAddress(this->m_serverAddr);
        this->m_socket->setBlocking(this->m_blocking);
        this->m_socket->setReUseAddr(true);
        this->m_socket->setNoDelay(true);

        DwSocket::Result res = this->m_socket->connect();
        if (DwSocket::Error == res)
        {
            DW_NET_LOG("DwTcpClient: failed to connect to host '%s(%s)' port '%d'!.\n",
                this->m_serverAddr.hostName().constString(),
                this->m_serverAddr.hostAddr().constString(),
                this->m_serverAddr.port());
            this->m_socket = 0;
            return Error;
        }
        else if (DwSocket::Success == res)
        {
            this->m_inConnectionState = true;
            return Success;
        }
        else
        {
            return Connecting;
        }
    }
    DW_NET_LOG("StdTcpClient: failed to open socket!\n");
    this->m_socket = 0;
    return Error;
}

//------------------------------------------------------------------------------
/**
    Return true if the socket is currently connected. This will actually
    probe the connection using a select().
*/
bool DwTcpClient::isConnected()
{
    if (this->m_socket)
    {
        if (this->m_socket->isConnected())
        {
            return true;
        }
        else
        {
            this->disconnect();
            return false;
        }
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
    This disconnects the current connection.
*/
void DwTcpClient::disconnect()
{
    if (this->m_inConnectionState)
    {
        DW_ASSERT(this->m_socket);
        this->m_socket->close();
        this->m_socket = 0;
    }
    this->m_inConnectionState = false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwTcpClient::send()
{
    DW_ASSERT(this->m_sendStream);
    if (this->m_sendStream->size() == 0)
    {
        // nothing to send
        return true;
    }

    this->m_sendStream->setAccessMode(DwStream::ReadAccess);
    if (this->m_sendStream->open())
    {
        // put the socket into blocking mode, so that if the
        // outgoing data doesn't fit into the send buffer the
        // socket will block until the next block of data can be written
        bool wasBlocking = this->m_socket->blocking();
        if (!wasBlocking)
        {
            this->m_socket->setBlocking(true);
        }

        // we may not exceed the maximum message size...
        // so we may have to split the send data into
        // multiple packets
        size_t maxMsgSize = this->m_socket->maxMsgSize();
        size_t sendSize = this->m_sendStream->size();
        uint8_t* ptr = (uint8_t*) this->m_sendStream->map();
        size_t overallBytesSent = 0;
        DwSocket::Result socketResult = DwSocket::Success;
        while ((DwSocket::Success == socketResult) && (overallBytesSent < sendSize))
        {
            size_t bytesToSend = sendSize - overallBytesSent;
            if (bytesToSend > maxMsgSize)
            {
                bytesToSend = maxMsgSize;
            }
            size_t bytesSent = 0;
            socketResult = this->m_socket->send(ptr, bytesToSend, bytesSent);
            if (DwSocket::Success == socketResult)
            {
                ptr += bytesSent;
                overallBytesSent += bytesSent;
            }
            else
            {
                // send failed, try to re-connect, and re-send
                bool clientWasBlocking = this->m_blocking;
                this->setBlocking(true);
                this->disconnect();
                Result connectResult = this->connect();
                if (Success == connectResult)
                {
                    this->m_socket->setBlocking(true);
                    DW_NET_LOG("StdTcpClient re-connected!\n");
                    socketResult = DwSocket::Success;
                }
                else if (Connecting == connectResult)
                {
                    DW_NET_LOG("StdTcpClient re-connect failed with 'connecting' (can't happen)\n");
                }
                else
                {
                    DW_NET_LOG("StdTcpClient re-connect failed with 'error'\n");
                }
                this->setBlocking(clientWasBlocking);
            }
        }
        this->m_sendStream->unmap();
        if (!wasBlocking)
        {
            this->m_socket->setBlocking(false);
        }
        this->m_sendStream->close();
        this->m_sendStream->setSize(0);
        if ((DwSocket::Success == socketResult) && (overallBytesSent == sendSize))
        {
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwTcpClient::recv()
{
    DW_ASSERT(this->m_recvStream);
    this->m_recvStream->setAccessMode(DwStream::WriteAccess);
    this->m_recvStream->setSize(0);
    if (this->m_recvStream->open())
    {
        uint8_t buf[1024];
        DwSocket::Result res = DwSocket::Success;
        do
        {
            // NOTE: if this is a blocking client, the first call
            // to Recv() will block until data is available
            size_t bytesReceived = 0;
            res = this->m_socket->recv(&buf, sizeof(buf), bytesReceived);
            if ((bytesReceived > 0) && (DwSocket::Success == res))
            {
                this->m_recvStream->write(buf, bytesReceived);
            }
        }
        while ((DwSocket::Success == res) && this->m_socket->hasRecvData());
        this->m_recvStream->close();
    }
    this->m_recvStream->setAccessMode(DwStream::ReadAccess);
    return (this->m_recvStream->size() > 0);
}

//------------------------------------------------------------------------------
/**
*/
const DwPtr<DwStream>& DwTcpClient::sendStream()
{
    return this->m_sendStream;
}

//------------------------------------------------------------------------------
/**
*/
const DwPtr<DwStream>& DwTcpClient::recvStream()
{
    return this->m_recvStream;
}
