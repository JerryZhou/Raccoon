#include "stable.h"
#include "dwdelegatelib.h"

#include "dwgui/dwgraphicsitem.h"
#include "dwgui/dwgraphicsrectitem.h"
#include "dwgui/dwgraphicspixmapitem.h"
#include "dwgui/dwgraphicssimpletextitem.h"
#include "dwgui/dwgraphicsframeitem.h"
#include "dwgui/dwgraphicsmarginspixmapitem.h"
#include "dwgui/dwbasepositioner.h"
#include "dwgui/dwgridpositioner.h"
#include "dwgui/dwframewindow.h"
#include "dwgui/dwscrollarea.h"
#include "dwgui/dwscrollbar.h"
#include "dwgui/dwgraphicsevent.h"
#include "dwgui/dwgraphicsbackgrounditem.h"
#include "dwgui/dwprogressbar.h"
#include "dwgui/dwlineedit.h"

#include "dwitemdelegate.h"
#include "itemdelegate/dwitemdelegatepixmap.h"
#include "itemdelegate/dwitemdelegaterect.h"
#include "itemdelegate/dwitemdelegatesimpletext.h"
#include "itemdelegate/dwitemdelegateframeitem.h"
#include "itemdelegate/dwitemdelegatebasepositioner.h"
#include "itemdelegate/dwitemdelegatewidgetitem.h"
#include "itemdelegate/dwitemdelegateframewindow.h"
#include "itemdelegate/dwitemdelegatemarginspixmapitem.h"
#include "itemdelegate/dwitemdelegategridpositioner.h"
#include "itemdelegate/dwitemdelegatescrollarea.h"
#include "itemdelegate/dwitemdelegatescrollbar.h"
#include "itemdelegate/dwitemdelegatebackgrounditem.h"
#include "itemdelegate/dwitemdelegateprogressbar.h"
#include "itemdelegate/dwitemdelegatelineedit.h"

#include "dweventdelegate.h"
#include "eventdelegate/dweventdelegatescene.h"
#include "eventdelegate/dweventdelegatemouse.h"
#include "eventdelegate/dweventdelegatemousewheel.h"
#include "eventdelegate/dweventdelegatemouseclicked.h"
#include "eventdelegate/dweventdelegatemousemove.h"
#include "eventdelegate/dweventdelegatekey.h"
#include "eventdelegate/dweventdelegatechar.h"
#include "eventdelegate/dweventdelegatefocus.h"
#include "eventdelegate/dweventdelegatedrag.h"
#include "eventdelegate/dweventdelegatedragmove.h"
#include "eventdelegate/dweventdelegatetips.h"
#include "eventdelegate/dweventdelegategraphicsitem.h"
#include "eventdelegate/dweventdelegateitemchange.h"


DW_IMPLEMENT_CLASS(DwDelegateLib, 'DLIB', DwRttiObject);

//------------------------------------------------------------------------------
/**
*/
DwDelegateLib::DwDelegateLib()
{
    // item delegate
    registerDelegate(DwGraphicsItem::RTTI(),            DwItemDelegate::create());
    registerDelegate(DwGraphicsPixmapItem::RTTI(),      DwItemDelegatePixmap::create());
    registerDelegate(DwGraphicsRectItem::RTTI(),        DwItemDelegateRect::create());
    registerDelegate(DwGraphicsSimpleTextItem::RTTI(),  DwItemDelegateSimpleText::create());
    registerDelegate(DwGraphicsFrameItem::RTTI(),       DwItemDelegateFrameItem::create());
    registerDelegate(DwGraphicsMarginsPixmapItem::RTTI(),DwItemDelegateMarginsPixmapItem::create());
    registerDelegate(DwBasePositioner::RTTI(),          DwItemDelegateBasePositioner::create());
    registerDelegate(DwGridPositioner::RTTI(),          DwItemDelegateGridPositioner::create());
    registerDelegate(DwWidgetItem::RTTI(),              DwItemDelegateWidgetItem::create());
    registerDelegate(DwFrameWindow::RTTI(),             DwItemDelegateFrameWindow::create());
    registerDelegate(DwScrollArea::RTTI(),              DwItemDelegateScrollArea::create());
    registerDelegate(DwScrollBar::RTTI(),               DwItemDelegateScrollBar::create());
    registerDelegate(DwGraphicsBackgroundItem::RTTI(),  DwItemDelegateBackgroundItem::create());
    registerDelegate(DwProgressBar::RTTI(),             DwItemDelegateProgressBar::create());
    registerDelegate(DwLineEdit::RTTI(),                DwItemDelegateLineEdit::create());

    // event delegate
    registerDelegate(DwEvent::RTTI(),               DwEventDelegate::create());
    registerDelegate(DwSceneEvent::RTTI(),          DwEventDelegateScene::create());

    registerDelegate(DwMouseEvent::RTTI(),          DwEventDelegateMouse::create());
    registerDelegate(DwMouseWheelEvent::RTTI(),     DwEventDelegateMouseWheel::create());
    registerDelegate(DwMouseClickedEvent::RTTI(),   DwEventDelegateMouseClicked::create());
    registerDelegate(DwMouseMoveEvent::RTTI(),      DwEventDelegateMouseMove::create());

    registerDelegate(DwKeyEvent::RTTI(),            DwEventDelegateKey::create());
    registerDelegate(DwCharEvent::RTTI(),           DwEventDelegateChar::create());

    registerDelegate(DwFocusEvent::RTTI(),          DwEventDelegateFocus::create());

    registerDelegate(DwSceneDragEvent::RTTI(),      DwEventDelegateDrag::create());
    registerDelegate(DwDragMoveEvent::RTTI(),       DwEventDelegateDragMove::create());

    registerDelegate(DwTipsEvent::RTTI(),           DwEventDelegateTips::create());

    registerDelegate(DwGraphicsItemEvent::RTTI(),   DwEventDelegateGraphicsItem::create());
    registerDelegate(DwItemChangeEvent::RTTI(),     DwEventDelegateItemChange::create());
}

//------------------------------------------------------------------------------
/**
*/
DwDelegateLib::~DwDelegateLib()
{
    DelegatesMap::iterator ite = m_delegates.begin();
    while(ite != m_delegates.end())
    {
        delete ite->second;
        ++ite;
    }
    m_delegates.clear();
}

//------------------------------------------------------------------------------
/**
*/
bool DwDelegateLib::registerDelegate(DwRtti* obj, DwDelegate* gate)
{
    DW_ASSERT(!m_delegates.contains(obj));
    m_delegates[obj] = gate;
    gate->setKey(obj);

    DwDelegate* p = parentOf(obj);
    gate->setParent(p);
    DW_ASSERT(( p || (obj->parent() == DwRttiObject::RTTI())) && "should register the ancestor");
    return true;
}

//------------------------------------------------------------------------------
/**
*/
DwDelegate *DwDelegateLib::delegateOf(DwRtti* obj) const
{
    return m_delegates.value(obj, NULL);
}

//------------------------------------------------------------------------------
/**
*/
DwDelegate *DwDelegateLib::parentOf(DwRtti* obj) const
{
    DW_ASSERT(obj);
    DwDelegate *gate = NULL;
    DwRtti* p = obj;
    do
    {
        p = (DwRtti*)(p->parent());
        gate = delegateOf(p);
    }
    while (!gate && p->parent() && p->parent() != DwRttiObject::RTTI());

    return gate;
}

//------------------------------------------------------------------------------
/**
*/
DW_GLOBAL_STATIC(DwDelegateLib, _dw_delegateLib);
DwDelegateLib* dwDelegateLib()
{
    return _dw_delegateLib();
}