#pragma once

#include "gtest/gtest.h"
#ifdef _DEBUG
#pragma comment(lib, "../../../depends/dw3rd/gtest-1.5.0/lib/debug/gtestd.lib")
#else
#pragma comment(lib, "../../../depends/dw3rd/gtest-1.5.0/lib/release/gtest.lib")
#endif

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

inline void PrintTo(const DwPoint& p, ::std::ostream* os) 
{
	char info[64];
	sprintf_s(info, 64, "DwPoint(%d, %d)", p.x(), p.y());
	testing::internal::PrintTo(info, os);
}

inline void PrintTo(const DwPointF& p, ::std::ostream* os) 
{
	char info[64];
	sprintf_s(info, 64, "DwPointF(%.3f, %.3f)", p.x(), p.y());
	testing::internal::PrintTo(info, os);
}

inline void PrintTo(const DwSize& s, ::std::ostream* os) 
{
	char info[64];
	sprintf_s(info, 64, "DwSizeF(%d, %d)", s.width(), s.height());
	testing::internal::PrintTo(info, os);
}

inline void PrintTo(const DwSizeF& s, ::std::ostream* os) 
{
	char info[64];
	sprintf_s(info, 64, "DwSizeF(%.3f, %.3f)", s.width(), s.height());
	testing::internal::PrintTo(info, os);
}

inline void PrintTo(const DwRect& r, ::std::ostream* os) 
{
	char info[128];
	sprintf_s(info, 128, "DwRectF(%d, %d, %d, %d)", r.x(), r.y(), r.width(), r.height());
	testing::internal::PrintTo(info, os);
}

inline void PrintTo(const DwRectF& r, ::std::ostream* os) 
{
	char info[128];
	sprintf_s(info, 128, "DwRectF(%.3f, %.3f, %.3f, %.3f)", r.x(), r.y(), r.width(), r.height());
	testing::internal::PrintTo(info, os);
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
