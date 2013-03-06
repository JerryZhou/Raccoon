
TEST(DwGraphicsScene, construct)
{
	DwGraphicsScene scene;

	EXPECT_EQ(scene.topLevelItems().size(), 0);
}

TEST(DwGraphicsScene, additem)
{
	DwGraphicsScene scene;

	DwGraphicsItem root;
	scene.addItem(&root);

	EXPECT_EQ(scene.topLevelItems().size(), 1);
	EXPECT_EQ(scene.topLevelItems().at(0), &root);
}

TEST(DwGraphicsScene, removeitem)
{
	DwGraphicsScene scene;

	DwGraphicsItem root;
	scene.addItem(&root);

	EXPECT_EQ(scene.topLevelItems().size(), 1);
	EXPECT_EQ(scene.topLevelItems().at(0), &root);

	scene.removeItem(&root);
	EXPECT_EQ(scene.topLevelItems().size(), 0);
}

TEST(DwGraphicsScene, toplevelitems)
{
	DwGraphicsScene scene;

	DwGraphicsItem root;
	scene.addItem(&root);

	EXPECT_EQ(scene.topLevelItems().size(), 1);
	EXPECT_EQ(scene.topLevelItems().at(0), &root);

	scene.removeItem(&root);
	EXPECT_EQ(scene.topLevelItems().size(), 0);
}

TEST(DwGraphicsScene, scenerect)
{
	DwGraphicsScene scene;
	EXPECT_EQ(scene.sceneRect(), DwRectF());

	DwGraphicsItem root;
	scene.addItem(&root);

	root.setBoundingRect(DwRectF(0, 0, 10, 10));
	EXPECT_EQ(scene.sceneRect(), DwRectF(0, 0, 10, 10));

	scene.setSceneRect(0, 0, 5, 5);
	EXPECT_EQ(scene.sceneRect(), DwRectF(0, 0, 5, 5));
}

TEST(DwGraphicsScene, width)
{
	DwGraphicsScene scene;
	EXPECT_EQ(scene.sceneRect(), DwRectF());

	DwGraphicsItem root;
	scene.addItem(&root);

	root.setBoundingRect(DwRectF(0, 0, 10, 10));
	EXPECT_EQ(scene.sceneRect(), DwRectF(0, 0, 10, 10));

	scene.setSceneRect(0, 0, 5, 5);
	EXPECT_EQ(scene.sceneRect(), DwRectF(0, 0, 5, 5));

	EXPECT_EQ(scene.width(), 5);
}

TEST(DwGraphicsScene, height)
{
	DwGraphicsScene scene;
	EXPECT_EQ(scene.sceneRect(), DwRectF());

	DwGraphicsItem root;
	scene.addItem(&root);

	root.setBoundingRect(DwRectF(0, 0, 10, 10));
	EXPECT_EQ(scene.sceneRect(), DwRectF(0, 0, 10, 10));

	EXPECT_EQ(scene.height(), 10);

	scene.setSceneRect(0, 0, 5, 5);
	EXPECT_EQ(scene.sceneRect(), DwRectF(0, 0, 5, 5));
	EXPECT_EQ(scene.height(), 5);
}

TEST(DwGraphicsScene, itemsBoundingRect)
{
	DwGraphicsScene scene;
	EXPECT_EQ(scene.itemsBoundingRect(), DwRectF());

	DwGraphicsItem root;
	scene.addItem(&root);

	root.setBoundingRect(DwRectF(0, 0, 10, 10));
	EXPECT_EQ(scene.itemsBoundingRect(), DwRectF(0, 0, 10, 10));
}

TEST(DwGraphicsScene, items)
{
	DwGraphicsScene scene;
	EXPECT_EQ(scene.items().size(), 0);

	DwGraphicsItem root;
	scene.addItem(&root);

	// descending order
	EXPECT_EQ(scene.items().size(), 1);
	EXPECT_EQ(scene.items().at(0), &root);

	DwGraphicsItem root1;
	scene.addItem(&root1);
	EXPECT_EQ(scene.items().size(), 2);
	EXPECT_EQ(scene.items().at(0), &root1);
	EXPECT_EQ(scene.items().at(1), &root);
}

TEST(DwGraphicsScene, itemspoint)
{
	DwGraphicsScene scene;
	EXPECT_EQ(scene.items().size(), 0);

	DwVector<DwGraphicsItem *> lookRets = scene.items(DwPointF(1, 1), Dw::IntersectsItemShape, Dw::DescendingOrder);
	EXPECT_EQ(lookRets.size() , 0);

	DwGraphicsItem root;
	scene.addItem(&root);
	lookRets = scene.items(DwPointF(1, 1), Dw::IntersectsItemShape, Dw::DescendingOrder);
	EXPECT_EQ(lookRets.size() , 0);

	root.setBoundingRect(DwRectF(0, 0, 5, 5));
	lookRets = scene.items(DwPointF(1, 1), Dw::IntersectsItemShape, Dw::DescendingOrder);
	EXPECT_EQ(lookRets.size() , 1);
	EXPECT_EQ(lookRets.at(0) , &root);
}

TEST(DwGraphicsScene, itemsrect)
{
	DwGraphicsScene scene;
	EXPECT_EQ(scene.items().size(), 0);

	DwVector<DwGraphicsItem *> lookRets = scene.items(DwRectF(0, 0, 5, 5), Dw::IntersectsItemShape, Dw::DescendingOrder);
	EXPECT_EQ(lookRets.size() , 0);

	DwGraphicsItem root;
	scene.addItem(&root);
	lookRets = scene.items(DwRectF(0, 0, 5, 5), Dw::IntersectsItemShape, Dw::DescendingOrder);
	EXPECT_EQ(lookRets.size() , 0);

	root.setBoundingRect(DwRectF(1, 1, 5, 5));
	lookRets = scene.items(DwRectF(0, 0, 5, 5), Dw::IntersectsItemShape, Dw::DescendingOrder);
	EXPECT_EQ(lookRets.size() , 1);
	EXPECT_EQ(lookRets.at(0) , &root);

	root.setBoundingRect(DwRectF(1, 1, 5, 5));
	lookRets = scene.items(DwRectF(0, 0, 5, 5), Dw::ContainsItemBoundingRect, Dw::DescendingOrder);
	EXPECT_EQ(lookRets.size() , 0);
}

TEST(DwGraphicsScene, itemat)
{
	DwGraphicsScene scene;
	EXPECT_EQ(scene.items().size(), 0);

	DwVector<DwGraphicsItem *> lookRets = scene.items(DwRectF(0, 0, 5, 5), Dw::IntersectsItemShape, Dw::DescendingOrder);
	EXPECT_EQ(lookRets.size() , 0);

	DwGraphicsItem root;
	scene.addItem(&root);
	DwGraphicsItem *findRet = scene.itemAt(DwPointF(0, 0));
	EXPECT_TRUE(findRet == NULL);

	root.setBoundingRect(DwRectF(0, 0, 5, 5));
	findRet = scene.itemAt(DwPointF(0, 0));
	EXPECT_EQ(findRet , &root);

	findRet = scene.itemAt(DwPointF(6, 6));
	EXPECT_TRUE(findRet == NULL);
	
	// no edge
	findRet = scene.itemAt(DwPointF(5, 5));
	EXPECT_TRUE(findRet == NULL);
}

TEST(DwGraphicsScene, index)
{
	DwGraphicsScene scene;
	EXPECT_TRUE(scene.index() != NULL);

	DwGraphicsSceneIndex *sceneIndex = scene.index();

	// default is a bsp tree index or linear index
	EXPECT_TRUE(sceneIndex->isA(DwGraphicsSceneBspTreeIndex::RTTI())
		|| sceneIndex->isA(DwGraphicsSceneLinearIndex::RTTI()));
}

TEST(DwGraphicsScene, clearsearchcache)
{
	DwGraphicsScene scene;

	scene.clearSearchCache();
	DwGraphicsItem root;
	scene.addItem(&root);
	root.setBoundingRect(DwRectF(0, 0, 5, 5));

	DwVector<DwGraphicsItem *> findRets = scene.items(DwPoint(1, 1));
	EXPECT_EQ(findRets.size() , 1);
	EXPECT_EQ(findRets.at(0) , &root);

	DwGraphicsItem root1;
	scene.addItem(&root1);
	root1.setBoundingRect(DwRectF(0, 0, 5, 5));

	findRets = scene.items(DwPoint(1, 1));
	EXPECT_EQ(findRets.size() , 1);
	EXPECT_EQ(findRets.at(0) , &root);

	scene.clearSearchCache();
	findRets = scene.items(DwPoint(1, 1));
	EXPECT_EQ(findRets.size() , 2);
	EXPECT_EQ(findRets.at(0) , &root1);
	EXPECT_EQ(findRets.at(1) , &root);	
}

// TODO: xx