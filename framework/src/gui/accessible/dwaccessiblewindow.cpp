#include "stable.h"
#include "dwaccessiblewindow.h"

DW_IMPLEMENT_CLASS(DwAccessibleWindow, 'AWOW', DwAccessibleItem);

//------------------------------------------------------------------------------
/**
*/
DwAccessibleWindow::DwAccessibleWindow( DwGraphicsItem *o/*=NULL*/, Role r /*= Client*/, 
                                       const DwString& name /*= DwString()*/ )
                                       :DwAccessibleItem(o, r, name)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
typedef DwVector<DwGraphicsItem*> WidgetList;
static void appendList(WidgetList &to, WidgetList &from)
{
    if (!from.isEmpty())
    {
        for (int i=0; i<from.size(); ++i)
        {
            to.append(from.at(i));
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
WidgetList DwAccessibleWindow::childWidgets()const
{
    WidgetList listBack = childWidgetsOf(window()->backgrondLayer());
    WidgetList listContent = childWidgetsOf(window()->contentLayer());
    WidgetList listPopup = childWidgetsOf(window()->popupLayer());

    if (listBack.isEmpty() && listPopup.isEmpty())
    {
        return listContent;
    }

    appendList(listBack, listContent);
    appendList(listBack, listPopup);
    return listBack;
}

//------------------------------------------------------------------------------
/**
*/
DwFrameWindow* DwAccessibleWindow::window()const
{
    return dwsafe_cast<DwFrameWindow>(widget());
}
