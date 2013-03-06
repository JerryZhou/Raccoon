
TEST(DwUrl, constructor)
{
    DwUrl url;
    EXPECT_TRUE(url.isEmpty());
}

TEST(DwUrl, values)
{
    DwUrl url("file://testdata/test.txt");
    EXPECT_FALSE(url.isEmpty());

    EXPECT_EQ(url.scheme(), DwString("file"));
    EXPECT_EQ(url.hostAndLocalPath(), DwString("//testdata/test.txt"));

    url.set("http://user:password@www.myserver.com:8080/index.html#main?query=empty&next=null");
    EXPECT_EQ(url.scheme(), DwString("http"));
    EXPECT_EQ(url.userInfo(), DwString("user:password"));
    EXPECT_EQ(url.host(), DwString("www.myserver.com"));
    EXPECT_EQ(url.port(), DwString("8080"));
    EXPECT_EQ(url.localPath(), DwString("index.html"));
    EXPECT_EQ(url.fragment(), DwString("main"));
    EXPECT_EQ(url.query(), DwString("query=empty&next=null"));
    EXPECT_EQ(url.tail(), DwString("index.html#main?query=empty&next=null"));
    EXPECT_EQ(url.hostAndLocalPath(), DwString("//www.myserver.com/index.html"));
}