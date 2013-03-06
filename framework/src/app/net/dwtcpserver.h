#pragma once
#include "dwapp/dwrefcounted.h"
#include "dwapp/dwthread.h"
#include "dwapp/dwtcpconnection.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwTcpServer : public DwRefCounted
{
    DW_DECLARE_CLASS(DwTcpServer);
public:
    /// constructor
    DwTcpServer();
    /// destructor
    virtual ~DwTcpServer();
    /// set address, hostname can be "any", "self" or "inetself"
    void setAddress(const DwIpAddress& addr);
    /// get address
    const DwIpAddress& address() const;
    /// set client connection class
    void setClientConnectionClass(const DwRtti& type);
    /// get client connection class
    const DwRtti& clientConnectionClass();
    /// open the server
    bool open();
    /// close the server
    void close();
    /// return true if server is open
    bool isOpen() const;
    /// poll clients connections for received data, call this frequently!
    DwVector<DwPtr<DwTcpClientConnection> > recv();
    /// broadcast a message to all clients
    bool broadcast(const DwPtr<DwStream>& msg);

private:
    /// a private listener thread class
    class ListenerThread : public DwThread
    {
        DW_DECLARE_CLASS(ListenerThread);
    public:
        /// set pointer to parent tcp server
        void setTcpServer(const DwPtr<DwTcpServer>& tcpServer);
        /// set ip address
        void setAddress(const DwIpAddress& a);
        /// set client connection class
        void setClientConnectionClass(const DwRtti& type);
    private:
        /// implements the actual listener method
        virtual void doWork();
        /// send a wakeup signal
        virtual void emitWakeupSignal();

        DwPtr<DwTcpServer> tcpServer;
        DwIpAddress ipAddress;
        DwPtr<DwSocket> socket;
        const DwRtti* connectionClassRtti;
    };
    friend class ListenerThread;
    /// add a client connection (called by the listener thread)
    void addClientConnection(const DwPtr<DwTcpClientConnection>& connection);

    DwIpAddress m_ipAddress;
    DwPtr<ListenerThread> m_listenerThread;
    bool m_isOpen;
    DwVector< DwPtr<DwTcpClientConnection> > m_clientConnections;
    DwCriticalSection m_connectionCritSect;
    const DwRtti* m_connectionClassRtti;
};// end of DwTcpServer

//------------------------------------------------------------------------------
/**
*/
inline bool
DwTcpServer::isOpen() const
{
    return this->m_isOpen;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DwTcpServer::setAddress(const DwIpAddress& addr)
{
    this->m_ipAddress = addr;
}

//------------------------------------------------------------------------------
/**
*/
inline const DwIpAddress&
DwTcpServer::address() const
{
    return this->m_ipAddress;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DwTcpServer::setClientConnectionClass(const DwRtti& type)
{
    this->m_connectionClassRtti = &type;
}

//------------------------------------------------------------------------------
/**	
*/
inline const DwRtti&
DwTcpServer::clientConnectionClass()
{
    return *this->m_connectionClassRtti;
}