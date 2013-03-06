TEST(DwSet, DwSet)
{
    DwSet<int> s1;
    s1.insert(100);
    s1.insert(200);
    s1.insert(300);
    s1.insert(200);
    s1.insert(100);

    EXPECT_EQ(3, s1.count());

    DwSet<int> s2 = s1;
    EXPECT_EQ(3, s2.count());

    EXPECT_EQ(600, sumOfContainer(s2));
}

TEST(DwSet, insert_remove)
{
    DwSet<int> s1;
    for(int i = 0; i < 1000; i++)
    {
        s1.insert(dwRand() % 100);
    }

    int sum = sumOfContainer(s1);
    EXPECT_EQ(4950, sum);

    s1.remove(20);
    s1.remove(50);
    s1.remove(80);
    sum = sumOfContainer(s1);
    EXPECT_EQ(4800, sum);

    EXPECT_FALSE(s1.isEmpty());
    s1.clear();
    EXPECT_TRUE(s1.isEmpty());
}

class Foo
{
public:
    Foo(const Foo &other)
    {
        m_number = other.m_number;
        ms_construct_count++;
    }
    Foo(int n)
    {
        ms_construct_count++;
        m_number = n;
    }

    ~Foo()
    {
        ms_destruct_count++;
    }

    bool operator<(const Foo &other) const
    {
        return m_number < other.m_number;
    }

    static int ms_construct_count;
    static int ms_destruct_count;

private:
    int m_number;
};

int Foo::ms_construct_count = 0;
int Foo::ms_destruct_count = 0;
TEST(DwSet, copy_count)
{
    DwSet<Foo> s1;

    for(int i = 0; i < 100; i ++)
    {
        s1.insert(Foo(i % 25));
    }

    EXPECT_EQ(125, Foo::ms_construct_count);

    s1.clear();
    EXPECT_EQ(125, Foo::ms_destruct_count);
}

TEST(DwSet, find)
{
    DwSet<Foo> s1;
    for(int i = 0; i < 100; i ++)
    {
        s1.insert(Foo(i % 25));
    }

    for(int i = 0; i < 25; i++)
    {
        EXPECT_TRUE(s1.contains(i));
    }
}

