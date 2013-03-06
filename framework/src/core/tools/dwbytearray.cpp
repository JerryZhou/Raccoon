#include "stable.h"

#include "dwcore/dwmalloc.h"
#include "3rdparty/crc32.h"
#include "dwbytearray.h"

static DwByteArray &bytearray_insert(DwByteArray *ba, int pos, const char *arr, int len)
{
    DW_ASSERT(pos >= 0);

    if (pos < 0 || len <= 0 || arr == 0)
    {
        return *ba;
    }

    int oldsize = ba->count();
    ba->resize(__max(pos, oldsize) + len);
    char *dst = ba->data();
    if (pos > oldsize)
    {
        ::memset(dst + oldsize, 0x20, pos - oldsize);
    }
    else
    {
        ::memmove(dst + pos + len, dst + pos, oldsize - pos);
    }
    memcpy(dst + pos, arr, len);
    return *ba;
}

#define REHASH(a) \
	if (ol_minus_1 < sizeof(unsigned int) * CHAR_BIT) \
	hashHaystack -= (a) << ol_minus_1; \
	hashHaystack <<= 1

static int lastIndexOfHelper(const char *haystack, int dataLen, const char *needle, int findLen, int from)
{
    int delta = dataLen - findLen;
    if (from < 0)
    {
        from = delta;
    }
    if (from < 0 || from > dataLen)
    {
        return -1;
    }
    if (from > delta)
    {
        from = delta;
    }

    const char *end = haystack;
    haystack += from;
    const unsigned int ol_minus_1 = findLen - 1;
    const char *n = needle + ol_minus_1;
    const char *h = haystack + ol_minus_1;
    unsigned int hashNeedle = 0, hashHaystack = 0;
    int idx;
    for (idx = 0; idx < findLen; ++idx)
    {
        hashNeedle = ((hashNeedle<<1) + *(n-idx));
        hashHaystack = ((hashHaystack<<1) + *(h-idx));
    }
    hashHaystack -= *haystack;
    while (haystack >= end)
    {
        hashHaystack += *haystack;
        if (hashHaystack == hashNeedle && memcmp(needle, haystack, findLen) == 0)
        {
            return haystack - end;
        }
        --haystack;
        REHASH(*(haystack + findLen));
    }
    return -1;
}

DwByteArray::Data * DwByteArray::fromAscii( const char *str, int len )
{
    DwByteArray::Data *d = NULL;
    if (!str)
    {
        d = &shared_null;
    }
    else if (!*str)
    {
        d = &shared_empty;
    }
    else
    {
        d = static_cast<Data *>(dwMalloc(sizeof(Data) + len));
        d->ref = 0;;
        d->alloc = d->size = len;
        d->data = d->array;
        memcpy(d->array, str, len); // include null terminator
        d->array[len] = 0;
    }
    d->ref.ref();

    return d;
}

DwByteArray::Data DwByteArray::shared_null = { 1, 0, 0, shared_null.array, {0} };
DwByteArray::Data DwByteArray::shared_empty = { 1, 0, 0, shared_empty.array, {0} };

DwByteArray::DwByteArray()
    : d(&shared_null)
{
    d->ref.ref();
}

DwByteArray::DwByteArray(const char *str)
    : d(fromAscii(str, strlen(str)))
{
}

DwByteArray::DwByteArray(const char *str, int len)
    : d(fromAscii(str, len))
{
}

DwByteArray::DwByteArray(char ch, int count)
    : d(&shared_null)
{
    d->ref.ref();
    resize(count);
    memset(d->data, ch, count);
}

DwByteArray::DwByteArray(const DwByteArray &other)
    : d(other.d)
{
    d->ref.ref();
}

void DwByteArray::reallocData( int alloc )
{
    if (d->ref != 1 || d->data != d->array)
    {
        Data *x = static_cast<Data *>(dwMalloc(sizeof(Data) + alloc));
        x->size = __min(alloc, d->size);
        ::memcpy(x->array, d->data, x->size);
        x->array[x->size] = '\0';
        x->ref = 1;
        x->alloc = alloc;
        x->data = x->array;
        if (!d->ref.deref())
        {
            dwFree(d);
        }
        d = x;
    }
    else
    {
        Data *x = static_cast<Data *>(dwRealloc(d, sizeof(Data) + alloc));
        x->alloc = alloc;
        x->data = x->array;
        d = x;
    }
}

void DwByteArray::resize( int size )
{
    if (size <= 0)
    {
        Data *x = &shared_empty;
        x->ref.ref();
        if (!d->ref.deref())
        {
            dwFree(d);
        }
        d = x;
    }
    else if (d == &shared_null)
    {
        //
        // Optimize the idiom:
        //    DwByteArray a;
        //    a.resize(sz);
        Data *x = static_cast<Data *>(dwMalloc(sizeof(Data) + size));
        x->ref = 1;
        x->alloc = x->size = size;
        x->data = x->array;
        x->array[size] = '\0';
        (void) d->ref.deref(); // cannot be 0, x points to shared_null
        d = x;
    }
    else
    {
        if (d->ref != 1 || size > d->alloc || (size < d->size && size < d->alloc >> 1))
        {
            reallocData(grow(size));
        }
        if (d->alloc >= size)
        {
            d->size = size;
            if (d->data == d->array)
            {
                d->array[size] = '\0';
            }
        }
    }
}

int DwByteArray::grow( int size ) const
{
    int nsize = size + sizeof(Data);
    if(nsize < 64)
    {
        nsize += 16;
        nsize &= (~16);
    }
    else if(nsize < 4096)
    {
        nsize += 64;
        nsize &= (~64);
    }
    else
    {
        nsize += 4096;
        nsize &= (~4096);
    }

    return nsize - sizeof(Data);
}

DwByteArray &DwByteArray::append(const char *str)
{
    if(str)
    {
        append(str, strlen(str));
    }
    return *this;
}

DwByteArray &DwByteArray::append(const char *str, int len)
{
    if (len < 0)
    {
        len = strlen(str);
    }

    if (str && len)
    {
        if (d->ref != 1 || d->size + len > d->alloc)
        {
            reallocData(grow(d->size + len));
        }
        memcpy(d->data + d->size, str, len); // include null terminator
        d->size += len;
        d->data[d->size] = '\0';
    }
    return *this;
}

DwByteArray & DwByteArray::append(char ch)
{
    if (d->ref != 1 || d->size + 1 > d->alloc)
    {
        reallocData(grow(d->size + 1));
    }
    d->data[d->size++] = ch;
    d->data[d->size] = '\0';
    return *this;
}

DwByteArray &DwByteArray::append(const DwByteArray &ba)
{
    if ((d == &shared_null || d == &shared_empty))
    {
        *this = ba;
    }
    else if(ba.d != &shared_null)
    {
        if (d->ref != 1 || d->size + ba.d->size > d->alloc)
        {
            reallocData(grow(d->size + ba.d->size));
        }
        memcpy(d->data + d->size, ba.d->data, ba.d->size);
        d->size += ba.d->size;
        d->data[d->size] = '\0';
    }
    return *this;
}

DwByteArray &DwByteArray::prepend(const DwByteArray &ba)
{
    if (d == &shared_null || d == &shared_empty)
    {
        *this = ba;
    }
    else if (ba.d != &shared_null)
    {
        DwByteArray tmp = *this;
        *this = ba;
        append(tmp);
    }
    return *this;
}

DwByteArray &DwByteArray::prepend(const char *str)
{
    return prepend(str, strlen(str));
}

DwByteArray &DwByteArray::prepend(const char *str, int len)
{
    if (str)
    {
        if (d->ref != 1 || d->size + len > d->alloc)
        {
            reallocData(grow(d->size + len));
        }
        memmove(d->data+len, d->data, d->size);
        memcpy(d->data, str, len);
        d->size += len;
        d->data[d->size] = '\0';
    }
    return *this;
}

DwByteArray &DwByteArray::prepend(char c)
{
    return prepend(&c, 1);
}

DwByteArray &DwByteArray::operator=(const DwByteArray &other)
{
    other.d->ref.ref();
    if (!d->ref.deref())
    {
        dwFree(d);
    }
    d = other.d;
    return *this;
}

DwByteArray & DwByteArray::operator=( const char * str )
{
    clear();
    append(str);
    return *this;
}

bool DwByteArray::isNull() const
{
    return d == &shared_null;
}

DwByteArray & DwByteArray::insert( int i, char c )
{
    return bytearray_insert(this, i, &c, 1);
}

DwByteArray & DwByteArray::insert( int i, const char *str )
{
    return bytearray_insert(this, i, str, strlen(str));
}

DwByteArray & DwByteArray::insert( int i, const char *str, int len )
{
    return bytearray_insert(this, i, str, len);
}

DwByteArray & DwByteArray::insert( int i, const DwByteArray &ba )
{
    return bytearray_insert(this, i, ba.constData(), ba.count());
}

void DwByteArray::clear()
{
    if (!d->ref.deref())
    {
        dwFree(d);
    }
    d = &shared_null;
    d->ref.ref();
}

int DwByteArray::indexOf(char ch, int from /*= 0*/) const
{
    if (from < 0)
    {
        from = __max(from + d->size, 0);
    }
    if (from < d->size)
    {
        const char *n = d->data + from - 1;
        const char *e = d->data + d->size;
        while (++n != e)
        {
            if (*n == ch)
            {
                return  n - d->data;
            }
        }
    }

    return -1;
}

int DwByteArray::indexOf(const char *str, int  /*= 0*/) const
{
    int len = strlen(str);
    int count = d->size - len + 1;
    for(int i = 0; i < count; i++)
    {
        if(memcmp(d->data + i, str, len) == 0)
        {
            return i;
        }
    }

    return -1;
}

DwByteArray & DwByteArray::remove( int pos, int len )
{
    if (len <= 0  || pos >= d->size || pos < 0)
    {
        return *this;
    }
    detach();

    if (pos + len >= d->size)
    {
        resize(pos);
    }
    else
    {
        memmove(d->data + pos, d->data + pos + len, d->size - pos - len);
        resize(d->size - len);
    }
    return *this;
}

DwByteArray DwByteArray::toHex() const
{
    DwByteArray hex;
    hex.resize(d->size * 2);
    char *hexData = hex.data();
    const uint8_t *data = (const uint8_t *)d->data;
    for (int i = 0; i < d->size; ++i)
    {
        int j = (data[i] >> 4) & 0xf;
        if (j <= 9)
        {
            hexData[i * 2] = (j + '0');
        }
        else
        {
            hexData[i * 2] = (j + 'a' - 10);
        }
        j = data[i] & 0xf;
        if (j <= 9)
        {
            hexData[i * 2 + 1] = (j + '0');
        }
        else
        {
            hexData[i * 2 + 1] = (j + 'a' - 10);
        }
    }
    return hex;
}

DwByteArray DwByteArray::fromHex( const DwByteArray & hexEncoded )
{
    DwByteArray res;
    res.resize((hexEncoded.size() + 1)/ 2);
    unsigned char *result = (unsigned char *)res.data() + res.size();

    bool odd_digit = true;
    for (int i = hexEncoded.size() - 1; i >= 0; --i)
    {
        int ch = hexEncoded.at(i);
        int tmp;
        if (ch >= '0' && ch <= '9')
        {
            tmp = ch - '0';
        }
        else if (ch >= 'a' && ch <= 'f')
        {
            tmp = ch - 'a' + 10;
        }
        else if (ch >= 'A' && ch <= 'F')
        {
            tmp = ch - 'A' + 10;
        }
        else
        {
            continue;
        }
        if (odd_digit)
        {
            --result;
            *result = tmp;
            odd_digit = false;
        }
        else
        {
            *result |= tmp << 4;
            odd_digit = true;
        }
    }

    res.remove(0, result - (const unsigned char *)res.constData());
    return res;
}

uint32_t DwByteArray::crc32() const
{
    Dw::CRC32State state;
    crc32_init(&state);
    crc32_update(&state, (uint8_t*)d->data, d->size);
    return crc32_final(&state);
}

bool DwByteArray::endsWith( const DwByteArray & ba ) const
{
    if (d == ba.d || ba.d->size == 0)
    {
        return true;
    }
    if (d->size < ba.d->size)
    {
        return false;
    }
    return memcmp(d->data + d->size - ba.d->size, ba.d->data, ba.d->size) == 0;
}

bool DwByteArray::endsWith( const char * str ) const
{
    if (!str || !*str)
    {
        return true;
    }
    int len = strlen(str);
    if (d->size < len)
    {
        return false;
    }
    return strncmp(d->data + d->size - len, str, len) == 0;
}

bool DwByteArray::endsWith( char ch ) const
{
    if (d->size == 0)
    {
        return false;
    }
    return d->data[d->size - 1] == ch;
}

int DwByteArray::lastIndexOf( char ch, int from /*= -1*/ ) const
{
    if (from < 0)
    {
        from += d->size;
    }
    else if (from > d->size)
    {
        from = d->size - 1;
    }
    if (from >= 0)
    {
        const char *b = d->data;
        const char *n = d->data + from + 1;
        while (n-- != b)
        {
            if (*n == ch)
            {
                return  n - b;
            }
        }
    }
    return -1;
}

int DwByteArray::lastIndexOf( const char * str, int from /*= -1*/ ) const
{
    const int findLen = strlen(str);
    if (findLen == 1)
    {
        return lastIndexOf(*str, from);
    }

    return lastIndexOfHelper(d->data, d->size, str, findLen, from);
}

int DwByteArray::lastIndexOf( const DwByteArray & ba, int from /*= -1*/ ) const
{
    const int findLen = ba.d->size;
    if (findLen == 1)
    {
        return lastIndexOf(*ba.d->data, from);
    }

    return lastIndexOfHelper(d->data, d->size, ba.d->data, findLen, from);
}

DwByteArray DwByteArray::left( int len ) const
{
    if (len >= d->size)
    {
        return *this;
    }
    if (len < 0)
    {
        len = 0;
    }
    return DwByteArray(d->data, len);
}

DwByteArray DwByteArray::right( int len ) const
{
    if (len >= d->size)
    {
        return *this;
    }
    if (len < 0)
    {
        len = 0;
    }
    return DwByteArray(d->data + d->size - len, len);
}

DwByteArray DwByteArray::mid( int pos, int len /*= -1*/ ) const
{
    if (d == &shared_null || d == &shared_empty || pos >= d->size)
    {
        return DwByteArray();
    }
    if (len < 0)
    {
        len = d->size - pos;
    }
    if (pos < 0)
    {
        len += pos;
        pos = 0;
    }
    if (len + pos > d->size)
    {
        len = d->size - pos;
    }
    if (pos == 0 && len == d->size)
    {
        return *this;
    }
    return DwByteArray(d->data + pos, len);
}
