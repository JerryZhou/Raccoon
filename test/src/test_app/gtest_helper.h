#pragma once
#include "dwcore.h"
#include "gtest/gtest.h"

inline void PrintTo(const DwString& s, ::std::ostream* os) 
{
	std::wstring w(s.constString());
	testing::internal::PrintTo(w, os);
}

inline void PrintTo(const DwByteArray& s, ::std::ostream* os) 
{
	std::string w(s.data());
	testing::internal::PrintTo(w, os);
}

template<class T>
int sumOfContainer(const T &v)
{
    int sum = 0;
    for(T::const_iterator it = v.begin(); it != v.end(); ++it)
    {
        sum += *it;
    }

    return sum;
}

#ifdef _DEBUG
#pragma comment(lib, "../../../depends/dw3rd/gtest-1.5.0/lib/debug/gtestd.lib")
#else
#pragma comment(lib, "../../../depends/dw3rd/gtest-1.5.0/lib/release/gtest.lib")
#endif

