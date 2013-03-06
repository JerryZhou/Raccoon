#pragma once
#include "dwnamespace.h"
#include "dwcore/dwvector.h"
#include "dwcore/dwrect.h"
#include "dwcore/dwvariant.h"
#include "dwgraphics/dwtransform.h"
#include "dwgui/dwrttiobject.h"
#include "dwgui/dwrtti.h"

//------------------------------------------------------------------------------
class DwGraphicsItem;
class DwGraphicsScene;
class DwGraphicsSceneIndexIntersector;
class DwGraphicsSceneIndexPointIntersector;
class DwGraphicsSceneIndexRectIntersector;

// NB! The default implement is the linear index manager
//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwGraphicsSceneIndex : public DwRttiObject
{
    DW_DECLARE_ABSTRACT_CLASS(DwGraphicsSceneIndex);
public:
    explicit DwGraphicsSceneIndex(DwGraphicsScene *scene = 0);
    virtual ~DwGraphicsSceneIndex();

    DwGraphicsScene* scene() const;

    virtual DwVector<DwGraphicsItem *> items(Dw::SortOrder order = Dw::DescendingOrder) const = 0;
    virtual DwVector<DwGraphicsItem *> items(const DwPointF &pos, Dw::ItemSelectionMode mode,
            Dw::SortOrder order, const DwTransform &deviceTransform = DwTransform()) const;
    virtual DwVector<DwGraphicsItem *> items(const DwRectF &rect, Dw::ItemSelectionMode mode,
            Dw::SortOrder order, const DwTransform &deviceTransform = DwTransform()) const;

    virtual DwVector<DwGraphicsItem *> estimateItems(const DwPointF &point, Dw::SortOrder order) const;
    virtual DwVector<DwGraphicsItem *> estimateItems(const DwRectF &rect, Dw::SortOrder order) const = 0;
    virtual DwVector<DwGraphicsItem *> estimateTopLevelItems(const DwRectF &rect, Dw::SortOrder order) const;

    virtual void debugString(DwDebug &stream)const;

protected:
    virtual void clear();
    virtual void addItem(DwGraphicsItem* item) = 0;
    virtual void removeItem(DwGraphicsItem* item) = 0;
    virtual void deleteItem(DwGraphicsItem* item);
    virtual void prepareBoundingRectChange(const DwGraphicsItem *);

    virtual void itemChange(const DwGraphicsItem *item, Dw::GraphicsItemChange change, const DwVariant& newOne, const DwVariant& oldOne);

    void recursive_items_helper(DwGraphicsItem *item, DwRectF exposeRect,
                                DwGraphicsSceneIndexIntersector *intersector, DwVector<DwGraphicsItem *> *items,
                                const DwTransform &viewTransform,
                                Dw::ItemSelectionMode mode, DwReal parentOpacity = 1.0) const;
    void items_helper(const DwRectF &rect, DwGraphicsSceneIndexIntersector *intersector,
                      DwVector<DwGraphicsItem *> *items, const DwTransform &viewTransform,
                      Dw::ItemSelectionMode mode, Dw::SortOrder order) const;
    static void sort_items_helper(DwVector<DwGraphicsItem *> *items, Dw::SortOrder order);

    void setGraphicsScene(DwGraphicsScene *scene);

    DwGraphicsScene *m_scene;
    DwGraphicsSceneIndexPointIntersector *m_pointIntersector;
    DwGraphicsSceneIndexRectIntersector *m_rectIntersector;

private:
    friend class DwGraphicsScene;
};// end of DwGraphicsSceneIndex

//------------------------------------------------------------------------------
class DwGraphicsSceneIndexIntersector
{
public:
    DwGraphicsSceneIndexIntersector() { }
    virtual ~DwGraphicsSceneIndexIntersector() { }
    virtual bool intersect(const DwGraphicsItem *item, const DwRectF &exposeRect, Dw::ItemSelectionMode mode,
                           const DwTransform &deviceTransform) const = 0;
};// end of DwGraphicsSceneIndexIntersector