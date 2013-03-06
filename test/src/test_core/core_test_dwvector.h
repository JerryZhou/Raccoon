DwVector<int> createVector(int count)
{
    DwVector<int> v1;
    for(int i = 0; i < count; i++)
    {
        v1.append( i );
    }
    return v1;
}

TEST(DwVector, DwVector)
{
    DwVector<int> v1( createVector(100) );
    int sum1 = sumOfContainer(v1);
    EXPECT_EQ(100 * 99 / 2, sum1);

    EXPECT_EQ(0, v1.front());
    v1.pop_front();

    EXPECT_EQ(99, v1.back());
    v1.pop_back();

    int sum2 = sumOfContainer(v1);
    EXPECT_EQ(100 * 99 / 2 - 99, sum2);
}

TEST(DwVector, append)
{
    DwVector<int> v;
    for(int i = 0; i < 1000; i++)
    {
        v.append(i);
    }

    int sum = 0;
    for(int i = 0; i < v.count(); i++)
    {
        sum += v[i];
    }
    
    EXPECT_EQ(999 * 500, sum);

    v.clear();

    EXPECT_EQ(0, v.count());
}

TEST(DwVector, insert)
{
    DwVector<int> v1;
    v1.append(10);
    v1.append(20);
    v1.append(30);
    v1.insert(0, 9);
    v1.insert(2, 19);
    v1.insert(4, 29);

    EXPECT_EQ(6, v1.count());
    EXPECT_EQ(9 + 10 + 19 + 20 + 29 + 30, sumOfContainer(v1));
}

TEST(DwVector, remove)
{
    DwVector<int> v;
    for(int i = 0; i < 1000; i++)
    {
        v.append(i);
    }

    int i = v.indexOf(50);
    v.remove(i);

    v.removeOne(100);

    int sum = 0;
    for(int i = 0; i < v.count(); i++)
    {
        sum += v[i];
    }

    EXPECT_EQ(998, v.count());
    EXPECT_EQ(999 * 500 - 50 - 100, sum);
}

TEST(DwVector, iterator)
{
    DwVector<int> v1;
    for(int i = 0; i < 1000; i++)
    {
        v1.append(i);
    }

    int sum = 0;
    for(DwVector<int>::const_iterator it = v1.begin(); it != v1.end(); ++it)
    {
        sum += *it;
    }
    EXPECT_EQ(999 * 500, sum);

    DwVector<DwString> v2;
    v2.append("123");
    v2.append("456");
    v2.append("789");

    v2.erase(v2.begin() + 1);

    DwString s1;
    for(DwVector<DwString>::const_iterator it = v2.begin(); it != v2.end(); ++it)
    {
        s1 += *it;
    }
    EXPECT_EQ(123789, s1.toInt32());
}

#include <algorithm>
TEST(DwVector, performance)
{
    const int KCount = 10000;
    DwVector<int> v;
    for(int i = 0; i < KCount; i++)
    {
        v.push_back(dwRand());
    }

    for(int i = 0; i < KCount; i++)
    {
        v.insert(v.begin() + dwRand() % v.size(), dwRand());
        v.erase(v.begin() + dwRand() % v.size());
    }

    std::sort(v.begin(), v.end());
    for(int i = 1; i < v.count(); i++)
    {
        EXPECT_TRUE(v[i] >= v[i - 1]);
    }
}
