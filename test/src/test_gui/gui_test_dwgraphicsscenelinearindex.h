
TEST(DwGraphicsSceneLinearIndex, construct)
{
	DwGraphicsScene scene;
	DwGraphicsSceneLinearIndex linearTreeIndex(&scene);

	EXPECT_EQ(linearTreeIndex.rtti(), DwGraphicsSceneLinearIndex::RTTI());
}

TEST(DwGraphicsSceneLinearIndex, items)
{
	DwGraphicsScene scene(DwGraphicsSceneLinearIndex::RTTI(), NULL);

	DwGraphicsSceneIndex *sceneIndex = scene.index();
	EXPECT_EQ(sceneIndex->rtti(), DwGraphicsSceneLinearIndex::RTTI());
	EXPECT_TRUE(sceneIndex->isA(DwGraphicsSceneLinearIndex::RTTI()));

	DwGraphicsSceneLinearIndex *linearIndex = dwsafe_cast<DwGraphicsSceneLinearIndex>(sceneIndex);
	DW_ASSERT(linearIndex != NULL);

	EXPECT_EQ(linearIndex->scene(), &scene);

	EXPECT_EQ(linearIndex->items().size(), 0);

	DwGraphicsItem DRoot;
	DwGraphicsItem *root = &DRoot;
	scene.addItem(root);
	EXPECT_EQ(linearIndex->items().size(), 1);

	DwGraphicsItem *child = new DwGraphicsItem(root);
	EXPECT_EQ(linearIndex->items().size(), 2);
	EXPECT_EQ(linearIndex->items().at(0), child);
	EXPECT_EQ(linearIndex->items().at(1), root);

	EXPECT_EQ(linearIndex->items(Dw::AscendingOrder).at(0), root);
	EXPECT_EQ(linearIndex->items(Dw::AscendingOrder).at(1), child);
}

TEST(DwGraphicsSceneLinearIndex, estimateitems)
{
	DwGraphicsScene scene(DwGraphicsSceneLinearIndex::RTTI(), NULL);

	DwGraphicsSceneIndex *sceneIndex = scene.index();
	EXPECT_EQ(sceneIndex->rtti(), DwGraphicsSceneLinearIndex::RTTI());
	EXPECT_TRUE(sceneIndex->isA(DwGraphicsSceneLinearIndex::RTTI()));

	DwGraphicsSceneLinearIndex *linearIndex = dwsafe_cast<DwGraphicsSceneLinearIndex>(sceneIndex);
	DW_ASSERT(linearIndex != NULL);

	EXPECT_EQ(linearIndex->scene(), &scene);

	EXPECT_EQ(linearIndex->estimateItems(DwRectF(0, 0, 1, 1), Dw::DescendingOrder).size(), 0);

	DwGraphicsItem DRoot;
	DwGraphicsItem *root = &DRoot;root->setBoundingRect(DwRectF(0, 0, 1, 1));
	scene.addItem(root);
	EXPECT_EQ(linearIndex->estimateItems(DwRectF(0, 0, 1, 1), Dw::DescendingOrder).size(), 1);

	DwGraphicsItem *child = new DwGraphicsItem(root);child->setBoundingRect(DwRectF(0, 0, 1, 1));
	EXPECT_EQ(linearIndex->estimateItems(DwRectF(0, 0, 1, 1), Dw::DescendingOrder).size(), 2);
	EXPECT_EQ(linearIndex->estimateItems(DwRectF(0, 0, 1, 1), Dw::DescendingOrder).at(0), child);
	EXPECT_EQ(linearIndex->estimateItems(DwRectF(0, 0, 1, 1), Dw::DescendingOrder).at(1), root);

	EXPECT_EQ(linearIndex->estimateItems(DwRectF(0, 0, 1, 1), Dw::AscendingOrder).at(0), root);
	EXPECT_EQ(linearIndex->estimateItems(DwRectF(0, 0, 1, 1), Dw::AscendingOrder).at(1), child);
}