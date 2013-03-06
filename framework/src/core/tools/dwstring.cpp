#include "stable.h"

#include "global/dwmalloc.h"
#include "dwstring.h"
#include "codecs/dwtextcodecs.h"
#include "tools/dwbytearray.h"

DwString DwString::fromAscii( const char *str, int size /*= -1*/ )
{
    return DwString(fromAscii_helper(str, size));
}

DwString DwString::fromUtf8( const char *str, int size /*= -1*/ )
{
    return DwString(fromUtf8_helper(str, size));
}

DwString DwString::fromWCharArray( const wchar_t *str, int size /*= -1*/ )
{
    return DwString(fromUnicode_helper(str, size));
}

DwString::Data *DwString::fromUtf8_helper(const char *str, int size/* = -1*/)
{
    if(size < 0)
    {
        size = strlen(str);
    }
    void *buf = dwMalloc(sizeof(DwString::Data) + sizeof(wchar_t) * size);
    DwString::Data *d = reinterpret_cast<DwString::Data *>(buf);
    d->ref = 1;
    d->alloc = size;
    d->data = d->array;
    size_t wlen = MultiByteToWideChar(CP_UTF8, 0, str, size, d->data, size + 1);
    d->array[wlen] = L'\0';
    d->size = wlen;

    return d;
}

DwString::Data *DwString::fromAscii_helper(const char *str, int size/* = -1*/)
{
    if(size < 0)
    {
        size = strlen(str);
    }
    void *buf = dwMalloc(sizeof(DwString::Data) + sizeof(wchar_t) * size);
    DwString::Data *d = reinterpret_cast<DwString::Data *>(buf);
    d->ref = 1;
    d->alloc = size;
    d->size = size;
    d->data = d->array;
    size_t wlen = MultiByteToWideChar(CP_ACP, 0, str, size, d->data, size + 1);
    d->array[wlen] = L'\0';
    d->size = wlen;

    return d;
}

DwString::Data *DwString::fromUnicode_helper(const wchar_t *str, int size /*= -1*/)
{
    if(size < 0)
    {
        size = wcslen(str);
    }
    void *buf = dwMalloc(sizeof(Data) + sizeof(wchar_t) * size);
    DwString::Data *d = reinterpret_cast<DwString::Data *>(buf);
    d->ref = 1;
    d->alloc = size;
    d->size = size;
    d->data = d->array;
    memcpy(d->array, str, sizeof(wchar_t) * size);
    d->array[size] = L'\0';
    return d;
}

DwByteArray DwString::toUtf8() const
{
    DwTextCodec c(DwTextCodec::Utf8);
    return c.fromUnicode(d->data, d->size);
}

DwByteArray DwString::toCap() const
{
    DwTextCodec c(DwTextCodec::Locale);
    return c.fromUnicode(d->data, d->size);
}


DwString::Data* DwString::shared_null()
{
    static DwString::Data shared_null_v = {1, 0, 0, shared_null_v.array, {0}};
    return &shared_null_v;
}

DwString::Data* DwString::shared_empty()
{
    static DwString::Data shared_empty_v = {1, 0, 0, shared_empty_v.array, {0}};
    return &shared_empty_v;
}

DwString::DwString()
    : d(shared_null())
{
    d->ref.ref();
}

DwString::DwString(const wchar_t *str)
    : d(fromUnicode_helper(str))
{
}

DwString::DwString(const wchar_t *str, int size)
    : d(fromUnicode_helper(str, size))
{
}

DwString::DwString(const char *str, int size)
    : d(fromAscii_helper(str, size))
{
}

DwString::DwString( const DwString &other )
    : d(other.d)
{
    d->ref.ref();
}

DwString::DwString( wchar_t ch )
    : d(fromUnicode_helper(&ch, 1))
{
}

DwString::~DwString()
{
    if ( !d->ref.deref() )
    {
        freeData(d);
    }
}

bool DwString::operator==(const DwString &other) const
{
    if (d->size != other.d->size)
    {
        return false;
    }

    return memcmp(d->data, other.d->data, d->size * sizeof(wchar_t)) == 0;
}

bool DwString::operator==( const char *other ) const
{
    return DwString::fromAscii(other) == *this;
}

bool DwString::operator==( const wchar_t *other ) const
{
    if (d->size != wcslen(other))
    {
        return false;
    }

    return memcmp(d->data, other, d->size * sizeof(wchar_t)) == 0;
}

DwString & DwString::operator=( const DwString &other )
{
    other.d->ref.ref();
    if ( !d->ref.deref() )
    {
        freeData(d);
    }
    d = other.d;
    return *this;
}

void DwString::freeData( Data* &d )
{
    dwFree(d);
    d = NULL;
}

DwString DwString::number( int i, int base )
{
    wchar_t buf[16];
    _itow_s(i, buf, 16, base);
    return DwString(buf);
}

DwString & DwString::append( const wchar_t *str )
{
    return append(DwString::fromWCharArray(str));
}

DwString & DwString::append( const char *str )
{
    return append(DwString::fromAscii(str));
}

DwString & DwString::append( const DwString &str )
{
    if (str.d != shared_null())
    {
        if (d == shared_null())
        {
            operator=(str);
        }
        else
        {
            if (d->ref != 1 || d->size + str.d->size > d->alloc)
            {
                reallocData( grow(d->size + str.d->size) );
            }
            memcpy(d->data + d->size, str.d->data, str.d->size * sizeof(wchar_t));
            d->size += str.d->size;
            d->data[d->size] = '\0';
        }
    }
    return *this;
}

DwString & DwString::append( const wchar_t ch )
{
    if (d->ref != 1 || d->size + 1 > d->alloc)
    {
        reallocData( grow(d->size + 1) );
    }

    d->data[d->size++] = ch;
    d->data[d->size] = '\0';
    return *this;
}

void DwString::reallocData(int alloc)
{
    if (d->ref != 1 || d->data != d->array)
    {
        Data *x = static_cast<Data *>(dwMalloc(sizeof(Data) + alloc * sizeof(wchar_t)));

        x->size = __min(alloc, d->size);
        ::memcpy(x->array, d->data, x->size * sizeof(wchar_t));
        x->array[x->size] = 0;
        x->ref = 1;
        x->alloc = alloc;
        x->data = x->array;
        if (!d->ref.deref())
        {
            DwString::freeData(d);
        }
        d = x;
    }
    else
    {
        d = static_cast<Data *>(dwRealloc(d, sizeof(Data) + alloc * sizeof(wchar_t)));
        d->alloc = alloc;
        d->data = d->array;
    }
}

void DwString::reallocData()
{
    reallocData(d->size);
}

void DwString::clear()
{
    if( !isNull() )
    {
        *this = DwString();
    }
}

static int ustring_compare(const wchar_t *a, const wchar_t *b, int l)
{
    while (l-- && *a == *b)
    {
        a++,b++;
    }
    if (l==-1)
    {
        return 0;
    }
    return *a - *b;
}

static int ustring_compare(const wchar_t *a, int alen, const wchar_t *b, int blen)
{
    if (a == b && alen == blen)
    {
        return 0;
    }

    int l = __min(alen, blen);
    int cmp = ustring_compare(a, b, l);
    return cmp ? cmp : (alen-blen);
}

int DwString::compare( const DwString &other ) const
{
    return ustring_compare(constString(), length(), other.constString(), other.length());
}

static int ustring_compare_nocase(const wchar_t *a, int alen, const wchar_t *b, int blen)
{
    if(a == b && alen == blen)
    {
        return 0;
    }

    int l = __min(alen, blen);
    int cmp = _wcsnicmp(a, b, l);
    return cmp ? cmp : (alen - blen);
}

int DwString::compareNoCase( const DwString &other ) const
{
    return ustring_compare_nocase(constString(), length(), other.constString(), other.length());
}

bool DwString::operator<( const DwString &other ) const
{
    return compare(other) < 0;
}

wchar_t DwString::at( int i ) const
{
    DW_ASSERT(i >= 0 && i < size());
    return d->data[i];
}

int DwString::indexOf( wchar_t ch, int from /*= 0*/ ) const
{
    if(from >= d->size)
    {
        return -1;
    }
    wchar_t *p = wcschr(d->data + from, ch);
    return p ? p - d->data : -1;
}

int DwString::indexOf( const DwString &str, int from /*= 0*/ ) const
{
    if (d->size <= 0)
    {
        return -1;
    }

    if(from >= d->size)
    {
        return -1;
    }
    wchar_t *p = wcsstr(d->data + from, str.constString());
    return p ? p - d->data : -1;
}

int DwString::lastIndexOf( wchar_t ch, int from /*= -1*/ ) const
{
    if (d->size <= 0)
    {
        return -1;
    }

    if (from < 0)
    {
        from += d->size;
    }
    DW_ASSERT(from >= 0 && from < d->size);

    wchar_t *p = d->data + from;
    while (p != d->data - 1)
    {
        if (*p == ch)
        {
            return p - d->data;
        }
        p --;
    }
    return -1;
}

int DwString::lastIndexOf( const DwString &str, int from /*= -1*/ ) const
{
    const int sl = str.d->size;
    if (sl == 1)
    {
        return lastIndexOf(str.d->data[0], from);
    }

    const int l = d->size;
    if (from < 0)
    {
        from += l;
    }
    int delta = l-sl;
    if (from == l && sl == 0)
    {
        return from;
    }
    if (from < 0 || from >= l || delta < 0)
    {
        return -1;
    }
    if (from > delta)
    {
        from = delta;
    }

    wchar_t *p = d->data + from;
    do
    {
        if(ustring_compare(p, str.d->size, str.d->data, str.d->size) == 0)
        {
            break;
        }
        p--;
    }
    while (p >= d->data);
    return p - d->data;
}

bool DwString::startsWith(const DwString &s) const
{
    if (d == shared_null())
    {
        return (s.d == shared_null());
    }
    if (d->size == 0)
    {
        return s.d->size == 0;
    }
    if (s.d->size > d->size)
    {
        return false;
    }
    return memcmp(d->data, s.d->data, s.d->size * sizeof(wchar_t)) == 0;
}

bool DwString::startsWith( wchar_t ch ) const
{
    return d->size
           ? d->data[0] == ch
           : ch == 0;
}

bool DwString::endsWith( const DwString &s ) const
{
    if (d == shared_null())
    {
        return (s.d == shared_null());
    }
    if (d->size == 0)
    {
        return s.d->size == 0;
    }
    int pos = d->size - s.d->size;
    if (pos < 0)
    {
        return false;
    }
    return memcmp(d->data + pos, s.d->data, s.d->size * sizeof(wchar_t)) == 0;
}

bool DwString::endsWith( wchar_t ch ) const
{
    return d->size
           ? d->data[d->size - 1] == ch
           : ch == 0;
}

DwString DwString::left( int n ) const
{
    if (n >= d->size || n < 0)
    {
        return *this;
    }
    return DwString(d->data, n);
}

DwString DwString::right( int n ) const
{
    if (n >= d->size || n < 0)
    {
        return *this;
    }
    return DwString(d->data + d->size - n, n);
}

DwString DwString::mid( int position, int n /*= -1*/ ) const
{
    if (d == shared_null() || position >= d->size)
    {
        return DwString();
    }
    if (n < 0)
    {
        n = d->size - position;
    }
    if (position < 0)
    {
        n += position;
        position = 0;
    }
    if (n + position > d->size)
    {
        n = d->size - position;
    }
    if (position == 0 && n == d->size)
    {
        return *this;
    }
    return DwString(d->data + position, n);
}

DwString & DwString::insert( int i, wchar_t ch )
{
    if (i < 0)
    {
        i += d->size;
    }
    if (i < 0)
    {
        return *this;
    }
    expand(__max(i, d->size));
    ::memmove(d->data + i + 1, d->data + i, (d->size - i) * sizeof(wchar_t));
    d->data[i] = ch;
    return *this;
}

DwString &DwString::insert( int i, const wchar_t *str, int size )
{
    if (i < 0 || size <= 0)
    {
        return *this;
    }

    const wchar_t *s = (const wchar_t *)str;
    if (s >= d->data && s < d->data + d->alloc)
    {
        // Part of me - take a copy
        wchar_t *tmp = static_cast<wchar_t *>(dwMalloc(size * sizeof(wchar_t)));
        memcpy(tmp, s, size * sizeof(wchar_t));
        insert(i, reinterpret_cast<const wchar_t *>(tmp), size);
        dwFree(tmp);
        return *this;
    }

    expand(__max(d->size, i) + size - 1);

    ::memmove(d->data + i + size, d->data + i, (d->size - i - size) * sizeof(wchar_t));
    memcpy(d->data + i, s, size * sizeof(wchar_t));
    return *this;
}

DwString & DwString::remove( int pos, int len )
{
    if (pos < 0)  // count from end of string
    {
        pos += d->size;
    }
    if (pos < 0 || pos >= d->size)
    {
        // range problems
    }
    else if (len >= d->size - pos)
    {
        resize(pos); // truncate
    }
    else if (len > 0)
    {
        detach();
        memmove(d->data + pos, d->data + pos + len,
                (d->size - pos - len + 1) * sizeof(wchar_t));
        d->size -= len;
    }
    return *this;
}

void DwString::expand( int size )
{
    int sz = d->size;
    resize(__max(size + 1, sz));
    if (d->size - 1 > sz)
    {
        wchar_t *n = d->data + d->size - 1;
        wchar_t *e = d->data + sz;
        while (n != e)
            * --n = ' ';
    }
}

void DwString::resize(int size)
{
    if (size < 0)
    {
        size = 0;
    }

    if (size == 0)
    {
        Data *x = shared_empty();
        x->ref.ref();
        if (!d->ref.deref())
        {
            freeData(d);
        }
        d = x;
    }
    else
    {
        if (d->ref != 1 || size > d->alloc)
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

int DwString::grow( int size )
{
    int nsize = size * sizeof(wchar_t) + sizeof(Data);
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

    return (nsize - sizeof(Data)) / sizeof(wchar_t);
}

void DwString::detach()
{
    if (d->ref != 1 || d->data != d->array) reallocData();
}

/*
this implementation is not efficient, but it's really simple.
could be optimized later
*/
DwString & DwString::replace( const DwString &before, const DwString &after )
{
    DwString t;
    int lastPos = 0;
    while(true)
    {
        int pos = indexOf(before, lastPos);
        if(pos == -1)
        {
            t.append( DwString::fromWCharArray(d->data + lastPos, d->size - lastPos) );
            break;
        }
        else
        {
            t.append( DwString::fromWCharArray(d->data + lastPos, pos - lastPos) );
            t.append(after);
            lastPos = pos + before.length();
        }
    }

    swap(t);
    return *this;
}

DwString &DwString::replaceNoCase( const DwString &before, const DwString &after )
{
    DW_UNUSED(before);
    DW_UNUSED(after);
    return *this;
}

void DwString::swap( DwString &other )
{
    Data *tmp = other.d;
    other.d = d;
    d = tmp;
}

int32_t DwString::toInt32( bool *ok /*= NULL*/ ) const
{
    for(int i = 0; i < d->size; i++)
    {
        if(d->data[i] < L'0' || d->data[i] > L'9')
        {
            if(ok)
            {
                *ok = false;
            }
            return 0;
        }
    }

    if(ok)
    {
        *ok = true;
    }
    return _wtoi(d->data);
}

uint32_t DwString::toUInt32( bool *ok /*= NULL*/ ) const
{
    for(int i = 0; i < d->size; i++)
    {
        if(d->data[i] < L'0' || d->data[i] > L'9')
        {
            if(ok)
            {
                *ok = false;
            }
            return 0;
        }
    }

    if(ok)
    {
        *ok = true;
    }
    return _wtoi(d->data);
}

DwString DwString::toLower() const
{
    DwString t(*this);
    _wcslwr(t.d->data);
    return t;
}

DwString DwString::toUpper() const
{
    DwString t(*this);
    _wcsupr(t.d->data);
    return t;
}
