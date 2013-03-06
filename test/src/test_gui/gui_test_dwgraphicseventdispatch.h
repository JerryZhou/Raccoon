TEST(DwGraphicsEventDispatch, construct)
{
	DwGraphicsEventDispatch * dispatch = dwGraphicsEventDispatch();
	EXPECT_TRUE(dispatch != NULL);
	EXPECT_TRUE(dispatch == dwGraphicsEventDispatch());
}

static bool handlered = false;
static void onSceneEventHandler(DwGraphicsScene *, DwEvent *)
{
	handlered = true;
}

TEST(DwGraphicsEventDispatch, dispatchscene)
{
	DwGraphicsScene scene;
	scene.setEventHandler(new DwDefaultSceneEventHandler(&scene));
	scene.eventHandler()->addEventCallBack(DwSceneEvent::EventId(), onSceneEventHandler);
	DwSceneEvent evt;
	dwGraphicsEventDispatch()->dispatchEvent(&scene, &evt);
	EXPECT_TRUE(handlered);
}

static bool viewHandlered = false;
static void onViewEventHandler(DwGraphicsView *, DwEvent *)
{
	viewHandlered = true;
}

TEST(DwGraphicsEventDispatch, dispatchview)
{
	DwGraphicsScene scene;
	DwGraphicsView view(&scene);
	view.setEventHandler(new DwDefaultViewEventHandler(&view));
	view.eventHandler()->addEventCallBack(DwSceneEvent::EventId(), onViewEventHandler);
	DwSceneEvent evt;
	dwGraphicsEventDispatch()->dispatchEvent(&view, &evt);
	EXPECT_TRUE(viewHandlered);
}

static bool itemHandlered = false;
static void onItemEventHandler(DwGraphicsItem *, DwEvent *)
{
	itemHandlered = true;
}


static bool itemParentHandlered = false;
static void onParentItemEventHandler(DwGraphicsItem *, DwEvent *)
{
	itemParentHandlered = true;
}

TEST(DwGraphicsEventDispatch, dispatchitem)
{
	DwGraphicsScene scene;
	scene.setEventHandler(new DwDefaultSceneEventHandler(&scene));
	scene.eventHandler()->addEventCallBack(DwSceneEvent::EventId(), onSceneEventHandler);
	DwGraphicsView view(&scene);
	view.setEventHandler(new DwDefaultViewEventHandler(&view));
	view.eventHandler()->addEventCallBack(DwSceneEvent::EventId(), onViewEventHandler);
	DwGraphicsItem item;
	item.setEventHandler(new DwDefaultItemEventHandler(&item));
	item.eventHandler()->addEventCallBack(DwSceneEvent::EventId(), onItemEventHandler);
	scene.addItem(&item);

	DwSceneEvent evt;
	dwGraphicsEventDispatch()->dispatchEvent(&item, &evt);
	EXPECT_TRUE(itemHandlered);

	DwGraphicsItem parentItem;
	item.setParentItem(&parentItem);
	parentItem.setEventHandler(new DwDefaultItemEventHandler(&parentItem));
	parentItem.eventHandler()->addEventCallBack(DwSceneEvent::EventId(), onParentItemEventHandler);
	dwGraphicsEventDispatch()->dispatchEvent(&item, &evt);
	EXPECT_TRUE(itemParentHandlered == false);

	parentItem.eventHandler()->setAcceptedChildEvents(true);
	dwGraphicsEventDispatch()->dispatchEvent(&item, &evt);
	EXPECT_TRUE(itemParentHandlered == true);

	itemHandlered = false;
	evt.setUp(false);
	dwGraphicsEventDispatch()->dispatchEvent(&parentItem, &evt);
	EXPECT_TRUE(itemHandlered == false);
	item.eventHandler()->setAcceptedParentEvents(true);
	dwGraphicsEventDispatch()->dispatchEvent(&parentItem, &evt);
	EXPECT_TRUE(itemHandlered == true);

	itemHandlered = false;
	viewHandlered = false;
	handlered = false;
	evt.setUp(true);
	dwGraphicsEventDispatch()->dispatchEvent(&view, &evt);
	EXPECT_TRUE(viewHandlered == true);
	EXPECT_TRUE(handlered == true);
	EXPECT_TRUE(itemHandlered == false);

	itemHandlered = false;
	viewHandlered = false;
	handlered = false;
	evt.setInterestItem(&item);
	dwGraphicsEventDispatch()->dispatchEvent(&view, &evt);
	EXPECT_TRUE(viewHandlered == true);
	EXPECT_TRUE(handlered == true);
	EXPECT_TRUE(itemHandlered == true);

	item.setParentItem(NULL);
}