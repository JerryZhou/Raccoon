
TEST(DwWin32Registry, exists)
{
    bool isExists = DwWin32Registry::exists(DwWin32Registry::ClassesRoot, L"VisualStudio.c.9.0\\DefaultIcon", L"");
    EXPECT_TRUE(isExists);

    isExists = DwWin32Registry::exists(DwWin32Registry::LocalMachine, L"SYSTEM\\Setup", L"CmdLine");
    EXPECT_TRUE(isExists);
}

TEST(DwWin32Registry, writestring)
{
    bool susscess = DwWin32Registry::writeString(DwWin32Registry::CurrentUser, L"SoftWare\\duowan\\raccoon", L"unitTest", L"Foundation");
    EXPECT_TRUE(susscess);
}

TEST(DwWin32Registry, writeint)
{
    bool susscess = DwWin32Registry::writeInt(DwWin32Registry::CurrentUser, L"SoftWare\\duowan\\raccoon", L"unitTest", 300);
    EXPECT_TRUE(susscess);
}

TEST(DwWin32Registry, readstring)
{
    DwWin32Registry::writeString(DwWin32Registry::CurrentUser, L"SoftWare\\duowan\\raccoon", L"unitTest", L"Foundation");
    DwString v = DwWin32Registry::readString(DwWin32Registry::CurrentUser, L"SoftWare\\duowan\\raccoon", L"unitTest");
    EXPECT_EQ(v, L"Foundation");
}

TEST(DwWin32Registry, readint)
{
    bool susscess = DwWin32Registry::writeInt(DwWin32Registry::CurrentUser, L"SoftWare\\duowan\\raccoon", L"unitTest", 200);
    EXPECT_TRUE(susscess);
    int v = DwWin32Registry::readInt(DwWin32Registry::CurrentUser, L"SoftWare\\duowan\\raccoon", L"unitTest");
    EXPECT_EQ(v, 200);
}

TEST(DwWin32Registry, deletekey)
{
    bool susscess = DwWin32Registry::writeInt(DwWin32Registry::CurrentUser, L"SoftWare\\duowan\\raccoon", L"unitTest", 200);
    EXPECT_TRUE(susscess);
    susscess = DwWin32Registry::deleteKey(DwWin32Registry::CurrentUser, L"SoftWare\\duowan\\raccoon");
    EXPECT_TRUE(susscess);
}

TEST(DwWin32Registry, asrootkey)
{
    EXPECT_EQ(DwWin32Registry::asRootKey(L"HKEY_CLASSES_ROOT"), DwWin32Registry::ClassesRoot);
    EXPECT_EQ(DwWin32Registry::asRootKey(L"HKEY_CURRENT_USER"), DwWin32Registry::CurrentUser);
    EXPECT_EQ(DwWin32Registry::asRootKey(L"HKEY_LOCAL_MACHINE"), DwWin32Registry::LocalMachine);
    EXPECT_EQ(DwWin32Registry::asRootKey(L"HKEY_USERS"), DwWin32Registry::Users);
}