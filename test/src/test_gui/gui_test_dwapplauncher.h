
TEST(DwAppLauncher, construct)
{
    DwAppLauncher launcher;
}

TEST(DwAppLauncher, launch)
{
    DwAppLauncher launcher;
    launcher.setExecutable(L"bin:widgets_test.exe");
    launcher.setWorkingDirectory(L"bin:");
    launcher.launchWait();
}