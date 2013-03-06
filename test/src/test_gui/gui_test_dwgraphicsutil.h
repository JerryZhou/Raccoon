
#define DW_DUMMY_STACKITEM(n) DwGraphicsItem D##n; DwGraphicsItem *n = &D##n

TEST(DwGraphicsUtil, sortchild)
{
	DW_DUMMY_STACKITEM(root);
	DwGraphicsItem *child0 = new DwGraphicsItem(root);
	DwGraphicsItem *child1 = new DwGraphicsItem(root);
	DwGraphicsItem *child2 = new DwGraphicsItem(root);

	DwVector<DwGraphicsItem *> sortedItems = root->children();
	EXPECT_EQ(sortedItems.size(), 3);
	EXPECT_EQ(sortedItems.at(0), child0);
	EXPECT_EQ(sortedItems.at(1), child1);
	EXPECT_EQ(sortedItems.at(2), child2);

	// Dw::AscendingOrder
	DwGraphicsUtil::sortChild(&sortedItems);
	EXPECT_EQ(sortedItems.size(), 3);
	EXPECT_EQ(sortedItems.at(0), child0);
	EXPECT_EQ(sortedItems.at(1), child1);
	EXPECT_EQ(sortedItems.at(2), child2);
}

TEST(DwGraphicsUtil, sortsiblingsitems)
{
	DW_DUMMY_STACKITEM(root);
	DwGraphicsItem *child0 = new DwGraphicsItem(root);
	DwGraphicsItem *child1 = new DwGraphicsItem(root);
	DwGraphicsItem *child2 = new DwGraphicsItem(root);

	DwVector<DwGraphicsItem *> sortedItems = root->children();
	EXPECT_EQ(sortedItems.size(), 3);
	EXPECT_EQ(sortedItems.at(0), child0);
	EXPECT_EQ(sortedItems.at(1), child1);
	EXPECT_EQ(sortedItems.at(2), child2);

	// Dw::AscendingOrder
	DwGraphicsUtil::sortSiblingsItems(&sortedItems, Dw::AscendingOrder);
	EXPECT_EQ(sortedItems.size(), 3);
	EXPECT_EQ(sortedItems.at(0), child0);
	EXPECT_EQ(sortedItems.at(1), child1);
	EXPECT_EQ(sortedItems.at(2), child2);

	// Dw::DescendingOrder
	DwGraphicsUtil::sortSiblingsItems(&sortedItems, Dw::DescendingOrder);
	EXPECT_EQ(sortedItems.size(), 3);
	EXPECT_EQ(sortedItems.at(0), child2);
	EXPECT_EQ(sortedItems.at(1), child1);
	EXPECT_EQ(sortedItems.at(2), child0);
}

TEST(DwGraphicsUtil, sortchildbysiblingindex)
{
	DW_DUMMY_STACKITEM(root);
	DwGraphicsItem *child0 = new DwGraphicsItem(root);
	DwGraphicsItem *child1 = new DwGraphicsItem(root);
	DwGraphicsItem *child2 = new DwGraphicsItem(root);

	DwVector<DwGraphicsItem *> sortedItems = root->children();
	EXPECT_EQ(sortedItems.size(), 3);
	EXPECT_EQ(sortedItems.at(0), child0);
	EXPECT_EQ(sortedItems.at(1), child1);
	EXPECT_EQ(sortedItems.at(2), child2);

	// Dw::AscendingOrder
	DwGraphicsUtil::sortChildBySiblingIndex(&sortedItems, Dw::AscendingOrder);
	EXPECT_EQ(sortedItems.size(), 3);
	EXPECT_EQ(sortedItems.at(0), child0);
	EXPECT_EQ(sortedItems.at(1), child1);
	EXPECT_EQ(sortedItems.at(2), child2);

	// Dw::DescendingOrder
	DwGraphicsUtil::sortChildBySiblingIndex(&sortedItems, Dw::DescendingOrder);
	EXPECT_EQ(sortedItems.size(), 3);
	EXPECT_EQ(sortedItems.at(0), child2);
	EXPECT_EQ(sortedItems.at(1), child1);
	EXPECT_EQ(sortedItems.at(2), child0);

	// Dw::AscendingOrder
	child1->stackBefore(child0);
	DwGraphicsUtil::sortChildBySiblingIndex(&sortedItems, Dw::AscendingOrder);
	EXPECT_EQ(sortedItems.size(), 3);
	EXPECT_EQ(sortedItems.at(0), child1);
	EXPECT_EQ(sortedItems.at(1), child0);
	EXPECT_EQ(sortedItems.at(2), child2);
}

TEST(DwGraphicsUtil, sortitems)
{
	DwGraphicsScene scene;
	DW_DUMMY_STACKITEM(root);
	DwGraphicsItem *child0 = new DwGraphicsItem(root);
	DwGraphicsItem *child1 = new DwGraphicsItem(root);
	DwGraphicsItem *child2 = new DwGraphicsItem(root);

	scene.addItem(root);
	DW_DUMMY_STACKITEM(root1);
	scene.addItem(root1);

	DwVector<DwGraphicsItem *> sortedItems = scene.items();
	EXPECT_EQ(sortedItems.size(), 5);

	// Dw::AscendingOrder
	DwGraphicsUtil::sortItems(&sortedItems, Dw::AscendingOrder);
	EXPECT_EQ(sortedItems.size(), 5);
	EXPECT_EQ(sortedItems.at(0), root);
	EXPECT_EQ(sortedItems.at(1), child0);
	EXPECT_EQ(sortedItems.at(2), child1);
	EXPECT_EQ(sortedItems.at(3), child2);
	EXPECT_EQ(sortedItems.at(4), root1);

	// Dw::DescendingOrder
	DwGraphicsUtil::sortItems(&sortedItems, Dw::DescendingOrder);
	EXPECT_EQ(sortedItems.size(), 5);
	EXPECT_EQ(sortedItems.at(4), root);
	EXPECT_EQ(sortedItems.at(3), child0);
	EXPECT_EQ(sortedItems.at(2), child1);
	EXPECT_EQ(sortedItems.at(1), child2);
	EXPECT_EQ(sortedItems.at(0), root1);
}

TEST(DwGraphicsUtil, sortitemsbystackorder)
{
	//DwGraphicsScene scene;
	//DW_DUMMY_STACKITEM(root);
	// USED IN BSP TREE INDEX
}

TEST(DwGraphicsUtil, ancestorofaccept)
{
	DwGraphicsScene scene;
	DW_DUMMY_STACKITEM(root);
	DwGraphicsItem *child0 = new DwGraphicsItem(root);
	DwGraphicsItem *child00 = new DwGraphicsItem(child0);
	DwGraphicsItem *child000 = new DwGraphicsItem(child00);

	root->setFlag(Dw::ItemIsMovable);
	child0->setFlag(Dw::ItemIsFocusable);
	child000->setFlag(Dw::ItemIsSelectable);

	EXPECT_TRUE(DwGraphicsUtil::ancestorOfAccept(NULL, Dw::ItemIsMovable) == NULL);
	EXPECT_EQ(DwGraphicsUtil::ancestorOfAccept(child000, Dw::ItemIsMovable), root);
	EXPECT_EQ(DwGraphicsUtil::ancestorOfAccept(child000, Dw::ItemIsFocusable), child0);
	EXPECT_EQ(DwGraphicsUtil::ancestorOfAccept(child000, Dw::ItemIsSelectable), child000);
	EXPECT_TRUE(DwGraphicsUtil::ancestorOfAccept(child000, Dw::ItemClipsToShape) == NULL);
}