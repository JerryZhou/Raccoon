
TEST(DwGraphicsView, constructor)
{
	DwGraphicsView view0;
	EXPECT_TRUE(view0.scene() == NULL);
	DwGraphicsView view1(NULL, NULL, NULL);
	EXPECT_TRUE(view1.scene() == NULL);
}

TEST(DwGraphicsView, backgroundcolor)
{
	DwGraphicsView view;
	EXPECT_EQ(view.backgroundColor() , DwColor(0, 0, 0, 0));
	
	view.setBackgroundColor(DwColor(100, 200, 2, 255));
	EXPECT_EQ(view.backgroundColor() , DwColor(100, 200, 2, 255));
}

TEST(DwGraphicsView, scene)
{
	DwGraphicsView view;
	EXPECT_TRUE(view.scene() == NULL);

	DwGraphicsScene scene;
	view.setScene(&scene);
	EXPECT_EQ(view.scene() , &scene);
}

TEST(DwGraphicsView, scenerect)
{
	DwGraphicsView view;
	EXPECT_TRUE(view.scene() == NULL);

	DwGraphicsScene scene;
	view.setScene(&scene);
	EXPECT_EQ(view.sceneRect() , scene.sceneRect());

	view.setSceneRect(0, 0, 100, 100);
	EXPECT_EQ(view.sceneRect() , DwRectF(0, 0, 100, 100));
}

TEST(DwGraphicsView, scrollheight)
{
	DwGraphicsView view;
	EXPECT_TRUE(view.scene() == NULL);

	DwGraphicsScene scene;
	view.setScene(&scene);
	EXPECT_EQ(view.sceneRect() , scene.sceneRect());

	view.setSceneRect(0, 0, 100, 100);
	EXPECT_EQ(view.sceneRect() , DwRectF(0, 0, 100, 100));

	view.scrollHeight(100);
	EXPECT_EQ(view.sceneRect() , DwRectF(0, 100, 100, 100));
}

TEST(DwGraphicsView, scrollwidth)
{
	DwGraphicsView view;
	EXPECT_TRUE(view.scene() == NULL);

	DwGraphicsScene scene;
	view.setScene(&scene);
	EXPECT_EQ(view.sceneRect() , scene.sceneRect());

	view.setSceneRect(0, 0, 100, 100);
	EXPECT_EQ(view.sceneRect() , DwRectF(0, 0, 100, 100));

	view.scrollWidth(100);
	EXPECT_EQ(view.sceneRect() , DwRectF(100, 0, 100, 100));
}

TEST(DwGraphicsView, viewportrect)
{
	DwGraphicsView view;
	EXPECT_TRUE(view.scene() == NULL);

	DwGraphicsScene scene;
	view.setScene(&scene);
	EXPECT_EQ(view.viewPortRect() , DwRectF());

	view.setViewPortRect(0, 0, 100, 100);
	EXPECT_EQ(view.viewPortRect() , DwRectF(0, 0, 100, 100));
}

//TODO ADD TEST