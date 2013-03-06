#pragma once

#include "../dwcore.h"
#include <dwcore/dwatomicint.h>
#include "dwcore/dwdebug.h"

class DwByteArray;
class DW_CORE_EXPORT DwString
{
private:
    DwString(bool);
    DwString(int);
public:
    DwString();
    DwString(wchar_t ch);
    DwString(const DwString &other);
    DwString(const wchar_t *str);
    DwString(const wchar_t *str, int size);
    DwString(const char *str, int size=-1);

    ~DwString();

public:
    static DwString number(int i, int base = 10);
    static DwString fromAscii(const char *str, int size = -1);
    static DwString fromUtf8(const char *str, int size = -1);
    static DwString fromWCharArray(const wchar_t *str, int size = -1);

    DwByteArray toUtf8() const;
    DwByteArray toCap() const;

public:
    inline bool isNull() const
    {
        return d == shared_null();
    }
    inline bool isEmpty() const
    {
        return d->size == 0;
    }

    inline wchar_t *constString() const
    {
        return d->data;
    }
    inline int length() const
    {
        return d->size;
    };
    inline int size() const
    {
        return d->size;
    };
    wchar_t at(int i) const;
    void clear();

    DwString &append(const wchar_t ch);
    DwString &append(const char *str);
    DwString &append(const wchar_t *str);
    DwString &append(const DwString &str);

    int compare(const DwString &other) const;
    int compareNoCase(const DwString &other) const;

    int indexOf(wchar_t ch, int from = 0) const;
    int indexOf(const DwString &str, int from = 0) const;
    int lastIndexOf(wchar_t ch, int from = -1) const;
    int lastIndexOf(const DwString &str, int from = -1) const;

    inline bool contains(wchar_t ch) const
    {
        return indexOf(ch, 0) != -1;
    }
    inline bool contains(const DwString &str) const
    {
        return indexOf(str, 0) != -1;
    }

    bool startsWith(wchar_t ch) const;
    bool startsWith(const DwString &s) const;
    bool endsWith(wchar_t ch) const;
    bool endsWith(const DwString &s) const;

    DwString left(int n) const;
    DwString right(int n) const;
    DwString mid(int position, int n = -1) const;

    DwString &insert(int i, wchar_t ch);
    DwString &insert( int i, const wchar_t *str, int size );
    inline DwString &insert(int i, const DwString &str)
    {
        return insert(i, str.constString(), str.length());
    }

    DwString &remove(int pos, int len);
    void resize(int size);

    DwString &replace(const DwString &before, const DwString &after);
    DwString &replaceNoCase(const DwString &before, const DwString &after);

    bool operator==(const char *other) const;
    bool operator==(const wchar_t *other) const;
    bool operator==(const DwString &other) const;
    inline bool operator!=(const char *other) const
    {
        return !operator==(other);
    }
    inline bool operator!=(const wchar_t *other) const
    {
        return !operator==(other);
    }
    inline bool operator!=(const DwString &other) const
    {
        return !operator==(other);
    }

    bool operator<(const DwString &other) const;
    inline bool operator<=(const DwString &other) const
    {
        return !operator>(other);
    }
    inline bool operator>(const DwString &other) const
    {
        return other < *this;
    }
    inline bool operator>=(const DwString &other) const
    {
        return !operator<(other);
    }

    DwString &operator=(const DwString &other);

    inline DwString &operator+=(const wchar_t ch)
    {
        return append(ch);
    };
    inline DwString &operator+=(const char *str)
    {
        return append(str);
    };
    inline DwString &operator+=(const wchar_t *str)
    {
        return append(str);
    };
    inline DwString &operator+=(const DwString &str)
    {
        return append(str);
    };

    inline wchar_t operator[](int i)const
    {
        return at(i);
    }

    int32_t toInt32(bool *ok = NULL) const;
    uint32_t toUInt32(bool *ok = NULL) const;

    DwString toLower() const;
    DwString toUpper() const;

private:
    struct Data
    {
        DwAtomicInt ref;
        int alloc;
        int size;
        wchar_t *data;
        wchar_t array[1];
    };
    DwString(Data *dd) : d(dd) {};
    static Data *fromAscii_helper(const char *str, int size = -1);
    static Data *fromUtf8_helper(const char *str, int size = -1);
    static Data *fromUnicode_helper(const wchar_t *str, int size = -1);
    static int grow(int size);

    void expand(int size);
    void detach();
    void reallocData();
    void reallocData(int alloc);
    void swap(DwString &other);
    static void freeData(Data* &d);
    static Data* shared_null();
    static Data* shared_empty();

private:
    Data *d;
};

inline const DwString operator+(const DwString &s1, const DwString &s2)
{
    DwString t(s1);
    t += s2;
    return t;
}
inline const DwString operator+(const DwString &s1, wchar_t s2)
{
    DwString t(s1);
    t += s2;
    return t;
}
inline const DwString operator+(wchar_t s1, const DwString &s2)
{
    DwString t(s1);
    t += s2;
    return t;
}
inline const DwString operator+(const DwString &s1, const char *s2)
{
    DwString t(s1);
    t += DwString::fromAscii(s2);
    return t;
}
inline const DwString operator+(const char *s1, const DwString &s2)
{
    DwString t = DwString::fromAscii(s1);
    t += s2;
    return t;
}
inline const DwString operator+(char c, const DwString &s)
{
    DwString t = DwString::fromAscii(&c, 1);
    t += s;
    return t;
}
inline const DwString operator+(const DwString &s, char c)
{
    DwString t = s;
    t += c;
    return t;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwDebug & DwDebug::operator<< <DwString>(const DwString &t)
{
    m_stream << t.constString() << KSpace;
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template<>
inline DwDebug & DwDebug::operator>> <DwString>(DwString &t)
{
    t = DwString(m_stream.str().c_str());
    return *this;
}