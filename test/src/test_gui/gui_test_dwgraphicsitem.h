
TEST(DwGraphicsItem, construct)
{
	DwGraphicsItem item;

	EXPECT_TRUE(item.parentItem() == NULL);
	EXPECT_TRUE(item.scene() == NULL);
}

TEST(DwGraphicsItem, setparent)
{
	DwGraphicsItem item;

	EXPECT_TRUE(item.parentItem() == NULL);
	DwGraphicsItem parent;
	item.setParentItem(&parent);
	EXPECT_EQ(item.parentItem(), &parent);
	item.setParentItem(NULL);
	EXPECT_TRUE(item.parentItem() == NULL);
}

TEST(DwGraphicsItem, toplevelitem)
{
	DwGraphicsItem item;

	EXPECT_TRUE(item.topLevelItem() == &item);
	DwGraphicsItem parent;
	item.setParentItem(&parent);
	EXPECT_EQ(item.topLevelItem(), &parent);
	item.setParentItem(NULL);
	EXPECT_TRUE(item.topLevelItem() == &item);
}

TEST(DwGraphicsItem, commonancestoritem)
{
	DwGraphicsItem item;

	EXPECT_TRUE(item.commonAncestorItem(NULL) == NULL);
	EXPECT_EQ(item.commonAncestorItem(&item), &item);
	DwGraphicsItem parent;
	item.setParentItem(&parent);
	EXPECT_EQ(item.commonAncestorItem(&parent), &parent);
	EXPECT_EQ(parent.commonAncestorItem(&item), &parent);

	DwGraphicsItem item2;
	EXPECT_TRUE(item.commonAncestorItem(&item2) == NULL);
	EXPECT_TRUE(item2.commonAncestorItem(&item) == NULL);

	item.setParentItem(NULL);
	EXPECT_TRUE(item.commonAncestorItem(&parent) == NULL);
}

TEST(DwGraphicsItem, children)
{
	DwGraphicsItem item;

	EXPECT_TRUE(item.children().size() == 0);
	DwGraphicsItem parent;
	EXPECT_TRUE(parent.children().size() == 0);
	item.setParentItem(&parent);
	EXPECT_TRUE(parent.children().size() == 1);
	EXPECT_TRUE(parent.children().at(0) == &item);

	item.setParentItem(NULL);
	EXPECT_TRUE(parent.children().size() == 0);
}

TEST(DwGraphicsItem, depth)
{
	DwGraphicsItem item;

	EXPECT_TRUE(item.depth() == 0);
	DwGraphicsItem parent;
	EXPECT_TRUE(parent.depth() == 0);
	item.setParentItem(&parent);
	EXPECT_TRUE(item.depth() == 1);
	EXPECT_TRUE(parent.depth() == 0);

	item.setParentItem(NULL);
	EXPECT_TRUE(item.depth() == 0);
	EXPECT_TRUE(parent.depth() == 0);
}

TEST(DwGraphicsItem, boundingrect)
{
	DwGraphicsItem item;

	EXPECT_EQ(item.boundingRect() , DwRectF());
	EXPECT_EQ(item.childrenBoundingRect() , DwRectF());
	EXPECT_EQ(item.sceneBoundingRect() , DwRectF());
	EXPECT_EQ(item.sceneEffectiveBoundingRect() , DwRectF());

	item.setBoundingRect(DwRectF(0, 0, 10, 10));
	EXPECT_EQ(item.boundingRect() , DwRectF(0, 0, 10, 10));
	EXPECT_EQ(item.childrenBoundingRect() , DwRectF());
	EXPECT_EQ(item.sceneBoundingRect() , DwRectF(0, 0, 10, 10));
	EXPECT_EQ(item.sceneEffectiveBoundingRect() , DwRectF(0, 0, 10, 10));

	DwGraphicsItem child(&item);
	child.setBoundingRect(DwRectF(0, 0, 20, 20));
	EXPECT_EQ(item.boundingRect() , DwRectF(0, 0, 10, 10));
	EXPECT_EQ(item.childrenBoundingRect() , DwRectF(0, 0, 20, 20));
	EXPECT_EQ(item.sceneBoundingRect() , DwRectF(0, 0, 10, 10));
	EXPECT_EQ(item.sceneEffectiveBoundingRect() , DwRectF(0, 0, 20, 20));

	child.setParentItem(NULL);
	EXPECT_EQ(item.boundingRect() , DwRectF(0, 0, 10, 10));
	EXPECT_EQ(item.childrenBoundingRect() , DwRectF());
	EXPECT_EQ(item.sceneBoundingRect() , DwRectF(0, 0, 10, 10));
	EXPECT_EQ(item.sceneEffectiveBoundingRect() , DwRectF(0, 0, 10, 10));
}

TEST(DwGraphicsItem, cliprect)
{
	DwGraphicsItem item;

	EXPECT_EQ(item.clipRect() , DwRectF());
	item.setBoundingRect(DwRectF(0, 0, 10, 10));
	EXPECT_EQ(item.clipRect() , DwRectF(0, 0, 10, 10));
	item.setClipRect(DwRectF(0, 0, 20, 20));
	EXPECT_EQ(item.boundingRect() , DwRectF(0, 0, 10, 10));
	EXPECT_EQ(item.clipRect() , DwRectF(0, 0, 20, 20));
}

TEST(DwGraphicsItem, tabOrder)
{
    DwGraphicsItem item;

    EXPECT_TRUE(item.focusNext()==&item);
    EXPECT_TRUE(item.focusPrev()==&item);

    {
        DwGraphicsItem next;
        item.setFocusNext(&next);
        EXPECT_TRUE(item.focusNext()==&next);
        EXPECT_TRUE(item.focusPrev()==&next);
        EXPECT_TRUE(next.focusNext()==&item);
        EXPECT_TRUE(next.focusPrev()==&item);
    }

    EXPECT_TRUE(item.focusNext()==&item);
    EXPECT_TRUE(item.focusPrev()==&item);
    
    {
        DwGraphicsItem next;
        item.setFocusNext(&next);
        EXPECT_TRUE(item.focusNext()==&next);
        EXPECT_TRUE(item.focusPrev()==&next);
        EXPECT_TRUE(next.focusNext()==&item);
        EXPECT_TRUE(next.focusPrev()==&item);

        {
            DwGraphicsItem item2;
            EXPECT_TRUE(item2.focusNext()==&item2);
            EXPECT_TRUE(item2.focusPrev()==&item2);

            {
                DwGraphicsItem next2;
                item2.setFocusNext(&next2);
                EXPECT_TRUE(item2.focusNext()==&next2);
                EXPECT_TRUE(item2.focusPrev()==&next2);
                EXPECT_TRUE(next2.focusNext()==&item2);
                EXPECT_TRUE(next2.focusPrev()==&item2);

                next.setFocusNext(&next2);
                EXPECT_TRUE(item2.focusNext()==&item2);
                EXPECT_TRUE(item2.focusPrev()==&item2);

                EXPECT_TRUE(item.focusNext()==&next);
                EXPECT_TRUE(item.focusPrev()==&next2);
                EXPECT_TRUE(next.focusNext()==&next2);
                EXPECT_TRUE(next.focusPrev()==&item);
                EXPECT_TRUE(next2.focusNext()==&item);
                EXPECT_TRUE(next2.focusPrev()==&next);
            }
            
        }
        EXPECT_TRUE(item.focusNext()==&next);
        EXPECT_TRUE(item.focusPrev()==&next);
        EXPECT_TRUE(next.focusNext()==&item);
        EXPECT_TRUE(next.focusPrev()==&item);
    }
}

// TODO: xx