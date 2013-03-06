
TEST(DwGraphicsSimpleTextItem, construct)
{
	DwGraphicsSimpleTextItem simpleText;
	EXPECT_EQ(simpleText.text(), L"");

	DwGraphicsSimpleTextItem nextSimpleText(L"NextSimpleText");
	EXPECT_EQ(nextSimpleText.text(), L"NextSimpleText");
}

TEST(DwGraphicsSimpleTextItem, text)
{
	DwGraphicsSimpleTextItem simpleText;
	EXPECT_EQ(simpleText.text(), L"");

	simpleText.setText(L"NextSimpleText");
	EXPECT_EQ(simpleText.text(), L"NextSimpleText");
}

TEST(DwGraphicsSimpleTextItem, font)
{
	DwGraphicsSimpleTextItem simpleText;
	EXPECT_EQ(simpleText.text(), L"");

	simpleText.setText(L"NextSimpleText");
	EXPECT_EQ(simpleText.text(), L"NextSimpleText");

	EXPECT_EQ(simpleText.font(), DwFont());

	DwFont newFont;
	newFont.setSize(28);
	EXPECT_TRUE(simpleText.font() != newFont);
	simpleText.setFont(newFont);
	EXPECT_EQ(simpleText.font(), newFont);
}

TEST(DwGraphicsSimpleTextItem, fontsize)
{
	DwGraphicsSimpleTextItem simpleText;
	EXPECT_EQ(simpleText.text(), L"");

	simpleText.setFontSize(24);
	EXPECT_EQ(simpleText.fontSize(), 24);
}

TEST(DwGraphicsSimpleTextItem, fontcolor)
{
	DwGraphicsSimpleTextItem simpleText;
	EXPECT_EQ(simpleText.text(), L"");

	simpleText.setFontColor(DwColor(211, 211, 100));
	EXPECT_EQ(simpleText.fontColor(), DwColor(211, 211, 100));
}