
#include "stable.h"
#include "dwhttpclientregistry.h"

DW_IMPLEMENT_CLASS(DwHttpClientRegistry, 'HTCR', DwRefCounted);

//------------------------------------------------------------------------------
/**
*/
DwHttpClientRegistry::DwHttpClientRegistry() :
    m_isValid(false)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwHttpClientRegistry::~DwHttpClientRegistry()
{
    if (this->isValid())
    {
        this->discard();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
DwHttpClientRegistry::setup()
{
    DW_ASSERT(!this->m_isValid);
    this->m_isValid = true;
}

//------------------------------------------------------------------------------
/**
*/
void
DwHttpClientRegistry::discard()
{
    DW_ASSERT(this->isValid());

    // force disconnect on all open connections
    DwMap<DwString, Connection>::const_iterator ite = this->m_connections.begin();
    while(ite != this->m_connections.end())
    {
        ite->second.httpClient->disconnect();
        ++ite;
    }
    this->m_connections.clear();

    this->m_isValid = false;
}

//------------------------------------------------------------------------------
/**
    This disconnects all connections with a use count of 0.
*/
void
DwHttpClientRegistry::disconnectIdle()
{
    // start at the back of the array since we're removing elements
    DwMap<DwString, Connection>::iterator ite = this->m_connections.begin();
    while(ite != this->m_connections.end())
    {
        const Connection& con = ite->second;
        if (0 == con.useCount)
        {
            if (con.httpClient->isConnected())
            {
                con.httpClient->disconnect();
            }
            ite = this->m_connections.remove(ite);
        }
        else
        {
            ++ite;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
DwPtr<DwHttpClient>
DwHttpClientRegistry::obtainConnection(const DwUrl& uri)
{
    DwString host = uri.host();
    if (this->m_connections.contains(host))
    {
        // connection already exists, increment connection count
        Connection& con = this->m_connections[host];
        con.useCount++;

        // reconnect if the connection has been dropped for some reason
        if (!con.httpClient->isConnected())
        {
            con.httpClient->connect(uri);
        }
        return con.httpClient;
    }
    else
    {
        // connection doesn't exist yet, create a new Http client and
        // add to connections
        Connection newCon;
        newCon.useCount = 1;
        newCon.httpClient = DwHttpClient::create();
        newCon.httpClient->connect(uri);
        this->m_connections.insert(host, newCon);
        return newCon.httpClient;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
DwHttpClientRegistry::releaseConnection(const DwUrl& uri)
{
    DwString host = uri.host();
    if (this->m_connections.contains(host))
    {
        Connection& con = this->m_connections[host];
        DW_ASSERT(con.useCount > 0);
        con.useCount--;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
DwHttpClientRegistry::isConnected(const DwUrl& uri) const
{
    DwString host = uri.host();
    if (this->m_connections.contains(host))
    {
        return this->m_connections[host].httpClient->isConnected();
    }
    else
    {
        return false;
    }
}