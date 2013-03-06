#include "stable.h"

#include "dwcore.h"

#include "gtest_helper.h"

#include "core_test_dwstring.h"
#include "core_test_dwvector.h"
#include "core_test_dwmap.h"
#include "core_test_dwset.h"
#include "core_test_dwhash.h"
#include "core_test_dwlinkedlist.h"
#include "core_test_dwarraylist.h"
#include "core_test_dwbytearray.h"
#include "core_test_dwobject.h"
#include "core_test_dwfile.h"
#include "core_test_dwstream.h"
#include "core_test_dwpoint.h"
#include "core_test_dwsize.h"
#include "core_test_dwrect.h"
#include "core_test_dwmargins.h"
#include "core_test_dwxml.h"
#include "core_test_dwjson.h"
#include "core_test_dwdir.h"
#include "core_test_dwcryptographichash.h"
#include "core_test_dwzip.h"
#include "core_test_dwsigslot.h"
#include "core_test_sigslot.h"
#include "core_test_eastl.h"
#include "core_test_dwobjectptr.h"

int wmain(int argc, wchar_t* argv[])
{
	DwString s = DwWinAPI::dwGetModuleDir(NULL);
	SetCurrentDirectory(s.constString());

	testing::InitGoogleTest(&argc, &(argv[0]));

    RUN_ALL_TESTS();
}
