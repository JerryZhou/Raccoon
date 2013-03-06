#pragma once
#include "dwgui/dwguiconfig.h"
#include "dwgui/dwwidgetenum.h"
#include "dwcore/sigslot.h"
#include "dwcore/dwstring.h"
#include "dwgui/dwgraphicsitem.h"
#include "dwgui/dwgraphicsevent.h"
#include "dwgui/dwurl.h"
#include "dwgui/dwresourceinterface.h"

//------------------------------------------------------------------------------
class DwEventSignalDispatch;
class DwGraphicsBackgroundItem;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwWidgetItem : public DwGraphicsItem
{
    DW_DECLARE_CLASS(DwWidgetItem);
public:
    explicit DwWidgetItem(DwGraphicsItem *parent = NULL);
    virtual ~DwWidgetItem();

    virtual void paint(DwPainter *painter, const DwStyleOptionGraphicsItem *option);// = 0;

    DwWidget::DragPolicy dragPolicy() const;
    void setDragPolicy(DwWidget::DragPolicy policy);

    inline DwGraphicsBackgroundItem *background() const;
    void setBackground(const DwUrl &uri);

protected:
    DwWidget::DragPolicy m_dragPolicy;
    DwGraphicsBackgroundItem *m_background;

#if DW_ENABLE_WIDGET_PROPERTY
    DW_WIDGET_PROPERTY_BEGIN(DwWidgetItem, DwGraphicsItem);
    DW_WIDGET_PROPERTY_END();
#endif
};// end of DwWidgetItem
DW_REGISTER_CLASS(DwWidgetItem);

//------------------------------------------------------------------------------
/**
*/
inline DwGraphicsBackgroundItem *DwWidgetItem::background() const
{
    return m_background;
}