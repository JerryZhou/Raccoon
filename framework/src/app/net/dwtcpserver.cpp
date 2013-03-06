#include "stable.h"
#include "dwtcpserver.h"
#include "dwapp/dwutil.h"


DW_IMPLEMENT_CLASS(DwTcpServer, 'STSV', DwRefCounted);
DW_IMPLEMENT_CLASS(DwTcpServer::ListenerThread, 'tclt', DwThread);

//------------------------------------------------------------------------------
/**
*/
DwTcpServer::DwTcpServer() :
    m_isOpen(false)
{
    this->m_connectionClassRtti = DwTcpClientConnection::RTTI();
}

//------------------------------------------------------------------------------
/**
*/
DwTcpServer::~DwTcpServer()
{
    DW_ASSERT(!this->isOpen());
}

//------------------------------------------------------------------------------
/**
*/
bool
DwTcpServer::open()
{
    DW_ASSERT(!this->m_isOpen);
    DW_ASSERT(!this->m_listenerThread);
    DW_ASSERT(this->m_clientConnections.isEmpty());

    // create the listener thread
    this->m_listenerThread = ListenerThread::create();
    this->m_listenerThread->setName("DwTcpServer::ListenerThread");
    this->m_listenerThread->setTcpServer(this);
    this->m_listenerThread->setAddress(this->m_ipAddress);
    this->m_listenerThread->setCoreId(DwCpu::MiscThreadCore);
    this->m_listenerThread->setClientConnectionClass(*this->m_connectionClassRtti);
    this->m_listenerThread->start();
    
    this->m_isOpen = true;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
DwTcpServer::close()
{
    DW_ASSERT(this->m_isOpen);
    DW_ASSERT(this->m_listenerThread);
    
    // stop the listener thread
    if (m_listenerThread->isRunning())
    {
        this->m_listenerThread->stop();
    }
    this->m_listenerThread = 0;

    // disconnect client connections
    this->m_connectionCritSect.lock();
    int clientIndex;
    for (clientIndex = 0; clientIndex < this->m_clientConnections.size(); clientIndex++)
    {
        const DwPtr<DwTcpClientConnection>& curConnection = this->m_clientConnections[clientIndex];
        curConnection->shutdown();
    }
    this->m_clientConnections.clear();
    this->m_connectionCritSect.unlock();

    this->m_isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
void
DwTcpServer::addClientConnection(const DwPtr<DwTcpClientConnection>& conn)
{
    DW_ASSERT(conn);
    this->m_connectionCritSect.lock();
    this->m_clientConnections.append((DwTcpClientConnection*)conn);
    this->m_connectionCritSect.unlock();
}

//------------------------------------------------------------------------------
/**
*/
DwVector<DwPtr<DwTcpClientConnection> > DwTcpServer::recv()
{
    DwVector<DwPtr<DwTcpClientConnection> > clientsWithData;

    // iterate over all clients, and check for new data,
    // if the client connection has been closed, remove
    // the client from the list
    this->m_connectionCritSect.lock();
    int clientIndex;
    for (clientIndex = 0; clientIndex < this->m_clientConnections.size();)
    {
        const DwPtr<DwTcpClientConnection>& cur = this->m_clientConnections[clientIndex];
        bool dropClient = false;
        if (cur->isConnected())
        {
            DwSocket::Result res = cur->recv();
            if (res == DwSocket::Success)
            {
                clientsWithData.append(cur);
            }
            else if ((res == DwSocket::Error) || (res == DwSocket::Closed))
            {
                // some error occured, drop the connection
                dropClient = true;
            }
        }
        else
        {
            dropClient = true;
        }
        if (dropClient)
        {
            // connection has been closed, remove the client
            cur->shutdown();
            this->m_clientConnections.remove(clientIndex);
        }
        else
        {
            clientIndex++;
        }
    }
    this->m_connectionCritSect.unlock();
    return clientsWithData;
}

//------------------------------------------------------------------------------
/**
*/
bool DwTcpServer::broadcast(const DwPtr<DwStream>& msg)
{
    bool result = true;
    this->m_connectionCritSect.lock();
    int i;
    for (i = 0; i < this->m_clientConnections.size(); i++)
    {
        if (DwSocket::Success != this->m_clientConnections[i]->send(msg))
        {
            result = false;
        }
    }
    this->m_connectionCritSect.unlock();
    return result;
}

//------------------------------------------------------------------------------
/**
*/
void DwTcpServer::ListenerThread::setTcpServer(const DwPtr<DwTcpServer>& serv)
{
    DW_ASSERT(0 != serv);
    this->tcpServer = serv;
}

//------------------------------------------------------------------------------
/**
*/
void DwTcpServer::ListenerThread::setAddress(const DwIpAddress& a)
{
    this->ipAddress = a;
}

//------------------------------------------------------------------------------
/**
*/
void DwTcpServer::ListenerThread::setClientConnectionClass(const DwRtti& type)
{
    this->connectionClassRtti = &type;
}

//------------------------------------------------------------------------------
/**
    This is the actual listening method which runs in a separat thread.
    It creates a server socket and listens for incoming connections. When
    a client connects, a new TcpClientConnection will be created and
    added to the array of active connections.
*/
void DwTcpServer::ListenerThread::doWork()
{
    // we need a minimal Nebula3 runtime to work
    DW_NET_LOG("ListenerThread started!\n");

    // create a server socket
    DwPtr<DwSocket> serverSocket = DwSocket::create();
    if (serverSocket->open(DwSocket::TCP))
    {
        serverSocket->setAddress(this->ipAddress);
        serverSocket->setReUseAddr(true);
        if (serverSocket->bind())
        {
            DW_NET_LOG("ListenerThread listening...\n");
            while ((!this->threadStopRequested() && serverSocket->listen()))
            {
                // check if this was the wakeup function which woke us up...
                if (!this->threadStopRequested())
                {
                    DwPtr<DwSocket> newSocket;
                    if (serverSocket->accept(newSocket))
                    {
                        // create a new connection object and add to connection array
                        DwTcpClientConnection* newConnection = (DwTcpClientConnection*) this->connectionClassRtti->create();
                        if (newConnection->connect(newSocket))
                        {
                            this->tcpServer->addClientConnection(newConnection);
                        }
                    }
                }
            }
        }
        else
        {
            DW_NET_LOG("StdTcpServer::ListenerThread: Socket::Bind() failed!");
        }
        serverSocket->close();
    }
    DW_NET_LOG("ListenerThread shutting down!\n");

    // NOTE: we don't close the thread locale console manually, because
    // the other objects may have something to say while they're destroyed
}

//------------------------------------------------------------------------------
/**
    Emit a wakeup signal to the listener thread, this will just connect
    to the socket which wakes up the Socket::Listen() method.
*/
void
DwTcpServer::ListenerThread::emitWakeupSignal()
{
    DwPtr<DwSocket> socket = DwSocket::create();
    if (socket->open(DwSocket::TCP))
    {
        socket->setAddress(DwIpAddress("127.0.0.1", this->ipAddress.port()));
        socket->connect();
        socket->close();
    }
}
