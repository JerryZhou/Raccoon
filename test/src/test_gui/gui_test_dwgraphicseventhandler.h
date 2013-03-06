
TEST(DwItemEventHandler, construct)
{
	DwItemEventHandler handler(NULL);

	EXPECT_TRUE(handler.isAcceptedChildEvents() == false);
	EXPECT_TRUE(handler.isAcceptedParentEvents() == false);

	handler.setAcceptedChildEvents(true);
	handler.setAcceptedParentEvents(true);
	EXPECT_TRUE(handler.isAcceptedChildEvents() == true);
	EXPECT_TRUE(handler.isAcceptedParentEvents() == true);
}

TEST(DwSceneEventHandler, construct)
{
	DwSceneEventHandler handler(NULL);
}

TEST(DwViewEventHandler, construct)
{
	DwSceneEventHandler handler(NULL);
}