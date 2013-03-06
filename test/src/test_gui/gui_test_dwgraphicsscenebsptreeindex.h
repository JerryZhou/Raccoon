
TEST(DwGraphicsSceneBspTreeIndex, construct)
{
	DwGraphicsScene scene;
	DwGraphicsSceneBspTreeIndex bspTreeIndex(&scene);

	EXPECT_EQ(bspTreeIndex.rtti(), DwGraphicsSceneBspTreeIndex::RTTI());
}

TEST(DwGraphicsSceneBspTreeIndex, bsptreedepth)
{
	DwGraphicsScene scene;
	DwGraphicsSceneBspTreeIndex bspTreeIndex(&scene);

	EXPECT_EQ(bspTreeIndex.bspTreeDepth(), 0);

	bspTreeIndex.setBspTreeDepth(10);
	EXPECT_EQ(bspTreeIndex.bspTreeDepth(), 10);
}

TEST(DwGraphicsSceneBspTreeIndex, initialize)
{
	DwGraphicsScene scene;
	DwGraphicsSceneBspTreeIndex bspTreeIndex(&scene);

	EXPECT_EQ(bspTreeIndex.bspTreeDepth(), 0);
	bspTreeIndex.setBspTreeDepth(2);
	bspTreeIndex.initialize(DwRectF(0, 0, 10, 10));
}

TEST(DwGraphicsSceneBspTreeIndex, items)
{
	DwGraphicsScene scene;
	DwGraphicsSceneIndex *sceneIndex = scene.index();
	if (!sceneIndex->isA("DwGraphicsSceneBspTreeIndex"))
	{
		return;
	}

	DwGraphicsSceneBspTreeIndex *bspTreeIndex = (DwGraphicsSceneBspTreeIndex *)sceneIndex;

	EXPECT_EQ(bspTreeIndex->bspTreeDepth(), 0);
	bspTreeIndex->setBspTreeDepth(2);
	bspTreeIndex->initialize(DwRectF(0, 0, 10, 10));

	EXPECT_EQ(bspTreeIndex->items().size(), 0);

	DwGraphicsItem Droot;
	DwGraphicsItem *root = &Droot;
	scene.addItem(root);

	EXPECT_EQ(bspTreeIndex->items().size(), 1);
	EXPECT_EQ(bspTreeIndex->items().at(0), root);
}

// SHOULD KNOW THE estimateItems() JUST RETURN THE RUTE RESULTS
TEST(DwGraphicsSceneBspTreeIndex, estimateitems)
{
	DwGraphicsScene scene;
	scene.setSceneRect(DwRectF(0, 0, 200, 200));
	DwGraphicsSceneIndex *sceneIndex = scene.index();
	if (!sceneIndex->isA("DwGraphicsSceneBspTreeIndex"))
	{
		return;
	}

	DwGraphicsSceneBspTreeIndex *bspTreeIndex = (DwGraphicsSceneBspTreeIndex *)sceneIndex;
	EXPECT_EQ(bspTreeIndex->estimateItems(DwRectF(0, 0, 10, 10), Dw::DescendingOrder).size(), 0);

	DwGraphicsItem Droot;
	DwGraphicsItem *root = &Droot;
	scene.addItem(root);
	root->setRect(DwRectF(0, 0, 10, 10));
	EXPECT_EQ(bspTreeIndex->estimateItems(DwRectF(0, 0, 10, 10), Dw::DescendingOrder).size(), 1);
	EXPECT_EQ(bspTreeIndex->estimateItems(DwRectF(0, 0, 10, 10), Dw::DescendingOrder).at(0), root);
}

// SHOULD KNOW THE estimateItems() JUST RETURN THE RUTE RESULTS
TEST(DwGraphicsSceneBspTreeIndex, estimatetoplevelitems)
{
	DwGraphicsScene scene;
	scene.setSceneRect(DwRectF(0, 0, 200, 200));
	DwGraphicsSceneIndex *sceneIndex = scene.index();
	if (!sceneIndex->isA("DwGraphicsSceneBspTreeIndex"))
	{
		return;
	}

	DwGraphicsSceneBspTreeIndex *bspTreeIndex = (DwGraphicsSceneBspTreeIndex *)sceneIndex;
	EXPECT_EQ(bspTreeIndex->estimateTopLevelItems(DwRectF(0, 0, 10, 10), Dw::DescendingOrder).size(), 0);

	DwGraphicsItem Droot;
	DwGraphicsItem *root = &Droot;
	scene.addItem(root);
	root->setRect(DwRectF(0, 0, 10, 10));
	EXPECT_EQ(bspTreeIndex->estimateTopLevelItems(DwRectF(0, 0, 10, 10), Dw::DescendingOrder).size(), 1);
	EXPECT_EQ(bspTreeIndex->estimateTopLevelItems(DwRectF(0, 0, 10, 10), Dw::DescendingOrder).at(0), root);

	DwGraphicsItem *child = new DwGraphicsItem(root); child;
	EXPECT_EQ(bspTreeIndex->estimateTopLevelItems(DwRectF(0, 0, 10, 10), Dw::DescendingOrder).size(), 1);
	EXPECT_EQ(bspTreeIndex->estimateTopLevelItems(DwRectF(0, 0, 10, 10), Dw::DescendingOrder).at(0), root);

	EXPECT_EQ(bspTreeIndex->estimateItems(DwRectF(0, 0, 10, 10), Dw::DescendingOrder).size(), 2);
	EXPECT_EQ(bspTreeIndex->estimateItems(DwRectF(0, 0, 10, 10), Dw::DescendingOrder).at(0), child);
	EXPECT_EQ(bspTreeIndex->estimateItems(DwRectF(0, 0, 10, 10), Dw::DescendingOrder).at(1), root);

	delete child; child = NULL;
}