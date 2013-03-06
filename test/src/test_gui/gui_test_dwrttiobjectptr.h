
TEST(DwRttiObjectPtr, objectconstruct)
{
	DwRttiObjectPtr<DwRttiObject> objPtr;
	EXPECT_TRUE(objPtr.isNull());

	{
		DwRttiObject obj;
		objPtr = &obj;
		EXPECT_TRUE(objPtr == &obj);
	}
	EXPECT_TRUE(objPtr.isNull());
}

class GuiTestRttiObject : public DwRttiObject
{
	DW_DECLARE_CLASS(GuiTestRttiObject);
public:
protected:
private:
};
DW_IMPLEMENT_CLASS(GuiTestRttiObject, 'TEST', DwRttiObject);

TEST(GuiTestRttiObject, objectconstruct)
{
	DwRttiObjectPtr<GuiTestRttiObject> objPtr;
	EXPECT_TRUE(objPtr.isNull());

	{
		GuiTestRttiObject obj;
		objPtr = &obj;
		EXPECT_TRUE(objPtr == &obj);
		EXPECT_TRUE(objPtr->isA(DwRttiObject::RTTI()));
		EXPECT_TRUE(objPtr->isA(GuiTestRttiObject::RTTI()));
	}
	EXPECT_TRUE(objPtr.isNull());
}
