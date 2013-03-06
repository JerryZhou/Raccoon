
TEST(DwAssign, constructor)
{
    DwAssign assign;
    EXPECT_EQ(assign.name(), DwString());
    EXPECT_EQ(assign.path(), DwString());
}

TEST(DwAssign, name)
{
    DwAssign assign(L"name", "");
    EXPECT_EQ(assign.name(), DwString(L"name"));
}

TEST(DwAssign, path)
{
    DwAssign assign(L"name", "path");
    EXPECT_EQ(assign.name(), DwString(L"name"));
    EXPECT_EQ(assign.path(), DwString(L"path"));
}