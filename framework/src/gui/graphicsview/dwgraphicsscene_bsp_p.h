#pragma once
#include "dwcore/dwrect.h"
#include "dwcore/dwset.h"
#include "dwcore/dwvector.h"
#include "dwcore/dwstring.h"
#include "dwgraphics/dwmatrix.h"

//------------------------------------------------------------------------------
class DwGraphicsItem;
class DwGraphicsSceneBspTreeVisitor;
class DwGraphicsSceneInsertItemBspTreeVisitor;
class DwGraphicsSceneRemoveItemBspTreeVisitor;
class DwGraphicsSceneFindItemBspTreeVisitor;

//------------------------------------------------------------------------------
class DwGraphicsSceneBspTree
{
public:
    struct Node
    {
        enum Type { Horizontal, Vertical, Leaf };
        union
        {
            DwReal offset;
            int leafIndex;
        };
        Type type;
    };

    DwGraphicsSceneBspTree();
    ~DwGraphicsSceneBspTree();

    void initialize(const DwRectF &rect, int depth);
    void clear();

    void insertItem(DwGraphicsItem *item, const DwRectF &rect);
    void removeItem(DwGraphicsItem *item, const DwRectF &rect);
    void removeItems(const DwSet<DwGraphicsItem *> &items);

    DwVector<DwGraphicsItem *> items(const DwRectF &rect, bool onlyTopLevelItems = false) const;
    int leafCount() const;

    inline int firstChildIndex(int index) const;
    inline int parentIndex(int index) const;

    DwString debug(int index) const;

private:
    friend class DwGraphicsItem;

    void initialize(const DwRectF &rect, int depth, int index);
    void climbTree(DwGraphicsSceneBspTreeVisitor *visitor, const DwRectF &rect, int index = 0) const;
    DwRectF rectForIndex(int index) const;

    DwVector<Node> nodes;
    DwVector<DwVector<DwGraphicsItem *> > leaves;
    int leafCnt;
    DwRectF rect;

    DwGraphicsSceneInsertItemBspTreeVisitor *insertVisitor;
    DwGraphicsSceneRemoveItemBspTreeVisitor *removeVisitor;
    DwGraphicsSceneFindItemBspTreeVisitor *findVisitor;
};// end of DwGraphicsSceneBspTree

//------------------------------------------------------------------------------
/**
*/
inline int DwGraphicsSceneBspTree::firstChildIndex(int index) const
{
    return index * 2 + 1;
}

//------------------------------------------------------------------------------
/**
*/
inline int DwGraphicsSceneBspTree::parentIndex(int index) const
{
    return index > 0 ? ((index & 1) ? ((index - 1) / 2) : ((index - 2) / 2)) : -1;
}

//------------------------------------------------------------------------------
class DwGraphicsSceneBspTreeVisitor
{
public:
    virtual ~DwGraphicsSceneBspTreeVisitor() { }
    virtual void visit(DwVector<DwGraphicsItem *> *items) = 0;
};// end of DwGraphicsSceneBspTreeVisitor

