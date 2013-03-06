TEST(DwVersion, construct)
{
    DwVersion v;
    EXPECT_EQ(v.version, 0);
}

TEST(DwVersion, constructInt)
{
    DwVersion v(1);
    EXPECT_EQ(v.version, 1);
}

TEST(DwVersion, constructpart)
{
    DwVersion v(1, 2, 3, 4);
    EXPECT_EQ(v.major, 1);
    EXPECT_EQ(v.minor, 2);
    EXPECT_EQ(v.build, 3);
    EXPECT_EQ(v.revision, 4);
}

TEST(DwVersion, constructstring)
{
    DwVersion v("1.2.3.4");
    EXPECT_EQ(v.major, 1);
    EXPECT_EQ(v.minor, 2);
    EXPECT_EQ(v.build, 3);
    EXPECT_EQ(v.revision, 4);
}

TEST(DwVersion, compare_gt)
{
    DwVersion v0("1.2.3.4");
    DwVersion v1("2.2.3.4");
    DwVersion v2("2.3.3.4");
    DwVersion v3("2.3.4.4");
    DwVersion v4("2.3.4.5");

    DwVersion v5("0.3.4.5");
    
    EXPECT_TRUE( v4 > v3 );
    EXPECT_TRUE( v3 > v2 );
    EXPECT_TRUE( v2 > v1 );
    EXPECT_TRUE( v1 > v0 );

    EXPECT_TRUE( v4 > v5 );
    EXPECT_TRUE( v3 > v5 );
    EXPECT_TRUE( v2 > v5 );
    EXPECT_TRUE( v1 > v5 );
    EXPECT_TRUE( v0 > v5 );
}

TEST(DwVersion, compare_eq)
{
    DwVersion v0("1.2.3.4");
    DwVersion v1(1, 2, 3, 4);
    EXPECT_EQ(v0, v1);
}