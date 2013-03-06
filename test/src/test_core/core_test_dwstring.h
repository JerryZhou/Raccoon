TEST(DwString, DwString)
{
    DwString s0;
    EXPECT_TRUE(s0.isNull());

    DwString s1(L'C');
    DwString s2("ascii");
    DwString s3(L"unicode");

    EXPECT_EQ(DwString(L"ascii"), s2);
    EXPECT_EQ(DwString("unicode"), s3);

    DwString s4 = DwString::fromUtf8("unicode");
    EXPECT_EQ(DwString("unicode"), s4);
	DwByteArray arr1 = s4.toUtf8();
	EXPECT_TRUE(!arr1.isNull());
	DwString s5 = DwString::fromUtf8(arr1.data());
	EXPECT_EQ(s4, s5);
}

TEST(DwString, append)
{
    DwString s1("111");
    s1.append("222");
    EXPECT_EQ(6, s1.length());
    EXPECT_EQ(6, s1.size());
    
    s1.append(L"333");
    EXPECT_EQ(9, s1.length());
    EXPECT_EQ(9, s1.size());

    s1.append(DwString(L"444"));
    EXPECT_EQ(12, s1.length());
    EXPECT_EQ(12, s1.size());
    
    EXPECT_EQ(DwString("111222333444"), s1);

	DwFile bigFile("../testdata/core_zip/readme.txt");
	if (bigFile.open(DwFile::ReadOnly))
	{
		DwByteArray fileData = bigFile.readAll();
		bigFile.close();
		DwString s2;
		s2.append(fileData.data());
		EXPECT_TRUE(s2.size() > 4096);
	}

}

TEST(DwString, set)
{
    DwString s1("123456");
    EXPECT_EQ(L'4', s1.at(3));

    s1.clear();
    EXPECT_EQ(0, s1.length());
}

TEST(DwString, format)
{
    EXPECT_EQ(DwString(L"1234"), DwString::number(1234));
    EXPECT_EQ(DwString(L"4d2"), DwString::number(1234, 16));
}

TEST(DwString, compare)
{
    DwString s1("12345");

    EXPECT_TRUE(s1 == "12345");
    EXPECT_TRUE(s1 == L"12345");
    EXPECT_TRUE(s1 == DwString("12345"));
    EXPECT_FALSE(s1 == "123456");
	EXPECT_FALSE(s1 != "12345");
    EXPECT_FALSE(s1 != L"12345");
    EXPECT_FALSE(s1 != DwString("12345"));

    DwString s2("22222");

    EXPECT_TRUE(s2 > "22221");
    EXPECT_TRUE(s2 > "12222");
    EXPECT_TRUE(s2 > "22122");
    EXPECT_TRUE(s2 > "2222");

    EXPECT_TRUE(s2 < "22223");
    EXPECT_TRUE(s2 < "32222");
    EXPECT_TRUE(s2 < "22322");
    EXPECT_TRUE(s2 < "222222");

    EXPECT_TRUE(s2 <= "22222");
    EXPECT_TRUE(s2 <= "22223");
    EXPECT_TRUE(s2 >= "22222");
    EXPECT_TRUE(s2 >= "22221");
}

TEST(DwString, compareNoCase)
{
    DwString s1("BBBBB");

    EXPECT_TRUE(s1.compareNoCase("bBBBB") == 0);
    EXPECT_TRUE(s1.compareNoCase("BBbBB") == 0);
    EXPECT_TRUE(s1.compareNoCase("BBBBb") == 0);

    EXPECT_TRUE(s1.compareNoCase("aBBBB") > 0);
    EXPECT_TRUE(s1.compareNoCase("bBBBc") < 0);
    EXPECT_TRUE(s1.compareNoCase("aBBBBccc") > 0);
    EXPECT_TRUE(s1.compareNoCase("bBBBbccc") < 0);
}

TEST(DwString, find)
{
    DwString s1("12345678");

    EXPECT_TRUE(s1.contains('5'));
    EXPECT_FALSE(s1.contains('9'));
    EXPECT_TRUE(s1.contains("3456"));
    EXPECT_FALSE(s1.contains("2456"));

    EXPECT_EQ(3, s1.indexOf('4'));
    EXPECT_EQ(-1, s1.indexOf('9'));
    EXPECT_EQ(3, s1.indexOf("456"));
    EXPECT_EQ(-1, s1.indexOf("4656"));
	EXPECT_EQ(-1, s1.indexOf("456", 10));

    DwString s2("123451234");
    EXPECT_EQ(6, s2.lastIndexOf('2'));
    EXPECT_EQ(5, s2.lastIndexOf("1234"));
    EXPECT_EQ(6, s2.lastIndexOf("2", -3));
    EXPECT_EQ(1, s2.lastIndexOf("2", -4));

    EXPECT_TRUE(s1.startsWith('1'));
    EXPECT_TRUE(s1.endsWith('8'));
    EXPECT_TRUE(s1.startsWith("123"));
    EXPECT_TRUE(s1.endsWith("678"));

    DwString s3;
    EXPECT_TRUE(s3.startsWith('\0'));
    EXPECT_TRUE(s3.endsWith('\0'));
	DwString s4;
	EXPECT_TRUE(s3.endsWith(s4));
	EXPECT_FALSE(s3.endsWith(DwString("123")));

	DwString s5("1234");
	EXPECT_FALSE(DwString("").endsWith(s5));
	EXPECT_FALSE(s5.endsWith(DwString("12345")));
}

TEST(DwString, substring)
{
    DwString s1("12345678");
    EXPECT_EQ(DwString("123"), s1.left(3));
    EXPECT_EQ(DwString("678"), s1.right(3));
    EXPECT_EQ(DwString("456"), s1.mid(3, 3));

	EXPECT_EQ(s1, s1.left(-1));
	EXPECT_EQ(s1, s1.left(10));
	EXPECT_EQ(s1, s1.right(10));
	EXPECT_EQ(s1, s1.right(-1));
	EXPECT_EQ(s1, s1.mid(0, 8));
	EXPECT_EQ(DwString("5678"), s1.mid(4, -1));
	EXPECT_EQ(DwString(), s1.mid(10, -1));
	EXPECT_EQ(s1, s1.mid(-1, -1));
}

TEST(DwString, insertdelete)
{
    DwString s1("12345678");
    EXPECT_EQ(DwString("1234a5678"), s1.insert(4, L'a'));
    EXPECT_EQ(DwString("1234abc5678"), s1.insert(5, L"bc", 2));
    EXPECT_EQ(DwString("1234abcd5678"), s1.insert(7, DwString("d")));
    EXPECT_EQ(DwString("12345678"), s1.remove(4, 4));

	EXPECT_EQ(s1, s1.insert(-10, L'0'));
	EXPECT_EQ(DwString("123456708"), s1.insert(-1, L'0'));	

	DwString s2("abc");
	EXPECT_EQ(s2, s2.insert(-1, L"123", 3));
	EXPECT_EQ(DwString("abc123"), s2.insert(3, L"123", 3));
	EXPECT_EQ(DwString("abc123123"), s2.insert(3, L"123", 3));
	EXPECT_EQ(DwString("abc12312"), s2.remove(-1, 2));
}

TEST(DwString, operators)
{
    DwString s1("12345");
    DwString s2("abcde");

    EXPECT_EQ(DwString("123456"), s1 += L'6');
    EXPECT_EQ(DwString("1234567"), s1 += "7");
    EXPECT_EQ(DwString("12345678"), s1 += L"8");
    EXPECT_EQ(DwString("123456789"), s1 += DwString("9"));

    EXPECT_EQ(DwString("abcde1"), s2 + '1');
    EXPECT_EQ(DwString("abcde2"), s2 + L'2');
    EXPECT_EQ(DwString("abcde3"), s2 + "3");
    EXPECT_EQ(DwString("abcde4"), s2 + L"4");
    EXPECT_EQ(DwString("abcde5"), s2 + DwString("5"));
}

TEST(DwString, replace)
{
    DwString s1("12345678901234567890");

    EXPECT_EQ(DwString("123abc7890123abc7890"), s1.replace("456", "abc"));

    DwString s2("11111111111111111");
    EXPECT_EQ(DwString("22222222222222222"), s2.replace("1", "2"));

    DwString s3("1234567890");
    EXPECT_EQ(DwString("1234567890"), s3.replace("a", "b"));
}

TEST(DwString, replaceNoCase)
{
    DwString s1("aBcDeFg");

    EXPECT_EQ(DwString("aB123Fg"), s1.replaceNoCase("cde", "123"));
}

TEST(DwString, toSomething)
{
    DwString s1 = DwString::number(1234567);

    EXPECT_EQ(1234567, s1.toInt32());
    EXPECT_EQ(1234567, s1.toUInt32());

    DwString s2("Hello World!");
    EXPECT_EQ(DwString("hello world!"), s2.toLower());
    EXPECT_EQ(DwString("HELLO WORLD!"), s2.toUpper());

	DwString s3("123a");
	bool ok = true;
	EXPECT_EQ(0, s3.toInt32(&ok));
	EXPECT_FALSE(ok);
	ok = true;
	EXPECT_EQ(0, s3.toUInt32(&ok));
	EXPECT_FALSE(ok);
	s3.remove(3, 1);
	EXPECT_EQ(123, s3.toUInt32(&ok));
	EXPECT_TRUE(ok);
}

TEST(DwString, others)
{
	DwString s1("123");
	s1.resize(-1);
	EXPECT_EQ(0, s1.size());
}