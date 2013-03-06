TEST(DwByteArray, DwByteArray)
{
    DwByteArray s0;
    EXPECT_TRUE(s0.isNull());

    DwByteArray s1("ascii");

    EXPECT_EQ(DwByteArray("ascii"), s1);

	DwByteArray s2((char*)NULL, 0);
	EXPECT_TRUE(s2.isNull());
	DwByteArray s3("test", 4);
	EXPECT_EQ(DwByteArray("test"), s3);

	DwByteArray s4('a', 1);
	for(int i = 0; i < s4.count(); i++)
	{
		EXPECT_EQ(s4.at(i), 'a');
	}
 }

TEST(DwByteArray, append)
{
    DwByteArray s1("111");
    s1.append("222");
    EXPECT_EQ(6, s1.count());

    s1.append("333");
    EXPECT_EQ(9, s1.count());

    s1.append(DwByteArray("444"));
    EXPECT_EQ(12, s1.count());

    EXPECT_EQ(DwByteArray("111222333444"), s1);

	DwByteArray s2(s1);
	s2.append('5');
	EXPECT_EQ(DwByteArray("1112223334445"), s2);
	s2.append('5');
	EXPECT_EQ(DwByteArray("11122233344455"), s2);

	DwByteArray s3;
	s3.append(s2);
	EXPECT_EQ(DwByteArray("11122233344455"), s3);

	s3.append("55", -1);
	EXPECT_EQ(DwByteArray("1112223334445555"), s3);
}


TEST(DwByteArray, prepend)
{
	DwByteArray s1("111");
	s1.prepend("222");
	EXPECT_EQ(6, s1.count());;

	EXPECT_EQ(DwByteArray("222111"), s1);

	DwByteArray s2(s1);
	s2.prepend(DwByteArray("333"));
	EXPECT_EQ(DwByteArray("333222111"), s2);
	
	s2.prepend(DwByteArray("444"));
	EXPECT_EQ(DwByteArray("444333222111"), s2);

	DwByteArray s3;
	s3.prepend(s2);
	EXPECT_EQ(DwByteArray("444333222111"), s3);

	s3.prepend("555", 3);
	EXPECT_EQ(DwByteArray("555444333222111"), s3);

	s3.prepend('6');
	EXPECT_EQ(DwByteArray("6555444333222111"), s3);
}


TEST(DwByteArray, set)
{
    DwByteArray s1("123456");
    EXPECT_EQ('4', s1.at(3));

    s1.clear();
    EXPECT_EQ(0, s1.count());
}

TEST(DwByteArray, find)
{
    DwByteArray s1("12345678");

    EXPECT_TRUE(s1.contains('5'));
    EXPECT_FALSE(s1.contains('9'));
    EXPECT_TRUE(s1.contains("3456"));
    EXPECT_FALSE(s1.contains("2456"));

    EXPECT_EQ(3, s1.indexOf('4'));
    EXPECT_EQ(-1, s1.indexOf('9'));
    EXPECT_EQ(3, s1.indexOf("456"));
    EXPECT_EQ(-1, s1.indexOf("4656"));
	EXPECT_EQ(-1, s1.indexOf("4656", -1));
	EXPECT_EQ(-1, s1.indexOf('9', -1));

	EXPECT_TRUE(s1.endsWith("678"));
	EXPECT_TRUE(s1.endsWith("12345678"));
	EXPECT_TRUE(s1.endsWith(DwByteArray("678")));
	EXPECT_TRUE(s1.endsWith("8"));
	EXPECT_FALSE(s1.endsWith("6789"));

	EXPECT_TRUE(s1.endsWith(""));
	EXPECT_TRUE(DwByteArray("").endsWith(""));
	EXPECT_FALSE(DwByteArray().endsWith('c'));
	EXPECT_FALSE(s1.endsWith("1111111111111111111"));
	EXPECT_FALSE(s1.endsWith('1'));

	EXPECT_FALSE(s1.endsWith(DwByteArray("1111111111111111111")));
	EXPECT_TRUE(s1.endsWith(DwByteArray()));

	DwByteArray s2("123456789");
	EXPECT_EQ(8, s2.lastIndexOf('9'));
	EXPECT_EQ(8, s2.lastIndexOf('9', 9));
	EXPECT_EQ(-1, s2.lastIndexOf('9', 7));

	EXPECT_EQ(8, s2.lastIndexOf("9"));
	EXPECT_EQ(8, s2.lastIndexOf("9", 9));
	EXPECT_EQ(-1, s2.lastIndexOf("9", 7));
	EXPECT_EQ(-1, s2.lastIndexOf("0", 9));

	EXPECT_EQ(8, s2.lastIndexOf(DwByteArray("9")));
	EXPECT_EQ(8, s2.lastIndexOf(DwByteArray("9"), 9));
	EXPECT_EQ(-1, s2.lastIndexOf(DwByteArray("9"), 7));
	EXPECT_EQ(-1, s2.lastIndexOf(DwByteArray("0"), 9));

	EXPECT_EQ(7, s2.lastIndexOf("89"));
	EXPECT_EQ(7, s2.lastIndexOf("89", 9));
	EXPECT_EQ(-1, s2.lastIndexOf("89", 6));
	EXPECT_EQ(-1, s2.lastIndexOf("79", 9));

	EXPECT_EQ(7, s2.lastIndexOf(DwByteArray("89")));
	EXPECT_EQ(7, s2.lastIndexOf(DwByteArray("89"), 8));
	EXPECT_EQ(-1, s2.lastIndexOf(DwByteArray("89"), 6));
	EXPECT_EQ(-1, s2.lastIndexOf(DwByteArray("79"), 9));
}

TEST(DwByteArray, insertdelete)
{
    DwByteArray s1("12345678");
    EXPECT_EQ(DwByteArray("1234abc5678"), s1.insert(4, "abc"));
    EXPECT_EQ(DwByteArray("1234abcd5678"), s1.insert(7, "d"));
    EXPECT_EQ(DwByteArray("12345678"), s1.remove(4, 4));
	EXPECT_EQ(DwByteArray("102345678"), s1.insert(1, '0'));
	EXPECT_EQ(DwByteArray("101232345678"), s1.insert(2, "123", 3));
	EXPECT_EQ(DwByteArray("101456232345678"), s1.insert(3, DwByteArray("456")));
	EXPECT_EQ(DwByteArray("1014562"), s1.remove(7, 20));
}

TEST(DwByteArray, operators)
{
    DwByteArray s1("12345");
    DwByteArray s2("abcde");

    EXPECT_EQ(DwByteArray("123456"), s1 += "6");
    EXPECT_EQ(DwByteArray("1234567"), s1 += DwByteArray("7"));

    EXPECT_EQ(DwByteArray("abcde1"), s2 + '1');
    EXPECT_EQ(DwByteArray("abcde2"), s2 + "2");
    EXPECT_EQ(DwByteArray("abcde3"), s2 + DwByteArray("3"));

    EXPECT_EQ('3', s1[2]);

	DwByteArray s3("abc");
	s3 = s1;
	EXPECT_EQ(s1, s3);
	s3 = "123456";
	EXPECT_EQ(DwByteArray("123456"), s3);

	DwByteRef s3ref2 = s3[2];
	EXPECT_EQ('3', s3ref2);
	s3ref2 = s3[3u];
	EXPECT_EQ(DwByteArray("124456"), s3);

	DwByteRef s3ref4 = s3[4u];
	EXPECT_EQ('5', s3ref4);

	DwByteRef s3ref5 = s3[5];
	EXPECT_TRUE(s3ref5 == '6');
	EXPECT_TRUE(s3ref5 != '7');
	EXPECT_TRUE(s3ref5 > '5');
	EXPECT_TRUE(s3ref5 < '7');
	EXPECT_TRUE(s3ref5 >= '5');
	EXPECT_TRUE(s3ref5 <= '7');
	s3.remove(4,10);
	EXPECT_EQ(DwByteArray("1244"), s3);
	s3ref5 = '6';
	EXPECT_TRUE(s3ref5 == '6');
	EXPECT_TRUE(s3ref5 != '7');
	EXPECT_TRUE(s3ref5 > '5');
	EXPECT_TRUE(s3ref5 < '7');
	EXPECT_TRUE(s3ref5 >= '5');
	EXPECT_TRUE(s3ref5 <= '7');
	EXPECT_EQ(6, s3.count());

	const DwByteArray s4(s3);
	char ch1 = s4.operator[](1);
	char ch1u = s4.operator[](1u);
	EXPECT_EQ('2', ch1);
	EXPECT_EQ('2', ch1u);
}

TEST(DwByteArray, transforms)
{
	DwByteArray s1("12345");
	DwByteArray s2("abcde");

	DwByteArray hex1 = s1.toHex();
	DwByteArray hex2 = s2.toHex();
	EXPECT_EQ(hex1, "3132333435");
	EXPECT_EQ(hex2, "6162636465");

	EXPECT_EQ(s1, DwByteArray::fromHex(hex1));
	EXPECT_EQ(s2, DwByteArray::fromHex(hex2));

	DwByteArray s3 = DwByteArray::fromHex("abcdABCD1234+-/*");
	EXPECT_EQ("«Í«Í4", s3);
}

TEST(DwByteArray, crc32)
{
    DwByteArray data1 = "1234567890";
    DwByteArray data2 = "abcdefghijklmnopqrstuvwxyx";
    DwByteArray data3 = "qwertyuiopasdfghjklzxcvbnm";

    for(int i = 0; i < 100; i++)
    {
        EXPECT_EQ(0x261daee5, data1.crc32());
        EXPECT_EQ(0xa2293191, data2.crc32());
        EXPECT_EQ(0xc06b40d9, data3.crc32());
    }
}

TEST(DwByteArray, compare)
{
	DwByteArray s3("abc");
	DwByteArray s4("def");
	EXPECT_TRUE(s3 < s4);
	EXPECT_TRUE(s3 <= s4);
	EXPECT_TRUE(s3 == "abc");
	EXPECT_TRUE(DwByteArray("abc") == s3);
	EXPECT_TRUE(s3 == DwByteArray("abc"));
	EXPECT_FALSE(s3 > s4);
	EXPECT_FALSE(s3 >= s4);


	DwByteArray s5("123");
	EXPECT_TRUE(s5 == "123");
	EXPECT_TRUE("123" == s5);
	EXPECT_FALSE(s5 == "");
	EXPECT_FALSE("" == s5);
}

TEST(DwByteArray, resize)
{
	DwByteArray s1;
	s1.resize(-1);
	EXPECT_TRUE(s1.isEmpty());
}

TEST(DwByteArray, split)
{
	DwByteArray s1("12345678");
	EXPECT_EQ(DwByteArray("123"), s1.left(3));
	EXPECT_EQ(DwByteArray("678"), s1.right(3));
	EXPECT_EQ(DwByteArray("456"), s1.mid(3, 3));

	EXPECT_EQ(DwByteArray(), s1.left(-1));
	EXPECT_EQ(s1, s1.left(10));
	EXPECT_EQ(s1, s1.right(10));
	EXPECT_EQ(DwByteArray(), s1.right(-1));
	EXPECT_EQ(s1, s1.mid(0, 8));
	EXPECT_EQ(DwByteArray("5678"), s1.mid(4, -1));
	EXPECT_EQ(DwByteArray(), s1.mid(10, -1));
	EXPECT_EQ(s1, s1.mid(-1, -1));
}