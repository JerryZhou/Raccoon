#include "stable.h"
#include "dwtcpconnection.h"
#include "dwapp/dwipaddress.h"
#include "dwapp/dwmemorystream.h"


DW_IMPLEMENT_CLASS(DwTcpClientConnection, 'STCC', DwRttiObject);

//------------------------------------------------------------------------------
/**
*/
DwTcpClientConnection::DwTcpClientConnection()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
DwTcpClientConnection::~DwTcpClientConnection()
{
    this->shutdown();
}

//------------------------------------------------------------------------------
/**
*/
bool DwTcpClientConnection::connect(const DwPtr<DwSocket>& sockk)
{
    DwSocket* sock = (DwSocket*)(sockk);
    DW_ASSERT(!this->m_socket);
    DW_ASSERT(sock);
    DW_ASSERT(sock->isOpen());

    // check if socket is really actually connected
    if (sock->isConnected())
    {
        this->m_socket = sock;
        this->m_socket->setBlocking(true);
        this->m_socket->setNoDelay(true);
        this->m_sendStream = DwMemoryStream::create();
        this->m_recvStream = DwMemoryStream::create();
        DW_NET_LOG("Client from addr=%s connected!\n", 
            this->m_socket->address().hostAddr().toUtf8().data());
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwTcpClientConnection::isConnected() const
{
    if (this->m_socket)
    {
        return this->m_socket->isConnected();
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwTcpClientConnection::shutdown()
{
    if (this->m_socket)
    {
        this->m_socket->close();
        this->m_socket = 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
const DwIpAddress& DwTcpClientConnection::clientAddress() const
{
    DW_ASSERT(this->m_socket);
    return this->m_socket->address();
}

//------------------------------------------------------------------------------
/**
*/
DwSocket::Result DwTcpClientConnection::send(const DwPtr<DwStream>& stream)
{
    DW_ASSERT(stream);
    if (stream->size() == 0)
    {
        // nothing to send
        return DwSocket::Success;
    }

    DwSocket::Result res = DwSocket::Success;
    stream->setAccessMode(DwStream::ReadAccess);
    if (stream->open())
    {
        // we may not exceed the maximum message size...
        // so we may have to split the send data into
        // multiple packets
        size_t maxMsgSize = this->m_socket->maxMsgSize();
        size_t sendSize = stream->size();
        uint8_t* ptr = (uint8_t*) stream->map();
        size_t overallBytesSent = 0;
        while ((DwSocket::Success == res) && (overallBytesSent < sendSize))
        {
            size_t bytesToSend = sendSize - overallBytesSent;
            if (bytesToSend > maxMsgSize)
            {
                bytesToSend = maxMsgSize;
            }
            size_t bytesSent = 0;
            res = this->m_socket->send(ptr, bytesToSend, bytesSent);
            ptr += bytesSent;
            overallBytesSent += bytesSent;
        }
        stream->unmap();
        stream->close();
    }
    return res;
}

//------------------------------------------------------------------------------
/**
*/
DwSocket::Result DwTcpClientConnection::send()
{
    DW_ASSERT(this->m_sendStream);
    DwSocket::Result res = this->send(this->m_sendStream);
    this->m_sendStream->setSize(0);
    return res;
}

//------------------------------------------------------------------------------
/**
*/
DwSocket::Result DwTcpClientConnection::recv()
{
    DW_ASSERT(this->m_recvStream);
    this->m_recvStream->setAccessMode(DwStream::WriteAccess);
    this->m_recvStream->setSize(0);
    DwSocket::Result res = DwSocket::Success;
    if (this->m_recvStream->open())
    {
        // NOTE: the following loop will make sure that Recv()
        // never blocks
        uint8_t buf[1024];
        while ((DwSocket::Success == res) && (this->m_socket->hasRecvData()))
        {
            size_t bytesReceived = 0;
            res = this->m_socket->recv(&buf, sizeof(buf), bytesReceived);
            if ((bytesReceived > 0) && (DwSocket::Success == res))
            {
                this->m_recvStream->write(buf, bytesReceived);
            }
        }
        this->m_recvStream->close();
    }
    this->m_recvStream->setAccessMode(DwStream::ReadAccess);
    if (DwSocket::Success == res)
    {
        if (this->m_recvStream->size() > 0)
        {
            return DwSocket::Success;
        }
        else
        {
            return DwSocket::WouldBlock;
        }
    }
    else
    {
        return res;
    }
}

//------------------------------------------------------------------------------
/**
*/
const DwPtr<DwStream>& DwTcpClientConnection::sendStream()
{
    return this->m_sendStream;
}

//------------------------------------------------------------------------------
/**
*/
const DwPtr<DwStream>& DwTcpClientConnection::recvStream()
{
    return this->m_recvStream;
}
