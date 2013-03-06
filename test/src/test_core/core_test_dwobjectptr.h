TEST(DwObjectPtr, DwObjectPtr)
{
    DwObject *o = new DwObject();
    DwObjectPtr<DwObject> p(o);

    EXPECT_EQ(o, p.data());
    EXPECT_TRUE(p);

    DwObjectPtr<DwObject> p2;
    p2 = p;
    EXPECT_EQ(o, p2.data());
    EXPECT_TRUE(p2);

    delete o;

    EXPECT_EQ(NULL, p.data());
    EXPECT_FALSE(p);

    EXPECT_EQ(NULL, p2.data());
    EXPECT_FALSE(p2);
}

TEST(DwObjectPtr, change)
{
    DwObject *o1 = new DwObject();
    DwObjectPtr<DwObject> p1(o1);
    DwObjectPtr<DwObject> p2(o1);

    DwObject *o2 = new DwObject();
    p1 = o2;
    p2 = o1;

    delete o1;
    EXPECT_EQ(NULL, p2.data());
    EXPECT_EQ(o2, p1.data());

    delete o2;
    EXPECT_EQ(NULL, p1.data());
    EXPECT_EQ(NULL, p2.data());
}

TEST(DwObjectPtr, performance)
{
    DwLinkedList<DwObject*> objList;
    DwLinkedList< DwObjectPtr<DwObject> > objPtrList;
    for(int i = 0; i < 1000; i++)
    {
        DwObject *o = new DwObject();
        objList.append(o);

        for(int j = 0; j < 100; j++)
        {
            objPtrList.append(DwObjectPtr<DwObject>(o));
        }
    }

    for(DwLinkedList<DwObject*>::iterator it = objList.begin(); it != objList.end(); ++it)
    {
        delete *it;        
    }
}

/*
TEST(DwObjectPointer, DwObjectPointer)
{
    DwObject *o = new DwObject();
    DwObjectPointer<DwObject> p(o);

    EXPECT_EQ(o, p.data());
    EXPECT_TRUE(p);

    DwObjectPointer<DwObject> p2;
    p2 = p;
    EXPECT_EQ(o, p2.data());
    EXPECT_TRUE(p2);

    delete o;

    EXPECT_EQ(NULL, p.data());
    EXPECT_FALSE(p);

    EXPECT_EQ(NULL, p2.data());
    EXPECT_FALSE(p2);
}*/