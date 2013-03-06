
TEST(DwGraphicsSceneIndex, construct)
{
	DwGraphicsScene scene;
	DwGraphicsSceneIndex *sceneIndex = scene.index();
	EXPECT_EQ(sceneIndex->scene() , &scene);
}

TEST(DwGraphicsSceneIndex, items)
{
	DwGraphicsScene scene;
	DwGraphicsSceneIndex *sceneIndex = scene.index();
	EXPECT_EQ(sceneIndex->scene(), &scene);

	EXPECT_EQ(sceneIndex->items().size(), 0);

	DwGraphicsItem DRoot;
	DwGraphicsItem *root = &DRoot;
	scene.addItem(root);
	EXPECT_EQ(sceneIndex->items().size(), 1);

	DwGraphicsItem *child = new DwGraphicsItem(root);
	EXPECT_EQ(sceneIndex->items().size(), 2);
	EXPECT_EQ(sceneIndex->items().at(0), child);
	EXPECT_EQ(sceneIndex->items().at(1), root);

	EXPECT_EQ(sceneIndex->items(Dw::AscendingOrder).at(0), root);
	EXPECT_EQ(sceneIndex->items(Dw::AscendingOrder).at(1), child);
}

TEST(DwGraphicsSceneIndex, itemspos)
{
	DwGraphicsScene scene;
	DwGraphicsSceneIndex *sceneIndex = scene.index();
	EXPECT_EQ(sceneIndex->scene(), &scene);

	EXPECT_EQ(sceneIndex->items(DwPointF(0, 0), Dw::IntersectsItemShape, Dw::DescendingOrder).size(), 0);

	DwGraphicsItem DRoot;
	DwGraphicsItem *root = &DRoot;root->setBoundingRect(DwRectF(0, 0, 1, 1));
	scene.addItem(root);
	EXPECT_EQ(sceneIndex->items(DwPointF(0, 0), Dw::IntersectsItemShape, Dw::DescendingOrder).size(), 1);

	DwGraphicsItem *child = new DwGraphicsItem(root);child->setBoundingRect(DwRectF(0, 0, 1, 1));
	EXPECT_EQ(sceneIndex->items(DwPointF(0, 0), Dw::IntersectsItemShape, Dw::DescendingOrder).size(), 2);
	EXPECT_EQ(sceneIndex->items(DwPointF(0, 0), Dw::IntersectsItemShape, Dw::DescendingOrder).at(0), child);
	EXPECT_EQ(sceneIndex->items(DwPointF(0, 0), Dw::IntersectsItemShape, Dw::DescendingOrder).at(1), root);

	EXPECT_EQ(sceneIndex->items(DwPointF(0, 0), Dw::IntersectsItemShape, Dw::AscendingOrder).at(0), root);
	EXPECT_EQ(sceneIndex->items(DwPointF(0, 0), Dw::IntersectsItemShape, Dw::AscendingOrder).at(1), child);
}

TEST(DwGraphicsSceneIndex, itemsrect)
{
	DwGraphicsScene scene;
	DwGraphicsSceneIndex *sceneIndex = scene.index();
	EXPECT_EQ(sceneIndex->scene(), &scene);

	EXPECT_EQ(sceneIndex->items(DwRectF(0, 0, 1, 1), Dw::IntersectsItemShape, Dw::DescendingOrder).size(), 0);

	DwGraphicsItem DRoot;
	DwGraphicsItem *root = &DRoot;root->setBoundingRect(DwRectF(0, 0, 1, 1));
	scene.addItem(root);
	EXPECT_EQ(sceneIndex->items(DwRectF(0, 0, 1, 1), Dw::IntersectsItemShape, Dw::DescendingOrder).size(), 1);

	DwGraphicsItem *child = new DwGraphicsItem(root);child->setBoundingRect(DwRectF(0, 0, 1, 1));
	EXPECT_EQ(sceneIndex->items(DwRectF(0, 0, 1, 1), Dw::IntersectsItemShape, Dw::DescendingOrder).size(), 2);
	EXPECT_EQ(sceneIndex->items(DwRectF(0, 0, 1, 1), Dw::IntersectsItemShape, Dw::DescendingOrder).at(0), child);
	EXPECT_EQ(sceneIndex->items(DwRectF(0, 0, 1, 1), Dw::IntersectsItemShape, Dw::DescendingOrder).at(1), root);

	EXPECT_EQ(sceneIndex->items(DwRectF(0, 0, 1, 1), Dw::IntersectsItemShape, Dw::AscendingOrder).at(0), root);
	EXPECT_EQ(sceneIndex->items(DwRectF(0, 0, 1, 1), Dw::IntersectsItemShape, Dw::AscendingOrder).at(1), child);
}

TEST(DwGraphicsSceneIndex, estimateitemspos)
{
	DwGraphicsScene scene;
	DwGraphicsSceneIndex *sceneIndex = scene.index();
	EXPECT_EQ(sceneIndex->scene(), &scene);

	EXPECT_EQ(sceneIndex->estimateItems(DwPointF(0, 0), Dw::DescendingOrder).size(), 0);

	DwGraphicsItem DRoot;
	DwGraphicsItem *root = &DRoot;root->setBoundingRect(DwRectF(0, 0, 1, 1));
	scene.addItem(root);
	EXPECT_EQ(sceneIndex->estimateItems(DwPointF(0, 0), Dw::DescendingOrder).size(), 1);

	DwGraphicsItem *child = new DwGraphicsItem(root);child->setBoundingRect(DwRectF(0, 0, 1, 1));
	EXPECT_EQ(sceneIndex->estimateItems(DwPointF(0, 0), Dw::DescendingOrder).size(), 2);
	EXPECT_EQ(sceneIndex->estimateItems(DwPointF(0, 0), Dw::DescendingOrder).at(0), child);
	EXPECT_EQ(sceneIndex->estimateItems(DwPointF(0, 0), Dw::DescendingOrder).at(1), root);

	EXPECT_EQ(sceneIndex->estimateItems(DwPointF(0, 0), Dw::AscendingOrder).at(0), root);
	EXPECT_EQ(sceneIndex->estimateItems(DwPointF(0, 0), Dw::AscendingOrder).at(1), child);
}

TEST(DwGraphicsSceneIndex, estimateitemsrect)
{
	DwGraphicsScene scene;
	DwGraphicsSceneIndex *sceneIndex = scene.index();
	EXPECT_EQ(sceneIndex->scene(), &scene);

	EXPECT_EQ(sceneIndex->estimateItems(DwRectF(0, 0, 1, 1), Dw::DescendingOrder).size(), 0);

	DwGraphicsItem DRoot;
	DwGraphicsItem *root = &DRoot;root->setBoundingRect(DwRectF(0, 0, 1, 1));
	scene.addItem(root);
	EXPECT_EQ(sceneIndex->estimateItems(DwRectF(0, 0, 1, 1), Dw::DescendingOrder).size(), 1);

	DwGraphicsItem *child = new DwGraphicsItem(root);child->setBoundingRect(DwRectF(0, 0, 1, 1));
	EXPECT_EQ(sceneIndex->estimateItems(DwRectF(0, 0, 1, 1), Dw::DescendingOrder).size(), 2);
	EXPECT_EQ(sceneIndex->estimateItems(DwRectF(0, 0, 1, 1), Dw::DescendingOrder).at(0), child);
	EXPECT_EQ(sceneIndex->estimateItems(DwRectF(0, 0, 1, 1), Dw::DescendingOrder).at(1), root);

	EXPECT_EQ(sceneIndex->estimateItems(DwRectF(0, 0, 1, 1), Dw::AscendingOrder).at(0), root);
	EXPECT_EQ(sceneIndex->estimateItems(DwRectF(0, 0, 1, 1), Dw::AscendingOrder).at(1), child);
}

TEST(DwGraphicsSceneIndex, estimatetoplevelitems)
{
	DwGraphicsScene scene;
	DwGraphicsSceneIndex *sceneIndex = scene.index();
	EXPECT_EQ(sceneIndex->scene(), &scene);

	EXPECT_EQ(sceneIndex->estimateTopLevelItems(DwRectF(0, 0, 1, 1), Dw::DescendingOrder).size(), 0);

	DwGraphicsItem DRoot;
	DwGraphicsItem *root = &DRoot;root->setBoundingRect(DwRectF(0, 0, 1, 1));
	scene.addItem(root);
	EXPECT_EQ(sceneIndex->estimateTopLevelItems(DwRectF(0, 0, 1, 1), Dw::DescendingOrder).size(), 1);

	DwGraphicsItem *child0 = new DwGraphicsItem(root);child0->setBoundingRect(DwRectF(0, 0, 1, 1));
	DwGraphicsItem *child = new DwGraphicsItem();child->setBoundingRect(DwRectF(0, 0, 1, 1));
	scene.addItem(child);
	EXPECT_EQ(sceneIndex->estimateTopLevelItems(DwRectF(0, 0, 1, 1), Dw::DescendingOrder).size(), 2);
	EXPECT_EQ(sceneIndex->estimateTopLevelItems(DwRectF(0, 0, 1, 1), Dw::DescendingOrder).at(0), child);
	EXPECT_EQ(sceneIndex->estimateTopLevelItems(DwRectF(0, 0, 1, 1), Dw::DescendingOrder).at(1), root);

	EXPECT_EQ(sceneIndex->estimateTopLevelItems(DwRectF(0, 0, 1, 1), Dw::AscendingOrder).at(0), root);
	EXPECT_EQ(sceneIndex->estimateTopLevelItems(DwRectF(0, 0, 1, 1), Dw::AscendingOrder).at(1), child);
	child->setParentItem(root);
}

TEST(DwGraphicsSceneIndex, debugstring)
{
	DwGraphicsScene scene;
	DwGraphicsSceneIndex *sceneIndex = scene.index();
	EXPECT_EQ(sceneIndex->scene(), &scene);

	sceneIndex->debugString(DwDebug());
}