#pragma once

#include "dwbytearray.h"

class DwCryptographicHashPrivate;

class DW_CORE_EXPORT DwCryptographicHash
{
    DW_DISABLE_COPY(DwCryptographicHash)

public:
    enum Algorithm
    {
        Md4,
        Md5,
        Sha1
    };

    DwCryptographicHash(Algorithm method);
    ~DwCryptographicHash();

    void reset();

    void addData(const char *data, int length);
    void addData(const DwByteArray &data);

    DwByteArray result() const;

    static DwByteArray hash(const DwByteArray &data, Algorithm method);
    static bool fileMd5(const wchar_t* fileName, DwByteArray &md5Hex);

private:
    DwCryptographicHashPrivate *d_ptr;
};