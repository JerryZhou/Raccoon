TEST(DwLinkedList, DwLinkedList)
{
    DwLinkedList<int> l1;
    l1.append(100);
    l1.append(200);
    l1.append(300);

    EXPECT_EQ(3, l1.count());

    DwLinkedList<int> l2 = l1;
    EXPECT_EQ(3, l2.count());
}

TEST(DwLinkedList, insert_remove)
{
    DwLinkedList<int> l1;

    for(int i = 0; i < 100; i++)
    {
        l1.append(i);
    }
    EXPECT_EQ(100, l1.count());

    int sum = sumOfContainer(l1);
    EXPECT_EQ(4950, sum);

    DwLinkedList<int>::iterator it = l1.begin();
    ++it;
    l1.insert(it, 100);
    sum = sumOfContainer(l1);
    EXPECT_EQ(5050, sum);

    l1.removeOne(99);
    sum = sumOfContainer(l1);
    EXPECT_EQ(5050 - 99, sum);
}