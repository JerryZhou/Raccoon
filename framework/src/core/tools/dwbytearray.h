#pragma once

#include "dwshareddata.h"
#include "eastl/string.h"

class DwByteRef;

class DW_CORE_EXPORT DwByteArray
{
private:
    struct Data
    {
        DwAtomicInt ref;
        int alloc, size;
        char *data;
        char array[1];
    };

public:
    DwByteArray();
    DwByteArray(const char *str);
    DwByteArray(const char *str, int len);
    DwByteArray(char ch, int count);
    DwByteArray(const DwByteArray &other);

    inline ~DwByteArray()
    {
        if (!d->ref.deref()) dwFree(d);
    }

    inline int count() const
    {
        return d->size;
    }
    inline int size() const
    {
        return d->size;
    }
    bool isNull() const;
    bool isEmpty() const
    {
        return d->size == 0;
    }

    char *data()
    {
        detach();
        return d->data;
    }
    const char *data() const
    {
        return d->data;
    }
    inline const char *constData() const
    {
        return d->data;
    }

    char at(int i) const
    {
        return d->array[i];
    }

    void resize(int size);

    inline void squeeze()
    {
        if (d->size < d->alloc) reallocData(d->size);
    }

    DwByteArray &append(char ch);
    DwByteArray &append(const char *str);
    DwByteArray &append(const char *str, int len);
    DwByteArray &append(const DwByteArray &ba);

    DwByteArray &prepend(char c);
    DwByteArray &prepend(const char *str);
    DwByteArray &prepend(const char *str, int len);
    DwByteArray &prepend(const DwByteArray &ba);

    DwByteArray &insert(int i, char c);
    DwByteArray &insert(int i, const char *str);
    DwByteArray &insert(int i, const char *str, int len);
    DwByteArray &insert(int i, const DwByteArray &ba);

    DwByteArray &remove(int pos, int len);
    void clear();

    bool contains(char ch) const
    {
        return indexOf(ch) != -1;
    }
    bool contains(const char *str) const
    {
        return indexOf(str) != -1;
    }
    bool contains(const DwByteArray &ba) const;

    int indexOf(char ch, int from  = 0) const;
    int indexOf(const char *str, int from = 0) const;

    bool endsWith(const DwByteArray & ba ) const;
    bool endsWith(const char * str ) const;
    bool endsWith(char ch ) const;

    int	lastIndexOf(const DwByteArray & ba, int from = -1) const;
    int	lastIndexOf(const char * str, int from = -1) const;
    int	lastIndexOf(char ch, int from = -1) const;

    DwByteArray	left(int len) const;
    DwByteArray	right(int len) const;
    DwByteArray	mid(int pos, int len = -1) const;

    DwByteArray toHex() const;
    static DwByteArray fromHex(const DwByteArray & hexEncoded);

    DwByteArray &operator+=(const DwByteArray & ba)
    {
        return append(ba);
    }
    DwByteArray &operator+=(const char * str)
    {
        return append(str);
    }
    DwByteArray &operator+=(char ch )
    {
        return append(ch);
    }
    DwByteArray &operator=(const DwByteArray & other );
    DwByteArray &operator=(const char * str );

    char operator[] (int i) const
    {
        return d->data[i];
    }
    char operator[] (unsigned int i) const
    {
        return d->data[i];
    }
    DwByteRef operator[](int i);
    DwByteRef operator[](unsigned int i);

    uint32_t crc32() const;

private:
    friend class DwByteRef;

    static Data *fromAscii(const char *str, int size);
    static Data shared_null;
    static Data shared_empty;

    inline void detach()
    {
        if (d->ref != 1 || d->data != d->array) reallocData(d->size);
    }

    int grow(int size) const;
    void reallocData(int alloc);
    void expand(int i)
    {
        resize(__max(i + 1, d->size));
    }

    Data *d;
};

inline bool operator==(const DwByteArray &a1, const DwByteArray &a2)
{
    return (a1.size() == a2.size()) && (memcmp(a1.constData(), a2.constData(), a1.size())==0);
}
inline bool operator==(const DwByteArray &a1, const char *a2)
{
    return a2 ? strcmp(a1.constData(), a2) == 0 : a1.isEmpty();
}
inline bool operator==(const char *a1, const DwByteArray &a2)
{
    return a1 ? strcmp(a1, a2.constData()) == 0 : a2.isEmpty();
}

inline bool operator!=(const DwByteArray &a1, const DwByteArray &a2)
{
    return !(a1==a2);
}
inline bool operator!=(const DwByteArray &a1, const char *a2)
{
    return a2 ? strcmp(a1.constData(), a2) != 0 : !a1.isEmpty();
}
inline bool operator!=(const char *a1, const DwByteArray &a2)
{
    return a1 ? strcmp(a1, a2.constData()) != 0 : !a2.isEmpty();
}

inline bool operator<( const DwByteArray & a1, const DwByteArray & a2 )
{
    return strcmp(a1.constData(), a2.constData()) < 0;
}
inline bool operator<( const DwByteArray & a1, const char * a2 )
{
    return strcmp(a1.constData(), a2) < 0;
}
inline bool operator<( const char * a1, const DwByteArray & a2 )
{
    return strcmp(a1, a2.constData()) < 0;
}

inline bool operator>( const DwByteArray & a1, const DwByteArray & a2 )
{
    return strcmp(a1.constData(), a2.constData()) > 0;
}
inline bool operator>( const DwByteArray & a1, const char * a2 )
{
    return strcmp(a1.constData(), a2) > 0;
}
inline bool operator>( const char * a1, const DwByteArray & a2 )
{
    return strcmp(a1, a2.constData()) > 0;
}

inline bool operator<=( const DwByteArray & a1, const DwByteArray & a2 )
{
    return !operator>(a1, a2);
}
inline bool operator<=( const DwByteArray & a1, const char * a2 )
{
    return !operator>(a1, a2);
}
inline bool operator<=( const char * a1, const DwByteArray & a2 )
{
    return !operator>(a1, a2);
}

inline bool operator>=( const DwByteArray & a1, const DwByteArray & a2 )
{
    return !operator<(a1, a2);
}
inline bool operator>=( const DwByteArray & a1, const char * a2 )
{
    return !operator<(a1, a2);
}
inline bool operator>=( const char * a1, const DwByteArray & a2 )
{
    return !operator<(a1, a2);
}

inline const DwByteArray operator+(const DwByteArray &a1, const DwByteArray &a2)
{
    return DwByteArray(a1) += a2;
}
inline const DwByteArray operator+(const DwByteArray &a1, const char *a2)
{
    return DwByteArray(a1) += a2;
}
inline const DwByteArray operator+(const DwByteArray &a1, char a2)
{
    return DwByteArray(a1) += a2;
}
inline const DwByteArray operator+(const char *a1, const DwByteArray &a2)
{
    return DwByteArray(a1) += a2;
}
inline const DwByteArray operator+(char a1, const DwByteArray &a2)
{
    return DwByteArray(&a1, 1) += a2;
}

class DW_CORE_EXPORT DwByteRef
{
    DwByteArray &a;
    int i;
    inline DwByteRef(DwByteArray &array, int idx)
        : a(array),i(idx) {}
    friend class DwByteArray;
public:
    inline operator const char() const
    {
        return i < a.d->size ? a.d->data[i] : char(0);
    }
    inline DwByteRef &operator=(char c)
    {
        if (i >= a.d->size) a.expand(i);
        else a.detach();
        a.d->data[i] = c;
        return *this;
    }
    inline DwByteRef &operator=(const DwByteRef &c)
    {
        if (i >= a.d->size) a.expand(i);
        else a.detach();
        a.d->data[i] = c.a.d->data[c.i];
        return *this;
    }
    inline bool operator==(char c) const
    {
        return a.d->data[i] == c;
    }
    inline bool operator!=(char c) const
    {
        return a.d->data[i] != c;
    }
    inline bool operator>(char c) const
    {
        return a.d->data[i] > c;
    }
    inline bool operator>=(char c) const
    {
        return a.d->data[i] >= c;
    }
    inline bool operator<(char c) const
    {
        return a.d->data[i] < c;
    }
    inline bool operator<=(char c) const
    {
        return a.d->data[i] <= c;
    }
};

inline DwByteRef DwByteArray::operator[](int i)
{
    DW_ASSERT(i >= 0 && i < d->size);
    return DwByteRef(*this, i);
}
inline DwByteRef DwByteArray::operator[](unsigned int i)
{
    return DwByteRef(*this, i);
}
