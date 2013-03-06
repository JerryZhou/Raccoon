
DwGraphicsScene * dependScene = NULL;
Dw::GraphicsSceneNotify sceneNotifyMark = Dw::SceneNotifyMax;
DwGraphicsScene *sceneNotifySceneMark = NULL;
DwGraphicsItem *sceneNotifyItemMark = NULL;

class GuiTestGraphicsSceneListener : public DwGraphicsSceneListener
{
public:
	GuiTestGraphicsSceneListener()
	{
		;
	}

	virtual ~GuiTestGraphicsSceneListener()
	{
		remAllDepends();
	}

	virtual void onAdded(DwGraphicsScene *scene)
	{
		dependScene = scene;
	}

	virtual void onRemoved(DwGraphicsScene *scene)
	{
		dependScene = scene;
	}

	virtual void onNotify(Dw::GraphicsSceneNotify notify, DwGraphicsScene *scene, DwGraphicsItem* item)
	{
		sceneNotifyMark = notify;
		sceneNotifySceneMark = scene;
		sceneNotifyItemMark = item;
	}
protected:
private:
};

TEST(DwGraphicsSceneListener, construct)
{
	GuiTestGraphicsSceneListener listener;
	EXPECT_EQ(listener.dependSize(), 0);
}

TEST(DwGraphicsSceneListener, onadded)
{
	GuiTestGraphicsSceneListener listener;
	EXPECT_EQ(listener.dependSize(), 0);

	DwGraphicsScene scene;
	dependScene = NULL;
	listener.addDepend(&scene);
	EXPECT_EQ(listener.dependSize(), 1);
	EXPECT_EQ(dependScene , &scene);
}

TEST(DwGraphicsSceneListener, onremoved)
{
	GuiTestGraphicsSceneListener listener;
	EXPECT_EQ(listener.dependSize(), 0);

	DwGraphicsScene scene;
	dependScene = NULL;
	listener.addDepend(&scene);
	EXPECT_EQ(listener.dependSize(), 1);
	EXPECT_EQ(dependScene , &scene);

	dependScene = NULL;
	listener.remDepend(&scene);
	EXPECT_EQ(listener.dependSize(), 0);
	EXPECT_EQ(dependScene , &scene);
}

TEST(DwGraphicsSceneListener, onnotify)
{
	GuiTestGraphicsSceneListener listener;
	EXPECT_EQ(listener.dependSize(), 0);

	DwGraphicsScene scene;
	dependScene = NULL;
	listener.addDepend(&scene);
	EXPECT_EQ(listener.dependSize(), 1);
	EXPECT_EQ(dependScene , &scene);

	DwGraphicsItem root;
	sceneNotifyMark = Dw::SceneNotifyMax;
	sceneNotifySceneMark = NULL;
	sceneNotifyItemMark = NULL;
	scene.addItem(&root);

	EXPECT_EQ(sceneNotifyMark, Dw::SceneNotifyAfterAdd);
	EXPECT_EQ(sceneNotifySceneMark, &scene);
	EXPECT_EQ(sceneNotifyItemMark, &root);
}

TEST(DwGraphicsSceneListener, adddepend)
{
	GuiTestGraphicsSceneListener listener;
	EXPECT_EQ(listener.dependSize(), 0);

	DwGraphicsScene scene;
	dependScene = NULL;
	listener.addDepend(&scene);
	EXPECT_EQ(listener.dependSize(), 1);
	EXPECT_EQ(dependScene , &scene);

	DwGraphicsScene scene1;
	dependScene = NULL;
	listener.addDepend(&scene1);
	EXPECT_EQ(listener.dependSize(), 2);
	EXPECT_EQ(dependScene , &scene1);
}

TEST(DwGraphicsSceneListener, remDepend)
{
	GuiTestGraphicsSceneListener listener;
	EXPECT_EQ(listener.dependSize(), 0);

	DwGraphicsScene scene;
	dependScene = NULL;
	listener.addDepend(&scene);
	EXPECT_EQ(listener.dependSize(), 1);
	EXPECT_EQ(dependScene , &scene);

	DwGraphicsScene scene1;
	dependScene = NULL;
	listener.addDepend(&scene1);
	EXPECT_EQ(listener.dependSize(), 2);
	EXPECT_EQ(dependScene , &scene1);

	dependScene = NULL;
	listener.remDepend(&scene);
	EXPECT_EQ(listener.dependSize(), 1);
	EXPECT_EQ(dependScene , &scene);

	dependScene = NULL;
	listener.remDepend(&scene1);
	EXPECT_EQ(listener.dependSize(), 0);
	EXPECT_EQ(dependScene , &scene1);
}

TEST(DwGraphicsSceneListener, hasdepend)
{
	GuiTestGraphicsSceneListener listener;
	EXPECT_EQ(listener.dependSize(), 0);

	DwGraphicsScene scene;
	dependScene = NULL;
	listener.addDepend(&scene);
	EXPECT_EQ(listener.dependSize(), 1);
	EXPECT_EQ(dependScene , &scene);
	EXPECT_TRUE(listener.hasDepend(&scene));

	DwGraphicsScene scene1;
	dependScene = NULL;
	listener.addDepend(&scene1);
	EXPECT_EQ(listener.dependSize(), 2);
	EXPECT_EQ(dependScene , &scene1);
	EXPECT_TRUE(listener.hasDepend(&scene1));

	dependScene = NULL;
	listener.remDepend(&scene);
	EXPECT_EQ(listener.dependSize(), 1);
	EXPECT_EQ(dependScene , &scene);
	EXPECT_FALSE(listener.hasDepend(&scene));

	dependScene = NULL;
	listener.remDepend(&scene1);
	EXPECT_EQ(listener.dependSize(), 0);
	EXPECT_EQ(dependScene , &scene1);
	EXPECT_FALSE(listener.hasDepend(&scene1));
}

TEST(DwGraphicsSceneListener, dependat)
{
	GuiTestGraphicsSceneListener listener;
	EXPECT_EQ(listener.dependSize(), 0);

	DwGraphicsScene scene;
	dependScene = NULL;
	listener.addDepend(&scene);
	EXPECT_EQ(listener.dependSize(), 1);
	EXPECT_EQ(dependScene , &scene);
	EXPECT_EQ(listener.dependAt(0), &scene);

	DwGraphicsScene scene1;
	dependScene = NULL;
	listener.addDepend(&scene1);
	EXPECT_EQ(listener.dependSize(), 2);
	EXPECT_EQ(dependScene , &scene1);
	EXPECT_TRUE(listener.hasDepend(&scene1));
	EXPECT_EQ(listener.dependAt(1), &scene1);
}

TEST(DwGraphicsSceneListener, remalldepends)
{
	GuiTestGraphicsSceneListener listener;
	EXPECT_EQ(listener.dependSize(), 0);

	DwGraphicsScene scene;
	dependScene = NULL;
	listener.addDepend(&scene);
	EXPECT_EQ(listener.dependSize(), 1);
	EXPECT_EQ(dependScene , &scene);
	EXPECT_EQ(listener.dependAt(0), &scene);

	DwGraphicsScene scene1;
	dependScene = NULL;
	listener.addDepend(&scene1);
	EXPECT_EQ(listener.dependSize(), 2);
	EXPECT_EQ(dependScene , &scene1);
	EXPECT_TRUE(listener.hasDepend(&scene1));
	EXPECT_EQ(listener.dependAt(1), &scene1);

	listener.remAllDepends();
	EXPECT_EQ(listener.dependSize(), 0);
}

DwGraphicsScene *sceneNotifySceneMark2 = NULL;
DwGraphicsItem *sceneNotifyItemMark2 = NULL;
static void eg_pfnSceneLister(DwGraphicsScene* scene, DwGraphicsItem* item)
{
	sceneNotifySceneMark2 = scene;
	sceneNotifyItemMark2 = item;
}

TEST(DwGraphicsSceneListener, addnotifylister)
{
	GuiTestGraphicsSceneListener listener;
	EXPECT_EQ(listener.dependSize(), 0);

	DwGraphicsScene scene;
	dependScene = NULL;
	listener.addDepend(&scene);
	EXPECT_EQ(listener.dependSize(), 1);
	EXPECT_EQ(dependScene , &scene);

	listener.addNotifyLister(Dw::SceneNotifyAddItem, eg_pfnSceneLister);

	DwGraphicsItem root;
	sceneNotifySceneMark2 = NULL;
	sceneNotifyItemMark2 = NULL;
	scene.addItem(&root);

	EXPECT_EQ(sceneNotifySceneMark2, &scene);
	EXPECT_EQ(sceneNotifyItemMark2, &root);
}

TEST(DwGraphicsSceneListener, removenotifylister)
{
	GuiTestGraphicsSceneListener listener;
	EXPECT_EQ(listener.dependSize(), 0);

	DwGraphicsScene scene;
	dependScene = NULL;
	listener.addDepend(&scene);
	EXPECT_EQ(listener.dependSize(), 1);
	EXPECT_EQ(dependScene , &scene);

	listener.addNotifyLister(Dw::SceneNotifyAddItem, eg_pfnSceneLister);

	DwGraphicsItem root;
	sceneNotifySceneMark2 = NULL;
	sceneNotifyItemMark2 = NULL;
	scene.addItem(&root);

	EXPECT_EQ(sceneNotifySceneMark2, &scene);
	EXPECT_EQ(sceneNotifyItemMark2, &root);

	listener.removeNotifyLister(Dw::SceneNotifyAddItem);

	DwGraphicsItem root1;
	sceneNotifySceneMark2 = NULL;
	sceneNotifyItemMark2 = NULL;
	scene.addItem(&root1);
	EXPECT_TRUE(sceneNotifySceneMark2 == NULL);
	EXPECT_TRUE(sceneNotifyItemMark2 == NULL);
}
