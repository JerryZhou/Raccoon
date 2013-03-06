
bool deleteDir( const DwString& dirName)
{
	WIN32_FIND_DATA findData;

	DwString curDir(dirName);
	curDir.append(L"/*.*");
	HANDLE hFirstFile = ::FindFirstFile(curDir.constString(), &findData);

	if( hFirstFile == INVALID_HANDLE_VALUE )
	{
		return false;
	}
	bool bRes = true;
	while(bRes)
	{
		bRes = ::FindNextFile(hFirstFile, &findData);
		if( (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) //发现目录
		{
			if( !wcscmp(findData.cFileName, L".") || !wcscmp(findData.cFileName, L"..") ) //.或..
			{
				continue;
			}
			else
			{
				DwString tmppath = dirName + "/" + findData.cFileName;
				deleteDir(tmppath);
			}
		}
		else               //发现文件
		{
			DwString tmppath = dirName + "/" + findData.cFileName;
			::DeleteFile(tmppath.constString());
		}
	}
	::FindClose(hFirstFile);
	if(!RemoveDirectory(dirName.constString()))
	{
		return false;
	}
	return true;
}

void cleanUp(const DwVector<DwByteArray>& fileVec)
{
	for (DwVector<DwByteArray>::const_iterator iter = fileVec.begin(); iter != fileVec.end(); ++iter)
	{
		WIN32_FIND_DATA findData;
		DwString curDir((*iter).data());
		if(::FindFirstFile(curDir.constString(), &findData))
		{
			if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				deleteDir(curDir);
			}
			else
			{
				::DeleteFile(curDir.constString());
			}
		}
	}
}

TEST(DwZipUtils, DwZipUtils)
{
	DwVector<DwByteArray> fileVec;
	DwByteArray zipName("../testdata/core_zip/test_zip.zip");
	DwByteArray unzipDir("../testdata/core_zip/test_unzip");
	fileVec.push_back(zipName);
	fileVec.push_back(unzipDir);
	cleanUp(fileVec);

	DwByteArray src("hello hellohello hellohello hellohello hellohello hello a1fd32a1s");
	DwByteArray str1;
	
	DwZipUtils::ZipGzipEx(src, src.size(), str1);

	DwByteArray str2;
	DwZipUtils::UnZipGzip(str1, str1.size(), str2);

	EXPECT_EQ(DwByteArray(src), str2);

	DwVector<DwByteArray> filesToAdd;
	filesToAdd.append(DwByteArray("../testdata/core_zip/minizip64_changes.txt"));
	filesToAdd.append(DwByteArray("../testdata/core_zip/minizip64_info.txt"));
	
	EXPECT_TRUE(DwZipUtils::Zip(zipName, filesToAdd));

	EXPECT_TRUE(DwZipUtils::Unzip(zipName, unzipDir));

	DwMap<DwByteArray, DwByteArray> *fileNameDataMap = new DwMap<DwByteArray, DwByteArray>;
	EXPECT_TRUE(DwZipUtils::UnzipToMemery(zipName, fileNameDataMap));
	delete fileNameDataMap;

	DwByteArray fileData;
	EXPECT_TRUE(DwZipUtils::UnzipOneFileToMemery(zipName, "../testdata/core_zip/minizip64_changes.txt", fileData));
}

TEST(DwGZip, DwGZip)
{
	DwByteArray src1("hello hellohello hellohello hellohello hellohello hello a1fd32a1s");
	DwByteArray str1;

	DwGZip gzip1;
	gzip1.gzipex(src1, src1.size(), str1);

	DwByteArray str2;
	gzip1.ungzip(str1, str1.size(), str2);
	DwByteArray str3;
	gzip1.ungzipex(str1, str1.size(), str3);

	EXPECT_EQ(DwByteArray(src1), str2);
	EXPECT_EQ(str3, str2);

	DwGZip gzip2;
	DwByteArray str4;
	gzip2.gzipex(DwByteArray(""), 0, str4);
	gzip2.ungzipex(DwByteArray(""), 0, str4);

	DwFile bigFile("../testdata/core_zip/readme.txt");
	if (bigFile.open(DwFile::ReadOnly))
	{
		DwByteArray src2 = bigFile.readAll();
		bigFile.close();
		EXPECT_TRUE(gzip2.gzipex(src2, src2.size(), str4));

		DwByteArray str5;
		gzip1.ungzip(str4, str4.size(), str5);
		DwByteArray str6;
		gzip1.ungzipex(str4, str4.size(), str6);

		EXPECT_EQ(DwByteArray(src2), str5);
		EXPECT_EQ(str6, str5);
	}	
}

TEST(DwZip, DwZip)
{
	DwVector<DwByteArray> fileVec;
	DwByteArray zipName("../testdata/core_zip/test_zip_2.zip");
	DwByteArray unzipDir("../testdata/core_zip/test_unzip_2");
	fileVec.push_back(zipName);
	fileVec.push_back(unzipDir);
	cleanUp(fileVec);

	DwZip zip(zipName);
	EXPECT_FALSE(zip.isExist());
	zip.setCompressLevel(6);
	zip.setPassword(DwByteArray());
	zip.setZipExcludePath(false);
	zip.setUnzipExcludePath(false);
	
	//zip.setZipExcludePath(true);

	DwVector<DwByteArray> filesToAdd;
	filesToAdd.append(DwByteArray("../testdata/core_zip/minizip64_changes.txt"));
	filesToAdd.append(DwByteArray("../testdata/core_zip/minizip64_info.txt"));
	filesToAdd.append(DwByteArray("../testdata/core_zip/cmakelists.txt"));

	EXPECT_TRUE(zip.zip(filesToAdd, DwZipUtils::CreateZip));
	EXPECT_TRUE(zip.unzip(unzipDir));
	EXPECT_FALSE(zip.unzip("$?not_exist_$"));

	DwMap<DwByteArray, DwByteArray> *fileNameDataMap = new DwMap<DwByteArray, DwByteArray>;
	EXPECT_TRUE(zip.unzip(fileNameDataMap));
	delete fileNameDataMap;

	DwByteArray fileData;
	EXPECT_TRUE(zip.unzip("../testdata/core_zip/minizip64_changes.txt", fileData));

	DwZip zip2("not_esixts.zip");
	EXPECT_FALSE(zip2.unzip(fileNameDataMap));
	EXPECT_FALSE(zip2.unzip("", fileData));
	EXPECT_FALSE(zip2.unzip(""));
}

TEST(DwZipArchive, DwZipArchive)
{
	DwVector<DwByteArray> fileVec;
	DwByteArray zipName("../testdata/core_zip/test_zip_3.zip");
	DwByteArray unzipDir("../testdata/core_zip/test_unzip_3");
	DwByteArray unzipOneFileDir("../testdata/core_zip/test_unzip_3/one_file");
	DwByteArray fileNameCopy("../testdata/core_zip/minizip64_info_copy.txt");
	fileVec.push_back(zipName);
	fileVec.push_back(unzipDir);
	fileVec.push_back(fileNameCopy);
	cleanUp(fileVec);

	DwZipArchive zipArchive(zipName);
	DwByteArray fileName1("../testdata/core_zip/minizip64_info.txt");
	DwByteArray fileName2("../testdata/core_zip/readme.txt");

	zipArchive.setZipMode(DwZipUtils::OverWriteZip);
	EXPECT_TRUE(zipArchive.open(DwZipArchive::Zip));
	EXPECT_TRUE(zipArchive.addFile(fileName1));
	EXPECT_TRUE(zipArchive.addFile(fileName2));
	//EXPECT_TRUE(zipArchive.extractTo("../testdata/core_zip/test_zip"));
	//压缩模式，无法获得文件数量
	EXPECT_EQ(-1, zipArchive.count());
	EXPECT_TRUE(zipArchive.close());

	EXPECT_TRUE(zipArchive.open(DwZipArchive::View));
	EXPECT_EQ(2, zipArchive.count());
	EXPECT_EQ(1, zipArchive.indexByName(fileName2));

	//压缩在.zip的文件名不会以"../"开始，如果Zip时传进去"../"，保存前会滤掉"../"。
	EXPECT_EQ(DwByteArray("testdata/core_zip/readme.txt"), zipArchive.nameByIndex(1));
	EXPECT_EQ(DwByteArray("testdata/core_zip"), zipArchive.currentPath());
	EXPECT_TRUE(zipArchive.close());

	zipArchive.open(DwZipArchive::UnZip);
	DwZipFile zipFile(fileNameCopy);
	zipArchive.fileByName("testdata/core_zip/minizip64_info.txt", zipFile);
	zipArchive.close();

	DwFile tempFile(fileName1.data());
	tempFile.open(DwIODevice::ReadOnly);
	zipFile.open(DwIODevice::ReadWrite);
	EXPECT_EQ(zipFile.readAll(), tempFile.readAll());

	char *readFromFile = new char[10 + 1];
	EXPECT_EQ(10, zipFile.read(readFromFile, 10));
	EXPECT_TRUE(strcmp("MiniZip - ", readFromFile) == 0);

	EXPECT_EQ(10, zipFile.read(readFromFile, 10));
	EXPECT_TRUE(strcmp("Copyright ", readFromFile) == 0);

	zipFile.seek(30);
	EXPECT_EQ(30, zipFile.pos());

	EXPECT_EQ(10, zipFile.read(readFromFile, 10));
	EXPECT_TRUE(strcmp("010 - by G", readFromFile) == 0);

	EXPECT_FALSE(zipFile.atEnd());
	zipFile.seek(zipFile.size());
	EXPECT_TRUE(zipFile.atEnd());

	DwByteArray addInfo("*****************\nadd by application.\n*****************");
	zipFile.write(addInfo);
	EXPECT_EQ(tempFile.size() + addInfo.size(), zipFile.saveToDisk(""));

	zipArchive.setZipMode(DwZipUtils::AppendToZip);
	zipArchive.open(DwZipArchive::Zip);
	EXPECT_TRUE(zipArchive.addFile(zipFile));
	zipFile.close();
	zipArchive.close();
	delete[] readFromFile;

	zipArchive.open(DwZipArchive::UnZip);

	EXPECT_TRUE(zipArchive.extractTo(unzipDir));
	EXPECT_TRUE(zipArchive.extractOneFileTo(unzipOneFileDir, "testdata/core_zip/minizip64_info_copy.txt"));

	EXPECT_TRUE(zipArchive.delFile(fileNameCopy));
	EXPECT_FALSE(zipArchive.delFile(fileNameCopy + ".temp"));
	EXPECT_TRUE(zipArchive.delFile(1));
	EXPECT_TRUE(zipArchive.delFile(0));
	EXPECT_FALSE(zipArchive.delFile(2));
	zipArchive.close();
}