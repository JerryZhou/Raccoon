#include "stable.h"
#include "dwgui/dwapplication.h"
#include "dwgui/dwmainwindow.h"

#include "dwgui/dwlineedit.h"
#include "dwgui/dwgraphicsedittext.h"
#include "dwgui/dwtextlayoutengine.h"
#include "dwgui/dwcontentsizevalidater.h"
#include "dwgui/dwgraphicsbackgrounditem.h"
#include "dwgui/dwanchors.h"
#include "dwgui/dwtrayicon.h"

//////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE , HINSTANCE , LPSTR , int )
{
	DwApplication app;

	DwMainWindow mainWindow;
    mainWindow.show();

    // show a centerin edit
    DwWidgetItem * containerEdit = new DwWidgetItem(&mainWindow);
    containerEdit->setSize(200, 50);
    containerEdit->setPos(200, 200);
    containerEdit->background()->setURL("dwgui:edit.png");
    containerEdit->setFlag(Dw::ItemClipsChildrenToShape);

    // a line edit
    DwLineEdit *edit = new DwLineEdit(containerEdit);
    edit->background()->setVisible(false);
    edit->setSize(10, 20);
    edit->editText()->layoutEngine().setOrgContentRect(DwRectF(0, 0, 10, 20));
    edit->anchors()->setCenterIn(containerEdit);
    containerEdit->setFocusProxy(edit);

    // will check the size
    DwContentSizeValidater * v = new DwContentSizeValidater(&(edit->editText()->doc()), edit);
    edit->editText()->doc().appendValidater(v);

    return app.exec();
}