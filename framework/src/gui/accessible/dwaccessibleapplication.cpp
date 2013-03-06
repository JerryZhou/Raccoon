#include "stable.h"
#include "dwaccessibleapplication.h"
#include "dwgui/dwapplication.h"
#include "dwgui/dwframewindow.h"
#include "dwgui/dwwindow.h"
#include "dwgui/dwgraphicsview.h"
#include "dwgui/dwgraphicsscene.h"

DW_IMPLEMENT_CLASS(DwAccessibleApplication, 'AAON', DwAccessibleObject);

//------------------------------------------------------------------------------
/**
*/
DwAccessibleApplication::DwAccessibleApplication()
: DwAccessibleObject(dwApp())
{
}

typedef DwVector<DwFrameWindow*> DwWindowList;
// all toplevel windows except popups, toolwindow and the desktop
static DwWindowList topLevelWindows()
{
    DwWindowList list;

    DwSet<DwWindow*> windows = dwApp()->topLevelWindows();
    if (!windows.isEmpty()){
        DwSet<DwWindow*>::const_iterator ite = windows.begin();
        while(ite != windows.end()){
            const DwWindow * w = *ite;
            if (w->winId() && w->view() && w->view()->scene()){
                DwFrameWindow* fw = dwsafe_cast<DwFrameWindow>(
                    w->view()->windowItem());
                if (fw && 
                    fw->winStyle() != Dw::ToolWindow &&
                    fw->winStyle() != Dw::PopupWindow){
                    list.append(fw);
                }
            }
            ++ite;
        }
    }

    return list;
}

// the active window
static DwFrameWindow * activeWindow()
{
    HWND hWnd = ::GetActiveWindow();
    DwWindowList windows = topLevelWindows();
    for (int i=0; i<windows.size(); ++i){
        if(hWnd == (HWND)(windows.at(i)->winId())){
            return windows.at(i);
        }
    }
    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
int DwAccessibleApplication::childCount() const
{
    return topLevelWindows().count();
}

//------------------------------------------------------------------------------
/**
*/
int DwAccessibleApplication::indexOfChild(const DwAccessibleInterface *child) const
{
    DwFrameWindow *w = dwsafe_cast<DwFrameWindow>(child->object());
    if (!w)
    {
        return -1;
    }

    const DwWindowList tlw(topLevelWindows());
    int index = tlw.indexOf(w);
    if (index != -1)
        ++index;
    return index;
}

//------------------------------------------------------------------------------
/**
*/
int DwAccessibleApplication::childAt(int x, int y) const
{
    const DwWindowList tlw(topLevelWindows());
    for (int i = 0; i < tlw.count(); ++i) {
        DwFrameWindow *w = tlw.at(i);
        if (w->rect().contains(x,y))
            return i+1;
    }
    return -1;
}

//------------------------------------------------------------------------------
/**
*/
DwAccessible::Relation DwAccessibleApplication::relationTo(int child, const
        DwAccessibleInterface *other, int otherChild) const
{
    DwRttiObject *o = other ? other->object() : 0;
    if (!o)
        return Unrelated;

    if(o == object()) {
        if (child && !otherChild)
            return Child;
        if (!child && otherChild)
            return Ancestor;
        if (!child && !otherChild)
            return Self;
    }

    DwWindowList tlw(topLevelWindows());
    if (tlw.indexOf(dwsafe_cast<DwFrameWindow>(o)) != -1)
        return Ancestor;

    DwGraphicsItem *childItem = dwsafe_cast<DwGraphicsItem>(o);
    for (int i = 0; i < tlw.count(); ++i) {
        DwFrameWindow *w = tlw.at(i);
        if (w->isAncestorOf(childItem))
            return Ancestor;
    }

    return Unrelated;
}

//------------------------------------------------------------------------------
/**
*/
int DwAccessibleApplication::navigate(RelationFlag relation, int entry,
                                     DwAccessibleInterface **target) const
{
    if (!target)
        return -1;

    *target = 0;
    DwRttiObject *targetObject = 0;

    switch (relation) {
    case Self:
        targetObject = object();
        break;
    case Child:
        if (entry > 0 && entry <= childCount()) {
            const DwWindowList tlw(topLevelWindows());
            if (tlw.count() >= entry)
                targetObject = tlw.at(entry-1);
        } else {
            return -1;
        }
        break;
    case FocusChild:
        targetObject = activeWindow();
        break;
    default:
        break;
    }
    *target = DwAccessible::queryAccessibleInterface(targetObject);
    return *target ? 0 : -1;
}

//------------------------------------------------------------------------------
/**
*/
DwString DwAccessibleApplication::text(Text t, int) const
{
    switch (t) {
    case Name:
        {
            DwFrameWindow * w = activeWindow();
            if (w)
                return w->caption();
        }
        break;
    case Description:
        return DwUrl("bin:").localPath();
    default:
        break;
    }
    return DwString();
}

//------------------------------------------------------------------------------
/**
*/
DwAccessible::Role DwAccessibleApplication::role(int) const
{
    return Application;
}

//------------------------------------------------------------------------------
/**
*/
DwAccessible::State DwAccessibleApplication::state(int) const
{
    return activeWindow() ? Focused : Normal;
}

//------------------------------------------------------------------------------
/**
*/
int DwAccessibleApplication::userActionCount(int) const
{
    return 1;
}

//------------------------------------------------------------------------------
/**
*/
bool DwAccessibleApplication::doAction(int action, int child, const DwVariantList &param)
{
    if (action == 0 || action == 1) {
        DwFrameWindow *w = activeWindow();
        if (!w)
            w = topLevelWindows().at(0);
        if (!w)
            return false;
        ::SetActiveWindow((HWND)(w->winId()));
        return true;
    }
    return DwAccessibleObject::doAction(action, child, param);
}

//------------------------------------------------------------------------------
/**
*/
DwString DwAccessibleApplication::actionText(int action, Text text, int child) const
{
    //TODO: i18n
    DwString str;
    if ((action == 0 || action == 1) && !child) switch (text) {
    case Name:
        return DwString("Activate");
    case Description:
        return DwString("Activates the program's main window");
    default:
        break;
    }
    return DwAccessibleObject::actionText(action, text, child);
}