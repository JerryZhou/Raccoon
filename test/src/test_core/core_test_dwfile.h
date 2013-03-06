TEST(DwFile, DwFile)
{
    DwFile f1("../testdata/core_io/exist.txt");
    EXPECT_TRUE(f1.open(DwIODevice::ReadOnly));
    f1.close();

    DwObject o;
    DwFile f2("../testdata/core_io/exist.txt", &o);
    EXPECT_TRUE(f2.open(DwIODevice::ReadOnly));
    f2.close();

    DwFile f3(&o);
}

TEST(DwFile, open)
{
    DwFile f1("../testdata/core_io/noexist.txt");
    EXPECT_FALSE(f1.open(DwIODevice::ReadOnly));

    DwFile::remove("../testdata/core_io/writetest.txt");
    DwFile f2("../testdata/core_io/writetest.txt");
    EXPECT_TRUE(f2.open(DwIODevice::WriteOnly));
    f2.write("hello ", 6);
    f2.close();

    DwFile f3("../testdata/core_io/writetest.txt");
    EXPECT_TRUE(f3.open(DwIODevice::OpenMode(DwIODevice::WriteOnly | DwIODevice::Append)));
    f3.write("world", 5);
    f3.close();

    DwFile f4("../testdata/core_io/writetest.txt");
    f4.open(DwIODevice::ReadOnly);
    DwByteArray data = f4.readAll();
    EXPECT_EQ(data, DwByteArray("hello world"));
    f4.close();

    DwFile::remove("../testdata/core_io/writetest.txt");
}

TEST(DwFile, exist)
{
    DwString f1("../testdata/core_io/exist.txt");
    DwString f2("../testdata/core_io/notexist.txt");
    EXPECT_TRUE(DwFile::exist(f1));
    EXPECT_FALSE(DwFile::exist(f2));

    DwString f3(":/layout/test.txt");
    EXPECT_FALSE(DwFile::exist(f3));
}

TEST(DwFile, pos)
{
    DwFile f1("../testdata/core_io/fileseek.txt");
    f1.open(DwIODevice::ReadWrite);

    for(int i = 0; i < 80; i++)
    {
        char c = i + '0';
        f1.write(&c, 1);
    }

    for(int i = 0; i < 80; i++)
    {
        int pos = dwRand() % 80;
        f1.seek(pos);

        char c = 0;
        EXPECT_EQ(f1.read(&c, 1), 1);
        EXPECT_EQ(c, pos + '0');

        EXPECT_EQ(f1.pos(), pos + 1);
    }

    f1.close();

    DwFile::remove("../testdata/core_io/fileseek.txt");
}

TEST(DwFile, size)
{
    DwFile f("../testdata/core_io/filesize.txt");
    f.open(DwIODevice::ReadOnly);

    EXPECT_EQ(11, f.size());

    f.close(); 
}

TEST(DwFile, read)
{
    DwFile f("../testdata/core_io/fileread.txt");
    f.open(DwIODevice::ReadOnly);

    DwByteArray data = f.readAll();
    EXPECT_EQ(DwByteArray("hello world"), data);

    f.close(); 
}

TEST(DwFile, write)
{
    const wchar_t KFileName[] = L"../testdata/core_io/filewrite.txt";
    DwFile f1(KFileName);
    f1.open(DwIODevice::WriteOnly);

    EXPECT_EQ(11, f1.write("hello world", 11));

    f1.close(); 

    DwFile f2(KFileName);

    f2.open(DwIODevice::ReadOnly);
    DwByteArray data = f2.readAll();
    EXPECT_EQ(DwByteArray("hello world"), data);
    f2.close();

    DwFile::remove(KFileName);
}

class DwZipFileEngine : public DwAbstractFileEngine
{
public:
    virtual bool atEnd() const { return false; }
    virtual int64_t pos() const { return 0; }
    virtual int64_t size() const { return 1000; }
    virtual bool open(DwIODevice::OpenMode mode) { return false; }
    virtual bool seek(int64_t pos) { return false; }
    virtual int64_t read(char *data, int64_t size) { return 0; }
    virtual int64_t write(const char *data, int64_t size) {return 0;}
    virtual void close() {}
};

class DwZipFileEngineHandler : public DwAbstractFileEngineHandler
{
    virtual DwAbstractFileEngine *create(const DwString &fileName) const
    {
        if(fileName.endsWith("zip"))
        {
            return new DwZipFileEngine();
        }
        return NULL;
    }
};
TEST(DwFile, customFileEngine)
{
    DwZipFileEngineHandler zipHandler;
    DwFile f("a.zip");

    EXPECT_EQ(f.size(), 1000);
}