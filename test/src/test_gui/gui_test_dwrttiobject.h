
TEST(DwRttiObject, constructorANDdeconstructor)
{
	DwRttiObject obj;
	EXPECT_TRUE(obj.isA(DwRttiObject::RTTI()));
	EXPECT_TRUE(obj.isA("DwRttiObject"));
	EXPECT_TRUE(obj.isA('DROT'));
	EXPECT_FALSE(obj.isA(0));

	DwRttiObject * obj2 = DwRttiObject::create();
	DwRttiObject::dumpObjects();
	delete obj2; obj2 = NULL;
}
