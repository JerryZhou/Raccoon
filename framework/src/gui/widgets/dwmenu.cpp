#include "stable.h"
#include "dwmenu.h"
#include "dwgui/dwcolumnpositioner.h"
#include "dwgui/dweventsignaldispatch.h"
#include "dwgui/dwrowpositioner.h"
#include "dwgui/dwgraphicsninepatchitem.h"
#include "dwgui/dwgraphicssimpletextitem.h"
#include "dwgui/dwanchors.h"
#include "dwgui/dwgraphicsbackgrounditem.h"
#include "dwgui/dwdefaultitemeventhandler.h"
#include "dwgui/dwgraphicsrectitem.h"
#include "dwgui/dwwidgetevent.h"

DW_IMPLEMENT_CLASS(DwMenu, 'MENU', DwFrameWindow);

//------------------------------------------------------------------------------
/**
*/
DwMenu::DwMenu( DwObject *parent /*= NULL*/ )
: DwFrameWindow(parent)
{
    setAttribute(Dw::WA_ResizeAble, false); // no resize
    setCenterWindowOnShow(false); //  no center
    setAttribute(Dw::WA_DeleteOnClose, true); // only can be new from heap
    setWinStyle(Dw::PopupWindow);

    m_content = new DwColumnPositioner(this);
    m_content->setSendChangedEvent(true);
    m_content->signalDispatch()->connect(DwItemChangeEvent::EventId(), this, &DwMenu::onContentChange);
    m_content->anchors()->setCenterIn(this);

    setSize(150, 80);
}

//------------------------------------------------------------------------------
/**
*/
DwMenu::~DwMenu()
{

}

//------------------------------------------------------------------------------
/**
*/
void DwMenu::onContentChange(DwRttiObject* obj, DwEvent* evt)
{
    if (obj == m_content && evt->isA(DwItemChangeEvent::EventId()))
    {
        DwItemChangeEvent* changeEvt = dwsafe_cast<DwItemChangeEvent>(evt);
        if ( changeEvt->m_change == Dw::ItemGemoryChange)
        {
            DwSizeF newSize = m_content->size() + DwSizeF(shadowEdage()*2+2, shadowEdage()*2+4);
            setSize(newSize);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
static void on_item_hover_enter(DwGraphicsItem* item, DwEvent*)
{
    DwWidgetItem* menuItem = dwsafe_cast<DwWidgetItem>(item);
    menuItem->background()->setColor(Dw::blue);
}

//------------------------------------------------------------------------------
/**
*/
static void on_item_hover_leave(DwGraphicsItem* item, DwEvent*)
{
    DwWidgetItem* menuItem = dwsafe_cast<DwWidgetItem>(item);
    menuItem->background()->setColor(Dw::transparent);
}

//------------------------------------------------------------------------------
/**
*/
static void on_item_clicked(DwGraphicsItem* item, DwEvent*)
{
    DwWidgetItem* menuItem = dwsafe_cast<DwWidgetItem>(item);

    DwMenuEvent evt;
    evt.m_actionName = menuItem->name();
    evt.m_action = menuItem;

    menuItem->sendEvent(&evt);
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem* DwMenu::addAction( const DwString& name, const DwString& text, const DwUrl& icon, int iconIdx/* = 0*/)
{
    DW_UNUSED(name);
    DW_UNUSED(text);
    DW_UNUSED(icon);
    DwWidgetItem *container = new DwWidgetItem(m_content);
    container->setName(name);
    container->eventHandler()->addEventCallBack(DwHoverEnterEvent::EventId(), on_item_hover_enter);
    container->eventHandler()->addEventCallBack(DwHoverLeaveEvent::EventId(), on_item_hover_leave);
    container->eventHandler()->addEventCallBack(DwMouseClickedEvent::EventId(), on_item_clicked);

    DwRowPositioner *row = new DwRowPositioner(container);
    row->setName("content");
    row->setSpacing(4);
    DwGraphicsNinePatchItem* nines = new DwGraphicsNinePatchItem(icon, iconIdx, row);
    nines->setName("icon");
    nines->anchors()->setVerticalCenter(row, Dw::VCenter);
    DwGraphicsSimpleTextItem* txt = new DwGraphicsSimpleTextItem(text, row);
    txt->setName("text");
    txt->anchors()->setVerticalCenter(row, Dw::VCenter);
    container->setSize(row->size());
    container->setWidth(m_content->width());
    container->anchors()->setLeft(m_content, Dw::Left);
    container->anchors()->setRight(m_content, Dw::Right);
    return container;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem* DwMenu::addLine(const DwString& name, const DwColor& c)
{
    DwWidgetItem *container = new DwWidgetItem(m_content);
    container->setName(name);
    container->setHoverable(false);

    DwGraphicsRectItem *line = new DwGraphicsRectItem(container);
    line->setColor(c);
    line->anchors()->setFill(container);
    container->setHeight(1);
    container->setWidth(m_content->width());
    container->anchors()->setLeft(m_content, Dw::Left);
    container->anchors()->setRight(m_content, Dw::Right);
    return container;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem* DwMenu::findAction( const DwString& name ) const
{
    DW_UNUSED(name);
    return m_content->findChild(name);
}

//------------------------------------------------------------------------------
/**
*/
void DwMenu::setActionIcon(const DwString& name, const DwUrl& icon, int iconIdx/* = 0*/)
{
    DwGraphicsItem *container = m_content->findChild(name);
    if (container)
    {
        DwGraphicsItem* content = container->findChild("content");
        DwGraphicsNinePatchItem * nines = dwsafe_cast<DwGraphicsNinePatchItem>(content->findChild("icon"));

        container->clearAnchors();
        nines->setNinePatch(icon, iconIdx);
        container->setSize(content->size());
        container->setWidth(m_content->width());
        container->anchors()->setLeft(m_content, Dw::Left);
        container->anchors()->setRight(m_content, Dw::Right);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwMenu::setActionText(const DwString& name, const DwString& text)
{
    DwGraphicsItem *container = m_content->findChild(name);
    if (container)
    {
        DwGraphicsItem* content = container->findChild("content");
        DwGraphicsSimpleTextItem  *txt = dwsafe_cast<DwGraphicsSimpleTextItem>(content->findChild("text"));

        container->clearAnchors();
        txt->setText(text);
        container->setSize(content->size());
        container->setWidth(m_content->width());
        container->anchors()->setLeft(m_content, Dw::Left);
        container->anchors()->setRight(m_content, Dw::Right);
    }
}

//------------------------------------------------------------------------------
/**
*/
bool DwMenu::remvoeAction( const DwString& name )
{
    DW_UNUSED(name);
    DwGraphicsItem *item = m_content->findChild(name);
    if (item)
    {
        item->setParentItem(NULL);
        item->deleteLater();
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwMenu::showAction(const DwString& name, bool b)
{
    DW_UNUSED(name);
    DwGraphicsItem *item = m_content->findChild(name);
    if (item)
    {
        item->setVisible(b);
        return true;
    }
    return false;
}