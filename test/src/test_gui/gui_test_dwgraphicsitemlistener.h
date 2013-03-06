
static DwGraphicsItem *opItem = NULL;

class GuiTestGraphicsItemListener : public DwGraphicsItemListener
{
public:
	virtual ~GuiTestGraphicsItemListener()
	{
		remAllDepends();
	}

	virtual void onChange(Dw::ChangeType change, 
		DwGraphicsItem* item, const DwVariant& newOne, const DwVariant& oldOne) 
	{
		DW_UNUSED(change);
		DW_UNUSED(item);
		DW_UNUSED(newOne);
		DW_UNUSED(oldOne);
		opItem = item;
	}

	virtual void onAdded(DwGraphicsItem* item) 
	{
		DW_UNUSED(item);
		opItem = item;
	}

	virtual void onRemoved(DwGraphicsItem* item) 
	{
		DW_UNUSED(item);
		opItem = item;
	}

protected:
private:
};

TEST(GuiTestGraphicsItemListener, construct)
{
	GuiTestGraphicsItemListener listener;
	EXPECT_TRUE(listener.dependSize() == 0);

	DwGraphicsItem item;
	EXPECT_FALSE(listener.hasDepend(&item));

	listener.addDepend(&item);
	EXPECT_TRUE(listener.dependSize() == 1);
	EXPECT_TRUE(listener.hasDepend(&item));
	EXPECT_EQ(listener.dependAt(0), &item);

	listener.remDepend(&item);
	EXPECT_TRUE(listener.dependSize() == 0);
	EXPECT_FALSE(listener.hasDepend(&item));
}

TEST(GuiTestGraphicsItemListener, hasdepend)
{
	GuiTestGraphicsItemListener listener;
	EXPECT_TRUE(listener.dependSize() == 0);

	DwGraphicsItem item;
	EXPECT_FALSE(listener.hasDepend(&item));
}

TEST(GuiTestGraphicsItemListener, adddepend)
{
	GuiTestGraphicsItemListener listener;
	EXPECT_TRUE(listener.dependSize() == 0);

	DwGraphicsItem item;
	EXPECT_FALSE(listener.hasDepend(&item));

	listener.addDepend(&item);
	EXPECT_TRUE(listener.dependSize() == 1);
	EXPECT_TRUE(listener.hasDepend(&item));
	EXPECT_EQ(listener.dependAt(0), &item);
}

TEST(GuiTestGraphicsItemListener, remdepend)
{
	GuiTestGraphicsItemListener listener;
	EXPECT_TRUE(listener.dependSize() == 0);

	DwGraphicsItem item;
	EXPECT_FALSE(listener.hasDepend(&item));

	listener.addDepend(&item);
	EXPECT_TRUE(listener.dependSize() == 1);
	EXPECT_TRUE(listener.hasDepend(&item));
	EXPECT_EQ(listener.dependAt(0), &item);

	listener.remDepend(&item);
	EXPECT_TRUE(listener.dependSize() == 0);
	EXPECT_FALSE(listener.hasDepend(&item));
}

TEST(GuiTestGraphicsItemListener, remalldepends)
{
	GuiTestGraphicsItemListener listener;
	EXPECT_TRUE(listener.dependSize() == 0);

	DwGraphicsItem item;
	EXPECT_FALSE(listener.hasDepend(&item));

	listener.addDepend(&item);
	EXPECT_TRUE(listener.dependSize() == 1);
	EXPECT_TRUE(listener.hasDepend(&item));
	EXPECT_EQ(listener.dependAt(0), &item);

	{
		DwGraphicsItem item2;
		listener.addDepend(&item2);
		EXPECT_TRUE(listener.dependSize() == 2);
		EXPECT_TRUE(listener.hasDepend(&item2));
	}
	EXPECT_TRUE(listener.dependSize() == 1);

	listener.remAllDepends();
	EXPECT_TRUE(listener.dependSize() == 0);
	EXPECT_FALSE(listener.hasDepend(&item));
}

TEST(GuiTestGraphicsItemListener, onadded)
{
	GuiTestGraphicsItemListener listener;
	EXPECT_TRUE(listener.dependSize() == 0);

	opItem = NULL;
	DwGraphicsItem item;
	EXPECT_FALSE(listener.hasDepend(&item));
	listener.addDepend(&item);
	EXPECT_TRUE(opItem == &item);
}

TEST(GuiTestGraphicsItemListener, onremoved)
{
	GuiTestGraphicsItemListener listener;
	EXPECT_TRUE(listener.dependSize() == 0);

	opItem = NULL;
	DwGraphicsItem item;
	EXPECT_FALSE(listener.hasDepend(&item));
	listener.addDepend(&item);
	EXPECT_TRUE(opItem == &item);
	opItem = NULL;
	listener.remDepend(&item);
	EXPECT_TRUE(opItem == &item);
}

TEST(GuiTestGraphicsItemListener, onchange)
{
	GuiTestGraphicsItemListener listener;
	EXPECT_TRUE(listener.dependSize() == 0);

	opItem = NULL;
	DwGraphicsItem item;
	EXPECT_FALSE(listener.hasDepend(&item));
	listener.addDepend(&item);
	EXPECT_TRUE(opItem == &item);
	opItem = NULL;
	listener.remDepend(&item);
	EXPECT_TRUE(opItem == &item);

	opItem = NULL;
	listener.setChange(Dw::Geometry, true);
	listener.addDepend(&item);
	item.setPos(20, 20);
	EXPECT_TRUE(opItem == &item);
}