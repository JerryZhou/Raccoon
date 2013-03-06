TEST(DwTextStream, DwTextStream)
{
    DwFile f("../testdata/core_io/ansi.txt");
    EXPECT_TRUE(f.open(DwIODevice::ReadOnly));

    DwTextStream s(&f);
    f.close();
}

TEST(DwTextStream, ansi_text)
{
    DwFile f("../testdata/core_io/ansi.txt");
    EXPECT_TRUE(f.open(DwIODevice::ReadOnly));

    DwTextStream s(&f);

    int lineCount = 0;
    DwString line;
    while(!s.atEnd())
    {
        line = s.readLine();
        if(line.length() > 0)
        {
            lineCount++;
            EXPECT_EQ(line.at(0) - L'0', line.length());
        }
        else
        {
            break;
        }
    }

    EXPECT_EQ(18, lineCount);

    f.close();
}

TEST(DwTextStream, utf8bom_text)
{
    DwFile f("../testdata/core_io/utf8_bom.txt");
    EXPECT_TRUE(f.open(DwIODevice::ReadOnly));

    DwTextStream s(&f);

    int lineCount = 0;
    DwString line;
    while(!s.atEnd())
    {
        line = s.readLine();
        if(line.length() > 0)
        {
            lineCount++;
            EXPECT_EQ(line.at(0) - L'0', line.length());
        }
        else
        {
            break;
        }
    }

    EXPECT_EQ(18, lineCount);

    f.close();
}

TEST(DwTextStream, utf8nobom_text)
{
    DwFile f("../testdata/core_io/utf8_no_bom.txt");
    EXPECT_TRUE(f.open(DwIODevice::ReadOnly));

    DwTextStream s(&f);

    int lineCount = 0;
    DwString line;
    while(!s.atEnd())
    {
        line = s.readLine();
        if(line.length() > 0)
        {
            lineCount++;
            EXPECT_EQ(line.at(0) - L'0', line.length());
        }
        else
        {
            break;
        }
    }

    EXPECT_EQ(18, lineCount);

    f.close();
}

DwString readFileText(const DwString &fileName)
{
    DwFile f(fileName);
    EXPECT_TRUE(f.open(DwIODevice::ReadOnly));

    DwTextStream s(&f);

    DwString text;
    while(!s.atEnd())
    {
        text += s.readLine();
    }

    f.close();

    return text;
}

TEST(DwTextStream, codecs)
{
    DwString t1 = readFileText("../testdata/core_io/gbk.txt");
    DwString t2 = readFileText("../testdata/core_io/unicode.txt");

    EXPECT_TRUE(!t1.isEmpty());
    EXPECT_TRUE(!t2.isEmpty());

    EXPECT_EQ(t1, t2);
}