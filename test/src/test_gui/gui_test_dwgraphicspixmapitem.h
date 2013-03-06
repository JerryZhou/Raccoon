
TEST(DwGraphicsPixmapItem, construct)
{
	DwGraphicsPixmapItem pixmapItem;
}

TEST(DwGraphicsPixmapItem, offset)
{
	DwGraphicsPixmapItem pixmapItem;
	EXPECT_EQ(pixmapItem.offset(), DwPointF());
	pixmapItem.setOffset(1, 1);
	EXPECT_EQ(pixmapItem.offset(), DwPointF(1, 1));
}