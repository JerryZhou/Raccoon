
//////////////////////////////////////////////////////////////////////////
// DwArrayList
//////////////////////////////////////////////////////////////////////////

//用于验证传入对象时是否能正确执行构造与析构函数
namespace ArrayListTest
{
	class ObjTest
	{
	public:
		ObjTest() : ptr(NULL)
		{

		}

		ObjTest(const ObjTest& other)
		{
			++i;
		}

		~ObjTest()
		{
			i--;
		}

		static int i;
		void* ptr;		//占位符：用于验证当对象大小不超过一个指针时，测试是否执行placement new
	};	
}

int ArrayListTest::ObjTest::i = 0;

TEST(DwArrayList, DwArrayList)
{
	const int KCount = 100;
	//int
	DwArrayList<int> list1;
	for(int i = KCount; i >= 1; --i)
	{
		list1.prepend(i);
	}
	DwArrayList<int> list2(list1);	
	EXPECT_EQ((1 + 100) * 100 / 2, sumOfContainer(list2));
	EXPECT_EQ(1, list2.front());
	list2.pop_front();
	EXPECT_EQ(2, list2.front());
	EXPECT_EQ(100, list2.back());
	list2.pop_back();
	EXPECT_EQ(99, list2.back());
	EXPECT_EQ(KCount - 2, list2.count());
	EXPECT_EQ(100 * 101 / 2 - 1 - 100, sumOfContainer(list2));
	EXPECT_EQ((1 + 100) * 100 / 2, sumOfContainer(list1));
	//ObjTest
	DwArrayList<ArrayListTest::ObjTest> objlist;
	ArrayListTest::ObjTest mai;
	for(int i = 1; i <= KCount; ++i)
	{
		objlist.push_back(mai);
	}
	EXPECT_EQ(100, ArrayListTest::ObjTest::i);
	for(int i = 0; i < KCount; ++i)
	{
		objlist.pop_front();
	}
	EXPECT_EQ(0, ArrayListTest::ObjTest::i);
}

TEST(DwArrayList, headTailOperation)
{
	const int KCount = 100;
	DwArrayList<int> list;
	EXPECT_TRUE(list.isEmpty());
	EXPECT_EQ(0,list.count());
	DwArrayList<int> intListIter;
	EXPECT_TRUE(list.begin() == list.end());
	for (int i = 1; i < KCount / 2; ++i)
	{
		list.push_back(i);
	}
	for (int i = KCount / 2; i <= KCount; ++i)
	{
		list.append(i);
	}
	int i=1;
	for (DwArrayList<int>::const_iterator it = list.begin(); it != list.end(); ++it)
	{
		EXPECT_EQ(i, *it);
		++i;
	}
	for(int i = KCount ; i > KCount/2; --i)
	{
		list.push_front(i);
	}
	for(int i = KCount / 2; i >=1 ; --i)
	{
		list.prepend(i);
	}
	EXPECT_EQ(2 * KCount, list.count());
	int sum=0;
	for (unsigned int i = 0; i < list.count(); i++)
	{
		sum += list[i];
	}
	EXPECT_EQ(5050 * 2, sum);
	for(int i = 1; i <= KCount / 2; ++i)
	{
		list.pop_front();
	}
	EXPECT_EQ(51, list.front());
	for(int i = 1; i <= KCount / 2; ++i)
	{
		list.pop_back();
	}
	EXPECT_EQ(50, list.back());
}

TEST(DwArrayList, insert)
{
	const int KCount = 511 * 3 - 2;
	DwArrayList<int> list;
	for (int i = 1; i <= KCount; i++)
	{
		list.push_back(i);
	}
	//在特殊位置上插入1至9
	list.insert(1530, 1);	//在最后一个元素之前插入1
	EXPECT_EQ(1, list[1530]);
	list.insert(1520, 2);	//三个结点均满
	list.remove(513);		//值为514
	list.insert(1021, 3);	//找到第三个结点，但正常应在第二个结点尾部插入(前提是循环队列长度为511)
	list.insert(515, 4);	//为增加代码覆盖率而设置的插入位置
	list.insert(1020, 5);	//删除最后一个元素
	list.removeOne(1530);
	list.insert(0, 6);
	list.insert(12, 7);
	list.insert(1537, 8);	//倒数第二个
	list.insert(1539, 9);	//最后一个之后
	EXPECT_EQ(9, list[1537]);
	EXPECT_EQ(511 * 3 - 2 + 9 - 2, list.count());
	EXPECT_EQ((1 + (511 * 3 - 2)) * (511 * 3 - 2) / 2 + 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 - 514 - 1530, sumOfContainer(list));
	DwArrayList<int>::iterator it;
	it = list.insert(it, 10);//使用未初始化的无效迭代器时返回的迭代器的索引为-1
	EXPECT_EQ(-1, it.index());
	list.pop_front();		//仅为增加代码覆盖率，删除前第一个结点只有一个元素
	list.erase(list.end() - 1);//仅为增加代码覆盖率，删除前最后一个结点只有一个元素
	EXPECT_EQ(511, list[511]);	//增加searchAfter代码覆盖率
	EXPECT_EQ(3, list[1535 - 511]);	//增加searchBefore代码覆盖率
	for(int i = 0; i < 511 - 256; ++i)	//仅为增加halveNode代码覆盖率，增加前第一个结点有256个元素
	{
		list.prepend(0);
	}
	list.insert(1, 10);
	EXPECT_EQ(10, list[1]);
	list.insert(1790, 11);
	EXPECT_EQ(11, list[1790]);
	for (int i = 0; i < 256; ++i)
	{
		list.insert(list.count() - 260, 0);	//仅为增加halveNode代码覆盖率，在倒数第二个结点上插入
	}
	EXPECT_EQ(8, list[list.count() - 1]);
}

TEST(DwArrayList, remove)
{
	const int KCount = 511 * 3;				//DequeList中一个结点（循环队列）的长度为511，插入3个结点的数据
	DwArrayList<int> intList;
	for (int i = 1; i <= KCount; i++)
	{
		intList << i;
	}
	DwArrayList<int>::iterator intIt;
	for (int i = 1; i <= 511; i++)			//删除中间结点的所有元素，中间结点的第一个元素为512
	{
		intIt = intList.erase(intList.begin() + 511);
		EXPECT_EQ(512 + i, *intIt);
	}

	DwArrayList<std::string> strList;
	strList.push_back("Duo");
	strList.push_back("Wan");
	strList.push_back("-");
	strList.push_back("Duo");
	strList.push_back("YY");
	strList.push_back("-");
	strList.push_back("China");
	strList.push_back("Duo");				//strList = DuoWan-DuoYY-ChinaDuo
	EXPECT_EQ(8, strList.count());
	EXPECT_EQ(3, strList.removeAll("Duo"));	//strList = Wan-YY-China
	EXPECT_EQ(0, strList.removeAll("Duo"));
	strList.push_back("Duo");
	strList.push_front("Duo");				//strList = DuoWan-YY-ChinaDuo
	std::string str;
	for (DwArrayList<std::string>::const_iterator it = strList.begin(); it != strList.begin() + 2; ++it)
	{
		str += *it;
	}
	EXPECT_STREQ("DuoWan", str.c_str());
	str.clear();
	DwArrayList<std::string>::iterator newIt = strList.erase(strList.begin() + 2);	//删除“-”后返回其下个元素“YY”的迭代器
	EXPECT_EQ("YY", *newIt);
	EXPECT_EQ(6, strList.count());			//strList = DuoWanYY-ChinaDuo
	for (DwArrayList<std::string>::const_iterator it = strList.begin(); it != strList.end() - 3; ++it)
	{
		str += *it;
	}
	EXPECT_STREQ("DuoWanYY",str.c_str());
	newIt = strList.erase(strList.createIterator(strList.indexOf("Duo")));
	EXPECT_EQ("Wan", *newIt);
	EXPECT_EQ(5, strList.count());			//strList = WanYY-ChinaDuo
	str.clear();
	for (DwArrayList<std::string>::const_iterator it = strList.begin(); it != strList.end(); ++it)
	{
		str += *it;
	}
	EXPECT_EQ(5, strList.count());
	EXPECT_STREQ("WanYY-ChinaDuo", str.c_str());
	str.clear();
	EXPECT_TRUE(strList.removeOne("Duo"));	//strList = WanYY-China
	EXPECT_EQ(4, strList.count());
	for (DwArrayList<std::string>::const_iterator it = strList.begin() + 1; it != strList.end(); ++it)
	{
		str += *it;
	}
	EXPECT_STREQ("YY-China", str.c_str());
	EXPECT_EQ(0, strList.removeAll("Duo"));	//strList = WanYY-China
	DwArrayList<std::string>::const_iterator it = strList.createIterator(6);	//超过范围，指向end()
	EXPECT_TRUE(strList.end() == it);
	DwArrayList<std::string>::iterator eraseIt = strList.erase(strList.begin(), strList.end() - 1);
	EXPECT_TRUE(strList.begin() == eraseIt);
	EXPECT_EQ(1, strList.count());
	strList.pop_back();						//用pop_back删除最后一个元素
	EXPECT_TRUE(strList.isEmpty());
	EXPECT_EQ(0, strList.count());
}

TEST(DwArrayList, iterator)
{
	const int KCount = 100;
	DwArrayList<int> list;
	for (int i = 1; i <= KCount; i++)
	{
		list.push_back(i);
	}
	DwArrayList<int>::iterator it1;							//测试默认构造函数
	it1 = list.begin() + 10;								//测试operator+(int n)，指向第11个元素，索引10
	EXPECT_EQ(10, it1.index());	
	//end的索引为100，指向第91个元素，索引90
	DwArrayList<int>::iterator it2 = list.end() - 10;		//测试复制构造函数与operator-(int n)
	EXPECT_EQ(90, it2.index());								//测试index
	EXPECT_TRUE(it1 != it2);								//测试operator!=
	EXPECT_TRUE(it1 < it2);									//测试operator<
	EXPECT_TRUE(it1 <= it2);								//测试operator<=
	EXPECT_EQ(80, it2 - it1);								//测试operator-(const Iterator&)
	EXPECT_EQ(11, *it1);									//测试operator*
	DwArrayList<int>::iterator it3 = it1;
	EXPECT_TRUE(it1 == it3);								//测试operator==
	EXPECT_TRUE(it3 >= it1);								//测试operator>=
	it3++;													//测试operator++(int)
	EXPECT_TRUE(it3 > it1);									//测试operator>
	EXPECT_TRUE(it3 == ++it1);								//测试operator++()
	it1--;
	--it3;
	EXPECT_TRUE(it1 == it3);								//测试operator--(int)与operator--()
	it1 += 80;
	EXPECT_TRUE(it2 == it1);								//测试operator+=
	//*it2 == 91
	EXPECT_TRUE(*it2 == it3[80]);							//测试operator[]
	DwArrayList<int>::iterator it4 = list.begin() + 0;		//边界测试
	EXPECT_EQ(0, it4.index());
	it4 = list.begin() + 100;								//刚好越界
	EXPECT_EQ(100, it4.index());							//指向end，id为100
	it4 = list.begin() + 200;								//越界
	EXPECT_EQ(100, it4.index());							//指向end，id为100
	EXPECT_TRUE(++it4 == list.end());
	it4 = list.begin() - 200;								//越界
	EXPECT_EQ(0, it4.index());								//指向begin，id为0	
	EXPECT_TRUE(--it4 == list.begin());
	DwArrayList<int>::iterator it5 = list.createIterator(15);
	EXPECT_TRUE(it5 - 1 == list.begin() + 14);
	list.clear();
	DwArrayList<int>::iterator it6 = list.begin();			//仅为增加代码覆盖率
	EXPECT_EQ(-1, it6.index());
	it6 = list.end();
	EXPECT_EQ(-1, it6.index());
	DwArrayList<int>::const_iterator it7 = list.begin();
	EXPECT_EQ(-1, it7.index());
	it7 = list.end();
	EXPECT_EQ(-1, it7.index());
	DwArrayList<int>::iterator it8 = list.createIterator(-1);
	EXPECT_EQ(-1, it8.index());
}

TEST(DwArrayList, sort)
{
	const int KCount = 1000;
	DwArrayList<int> list;
	for (int i = 0; i < KCount; i++)
	{
		list.push_back(dwRand() % KCount);
	}
	int sum = sumOfContainer(list);
	std::sort(list.begin(), list.end());
	EXPECT_EQ(sum, sumOfContainer(list));
	for (unsigned int i = 1; i < list.count(); i++)
	{
		EXPECT_TRUE(list[i] >= list[i - 1]);
	}
}
