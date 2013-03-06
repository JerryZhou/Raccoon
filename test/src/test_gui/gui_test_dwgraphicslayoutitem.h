
TEST(DwGraphicsLayoutItem, construct)
{
	DwGraphicsLayoutItem layoutItem;

	EXPECT_TRUE(layoutItem.rtti() == DwGraphicsLayoutItem::RTTI());
}