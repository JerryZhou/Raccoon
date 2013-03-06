
static void clearTestEnv()
{
    if( !dwAssignLib()->isValid() )
    {
        dwAssignLib()->setup();
    }
    DwUrl configFile("bin:unitTest.ini");
    ::DeleteFile(configFile.localPath().constString());
}

TEST(DwPrivateProfile, exists)
{
    clearTestEnv();
    DwUrl configFile("bin:unitTest.ini");
    bool sucess = DwPrivateProfile::exists(configFile, L"unitTest", L"test");
    EXPECT_TRUE(!sucess);
}

TEST(DwPrivateProfile, writeInt)
{
    DwUrl configFile("bin:unitTest.ini");
    bool sucess = DwPrivateProfile::writeInt(configFile, L"unitTest", L"test", 200);
    EXPECT_TRUE(sucess);
}

TEST(DwPrivateProfile, writeString)
{
    DwUrl configFile("bin:unitTest.ini");
    bool sucess = DwPrivateProfile::writeString(configFile, L"unitTest", L"testStr", L"Raccoon");
    EXPECT_TRUE(sucess);
}

struct GuiTestStruct 
{
    int i;
    char c;
    float f;
};

TEST(DwPrivateProfile, writeStruct)
{
    DwUrl configFile("bin:unitTest.ini");
    GuiTestStruct stuct = { 0, 0, 0.0f };
    bool sucess = DwPrivateProfile::writeStruct(configFile, L"unitTest", L"testStruct", &stuct, sizeof(GuiTestStruct));
    EXPECT_TRUE(sucess);
}

TEST(DwPrivateProfile, readInt)
{
    DwUrl configFile("bin:unitTest.ini");
    int i = DwPrivateProfile::readInt(configFile, L"unitTest", L"test");
    EXPECT_EQ(i, 200);
}

TEST(DwPrivateProfile, readString)
{
    DwUrl configFile("bin:unitTest.ini");
    DwString i = DwPrivateProfile::readString(configFile, L"unitTest", L"testStr");
    EXPECT_EQ(i, DwString(L"Raccoon"));
}

TEST(DwPrivateProfile, readStruct)
{
    DwUrl configFile("bin:unitTest.ini");
    GuiTestStruct stuct = { 20, 20, 20.0f };
    bool sucess = DwPrivateProfile::readStruct(configFile, L"unitTest", L"testStruct", &stuct, sizeof(GuiTestStruct));
    EXPECT_TRUE(sucess);
    EXPECT_EQ(stuct.i, 0);
    EXPECT_EQ(stuct.c, 0);
    EXPECT_EQ(stuct.f, 0.0f);
}