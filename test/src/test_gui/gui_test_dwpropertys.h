
TEST(DwPropertys, constructor)
{
	DwPropertys propertys;
	EXPECT_FALSE(propertys.hasProperty(L"id"));
}

class GuiTestDwPropertys
{
public:
	GuiTestDwPropertys()
		: m_id(0)
	{
		;
	}
	int id() const
	{
		return m_id;
	}

	void setId(int _id)
	{
		m_id = _id;
	}

	DwString name() const
	{
		return m_name;
	}

	void setName(const DwString& n)
	{
		m_name = n;
	}

protected:
	int m_id;
	DwString m_name;

	DW_DECLARE_PROPERTY(GuiTestDwPropertys);
	DW_WIDGET_ROOT_PROPERTY_BEGIN(GuiTestDwPropertys);
	DW_WIDGET_PROPERTY_NC(id, int, setId, id);
	DW_WIDGET_PROPERTY(name, DwString, setName, name);
	DW_WIDGET_PROPERTY_END();
};
DW_IMPLEMENT_PROPERTY(GuiTestDwPropertys);

TEST(DwPropertys, hasproperty)
{
	GuiTestDwPropertys propertys;

	EXPECT_TRUE(propertys.hasProperty(L"id"));
	EXPECT_TRUE(propertys.hasProperty(L"name"));
	EXPECT_FALSE(propertys.hasProperty(L"dummys"));

	propertys.setProperty(L"dummys", 10);
	EXPECT_TRUE(propertys.hasProperty(L"dummys"));

	EXPECT_EQ(propertys.propertyOf(L"id").as<int>(), 0);
	EXPECT_EQ(propertys.propertyOf(L"name").as<DwString>(), DwString());
	EXPECT_EQ(propertys.propertyOf(L"dummys").as<int>(), 10);
}

TEST(DwPropertys, propertyof)
{
	GuiTestDwPropertys propertys;

	EXPECT_TRUE(propertys.hasProperty(L"id"));

	EXPECT_EQ(propertys.propertyOf("id").as<int>(), 0);
	EXPECT_EQ(propertys.propertyOf("name").as<DwString>(), DwString());
}

TEST(DwPropertys, setproperty)
{
	GuiTestDwPropertys propertys;

	EXPECT_TRUE(propertys.hasProperty(L"id"));

	EXPECT_EQ(propertys.id(), 0);
	EXPECT_EQ(propertys.propertyOf("id").as<int>(), 0);
	EXPECT_EQ(propertys.name(), DwString());
	EXPECT_EQ(propertys.propertyOf("name").as<DwString>(), DwString());

	propertys.setProperty(L"id", 100);
	EXPECT_EQ(propertys.propertyOf("id").as<int>(), 100);
	EXPECT_EQ(propertys.id(), 100);
	propertys.setId(10000);
	EXPECT_EQ(propertys.id(), 10000);
	EXPECT_EQ(propertys.propertyOf("id").as<int>(), 10000);
}