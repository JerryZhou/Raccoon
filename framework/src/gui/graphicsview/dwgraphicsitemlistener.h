#pragma once
#include "dwnamespace.h"
#include "dwcore/dwvariant.h"
#include "dwcore/dwvector.h"

//------------------------------------------------------------------------------
class DwGraphicsItem;

//NB! LISTENER SELF MANGER THE LIFETIME, WHEN THE ITEM DESTROY,
//ITEM WILL JUST CALL ONREMOVED(ITEM)
//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwGraphicsItemListener
{
public:
    DwGraphicsItemListener();
    virtual ~DwGraphicsItemListener();

    virtual void onChange(Dw::ChangeType change,
                          DwGraphicsItem* item, const DwVariant& newOne, const DwVariant& oldOne) = 0;

    virtual void onAdded(DwGraphicsItem* item) = 0;

    virtual void onRemoved(DwGraphicsItem* item) = 0;

    // cared changes of this listener
    void setChange(Dw::ChangeType change, bool accept);
    inline uint32_t changes() const;

    // cared item's of this listener
    void addDepend(DwGraphicsItem* item);
    void remDepend(DwGraphicsItem* item);
    bool hasDepend(DwGraphicsItem* item) const;
    int  dependSize() const;
    DwGraphicsItem* dependAt(int index) const;
    void remAllDepends();

protected:
    uint32_t m_changes;
    DwVector<DwGraphicsItem *> m_dependItems;
private:
};// end of DwGraphicsItemListener

//------------------------------------------------------------------------------
/**
*/
inline uint32_t DwGraphicsItemListener::changes() const
{
    return m_changes;
}