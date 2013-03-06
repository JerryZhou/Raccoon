TEST(DwXmlDocument, LoadSave)
{
    //loadfromstring
    DwXmlDocument d;
    d.loadFromString(L"<root name=\"test\"><val1></val1><val1><val1_1><val1_1_1></val1_1_1></val1_1></val1><val2 type=\"hello\"></val2></root>");

    TCHAR ch[MAX_PATH]= {0};
    GetModuleFileName(NULL, ch, MAX_PATH);
    DwString s(ch);
    int n = s.lastIndexOf(L'\\');
    DwString dir = s.left(n);
    DwString pathR = dir + "\\orign.xml";

    //savetofile
    d.saveToFile(pathR);

    //loadfromfile
    DwXmlDocument dr;
    dr.loadFromFile(pathR);
}


TEST(DwXmlDocument, allelements)
{
    TCHAR ch[MAX_PATH]= {0};
    GetModuleFileName(NULL, ch, MAX_PATH);
    DwString s(ch);
    int n = s.lastIndexOf(L'\\');
    DwString dir = s.left(n);
    DwString pathR = dir + "\\orign.xml";

    //loadfromfile
    DwXmlDocument dr;
    dr.loadFromFile(pathR);

    //read all elment
    DwXmlElementRef root = dr.rootElement();
    EXPECT_EQ(root.tag(), DwString("root"));

    int cn = root.childCount();
    EXPECT_EQ(cn, 3);
    for (int i = 0; i < cn; i++)
    {
        EXPECT_EQ((int)root[i].isValid(), 1);
        int ccn = root[i].childCount();
        for (int j = 0; j < ccn; j++)
        {
            EXPECT_EQ((int)root[j].isValid(), 1);
        }
    }
}

TEST(DwXmlDocument, xPath)
{
    TCHAR ch[MAX_PATH]= {0};
    GetModuleFileName(NULL, ch, MAX_PATH);
    DwString s(ch);
    int n = s.lastIndexOf(L'\\');
    DwString dir = s.left(n);
    DwString pathR = dir + "\\orign.xml";

    //loadfromfile
    DwXmlDocument dr;
    dr.loadFromFile(pathR);

    //xpath readElement and val
    //if the element will find only one in document,you can use value() function
    EXPECT_EQ(dr.value("/root/val2@type"), DwString("hello"));
    DwXmlElementRefList val1_1_1 = dr["/root/val1/val1_1/val1_1_1"];
    EXPECT_EQ(val1_1_1[0].tag(), DwString("val1_1_1"));
}

TEST(DwXmlDocument, setVal)
{
    TCHAR ch[MAX_PATH]= {0};
    GetModuleFileName(NULL, ch, MAX_PATH);
    DwString s(ch);
    int n = s.lastIndexOf(L'\\');
    DwString dir = s.left(n);
    DwString pathR = dir + "\\orign.xml";

    //loadfromfile
    DwXmlDocument dr;
    dr.loadFromFile(pathR);

    //setValue
    dr.setValue("/root/val2", "i am val2");
    dr.setValue("/root/val2@attr", "attrval2");
    dr.setValue("/root/val1/val1_1/val1_1_1", "i am val1_1_1");
    EXPECT_EQ(dr["/root/val2"][0].text(), DwString("i am val2"));
    EXPECT_EQ(dr.value("/root/val1/val1_1/val1_1_1"), DwString("i am val1_1_1"));
    EXPECT_EQ(dr.value("/root/val2@attr"), DwString("attrval2"));
}

TEST(DwXmlDocument, createchild)
{
    TCHAR ch[MAX_PATH]= {0};
    GetModuleFileName(NULL, ch, MAX_PATH);
    DwString s(ch);
    int n = s.lastIndexOf(L'\\');
    DwString dir = s.left(n);
    DwString pathR = dir + "\\orign.xml";

    //loadfromfile
    DwXmlDocument dr;
    dr.loadFromFile(pathR);

    DwXmlElementRef root = dr.rootElement();

    //createchild
    DwXmlElementRef rc = root.createChild();
    rc.setTag("val3");
    rc.setText("i am val3");
    EXPECT_EQ(dr.value("/root/val3"), "i am val3");
}

TEST(DwXmlDocument, createchildByPath)
{
    TCHAR ch[MAX_PATH]= {0};
    GetModuleFileName(NULL, ch, MAX_PATH);
    DwString s(ch);
    int n = s.lastIndexOf(L'\\');
    DwString dir = s.left(n);
    DwString pathR = dir + "\\orign.xml";

    //loadfromfile
    DwXmlDocument dr;
    dr.loadFromFile(pathR);

    //createelementbypath
    DwXmlElementRefList crl = dr.createElementByPath("/root/val1/newVal1");
    EXPECT_EQ(crl.size(), 2);
    crl[0].setText("first");
    crl[1].setText("second");

    EXPECT_EQ(dr["/root/val1/newVal1"][0].text(), DwString("first"));
    EXPECT_EQ(dr["/root/val1/newVal1"][1].text(), DwString("second"));

    //saveToFile
    DwString pathNew = dir + "\\new.xml";
    EXPECT_TRUE(dr.saveToFile(pathNew));

    //saveToStream
    DwString tos;
    EXPECT_TRUE(dr.saveToStream(tos));
}

TEST(DwXmlElementRef, delChild)
{
    //loadfromstring
    DwXmlDocument d;
    d.loadFromString(L"<root><val1></val1><val1><val1_1><val1_1_1></val1_1_1></val1_1></val1><val2 type=\"hello\"></val2></root>");
    
    DwXmlElementRef root = d.rootElement();
    int n = root.childCount();
    DwXmlElementRef c1 = root.child(0);
    c1.setAttribute("hello", "world");
    EXPECT_EQ(c1.attribute("hello"), "world");

    c1.removeAttribute("hello");
    EXPECT_EQ(c1.attributeCount(), 0);

    root.deleteChild(c1);

    EXPECT_EQ(n - root.childCount(), 1);
}

TEST(DwXmlDocument, Excp)
{
    DwXmlDocument d;
    DwXmlElementRefList r = d.createElementByPath("/root");
    EXPECT_EQ(r.size(), 1);

    DwXmlElementRefList r2 = d.createElementByPath("root2");
    EXPECT_EQ(r2.size(), 0);

    DwXmlElementRefList r3 = d.createElementByPath("root/hello/");
    EXPECT_EQ(r3.size(), 0);

    DwXmlDocument d2;
    EXPECT_FALSE(d.loadFromString(L"<root></root><root1></root1>"));

    DwXmlDocument d3;
    d3.loadFromString("<root></root>");
    //DwXmlElementRef dr = d3.rootElement();
    EXPECT_EQ(d3.value("/root@attr"), DwString(""));
    EXPECT_EQ(d3.value("/root@"), DwString(""));
}