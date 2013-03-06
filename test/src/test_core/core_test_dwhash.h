TEST(DwHash, DwHash)
{
    DwHash<int, DwString> m;
    EXPECT_TRUE(m.isEmpty());

    for(int i = 0; i < 10000; i++)
    {
        m[i] = DwString::number(i, 16);
    }

    EXPECT_FALSE(m.isEmpty());
    EXPECT_EQ(10000, m.count());
}

TEST(DwHash, iterator)
{
    DwHash<int, int> m;

    int sum1 = 0;
    for(int i = 0; i < 100; i++)
    {
        int n = dwRand();
        sum1 += n;
        m[i] = n;
    }

    int sum2 = 0;
    for(DwHash<int, int>::const_iterator it = m.begin(); it != m.end(); ++it)
    {
        sum2 += it->second;
    }

    EXPECT_EQ(sum1, sum2);
}

TEST(DwHash, find)
{
    DwHash<int, DwString> m;
    m[300] = "ccc";
    m[100] = "aaa";
    m[200] = "bbb";

    DwHash<int, DwString>::const_iterator it = m.find(100);

    EXPECT_TRUE(it != m.end());

    EXPECT_EQ(DwString("aaa"), it->second);

    EXPECT_EQ(DwString("bbb"), m[200]);
    EXPECT_EQ(DwString("ccc"), m.value(300));
}

TEST(DwHash, insert_remove)
{
    DwHash<int, int> m;
    m[300] = 10;
    m[100] = 20;
    m[200] = 30;

    m.insert(150, 40);
    m.remove(200);

    DwString s;
    int keySum = 0;
    int valueSum = 0;
    for(DwHash<int, int>::const_iterator it = m.begin(); it != m.end(); ++it)
    {
        keySum += it->first;
        valueSum += it->second;        
    }

    EXPECT_EQ(100 + 150 + 300, keySum);
    EXPECT_EQ(10 + 20 + 40, valueSum);
}