TEST(DwPluginVersion, construct)
{
    DwPluginVersion v;
}

TEST(DwPluginVersion, constructv)
{
    DwVersion v("1.0.0.0");
    DwPluginVersion pinv(v);
    EXPECT_EQ(pinv.major, 1);
}

TEST(DwPluginVersion, iscompatiblewith)
{
    DwVersion v("1.0.0.0");
    DwPluginVersion pinv(v);
    EXPECT_TRUE(pinv.isCompatibleWith(v));
    DwPluginVersion pinv2(DwVersion("1.1.0.0"));
    EXPECT_FALSE(pinv.isCompatibleWith(pinv2));
}