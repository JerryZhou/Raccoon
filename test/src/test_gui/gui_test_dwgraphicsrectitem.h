
TEST(DwGraphicsRectItem, construct)
{
	DwGraphicsRectItem rectItem;
	EXPECT_EQ(rectItem.boundingRect(), DwRectF());
	DwGraphicsRectItem rectItem1(DwRectF(0, 0, 20, 20));
	EXPECT_EQ(rectItem1.boundingRect(), DwRectF(0, 0, 20, 20));
	DwGraphicsRectItem rectItem2(0, 0, 10, 10);
	EXPECT_EQ(rectItem2.boundingRect(), DwRectF(0, 0, 10, 10));
}

TEST(DwGraphicsRectItem, color)
{
	DwGraphicsRectItem rectItem;
	
	EXPECT_EQ(rectItem.color(), DwColor(255, 255, 255));
	rectItem.setColor(DwColor(0, 0, 0, 128));
	EXPECT_EQ(rectItem.color(), DwColor(0, 0, 0, 128));
}

TEST(DwGraphicsRectItem, type)
{
	DwGraphicsRectItem rectItem;

	EXPECT_EQ(rectItem.rtti(), DwGraphicsRectItem::RTTI());
}