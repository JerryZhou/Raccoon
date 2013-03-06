
TEST(DwEvent, isaccepted)
{
	DwEvent evt;

	EXPECT_TRUE(evt.isAccepted() == false);
	evt.setAccepted(true);
	EXPECT_TRUE(evt.isAccepted() == true);

	evt.ignore();
	EXPECT_TRUE(evt.isAccepted() == false);
	evt.accept();
	EXPECT_TRUE(evt.isAccepted() == true);
}

TEST(DwEvent, isup)
{
	DwEvent evt;

	EXPECT_TRUE(evt.isUp() == true);
	evt.setUp(false);
	EXPECT_TRUE(evt.isUp() == false);
}

TEST(DwEvent, isbacktrace)
{
	DwEvent evt;

	EXPECT_TRUE(evt.isBacktrace() == false);
	evt.setBacktrace(true);
	EXPECT_TRUE(evt.isBacktrace() == true);
}