#pragma once

#include "dwcore/dwstring.h"
#include "dwcore/dwmap.h"

class DW_GUI_EXPORT DwUrl
{
public:
    /// default constructor
    DwUrl();
    DwUrl(const DwString& s);
    DwUrl(const char *str);
    DwUrl(const wchar_t *str);
    DwUrl(const DwUrl& rhs);

    void operator=(const DwUrl& rhs);
    bool operator==(const DwUrl& rhs) const;
    bool operator!=(const DwUrl& rhs) const;

    void set(const DwString& s);
    DwString asString() const;

    bool isEmpty() const;
    bool isValid() const;
    void clear();
    void setScheme(const DwString& s);
    const DwString& scheme() const;
    void setUserInfo(const DwString& s);
    const DwString& userInfo() const;
    void setHost(const DwString& s);
    const DwString& host() const;
    void setPort(const DwString& s);
    const DwString& port() const;
    void setLocalPath(const DwString& s);
    const DwString& localPath() const;
    void appendLocalPath(const DwString& pathComponent);
    void setFragment(const DwString& s);
    const DwString& fragment() const;
    void setQuery(const DwString& s);
    const DwString& query() const;
    DwMap<DwString, DwString> parseQuery() const;
    DwString tail() const;
    DwString hostAndLocalPath() const;

private:
    /// split string into components
    bool split(const DwString& s);
    /// build string from components
    DwString build() const;

    bool m_isEmpty;
    DwString m_scheme;
    DwString m_userInfo;
    DwString m_host;
    DwString m_port;
    DwString m_localPath;
    DwString m_fragment;
    DwString m_query;
};// end of DwUrl