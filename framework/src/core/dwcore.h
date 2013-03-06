#pragma once
//#include "D:\\program files\\Visual Leak Detector\\include\\vld.h"

#include <eastl/type_traits.h>
#include <math.h>

#include "global/dwtypes.h"

DW_CORE_EXPORT int dwRand();

/************************************************************************
	Utility inline functions
************************************************************************/


template<typename T>
inline T dwAbs(const T &t)
{
    return t >=0 ? t : -t;
}

template<typename T>
inline void dwSwap(T &t1, T &t2)
{
    T temp = t1;
    t1 = t2;
    t2 = temp;
}

template<typename T>
inline T dwMax(const T &t1, const T &t2)
{
    return t1 > t2 ? t1 : t2;
}

template<typename T>
inline T dwMin(const T &t1, const T &t2)
{
    return t1 < t2 ? t1 : t2;
}

inline int dwRound(double d)
{
    return d >= 0.0 ? int(d + 0.5) : int(d - int(d-1) + 0.5) + int(d-1);
}

/*
   This function tests a double for a null value. It doesn't
   check whether the actual value is 0 or close to 0, but whether
   it is binary 0.
*/

static inline bool dwIsNull(double f)
{
    union U
    {
        double f;
        uint64_t u;
    };
    U val;
    val.f = f;
    return val.u == uint64_t(0);
}

static inline bool dwFuzzyIsNull(float f)
{
    return dwAbs(f) <= 0.00001f;
}

static inline bool dwFuzzyIsNull(double d)
{
    return dwAbs(d) <= 0.000000000001;
}

static inline bool dwFuzzyCompare(double p1, double p2)
{
    return (dwAbs(p1 - p2) <= 0.000000000001 * dwMin(dwAbs(p1), dwAbs(p2)));
}

static inline bool dwFuzzyCompare(float p1, float p2)
{
    return (dwAbs(p1 - p2) <= 0.00001f * dwMin(dwAbs(p1), dwAbs(p2)));
}

inline int dwFloor(double d)
{
    return (int)(floor(d));
}

inline int dwCeil(double d)
{
    return (int)(ceil(d));
}

template<typename T>
inline T dwClamp(T var, T min, T max)
{
    if(var > max) return max;
    if(var < min) return min;
    return var;
}
/************************************************************************
	Endian
************************************************************************/

/*
 * ENDIAN FUNCTIONS
 */
inline void dwbswap_helper(const unsigned char *src, unsigned char *dest, int size)
{
    for (int i = 0; i < size ; ++i) dest[i] = src[size - 1 - i];
}

/*
 * qbswap(const T src, const uchar *dest);
 * Changes the byte order of \a src from big endian to little endian or vice versa
 * and stores the result in \a dest.
 * There is no alignment requirements for \a dest.
 */
template <typename T> inline void dwbswap(const T src, unsigned char *dest)
{
    dwbswap_helper(reinterpret_cast<const unsigned char *>(&src), dest, sizeof(T));
}

template <typename T> T dwbswap(T source);

template <>
inline uint32_t dwbswap<uint32_t>(uint32_t source)
{
    return 0
           | ((source & 0x000000ff) << 24)
           | ((source & 0x0000ff00) << 8)
           | ((source & 0x00ff0000) >> 8)
           | ((source & 0xff000000) >> 24);
}


template <typename T>
inline T dwToBigEndian(T source)
{
    return dwbswap<T>(source);
}

template <typename T>
inline T dwFromBigEndian(T source)
{
    return dwbswap<T>(source);
}

template <typename T>
inline void dwToBigEndian(T src, unsigned char *dest)
{
    dwbswap<T>(src, dest);
}