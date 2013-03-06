
#include "dwstable.h"
#include "dwgraphicsscene_bsp_p.h"
#include "dwgraphicsitem.h"

class DwGraphicsSceneInsertItemBspTreeVisitor : public DwGraphicsSceneBspTreeVisitor
{
public:
    DwGraphicsItem *item;

    void visit(DwVector<DwGraphicsItem *> *items)
    {
        items->append(item);    //  items->prepend(item);
    }
};

class DwGraphicsSceneRemoveItemBspTreeVisitor : public DwGraphicsSceneBspTreeVisitor
{
public:
    DwGraphicsItem *item;

    void visit(DwVector<DwGraphicsItem *> *items)
    {
        items->removeAll(item);    // items->removeAll(item);
    }
};

class DwGraphicsSceneFindItemBspTreeVisitor : public DwGraphicsSceneBspTreeVisitor
{
public:
    DwVector<DwGraphicsItem *> *foundItems;
    bool onlyTopLevelItems;

    void visit(DwVector<DwGraphicsItem *> *items)
    {
        for (int i = 0; i < items->size(); ++i)
        {
            DwGraphicsItem *item = items->at(i);
            if (onlyTopLevelItems && item->m_parent)
                item = item->topLevelItem();
            if (!item->m_itemDiscovered && item->isVisible())
            {
                item->m_itemDiscovered = 1;
                foundItems->append(item); // fountitems->prepend(item);
            }
        }
    }
};

DwGraphicsSceneBspTree::DwGraphicsSceneBspTree()
    : leafCnt(0)
{
    insertVisitor = new DwGraphicsSceneInsertItemBspTreeVisitor;
    removeVisitor = new DwGraphicsSceneRemoveItemBspTreeVisitor;
    findVisitor = new DwGraphicsSceneFindItemBspTreeVisitor;
}

DwGraphicsSceneBspTree::~DwGraphicsSceneBspTree()
{
    delete insertVisitor;
    delete removeVisitor;
    delete findVisitor;
}

void DwGraphicsSceneBspTree::initialize(const DwRectF &rect, int depth)
{
    this->rect = rect;
    leafCnt = 0;
    nodes.resize((1 << (depth + 1)) - 1);
    eastl::fill(nodes.begin(), nodes.end(), Node());
    leaves.resize(1 << depth);
    eastl::fill(leaves.begin(), leaves.end(), DwVector<DwGraphicsItem *>());

    initialize(rect, depth, 0);
}

void DwGraphicsSceneBspTree::clear()
{
    leafCnt = 0;
    nodes.clear();
    leaves.clear();
}

void DwGraphicsSceneBspTree::insertItem(DwGraphicsItem *item, const DwRectF &rect)
{
    insertVisitor->item = item;
    climbTree(insertVisitor, rect);
}

void DwGraphicsSceneBspTree::removeItem(DwGraphicsItem *item, const DwRectF &rect)
{
    removeVisitor->item = item;
    climbTree(removeVisitor, rect);
}

void DwGraphicsSceneBspTree::removeItems(const DwSet<DwGraphicsItem *> &items)
{
    for (int i = 0; i < leaves.size(); ++i)
    {
        DwVector<DwGraphicsItem *> newItemList;
        const DwVector<DwGraphicsItem *> &oldItemList = leaves[i];
        for (int j = 0; j < oldItemList.size(); ++j)
        {
            DwGraphicsItem *item = oldItemList.at(j);
            if (!items.contains(item))
                newItemList.append(item);
        }
        leaves[i] = newItemList;
    }
}

DwVector<DwGraphicsItem *> DwGraphicsSceneBspTree::items(const DwRectF &rect, bool onlyTopLevelItems) const
{
    DwVector<DwGraphicsItem *> tmp;
    findVisitor->foundItems = &tmp;
    findVisitor->onlyTopLevelItems = onlyTopLevelItems;
    climbTree(findVisitor, rect);
    // Reset discovery bits.
    for (int i = 0; i < tmp.size(); ++i)
        tmp.at(i)->m_itemDiscovered = 0;
    return tmp;
}

int DwGraphicsSceneBspTree::leafCount() const
{
    return leafCnt;
}

// TODO: debug string
DwString DwGraphicsSceneBspTree::debug(int index) const
{
    DW_UNUSED(index);
    //const Node *node = &nodes.at(index);

    DwString tmp;
    /*if (node->type == Node::Leaf) {
        DwRectF rect = rectForIndex(index);
        if (!leaves[node->leafIndex].isEmpty()) {
            tmp += DwString::fromLatin1("[%1, %2, %3, %4] contains %5 items\n")
                   .arg(rect.left()).arg(rect.top())
                   .arg(rect.width()).arg(rect.height())
                   .arg(leaves[node->leafIndex].size());
        }
    } else {
        if (node->type == Node::Horizontal) {
            tmp += debug(firstChildIndex(index));
            tmp += debug(firstChildIndex(index) + 1);
        } else {
            tmp += debug(firstChildIndex(index));
            tmp += debug(firstChildIndex(index) + 1);
        }
    }*/

    return tmp;
}

void DwGraphicsSceneBspTree::initialize(const DwRectF &rect, int depth, int index)
{
    Node *node = &nodes[index];
    if (index == 0)
    {
        node->type = Node::Horizontal;
        node->offset = rect.center().x();
    }

    if (depth)
    {
        Node::Type type;
        DwRectF rect1, rect2;
        DwReal offset1, offset2;

        if (node->type == Node::Horizontal)
        {
            type = Node::Vertical;
            rect1.setRect(rect.left(), rect.top(), rect.width(), rect.height() / 2);
            rect2.setRect(rect1.left(), rect1.bottom(), rect1.width(), rect.height() - rect1.height());
            offset1 = rect1.center().x();
            offset2 = rect2.center().x();
        }
        else
        {
            type = Node::Horizontal;
            rect1.setRect(rect.left(), rect.top(), rect.width() / 2, rect.height());
            rect2.setRect(rect1.right(), rect1.top(), rect.width() - rect1.width(), rect1.height());
            offset1 = rect1.center().y();
            offset2 = rect2.center().y();
        }

        int childIndex = firstChildIndex(index);

        Node *child = &nodes[childIndex];
        child->offset = offset1;
        child->type = type;

        child = &nodes[childIndex + 1];
        child->offset = offset2;
        child->type = type;

        initialize(rect1, depth - 1, childIndex);
        initialize(rect2, depth - 1, childIndex + 1);
    }
    else
    {
        node->type = Node::Leaf;
        node->leafIndex = leafCnt++;
    }
}

void DwGraphicsSceneBspTree::climbTree(DwGraphicsSceneBspTreeVisitor *visitor, const DwRectF &rect, int index) const
{
    if (nodes.isEmpty())
        return;

    const Node &node = nodes.at(index);
    const int childIndex = firstChildIndex(index);

    switch (node.type)
    {
    case Node::Leaf:
    {
        DW_ASSERT(node.leafIndex < leaves.count());
        visitor->visit(const_cast<DwVector<DwGraphicsItem*>*>(&leaves[node.leafIndex]));
        break;
    }
    case Node::Vertical:
        if (rect.left() < node.offset)
        {
            climbTree(visitor, rect, childIndex);
            if (rect.right() >= node.offset)
                climbTree(visitor, rect, childIndex + 1);
        }
        else
        {
            climbTree(visitor, rect, childIndex + 1);
        }
        break;
    case Node::Horizontal:
        if (rect.top() < node.offset)
        {
            climbTree(visitor, rect, childIndex);
            if (rect.bottom() >= node.offset)
                climbTree(visitor, rect, childIndex + 1);
        }
        else
        {
            climbTree(visitor, rect, childIndex + 1);
        }
    }
}

DwRectF DwGraphicsSceneBspTree::rectForIndex(int index) const
{
    if (index <= 0)
        return rect;

    int parentIdx = parentIndex(index);
    DwRectF rect = rectForIndex(parentIdx);
    const Node *parent = &nodes.at(parentIdx);

    if (parent->type == Node::Horizontal)
    {
        if (index & 1)
            rect.setRight(parent->offset);
        else
            rect.setLeft(parent->offset);
    }
    else
    {
        if (index & 1)
            rect.setBottom(parent->offset);
        else
            rect.setTop(parent->offset);
    }

    return rect;
}
