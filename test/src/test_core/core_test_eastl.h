#include <map>
#include <eastl/utility.h>
const int KCount = 100000;

TEST(STL, rand)
{
    int count = KCount;
    while(count --)
    {
        dwRand();
    }
}

TEST(STL, eastl)
{
    eastl::map<int, DwString> s;
    int count = KCount;
    while(count --)
    {
        s.insert(eastl::pair<int, DwString>(dwRand(), DwString::number(count)));
    }
}

TEST(STL, stl)
{
    std::map<int, DwString> s;
    int count = KCount;
    while(count --)
    {
        s.insert(std::pair<int, DwString>(dwRand(), DwString::number(count)));
    }
}