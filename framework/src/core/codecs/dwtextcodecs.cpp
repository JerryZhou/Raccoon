#include "stable.h"

#include "dwcore/dwdebug.h"
#include "dwtextcodecs.h"

int utf8ValidCharCount(const char *str, int size)
{
    int pos = 0;
    while(pos < size)
    {
        char c = *(str + pos);
        if((c & 0x80) == 0)
        {
            pos++;
        }
        else if((c & 0xE0) == 0xC0)
        {
            if(pos + 2 > size)
            {
                break;
            }
            pos += 2;
        }
        else if((c & 0xF0) == 0xE0)
        {
            if(pos + 3 > size)
            {
                break;
            }
            pos += 3;
        }
    }

    return pos;
}

DwTextCodec::DwTextCodec( Encoding encoding )
    : m_encoding(encoding)
{

}

DwTextCodec::~DwTextCodec()
{

}

DwString DwTextCodec::toUnicode( const char *str, int size, int *remainBytes )
{
    uint32_t cp = codepage();
    if(cp == -1 || !size)
    {
        return DwString();
    }

    const char *mb = str;
    int mblen = size;
    const int wclen_auto = 4096;
    wchar_t wc_auto[wclen_auto];
    int wclen = wclen_auto;
    wchar_t *wc = wc_auto;

    int len = 0;
    while (!(len = MultiByteToWideChar(cp, MB_ERR_INVALID_CHARS, mb, mblen, wc, wclen)))
    {
        int r = GetLastError();
        if (r == ERROR_INSUFFICIENT_BUFFER)
        {
            if (wc != wc_auto)
            {
                dwDebug() << "MultiByteToWideChar: Size changed";
                break;
            }
            else
            {
                wclen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED,
                                            mb, mblen, 0, 0);
                wc = new wchar_t[wclen];
                // and try again...
            }
        }
        else if (r == ERROR_NO_UNICODE_TRANSLATION)
        {
            mblen--;
            *remainBytes++;
        }
        else
        {
            // Fail.
            dwDebug() << "MultiByteToWideChar: Cannot convert multibyte text";
            break;
        }
    };

    DwString ret = DwString::fromWCharArray(wc, len);

    if(wc != wc_auto)
    {
        delete[] wc;
    }
    return ret;
}

DwByteArray DwTextCodec::fromUnicode(const wchar_t *str, int size)
{
    uint32_t cp = codepage();
    if(cp == -1 || !size)
    {
        return DwByteArray();
    }

    char auto_buf[4096];
    char *mb = auto_buf;
    int mblen = 4096;
    mblen = WideCharToMultiByte(cp, 0, str, size, mb, mblen, NULL, NULL);

    DWORD dwErr = GetLastError();
    if(dwErr == ERROR_INSUFFICIENT_BUFFER)
    {
        mb = new char[mblen];
        mblen = WideCharToMultiByte(cp, 0, str, size, mb, mblen, NULL, NULL);
    }
    if (0 == mblen)
    {
        dwDebug() << "DwTextCodec::fromUnicode WideCharToMultiByte error: " << dwErr;
    }

    DwByteArray ret(mb, mblen);

    if(mb != auto_buf)
    {
        delete[] mb;
    }

    return ret;
}

unsigned int DwTextCodec::codepage() const
{
    unsigned int codepage = -1;
    switch(m_encoding)
    {
    case Utf8:
    {
        codepage = CP_UTF8;
        break;
    }
    case Locale:
    {
        codepage = CP_ACP;
        break;
    }
    default:
    {
        break;
    }
    }

    return codepage;
}
