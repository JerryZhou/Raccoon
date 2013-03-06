
TEST(DwSystemInfo, construct)
{
    DwSystemInfo * info = DwSystemInfo::systemInfo();
    EXPECT_TRUE(info != NULL);
}

TEST(DwSystemInfo, platform)
{
    DwSystemInfo * info = DwSystemInfo::systemInfo();
    EXPECT_TRUE(info->platform() == DwSystemInfo::Win32);
}

TEST(DwSystemInfo, cputype)
{
    DwSystemInfo * info = DwSystemInfo::systemInfo();
    EXPECT_TRUE(info->cpuType() == DwSystemInfo::X86_32 ||
        info->cpuType() == DwSystemInfo::X86_64);
}

TEST(DwSystemInfo, numcpucores)
{
    DwSystemInfo * info = DwSystemInfo::systemInfo();
    EXPECT_TRUE(info->numCpuCores() >= 1 && info->numCpuCores() < 8);
}

TEST(DwSystemInfo, pagesize)
{
    DwSystemInfo * info = DwSystemInfo::systemInfo();
    EXPECT_TRUE(info->pageSize() != 0);
}