TEST(DwJsonObject, load)
{
	DwString s("{ \"firstName\": \"Brett\", \"lastName\":\"McLaughlin\", \"email\": \"aaaa\" }");

	DwJsonDocument d;
	d.loadFromData(s);

	DwJsonObjectRef jr = d.rootObject();
	DwJsonObjectRef jrc = jr.child("lastName");
	EXPECT_EQ(jrc.value(), "McLaughlin");
	EXPECT_EQ(jr.childCount(), 3);
	EXPECT_EQ(jr["lastName"].value(),"McLaughlin");
}

TEST(DwJsonObject, loadFromFile)
{
	DwJsonDocument d;
	d.loadFromFile("../testdata/core_json/orign.json");

	DwJsonObjectRef jr = d.rootObject();
	DwJsonObjectRef jrc = jr.child("lastName");
	EXPECT_EQ(jrc.value(), "McLaughlin");
	EXPECT_EQ(jr.childCount(), 3);
	EXPECT_EQ(jr["lastName"].value(),"McLaughlin");

    DwJsonDocument d2;
    EXPECT_FALSE(d2.loadFromFile("../testdata/core_json/orign.json2"));

    DwJsonDocument d3;
    EXPECT_FALSE(d3.loadFromFile("../testdata/core_json/noread.json"));
}

TEST(DwJsonObject, addValue)
{
	DwString s("{ \"firstName\": \"Brett\", \"lastName\":\"McLaughlin\", \"email\": \"aaaa\" }");

	DwJsonDocument d;
	d.loadFromData(s);

	DwJsonObjectRef jr = d.rootObject();
	jr.insert("sex").setValue("male");
	EXPECT_EQ(jr.child("sex").value(), "male");
}

TEST(DwJsonObject, addArray)
{
	DwString s("{ \"firstName\": \"Brett\", \"lastName\":\"McLaughlin\", \"email\": \"aaaa\" }");

	DwJsonDocument d;
	d.loadFromData(s);

	DwJsonObjectRef jr = d.rootObject();
	jr.insert("sex").insert(0).setValue("male");
	jr["sex"].insert(1).setValue("fmale");
	EXPECT_EQ(jr["sex"][0].value(), "male");
	EXPECT_EQ(jr["sex"][1].value(), "fmale");

   jr["sex"].insert(1).setValue("fmale2");
   EXPECT_EQ(jr["sex"][1].value(), "fmale2");
   EXPECT_EQ(jr["sex"][2].value(), "fmale");

	jr.child("sex").child(0).setValue("male2");
	EXPECT_EQ(jr["sex"][0].value(), "male2");
}

TEST(DwJsonObject, add2Level)
{
	DwString s("{ \"firstName\": \"Brett\", \"lastName\":\"McLaughlin\", \"email\": \"aaaa\" }");

	DwJsonDocument d;
	d.loadFromData(s);

	DwJsonObjectRef jr = d.rootObject();
    jr.insert("sex").insert("male").insert(0).setValue("true");
    jr["sex"]["male"].insert(1).setValue("false");
    jr["sex"].insert("fmale").insert(0).setValue("true");
    jr["sex"]["fmale"].insert(1).setValue("false");
    /*
	jr["sex"]["male"][0].setValue("true");
	jr["sex"]["male"][1].setValue("false");
	jr["sex"]["fmale"][0].setValue("true");
	jr["sex"]["fmale"][1].setValue("false");
    */
	EXPECT_EQ(jr["sex"]["male"][0].value(), "true");
	EXPECT_EQ(jr["sex"]["fmale"][1].value(), "false");
}

TEST(DwJsonObject, addobjtoarray)
{
	DwString s("{ \"firstName\": \"Brett\", \"lastName\":\"McLaughlin\", \"email\": \"aaaa\" }");

	DwJsonDocument d;
	d.loadFromData(s);

	DwJsonObjectRef jr = d.rootObject();
	EXPECT_EQ(jr.childCount(), 3);
    jr.insert("sex").insert(0).insert("male").setValue("true");
    jr["sex"].insert(1).insert("fmale").setValue("false");
    /*
	jr["sex"][0]["male"].setValue("true");
	jr["sex"][1]["fmale"].setValue("false");
    */
	EXPECT_EQ(jr.childCount(), 4);
	EXPECT_EQ(jr["sex"][0]["male"].value(), "true");
	EXPECT_EQ(jr["sex"][1]["fmale"].value(), "false");

	EXPECT_EQ(jr["sex"].type(), DwJsonArrayValue);
}

TEST(DwJsonObjectRef, delobj)
{
	DwString s("{ \"firstName\": \"Brett\", \"lastName\":\"McLaughlin\", \"email\": \"aaaa\" }");

	DwJsonDocument d;
	d.loadFromData(s);

	DwJsonObjectRef jr = d.rootObject();
    /*
	jr["sex"][0]["male"].setValue("true");
	jr["sex"][1]["fmale"].setValue("false");
    */

    jr.insert("sex").insert(0).insert("male").setValue("true");
    jr["sex"].insert(1).insert("fmale").setValue("false");

	jr["email"].parent().deleteObject();
	EXPECT_EQ(jr.childCount(), 3);
	EXPECT_EQ(jr["sex"].childCount(), 2);
    DwJsonObjectRef dobj = jr["sex"][0];
	jr["sex"][0].deleteObject();
    EXPECT_FALSE(dobj.isValid());
	EXPECT_EQ(jr["sex"].childCount(), 1);
	EXPECT_EQ(jr["sex"].type(), DwJsonArrayValue);
}

TEST(DwJsonDocument, createDoc)
{
	DwJsonDocument d;
	
	DwJsonObjectRef  root;
    root = d.createRoot(DwJsonArrayValue);

    root.insert(0).setValue("1");
    root.insert(1).setValue("2");
    root.insert(2).setValue("3");
    /*
	root[0].setValue(1);
	root[1].setValue(2);
	root[2].setValue(3);
    */

	EXPECT_EQ(root.child(2).value(), "3");
	EXPECT_EQ(root.childCount(), 3);
}

TEST(DwJsonObjectRef, setBoolValue)
{
    DwJsonDocument d;

    DwJsonObjectRef  root;
    root = d.createRoot(DwJsonObjectValue);

    root.insert("0").setBoolValue(true);
    root.insert("1").setBoolValue(false);
    root.insert("2").setBoolValue(true);

    DwString s ;
    d.saveToStream(s);

    DwJsonDocument d2;
    d2.loadFromData(s);

    DwJsonObjectRef v0 = d2.rootObject()["0"];
    DwJsonObjectRef v1 = d2.rootObject()["1"];
    DwJsonObjectRef v2 = d2.rootObject()["2"];
    EXPECT_TRUE(v0.boolValue());
    EXPECT_FALSE(v1.boolValue());
    EXPECT_TRUE(v2.boolValue());

    EXPECT_TRUE(d2.saveToFile("../testdata/core_json/testjson.json"));
}

TEST(DwJsonDocument, excp)
{
    DwJsonDocument d;
    EXPECT_FALSE(d.isValid());

    DwJsonObjectRef  root;
    //EXPECT_FALSE(root.createObject(DwJsonArrayValue).isValid());
    EXPECT_FALSE(root.child(2).isValid());
    EXPECT_FALSE(root.child("name").isValid());
    EXPECT_FALSE(d.createRoot(DwJsonStringValue).isValid());
    EXPECT_FALSE(root.insert("name").isValid());
    EXPECT_FALSE(root.insert(1).isValid());
    EXPECT_FALSE(root[1].isValid());
    EXPECT_FALSE(root["me"].isValid());
    root.setValue("me");
    EXPECT_TRUE(root.value().isEmpty());

    root = d.createRoot(DwJsonArrayValue);
    EXPECT_FALSE(root.child(1).isValid());
    root.insert(0).setValue("hello");
    EXPECT_TRUE(root.insert(0).isValid());
    EXPECT_TRUE(root.value().isEmpty());

    EXPECT_FALSE(root.child("me").isValid());

    DwJsonDocument d2;
    EXPECT_FALSE(d2.loadFromData(0));

    DwJsonDocument d3;
}

TEST(DwJsonObject, performance)
{
    int count = 1000;
    while(count--)
    {
        DwString s("{ \"firstName\": \"Brett\", \"lastName\":\"McLaughlin\", \"email\": \"aaaa\" }");

        DwJsonDocument d;
        d.loadFromData(s);

        DwJsonObjectRef jr = d.rootObject();
        DwJsonObjectRef jrc = jr.child("lastName");
        EXPECT_EQ(jrc.value(), "McLaughlin");
        EXPECT_EQ(jr.childCount(), 3);
        EXPECT_EQ(jr["lastName"].value(),"McLaughlin");
    }
}
