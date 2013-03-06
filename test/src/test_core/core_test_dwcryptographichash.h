TEST(DwCryptographicHash, DwCryptographicHash)
{
	DwByteArray str1 = "HelloYY";
	DwByteArray str2 = "abc";

	//From: http://www.md5.com.cn/hash
	DwByteArray helloYYMd5Hex	= "81ef2e4d53e506e8c14dd0e863cdde29";
	DwByteArray abcMd5Hex		= "900150983cd24fb0d6963f7d28e17f72";
	DwByteArray	abcMd4Hex		= "a448017aaf21d8525fc10ae87aa6729d";
	DwByteArray	abcSha1Hex		= "a9993e364706816aba3e25717850c26c9cd0d89d";

	DwCryptographicHash hashMd5(DwCryptographicHash::Md5);
	hashMd5.addData(str1);
	DwByteArray data1 = hashMd5.result();
	DwByteArray data1Copy = hashMd5.result();
	DwByteArray data2 = DwCryptographicHash::hash(str1, DwCryptographicHash::Md5);

	EXPECT_EQ(data1, data2);
	EXPECT_EQ(helloYYMd5Hex, data1.toHex());

	EXPECT_EQ(abcMd5Hex, DwCryptographicHash::hash(str2, DwCryptographicHash::Md5).toHex());
	
	DwCryptographicHash hashMd4(DwCryptographicHash::Md4);
	hashMd4.addData(str2);
	DwByteArray data3 = hashMd4.result();
	DwByteArray data4 = DwCryptographicHash::hash(str2, DwCryptographicHash::Md4);
	
	EXPECT_EQ(data3, data4);
	EXPECT_EQ(abcMd4Hex, data3.toHex());

	DwCryptographicHash hashSha1(DwCryptographicHash::Sha1);
	hashSha1.addData(str2);
	DwByteArray data5 = hashSha1.result();
	DwByteArray data6 = DwCryptographicHash::hash(str2, DwCryptographicHash::Sha1);

	EXPECT_EQ(data5, data6);
	EXPECT_EQ(abcSha1Hex, data5.toHex());

	DwByteArray fileMd5Hex;
	DwCryptographicHash::fileMd5(L"../testdata/core_io/fileread.txt", fileMd5Hex);

	//From: http://www.nirsoft.net/utils/hash_my_files.html
	EXPECT_EQ("5eb63bbbe01eeed093cb22bb8f5acdc3", fileMd5Hex);

	DwByteArray invalidFileMd5;
	EXPECT_FALSE(DwCryptographicHash::fileMd5(L"../testdata/core_io/not_exists.txt", invalidFileMd5));
}
