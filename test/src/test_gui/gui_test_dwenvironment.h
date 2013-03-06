
TEST(DwEnviroment, exists)
{
    bool isPathExists = DwEnviroment::exists(L"path");
    EXPECT_TRUE(isPathExists);
}

TEST(DwEnviroment, read)
{
    DwString path = DwEnviroment::read(L"path");
    EXPECT_TRUE(!path.isEmpty());
}