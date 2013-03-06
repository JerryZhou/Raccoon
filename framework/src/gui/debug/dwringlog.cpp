#include "stable.h"
#include "dwringlog.h"

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwRingLog, 'RLOG', DwRttiObject);

//------------------------------------------------------------------------------
/**
    THE MINIMUM SIZE IS 10 BYTES
*/
DwRingLog::DwRingLog(int len/* = 4096*/)
    : m_buffer (NULL)
    , m_cur(0)
    , m_bufLen(len)
    , m_unicode(false)
{
    DW_ASSERT(len > 10);
    m_buffer = new char[m_bufLen+1];
    ::memset(m_buffer, 0, m_bufLen+1);
}

//------------------------------------------------------------------------------
/**
*/
DwRingLog::~DwRingLog()
{
    DW_SAFE_DELETE_ARRAY(m_buffer);
}

//------------------------------------------------------------------------------
/**
*/
bool DwRingLog::format(const wchar_t *fmt, ...)
{
    va_list argList;
    va_start(argList, fmt);
    int bufLen = dwClamp<int>(m_bufLen/(10*sizeof(wchar_t)), 0, m_bufLen/sizeof(wchar_t));
    wchar_t *buf = new wchar_t[bufLen];
    HRESULT hr = ::StringCchVPrintf(buf, bufLen, fmt, argList);
    bool isOk = SUCCEEDED(hr) || hr == STRSAFE_E_INSUFFICIENT_BUFFER;
    if (isOk)
    {
        isOk = copyToBuffer(buf);
    }
    DW_SAFE_DELETE_ARRAY(buf);
    va_end(argList);
    return isOk;
}

//------------------------------------------------------------------------------
/**
*/
bool DwRingLog::format(const char *fmt, ...)
{
    va_list argList;
    va_start(argList, fmt);
    char *buf = new char[m_bufLen/10];
    HRESULT hr = ::StringCchVPrintfA(buf, m_bufLen/10, fmt, argList);
    bool isOk = SUCCEEDED(hr) || hr == STRSAFE_E_INSUFFICIENT_BUFFER;
    if (isOk)
    {
        isOk = copyToBuffer(buf);
    }
    DW_SAFE_DELETE_ARRAY(buf);
    va_end(argList);
    return isOk;
}

//------------------------------------------------------------------------------
/**
*/
void DwRingLog::dump(const DwUrl &url)
{
    dump(url.localPath());
}

//------------------------------------------------------------------------------
/**
*/
void DwRingLog::dump(const char * path)
{
    dump(DwString(path));
}

//------------------------------------------------------------------------------
/**
*/
bool DwRingLog::isUnicode() const
{
    return m_unicode;
}

//------------------------------------------------------------------------------
/**
*/
void DwRingLog::setUnicode(bool unicode)
{
    m_unicode = unicode;
}

//------------------------------------------------------------------------------
/**
*/
const char* DwRingLog::data() const
{
    if (m_buffer[m_bufLen])
    {
        DW_ASSERT(false);
        const_cast<DwRingLog*>(this)->m_buffer[m_bufLen] = 0;
    }

    return m_buffer;
}

//------------------------------------------------------------------------------
/**
*/
void DwRingLog::clear()
{
    m_cur = 0;
    m_buffer[m_cur] = 0;
}

//------------------------------------------------------------------------------
/**
*/
bool DwRingLog::copyToBuffer(const wchar_t *buf)
{
    int len = wcslen(buf);
    return copyToBuffer((void*)buf, len * sizeof(wchar_t));
}

//------------------------------------------------------------------------------
/**
*/
bool DwRingLog::copyToBuffer(const char *buf)
{
    int len = strlen(buf);
    return copyToBuffer((void*)buf, len);
}

//------------------------------------------------------------------------------
/**
*/
bool DwRingLog::copyToBuffer(void *buf, int len)
{
    if (len < m_bufLen && len > 0 )
    {
        if (m_cur + len > m_bufLen)
        {
            ::memcpy_s(m_buffer + m_cur, m_bufLen - m_cur, buf, m_bufLen - m_cur);
            ::memcpy_s(m_buffer, m_bufLen, (void*)( (char*)buf + m_bufLen - m_cur), len + m_cur - m_bufLen);
            m_cur = len + m_cur - m_bufLen;
        }
        else
        {
            ::memcpy_s(m_buffer + m_cur, m_bufLen - m_cur, buf, len);
            m_cur += len;
            if (m_cur >= m_bufLen)
            {
                m_cur = 0;
            }
        }
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void DwRingLog::dump(DwString path)
{
    DwFile file(path);
    if (file.open((DwIODevice::OpenMode )
                  (DwIODevice::WriteOnly|DwIODevice::Truncate)))
    {
        if (isUnicode())
        {
            // unicode
            unsigned char c[] = {0xFF, 0xFE};
            file.write((char*)(c), sizeof(c));
        }
        else
        {
            // utf8
            // unsigned char c[] = {0xEF, 0xBB, 0xBF};
            // (NO BOM UTF8)
            // file.write((char*)(c), sizeof(c));
        }

        // be sure the last is 0
        m_buffer[m_bufLen] = 0;
        // skip zero characters
        int cur = m_cur;
        while(cur < m_bufLen && m_buffer[cur] == 0)
        {
            ++cur;
        }

        // write the head of logs
        if (cur < m_bufLen && cur > 0)
        {
            file.write((char*)(m_buffer + cur), (m_bufLen - cur));
        }
        // write the tail of logs
        if (m_cur)
        {
            file.write((char*)m_buffer, m_cur);
        }
        file.close();
    }
}

void DwRingLog::dumpToString(DwString &str)
{
    // be sure the last is 0
    m_buffer[m_bufLen] = 0;
    // skip zero characters
    int cur = m_cur;
    while(cur < m_bufLen && m_buffer[cur] == 0)
    {
        ++cur;
    }

    // write the head of logs
    if (cur < m_bufLen && cur > 0)
    {
        DwString tmp((char*)(m_buffer + cur), (m_bufLen - cur));
        str.append(tmp);
    }

    // write the tail of logs
    if (m_cur)
    {
        DwString tmp((char*)m_buffer, m_cur);
        str.append(tmp);
    }
}
