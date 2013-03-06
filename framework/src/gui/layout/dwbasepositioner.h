#pragma once
#include "dwgui/dwgraphicsitem.h"
#include "dwgui/dwgraphicsitemlistener.h"


//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwBasePositioner : public DwGraphicsItem, public DwGraphicsItemListener
{
    DW_DECLARE_CLASS(DwBasePositioner);
public:
    enum PositionerType { None = 0x0, Horizontal = 0x1, Vertical = 0x2, Both = 0x3 };

    DwBasePositioner(DwGraphicsItem *parent = NULL, DwGraphicsScene *scene = NULL);
    virtual ~DwBasePositioner();

    DwReal spacing() const;
    void setSpacing(DwReal);

    // override base class
    virtual DwVariant itemChange(Dw::GraphicsItemChange change, const DwVariant &val);
    virtual void paint(DwPainter *painter, const DwStyleOptionGraphicsItem *option);

    // Implement Listener Stuffs
    virtual void onChange(Dw::ChangeType change,
                          DwGraphicsItem* item, const DwVariant& newOne, const DwVariant& oldOne);
    virtual void onAdded(DwGraphicsItem* item);
    virtual void onRemoved(DwGraphicsItem* item);

    void setApplyImplicitWidth(bool);
    bool isApplyImplicitWidth();
    void setApplyImplicitHeight(bool);
    bool isApplayImplicitHeight();

protected:
    virtual void doPositioning(DwSizeF *){ }
    virtual void reportConflictingAnchors(){ }

    class PositionedItem
    {
    public :
        PositionedItem(DwGraphicsItem *i) : item(i), isNew(false), isVisible(true) {}
        bool operator==(const PositionedItem &other) const
        {
            return other.item == item;
        }
        DwGraphicsItem *item;
        bool isNew;
        bool isVisible;
    };

    void positionX(DwReal, const PositionedItem &target);
    void positionY(DwReal, const PositionedItem &target);

    void setImplicitWidth(DwReal);
    bool widthValid() const;
    void setImplicitHeight(DwReal);
    bool heightValid() const;

    void watchChanges(DwGraphicsItem *other);
    void unwatchChanges(DwGraphicsItem* other);
    void unwatchAllChanges();

    virtual void itemSiblingOrderChanged(DwGraphicsItem* other);
    virtual void itemGeometryChanged(DwGraphicsItem *, const DwRectF &newGeometry, const DwRectF &oldGeometry);
    virtual void itemVisibilityChanged(DwGraphicsItem *);
    virtual void itemOpacityChanged(DwGraphicsItem *);
    virtual void itemDestroyed(DwGraphicsItem *item);

    void schedulePositioning();
    void prePositioning();

    DwVector<PositionedItem> m_positionedItems;
    PositionerType m_positionType;
    DwReal m_spaceing;
    bool m_queuedPositioning : 1;
    bool m_doingPositioning : 1;
    bool m_anchorConflict : 1;

    bool m_widthValid : 1;
    bool m_heightValid : 1;
    bool m_applyImplicitWidth : 1;
    bool m_applyImplicitHeight : 1;
    //DwReal m_width;
    //DwReal m_height;
    DwReal m_implicitWidth;
    DwReal m_implicitHeight;
private:
};// end of DwBasePositioner
DW_REGISTER_CLASS(DwBasePositioner);