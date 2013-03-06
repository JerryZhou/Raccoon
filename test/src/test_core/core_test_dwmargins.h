TEST(DwMargins, DwMargins)
{
    DwMargins m(10, 20, 30, 40);

    EXPECT_EQ(m.left(), 10);
    EXPECT_EQ(m.top(), 20);
    EXPECT_EQ(m.right(), 30);
    EXPECT_EQ(m.bottom(), 40);
}