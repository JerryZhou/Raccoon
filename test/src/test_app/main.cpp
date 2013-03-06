#include "stable.h"

#include "dwgraphics.h"
#include "dwgui/dwgui.h"
#include "dwapp/dwapp.h"
#include "gtest_helper.h"

//#include "app_test_dwmainapplication.h"

int wmain(int argc, wchar_t* argv[])
{
	testing::InitGoogleTest(&argc, &(argv[0]));
    dwInitGraphics();
    RUN_ALL_TESTS();
    DW_UI_LOG_FLUSH(DwUrl("bin:lg.txt"));
    dwUninitGraphics();
	system("pause");
}
