#include "stable.h"
#include "dwurl.h"
#include "dwfswrapper.h"
#include "dwassignlib.h"
#include "graphicsview/dwdummys.h"

//------------------------------------------------------------------------------
/**
*/
DwUrl::DwUrl()
    : m_isEmpty(true)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
DwUrl::DwUrl(const DwString& s)
    : m_isEmpty(true)
{
    bool validDwUrl = split(s);
    DW_UNUSED(validDwUrl);
}

//------------------------------------------------------------------------------
/**
*/
DwUrl::DwUrl(const char *str)
    : m_isEmpty(true)
{
    set(DwString(str));
}

//------------------------------------------------------------------------------
/**
*/
DwUrl::DwUrl(const wchar_t *str)
    : m_isEmpty(true)
{
    set(DwString(str));
}

//------------------------------------------------------------------------------
/**
*/
DwUrl::DwUrl(const DwUrl& rhs)
    : m_isEmpty(rhs.m_isEmpty)
    , m_scheme(rhs.m_scheme)
    , m_userInfo(rhs.m_userInfo)
    , m_host(rhs.m_host)
    , m_port(rhs.m_port)
    , m_localPath(rhs.m_localPath)
    , m_fragment(rhs.m_fragment)
    , m_query(rhs.m_query)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void DwUrl::operator=(const DwUrl& rhs)
{
    if (this == &rhs)
    {
        return;
    }
    m_isEmpty = rhs.m_isEmpty;
    m_scheme = rhs.m_scheme;
    m_userInfo = rhs.m_userInfo;
    m_host = rhs.m_host;
    m_port = rhs.m_port;
    m_localPath = rhs.m_localPath;
    m_fragment = rhs.m_fragment;
    m_query = rhs.m_query;
}

//------------------------------------------------------------------------------
/**
*/
bool DwUrl::operator==(const DwUrl& rhs) const
{
    if (m_isEmpty && rhs.m_isEmpty)
    {
        return true;
    }
    else
    {
        return ((m_scheme == rhs.m_scheme) &&
                (m_userInfo == rhs.m_userInfo) &&
                (m_host == rhs.m_host) &&
                (m_port == rhs.m_port) &&
                (m_localPath == rhs.m_localPath) &&
                (m_fragment == rhs.m_fragment) &&
                (m_query == rhs.m_query));
    }
}

//------------------------------------------------------------------------------
/**
*/
bool DwUrl::operator!=(const DwUrl& rhs) const
{
    return !(*this == rhs);
}

//------------------------------------------------------------------------------
/**
*/

bool DwUrl::isEmpty() const
{
    return m_isEmpty;
}

//------------------------------------------------------------------------------
/**
*/

bool DwUrl::isValid() const
{
    return !(m_isEmpty);
}

//------------------------------------------------------------------------------
/**
*/

void DwUrl::clear()
{
    m_isEmpty = true;
    m_scheme.clear();
    m_userInfo.clear();
    m_host.clear();
    m_port.clear();
    m_localPath.clear();
    m_fragment.clear();
    m_query.clear();
}

//------------------------------------------------------------------------------
/**
*/

void DwUrl::set(const DwString& s)
{
    bool validDwUrl = split(s);
    DW_UNUSED(validDwUrl);
}

//------------------------------------------------------------------------------
/**
*/
DwString DwUrl::asString() const
{
    return build();
}

//------------------------------------------------------------------------------
/**
*/
void DwUrl::setScheme(const DwString& s)
{
    m_isEmpty = false;
    m_scheme = s;
}

//------------------------------------------------------------------------------
/**
*/
const DwString& DwUrl::scheme() const
{
    return m_scheme;
}

//------------------------------------------------------------------------------
/**
*/
void DwUrl::setUserInfo(const DwString& s)
{
    m_isEmpty = false;
    m_userInfo = s;
}

//------------------------------------------------------------------------------
/**
*/
const DwString& DwUrl::userInfo() const
{
    return m_userInfo;
}

//------------------------------------------------------------------------------
/**
*/
void DwUrl::setHost(const DwString& s)
{
    m_isEmpty = false;
    m_host = s;
}

//------------------------------------------------------------------------------
/**
*/
const DwString& DwUrl::host() const
{
    return m_host;
}

//------------------------------------------------------------------------------
/**
*/
void DwUrl::setPort(const DwString& s)
{
    m_isEmpty = false;
    m_port = s;
}

//------------------------------------------------------------------------------
/**
*/
const DwString& DwUrl::port() const
{
    return m_port;
}

//------------------------------------------------------------------------------
/**
*/
void DwUrl::setLocalPath(const DwString& s)
{
    m_isEmpty = false;
    m_localPath = s;
}

//------------------------------------------------------------------------------
/**
*/
const DwString& DwUrl::localPath() const
{
    return m_localPath;
}

//------------------------------------------------------------------------------
/**
*/
void DwUrl::setFragment(const DwString& s)
{
    m_isEmpty = false;
    m_fragment = s;
}

//------------------------------------------------------------------------------
/**
*/
const DwString& DwUrl::fragment() const
{
    return m_fragment;
}

//------------------------------------------------------------------------------
/**
*/
void DwUrl::setQuery(const DwString& s)
{
    m_isEmpty = false;
    m_query = s;
}

//------------------------------------------------------------------------------
/**
*/
const DwString& DwUrl::query() const
{
    return m_query;
}

//------------------------------------------------------------------------------
/**
    Resolve assigns and split DwUrl DwString into its components.

    @TODO: this is too complicated...
*/
bool DwUrl::split(const DwString& s)
{
    clear();
    m_isEmpty = false;

    // resolve assigns
    DwString str = dwAssignLib()->resolveAssignsInString(s);

    // scheme is the first components and ends with a :
    IndexT schemeColonIndex = str.indexOf(L':');
    DwString potentialScheme;
    bool schemeIsDevice = false;
    if (InvalidIndex != schemeColonIndex)
    {
        potentialScheme = str.mid(0, schemeColonIndex);
        if (DwFsWrapper::isDeviceName(potentialScheme))
        {
            // there is either no scheme given, or the "scheme"
            // is actually a device name
            // in both cases we fall back to the default scheme "file", and
            // just set the whole DwString as local path, there will be no
            // other components
            schemeIsDevice = true;
        }
    }
    if ((InvalidIndex == schemeColonIndex) || schemeIsDevice)
    {
        setScheme(DEFAULT_IO_SCHEME);
        setLocalPath(str);
        return true;
    }

    // check is a valid scheme was provided
    if (InvalidIndex != schemeColonIndex)
    {
        // a valid scheme is given
        setScheme(potentialScheme);

        // after the scheme, and before the host, there must be a double slash
        if (!((str[schemeColonIndex + 1] == L'/') && (str[schemeColonIndex + 2] == L'/')))
        {
            return false;
        }
    }

    // extract UserInfo, Host and Port components
    IndexT hostStartIndex = schemeColonIndex + 3;
    IndexT hostEndIndex = str.indexOf(L'/', hostStartIndex);
    DwString userInfoHostPort;
    DwString path;
    if (InvalidIndex == hostEndIndex)
    {
        userInfoHostPort = dwExtractToEnd(str, hostEndIndex);
    }
    else
    {
        userInfoHostPort = str.mid(hostStartIndex, hostEndIndex - hostStartIndex);
        path = dwExtractToEnd(str, hostEndIndex + 1);
    }

    // extract port number if exists
    IndexT portIndex = userInfoHostPort.lastIndexOf(L':');
    IndexT atIndex = userInfoHostPort.indexOf(L'@');
    if (InvalidIndex != portIndex)
    {
        if (InvalidIndex != atIndex && portIndex > atIndex)
        {
            setPort(dwExtractToEnd( userInfoHostPort, portIndex + 1));
            dwTerminateAtIndex(userInfoHostPort, portIndex);
        }
    }
    if (InvalidIndex != atIndex)
    {
        setHost(dwExtractToEnd( userInfoHostPort, atIndex + 1));

        dwTerminateAtIndex(userInfoHostPort, atIndex);
        setUserInfo(userInfoHostPort);
    }
    else
    {
        setHost(userInfoHostPort);
    }

    // split path part into components
    if (!path.isEmpty())
    {
        IndexT fragmentIndex = path.indexOf(L'#');
        IndexT queryIndex = path.indexOf(L'?');
        if (InvalidIndex != queryIndex)
        {
            if (InvalidIndex != fragmentIndex)
            {
                DW_ASSERT(queryIndex > fragmentIndex);
            }
            setQuery(dwExtractToEnd( path, queryIndex + 1));

            dwTerminateAtIndex(path, queryIndex);
        }
        if (InvalidIndex != fragmentIndex)
        {
            setFragment(dwExtractToEnd ( path, fragmentIndex + 1));

            dwTerminateAtIndex(path, fragmentIndex);
        }
        setLocalPath(path);
    }
    return true;
}

//------------------------------------------------------------------------------
/**
    This builds an DwUrl DwString from its components.
*/
DwString DwUrl::build() const
{
    DW_ASSERT(!isEmpty());

    DwString str;
    if (!m_scheme.isEmpty())
    {
        str.append(m_scheme);
        str.append(L"://");
    }
    if (!m_userInfo.isEmpty())
    {
        str.append(m_userInfo);
        str.append(L"@");
    }
    if (!m_host.isEmpty())
    {
        str.append(m_host);
    }
    if (!m_port.isEmpty())
    {
        str.append(L":");
        str.append(m_port);
    }
    if (!m_localPath.isEmpty())
    {
        str.append(L"/");
        str.append(m_localPath);
    }
    if (!m_fragment.isEmpty())
    {
        str.append(L"#");
        str.append(m_fragment);
    }
    if (!m_query.isEmpty())
    {
        str.append(L"?");
        str.append(m_query);
    }
    return str;
}

//------------------------------------------------------------------------------
/**
    This returns the "tail", which is the local path, the fragment and
    the query concatenated into one DwString.
*/
DwString DwUrl::tail() const
{
    DwString str;
    if (!m_localPath.isEmpty())
    {
        str.append(m_localPath);
    }
    if (!m_fragment.isEmpty())
    {
        str.append(L"#");
        str.append(m_fragment);
    }
    if (!m_query.isEmpty())
    {
        str.append(L"?");
        str.append(m_query);
    }
    return str;
}

//------------------------------------------------------------------------------
/**
    Returns the host and local path in the form "//host/localpath".
    If no host has been set, only "/localpath" will be returned.
*/
DwString DwUrl::hostAndLocalPath() const
{
    DwString str;
    if (!m_host.isEmpty())
    {
        str.append(L"//");
        str.append(m_host);
        str.append(L"/");
    }
    str.append(m_localPath);
    return str;
}

//------------------------------------------------------------------------------
/**
    Appends an element to the local path. Automatically inserts
    a path delimiter "/".
*/
void DwUrl::appendLocalPath(const DwString& pathComponent)
{
    DW_ASSERT(!pathComponent.isEmpty());
    m_localPath.append(L"/");
    m_localPath.append(pathComponent);
}

//------------------------------------------------------------------------------
/**
*/
static DwVector<DwString> tokenize(DwString str, wchar_t token)
{
    DwVector<DwString> tokens;

    IndexT startIndex = 0;
    IndexT index = str.indexOf(token, startIndex);
    while (index != InvalidIndex && startIndex < str.size())
    {
        tokens.append(str.mid(startIndex, index - startIndex));
        startIndex = index + 1;
        index = str.indexOf(token, startIndex);
    };

    if (startIndex < str.size())
    {
        tokens.append(str.right(str.size() - startIndex));
    }
    return tokens;
}

//------------------------------------------------------------------------------
/**
    This parses the query part of the DwUrl (in the form
    param1=value&param2=value&param3=value ...) into a dictionary. Ill-formatted
    query fragments will be ignored.
*/
DwMap<DwString, DwString> DwUrl::parseQuery() const
{
    DwMap<DwString, DwString> result;
    DwVector<DwString> keyValuePairs = tokenize(m_query, L'&');
    IndexT i;
    for (i = 0; i < keyValuePairs.size(); i++)
    {
        DwVector<DwString> keyValueTokens = tokenize(keyValuePairs[i], L'=');
        if (keyValueTokens.size() == 2)
        {
            result.insert(keyValueTokens[0], keyValueTokens[1]);
        }
    }
    return result;
}
