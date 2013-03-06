
#define DW_DUMMY_STACKITEM(n) DwGraphicsItem D##n; DwGraphicsItem *n = &D##n

TEST(DwAnchorLine, constructor)
{
	DwAnchorLine anchorLine;
	EXPECT_TRUE(anchorLine.item == NULL);
	EXPECT_TRUE(anchorLine.anchorLine == Dw::Invalid);

	DW_DUMMY_STACKITEM(root);
	DwAnchorLine anchorLine1(root, Dw::Left);
	EXPECT_TRUE(anchorLine1.item == root);
	EXPECT_TRUE(anchorLine1.anchorLine == Dw::Left);
}

TEST(DwAnchorLine, equal)
{
	DwAnchorLine anchorLine0;
	DwAnchorLine anchorLine1;
	DW_DUMMY_STACKITEM(root);
	DwAnchorLine anchorLine2(root, Dw::Invalid);
	DwAnchorLine anchorLine3(root, Dw::Left);
	DwAnchorLine anchorLine4(root, Dw::Left);

	EXPECT_EQ(anchorLine0, anchorLine0);
	EXPECT_EQ(anchorLine0, anchorLine1);

	EXPECT_FALSE(anchorLine2 == anchorLine3);
	EXPECT_EQ(anchorLine3, anchorLine4);
}