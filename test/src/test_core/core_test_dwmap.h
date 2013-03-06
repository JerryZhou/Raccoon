TEST(DwMap, DwMap)
{
    DwMap<int, DwString> m;
    EXPECT_TRUE(m.isEmpty());

    for(int i = 0; i < 10000; i++)
    {
        m[i] = DwString::number(i, 16);
    }

    EXPECT_FALSE(m.isEmpty());
    EXPECT_EQ(10000, m.count());
}

TEST(DwMap, iterator)
{
    DwMap<int, DwString> m;
    m[100] = "aaa";
    m[200] = "bbb";
    m[300] = "ccc";

    DwString s;
    for(DwMap<int, DwString>::const_iterator it = m.begin(); it != m.end(); ++it)
    {
        s += it->second;        
    }

    EXPECT_EQ(DwString("aaabbbccc"), s);
}

TEST(DwMap, find)
{
    DwMap<int, DwString> m;
    m[300] = "ccc";
    m[100] = "aaa";
    m[200] = "bbb";

    m.insert(400, "sdfs");

    DwMap<int, DwString>::const_iterator it = m.find(100);

    EXPECT_TRUE(it != m.end());

    EXPECT_EQ(DwString("aaa"), it->second);

    EXPECT_EQ(DwString("bbb"), m[200]);
    EXPECT_EQ(DwString("ccc"), m.value(300));
}

TEST(DwMap, insert_remove)
{
    DwMap<int, DwString> m;
    m[300] = "ccc";
    m[100] = "aaa";
    m[200] = "bbb";

    m.insert(150, "111");
    m.remove(200);

    DwString s;
    int keySum = 0;
    for(DwMap<int, DwString>::const_iterator it = m.begin(); it != m.end(); ++it)
    {
        keySum += it->first;
        s += it->second;        
    }

    EXPECT_EQ(100 + 150 + 300, keySum);
    EXPECT_EQ(DwString("aaa111ccc"), s);
}