#include "stable.h"

#include "dwgraphics.h"
#include "dwgui/dwgui.h"
#include "gtest_helper.h"

#include "gui_test_dwanchorline.h"
#include "gui_test_dwassign.h"
#include "gui_test_dwassignlib.h"
#include "gui_test_dwevent.h"
#include "gui_test_dweventid.h"
#include "gui_test_dwgraphicsevent.h"
#include "gui_test_dwgraphicseventdispatch.h"
#include "gui_test_dwgraphicseventhandler.h"
#include "gui_test_dwgraphicsitem.h"
#include "gui_test_dwgraphicsitemlistener.h"
#include "gui_test_dwgraphicsitemvisitor.h"
#include "gui_test_dwgraphicslayoutitem.h"
#include "gui_test_dwgraphicspixmapitem.h"
#include "gui_test_dwgraphicsrectitem.h"
#include "gui_test_dwgraphicsscene.h"
#include "gui_test_dwgraphicsscenebsptreeindex.h"
#include "gui_test_dwgraphicssceneindex.h"
#include "gui_test_dwgraphicsscenelinearindex.h"
#include "gui_test_dwgraphicsscenelistener.h"
#include "gui_test_dwgraphicssimpletextitem.h"
#include "gui_test_dwgraphicsutil.h"
#include "gui_test_dwgraphicsview.h"
#include "gui_test_dwpropertys.h"
#include "gui_test_dwrttiobject.h"
#include "gui_test_dwrttiobjectptr.h"
#include "gui_test_dwurl.h"
#include "gui_test_dwsysteminfo.h"
#include "gui_test_dwenvironment.h"
#include "gui_test_dwwin32registry.h"
#include "gui_test_dwprivateprofile.h"
#include "gui_test_dwapplauncher.h"
#include "gui_test_dwringlogcase.h"
#include "gui_test_dwpluginlib.h"
#include "gui_test_dwversion.h"
#include "gui_test_dwpluginversion.h"

int wmain(int argc, wchar_t* argv[])
{
	testing::InitGoogleTest(&argc, &(argv[0]));
    dwInitGraphics();
    RUN_ALL_TESTS();
    DW_UI_LOG_FLUSH(DwUrl("bin:lg.txt"));
    dwUninitGraphics();
	system("pause");
}
