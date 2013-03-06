#pragma once
#include "dwgui/dwrttiobjectptr.h"

//------------------------------------------------------------------------------
class DwGraphicsItem;

// Visitor
//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwGraphicsItemVisitor
{
public:
    DwGraphicsItemVisitor();
    virtual ~DwGraphicsItemVisitor();

    virtual void interest(DwGraphicsItem* item) = 0;
protected:
private:
};// end of DwGraphicsItemVisitor

//------------------------------------------------------------------------------
// Iterator
// Aim: slice control,  so we can divide one thing in multiple frames
// eg. a frame run 5 item, 30 number of items ,will deal in 6 frame
class DW_GUI_EXPORT DwGraphicsItemIterator
{
public:
    DwGraphicsItemIterator(DwGraphicsItem* root, DwGraphicsItemVisitor* visitor);
    virtual ~DwGraphicsItemIterator();

    virtual int run(int nCount = -1);
    virtual DwGraphicsItem* next()=0;

    virtual void initIterator(DwGraphicsItem* root, DwGraphicsItemVisitor* visitor);

    // TODO: need be weakref
    DwRttiObjectPtr<DwGraphicsItem> m_root;
    DwGraphicsItemVisitor* m_visitor;
};// end of DwGraphicsItemIterator

// preorder iterator
class DW_GUI_EXPORT DwGraphicsItemPreorderIterator : public DwGraphicsItemIterator
{
public:
    DwGraphicsItemPreorderIterator(DwGraphicsItem* root, DwGraphicsItemVisitor* visitor);
    virtual ~DwGraphicsItemPreorderIterator();

    virtual DwGraphicsItem* next();

    virtual void initIterator(DwGraphicsItem* root, DwGraphicsItemVisitor* visitor);

private:
    bool m_rootVisited;
    int m_curVisited;
    DwGraphicsItemPreorderIterator* m_childIterator;
};// end of DwGraphicsItemPreorderIterator

// postorder iterator
class DW_GUI_EXPORT DwGraphicsItemPostorderIterator : public DwGraphicsItemIterator
{
public:
    DwGraphicsItemPostorderIterator(DwGraphicsItem* root, DwGraphicsItemVisitor* visitor);
    virtual ~DwGraphicsItemPostorderIterator();

    virtual DwGraphicsItem* next();

    virtual void initIterator(DwGraphicsItem* root, DwGraphicsItemVisitor* visitor);

private:
    bool m_rootVisited;
    int m_curVisited;
    DwGraphicsItemPostorderIterator* m_childIterator;
};// end of DwGraphicsItemPostorderIterator