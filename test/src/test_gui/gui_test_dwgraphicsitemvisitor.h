
class GuiTestGraphicsItemVisitor : public DwGraphicsItemVisitor
{
public:
	virtual void interest(DwGraphicsItem* item)
	{
		m_visitItems.append(item);
	}

	DwVector<DwGraphicsItem *>& visitItems()
	{
		return m_visitItems;
	}

protected:
	DwVector<DwGraphicsItem *> m_visitItems;
};

TEST(GuiTestGraphicsItemVisitor, constructor)
{
	GuiTestGraphicsItemVisitor visitor;
	EXPECT_EQ(visitor.visitItems().size(), 0);
}

TEST(DwGraphicsItemPreorderIterator, constructor)
{
	GuiTestGraphicsItemVisitor visitor;
	EXPECT_EQ(visitor.visitItems().size(), 0);

	DwGraphicsItemPreorderIterator preorderIterator(NULL, &visitor);
	EXPECT_TRUE(preorderIterator.next() == NULL);
}

TEST(DwGraphicsItemPreorderIterator, run)
{
	GuiTestGraphicsItemVisitor visitor;
	EXPECT_EQ(visitor.visitItems().size(), 0);

	DwGraphicsItemPreorderIterator preorderIterator(NULL, &visitor);
	EXPECT_TRUE(preorderIterator.next() == NULL);

	DwGraphicsItem Droot;
	DwGraphicsItem *root = &Droot;
	preorderIterator.initIterator(root, &visitor);
	preorderIterator.run();
	EXPECT_EQ(visitor.visitItems().size(), 1);
	EXPECT_EQ(visitor.visitItems().at(0), root);

	DwGraphicsItem *child0 = new DwGraphicsItem(root);
	DwGraphicsItem *child1 = new DwGraphicsItem(root);
	
	visitor.visitItems().clear();
	preorderIterator.initIterator(root, &visitor);
	preorderIterator.run();
	EXPECT_EQ(visitor.visitItems().size(), 3);
	EXPECT_EQ(visitor.visitItems().at(0), root);
	EXPECT_EQ(visitor.visitItems().at(1), child0);
	EXPECT_EQ(visitor.visitItems().at(2), child1);
}

TEST(DwGraphicsItemPostorderIterator, constructor)
{
	GuiTestGraphicsItemVisitor visitor;
	EXPECT_EQ(visitor.visitItems().size(), 0);

	DwGraphicsItemPostorderIterator postorderIterator(NULL, &visitor);
	EXPECT_TRUE(postorderIterator.next() == NULL);
}

TEST(DwGraphicsItemPostorderIterator, run)
{
	GuiTestGraphicsItemVisitor visitor;
	EXPECT_EQ(visitor.visitItems().size(), 0);

	DwGraphicsItemPostorderIterator postorderIterator(NULL, &visitor);
	EXPECT_TRUE(postorderIterator.next() == NULL);

	DwGraphicsItem Droot;
	DwGraphicsItem *root = &Droot;
	postorderIterator.initIterator(root, &visitor);
	postorderIterator.run();
	EXPECT_EQ(visitor.visitItems().size(), 1);
	EXPECT_EQ(visitor.visitItems().at(0), root);

	DwGraphicsItem *child0 = new DwGraphicsItem(root);
	DwGraphicsItem *child1 = new DwGraphicsItem(root);

	visitor.visitItems().clear();
	postorderIterator.initIterator(root, &visitor);
	EXPECT_EQ(postorderIterator.run(), 3);
	EXPECT_EQ(visitor.visitItems().size(), 3);
	EXPECT_EQ(visitor.visitItems().at(0), child1);
	EXPECT_EQ(visitor.visitItems().at(1), child0);
	EXPECT_EQ(visitor.visitItems().at(2), root);

	EXPECT_EQ(postorderIterator.run(-2), 0);
	EXPECT_EQ(postorderIterator.run(-1), 0);
	EXPECT_EQ(postorderIterator.run(1), 0);
}