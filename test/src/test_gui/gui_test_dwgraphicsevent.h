
TEST(DwSceneEvent, construct)
{
	DwSceneEvent evt;

	EXPECT_EQ(evt.eventId()->isDerivedFrom(DwEvent::EventId()) , true);
	EXPECT_EQ(evt.eventId()->isDerivedFrom(DwSceneEvent::EventId()) , true);

	EXPECT_TRUE(evt.indexInterestItemFromScene(NULL) == NULL);
	EXPECT_TRUE(evt.isInterestedBy(NULL));

	DwGraphicsItem item;
	evt.setInterestItem(&item);
	EXPECT_TRUE(evt.isInterestedBy(&item));
	DwGraphicsItem item2;
	EXPECT_TRUE(evt.isInterestedBy(&item2));

	EXPECT_TRUE(evt.doneItem() == NULL);
	evt.setDoneItem(&item2);
	EXPECT_TRUE(evt.doneItem() == &item2);

	EXPECT_TRUE(evt.view() == NULL);

	DwGraphicsView view;
	evt.setView(&view);
	EXPECT_TRUE(evt.view() == &view);
}