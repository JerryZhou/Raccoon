#include "stable.h"
#include "dwgraphicsitemlistener.h"
#include "dwgraphicsitem.h"
#include "dwdummys.h"

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItemListener::DwGraphicsItemListener()
{
    ;
}

//------------------------------------------------------------------------------
/**
    NB! BEFORE INHEREST DESTRUCTOR, SHOULD CALL remAllDepends();
*/
DwGraphicsItemListener::~DwGraphicsItemListener()
{
    DW_ASSERT(m_dependItems.size() == 0 && "Should Call remAllDepends() before destructor");
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItemListener::setChange(Dw::ChangeType change, bool accept)
{
    if (accept)
    {
        m_changes |= change;
    }
    else
    {
        m_changes &= ~change;
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItemListener::addDepend(DwGraphicsItem* item)
{
    DW_ASSERT(item);
    DW_ASSERT(m_dependItems.indexOf(item) == InvalidIndex);
    item->attachListener(this);
    m_dependItems.append(item);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItemListener::remDepend(DwGraphicsItem* item)
{
    DW_ASSERT(item);
    DW_ASSERT(m_dependItems.indexOf(item) != InvalidIndex);
    item->detachListener(this);
    m_dependItems.removeOne(item);
}

//------------------------------------------------------------------------------
/**
*/
bool DwGraphicsItemListener::hasDepend(DwGraphicsItem* item) const
{
    return m_dependItems.indexOf(item) != InvalidIndex;
}

//------------------------------------------------------------------------------
/**
*/
int  DwGraphicsItemListener::dependSize() const
{
    return m_dependItems.size();
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem* DwGraphicsItemListener::dependAt(int index) const
{
    DW_ASSERT(index >=0 && index < m_dependItems.size());
    return m_dependItems.at(index);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsItemListener::remAllDepends()
{
    DwVector<DwGraphicsItem *> depends = m_dependItems;
    for (int i=0; i<depends.size(); ++i)
    {
        remDepend(depends.at(i));
    }
}