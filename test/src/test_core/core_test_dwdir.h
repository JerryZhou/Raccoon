TEST(DwDir, appDir)
{
    DwString dir = DwDir::applicationDir();

    EXPECT_TRUE(DwFile::exist(dir));
}

TEST(DwDir, absolutePath)
{
	DwDir dir("../testdata/core_json/");

	DwString path = dir.absolutePath();
	EXPECT_TRUE(DwFile::exist(path));

	DwDir dir2("c:\\Program Files");

	EXPECT_TRUE(DwFile::exist(dir2.path()));
}

TEST(DwDir, pathFromFilePath)
{
	DwDir dir("../testdata/core_json/");

	DwString path = dir.absolutePath();
	EXPECT_TRUE(DwFile::exist(path));

	DwDir dir2("c:\\Program Files");

	EXPECT_TRUE(DwFile::exist(dir2.path()));
}

TEST(DwDir, cd)
{
    DwDir dir(DwDir::applicationDir());
    dir.cdUp();

    DwString path = DwDir::pathAppend(dir.path(), "testdata");

    EXPECT_TRUE(DwFile::exist(path));

    dir.cd("testdata/core_io");

    EXPECT_TRUE(DwFile::exist(dir.path()));

    EXPECT_FALSE(dir.cd(""));

    DwDir root("D:");
    EXPECT_FALSE(root.cd(".."));

    EXPECT_FALSE(root.cd("12312312312"));

    DwDir rdir("..\\testdata");
    EXPECT_TRUE(rdir.cd("../testdata"));
}
