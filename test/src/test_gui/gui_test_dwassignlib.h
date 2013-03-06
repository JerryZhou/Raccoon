
TEST(DwAssignLib, constructor)
{
    DwAssignLib *assignLib = dwAssignLib();
    EXPECT_TRUE(assignLib != NULL);
}

TEST(DwAssignLib, setup)
{
    DwAssignLib *assignLib = dwAssignLib();
    EXPECT_TRUE(assignLib != NULL);

    assignLib->setup();
    EXPECT_EQ(dwAssignLib()->isValid() , true);
}

TEST(DwAssignLib, discard)
{
    DwAssignLib *assignLib = dwAssignLib();
    EXPECT_TRUE(assignLib != NULL);

    assignLib->discard();
    EXPECT_EQ(dwAssignLib()->isValid() , false);
}