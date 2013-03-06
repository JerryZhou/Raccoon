#pragma once

#include <dwcore/dwstring.h>
#include <dwcore/dwbytearray.h>

class DW_CORE_EXPORT DwTextCodec
{
public:
    enum Encoding
    {
        Ascii,
        Utf8,
        Unicode,
        Locale
    };
    explicit DwTextCodec(Encoding encoding);
    ~DwTextCodec();

public:
    DwString toUnicode(const char *str, int size, int *remainBytes);
    DwByteArray fromUnicode(const wchar_t *str, int size);

private:
    unsigned int codepage() const;

private:
    Encoding m_encoding;
};// end of DwTextCodec