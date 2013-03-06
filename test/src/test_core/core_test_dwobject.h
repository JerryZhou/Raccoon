int level = 0;
int object_count = 0;
int object_bytes = 0;
void createObjectTree(DwObject *parent)
{
    if(level > 3)
        return;

    for(int i = 0; i < 3; i++)
    {
        DwObject *p = new DwObject(parent);
        //		p->setName(DwString::number((int)p, 16));
        level++;

        createObjectTree(p);

        level--;
    }
}

void dumpObjectTree(DwObject *parent)
{
    object_count++;

    int count = parent->childCount();
    for(int i = 0; i < count; i++)
    {
        DwObject *p = parent->child(i);

        dumpObjectTree(p);
    }
}

TEST(DwObject, DwObject)
{
    DwObject root;
    root.setObjectName(L"root");

    EXPECT_EQ(DwString(L"root"), root.objectName());

    createObjectTree(&root);

    dumpObjectTree(&root);

    EXPECT_EQ(121, object_count);
}

class Window : public DwObject
{

};

TEST(DwObject, subclass)
{
}