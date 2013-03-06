#include "stable.h"
#include "dwaccessiblewidget.h"
#include "dwgui/dwgraphicssimpletextitem.h"
#include "dwgui/dwgraphicsedittext.h"
#include "dwgui/dwtextdocument.h"

#include "dwgui/dwpushbutton.h"
#include "dwgui/dwcheckbox.h"
#include "dwgui/dwlabel.h"
#include "dwgui/dwlineedit.h"
#include "dwgui/dwmultiedit.h"
#include "dwgui/dwprogressbar.h"
#include "dwgui/dwscrollarea.h"
#include "dwgui/dwscrollbar.h"
#include "dwgui/dwtitlebar.h"
#include "dwgui/dwimagewidget.h"
#include "dwgui/dwmessagebox.h"

#define _widget_text(w) w->text() ? w->text()->text() : ""
#define _widget_caption(w) (w->caption() && w->caption()->text()) ? w->caption()->text()->text() : ""
#define _widget_edit_plain(w) w->editText()? w->editText()->doc().plainText() : ""
//------------------------------------------------------------------------------
/**
*/
DwAccessibleInterface * accessibleFactoryPushButton(DwRttiObject *obj)
{
    DW_ASSERT(obj->rtti() == DwPushButton::RTTI());

    DwPushButton *widget = dwsafe_cast<DwPushButton>(obj);
    return new DwAccessibleItem(widget, DwAccessible::PushButton, _widget_text(widget));
}

//------------------------------------------------------------------------------
/**
*/
DwAccessibleInterface * accessibleFactoryCheckBox(DwRttiObject *obj)
{
    DW_ASSERT(obj->rtti() == DwCheckBox::RTTI());

    DwCheckBox *widget = dwsafe_cast<DwCheckBox>(obj);
    return new DwAccessibleItem(widget, DwAccessible::CheckBox, _widget_text(widget));
}

//------------------------------------------------------------------------------
/**
*/
DwAccessibleInterface * accessibleFactoryLabel(DwRttiObject *obj)
{
    DW_ASSERT(obj->rtti() == DwLabel::RTTI());

    DwLabel *widget = dwsafe_cast<DwLabel>(obj);
    return new DwAccessibleItem(widget, DwAccessible::StaticText, _widget_text(widget));
}

//------------------------------------------------------------------------------
/**
*/
DwAccessibleInterface * accessibleFactoryLineEdit(DwRttiObject *obj)
{
    DW_ASSERT(obj->rtti() == DwLineEdit::RTTI());

    DwLineEdit *widget = dwsafe_cast<DwLineEdit>(obj);
    return new DwAccessibleItem(widget, DwAccessible::EditableText, _widget_edit_plain(widget));
}

//------------------------------------------------------------------------------
/**
*/
DwAccessibleInterface * accessibleFactoryMultiEdit(DwRttiObject *obj)
{
    DW_ASSERT(obj->rtti() == DwMultiEdit::RTTI());

    DwMultiEdit *widget = dwsafe_cast<DwMultiEdit>(obj);
    return new DwAccessibleItem(widget, DwAccessible::EditableText, _widget_edit_plain(widget));
}

//------------------------------------------------------------------------------
/**
*/
DwAccessibleInterface * accessibleFactoryProgressBar(DwRttiObject *obj)
{
    DW_ASSERT(obj->rtti() == DwProgressBar::RTTI());

    DwProgressBar *widget = dwsafe_cast<DwProgressBar>(obj);
    return new DwAccessibleItem(widget, DwAccessible::ProgressBar, widget->toolTips());
}

//------------------------------------------------------------------------------
/**
*/
DwAccessibleInterface * accessibleFactoryScrollArea(DwRttiObject *obj)
{
    DW_ASSERT(obj->rtti() == DwScrollArea::RTTI());

    // There is no scroll area
    DwScrollArea *widget = dwsafe_cast<DwScrollArea>(obj);
    return new DwAccessibleItem(widget, DwAccessible::Pane);
}

//------------------------------------------------------------------------------
/**
*/
DwAccessibleInterface * accessibleFactoryScrollBar(DwRttiObject *obj)
{
    DW_ASSERT(obj->rtti() == DwScrollBar::RTTI());

    // There is no scroll area
    DwScrollBar *widget = dwsafe_cast<DwScrollBar>(obj);
    return new DwAccessibleItem(widget, DwAccessible::ScrollBar);
}
//------------------------------------------------------------------------------
/**
*/
DwAccessibleInterface * accessibleFactoryTitleBar(DwRttiObject *obj)
{
    DW_ASSERT(obj->rtti() == DwTitleBar::RTTI());

    DwTitleBar *widget = dwsafe_cast<DwTitleBar>(obj);
    return new DwAccessibleItem(widget, DwAccessible::TitleBar, _widget_caption(widget));
}

//------------------------------------------------------------------------------
/**
*/
DwAccessibleInterface * accessibleFactoryImageWidget(DwRttiObject *obj)
{
    DW_ASSERT(obj->rtti() == DwImageWidget::RTTI());

    // there is no image widget role
    DwImageWidget *widget = dwsafe_cast<DwImageWidget>(obj);
    return new DwAccessibleItem(widget, DwAccessible::Canvas);
}

//------------------------------------------------------------------------------
/**
*/
class DwAccessibleMessageBox : public DwAccessibleItem
{
    DW_DECLARE_CLASS(DwAccessibleMessageBox);
public:
    explicit DwAccessibleMessageBox(DwGraphicsItem *o=NULL, Role r = Client, const DwString& name = DwString())
        : DwAccessibleItem(o, r, name)
    {
        ;
    }
protected:
#define _add_widget_of(s)  { DwGraphicsItem * item = message->buttonOf(s); if(item) list.append(item); }
    typedef DwVector<DwGraphicsItem*> WidgetList;
    virtual WidgetList childWidgets() const
    {
        DwMessageBox *message = dwsafe_cast<DwMessageBox>(widget());
        WidgetList list;
        _add_widget_of(DwWidget::MBB_CLOSED);
        _add_widget_of(DwWidget::MBB_OK);
        _add_widget_of(DwWidget::MBB_CANCLE);
        return list;
    }
private:
};// end of DwAccessibleMessageBox
DW_IMPLEMENT_CLASS(DwAccessibleMessageBox, 'AMOX', DwAccessibleItem);

//------------------------------------------------------------------------------
/**
*/
DwAccessibleInterface * accessibleFactoryMessageBox(DwRttiObject *obj)
{
    DW_ASSERT(obj->rtti() == DwMessageBox::RTTI());

    // there is no image widget role
    DwMessageBox *widget = dwsafe_cast<DwMessageBox>(obj);
    return new DwAccessibleMessageBox(widget, DwAccessible::Dialog, _widget_text(widget));
}

//------------------------------------------------------------------------------
/**
*/
void DW_GUI_EXPORT setupAccessibleWidget()
{
    DwAccessible::installFactory(DwPushButton::RTTI(),      accessibleFactoryPushButton);
    DwAccessible::installFactory(DwCheckBox::RTTI(),        accessibleFactoryCheckBox);
    DwAccessible::installFactory(DwLabel::RTTI(),           accessibleFactoryLabel);
    DwAccessible::installFactory(DwLineEdit::RTTI(),        accessibleFactoryLineEdit);
    DwAccessible::installFactory(DwMultiEdit::RTTI(),       accessibleFactoryMultiEdit);
    DwAccessible::installFactory(DwProgressBar::RTTI(),     accessibleFactoryProgressBar);
    DwAccessible::installFactory(DwScrollArea::RTTI(),      accessibleFactoryScrollArea);
    DwAccessible::installFactory(DwScrollBar::RTTI(),       accessibleFactoryScrollBar);
    DwAccessible::installFactory(DwTitleBar::RTTI(),        accessibleFactoryTitleBar);
    DwAccessible::installFactory(DwImageWidget::RTTI(),     accessibleFactoryImageWidget);
    DwAccessible::installFactory(DwMessageBox::RTTI(),      accessibleFactoryMessageBox);
}
