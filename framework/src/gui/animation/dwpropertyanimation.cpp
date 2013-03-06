#include "stable.h"
#include "dwpropertyanimation.h"
#include "widgets/dwwidgetitem.h"

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwPropertyAnimation, 'PPAN', DwVariantAnimation);

//------------------------------------------------------------------------------
/**
*/
DwPropertyAnimation::DwPropertyAnimation(DwObject *parent)
    : DwVariantAnimation(parent)
    , m_targetItem(NULL)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwPropertyAnimation::~DwPropertyAnimation()
{
    dettach();
}

//------------------------------------------------------------------------------
/**
*/
void DwPropertyAnimation::setTargetItem(DwGraphicsItem *item)
{
    item->addAnimation(this);
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem *DwPropertyAnimation::targetItem() const
{
    return m_targetItem;
}

//------------------------------------------------------------------------------
/**
*/
void DwPropertyAnimation::setProperty(const DwString& name)
{
    m_property = name;
}

//------------------------------------------------------------------------------
/**
*/
DwString DwPropertyAnimation::propertyOfThis() const
{
    return m_property;
}

//------------------------------------------------------------------------------
/**
*/
void DwPropertyAnimation::onAttachTo(DwRttiObject *obj)
{
    m_targetItem = dwsafe_cast<DwGraphicsItem>(obj);
}

//------------------------------------------------------------------------------
/**
*/
void DwPropertyAnimation::onDettachTo(DwRttiObject *obj)
{
    if (m_targetItem == dwsafe_cast<DwGraphicsItem>(obj))
    {
        m_targetItem = NULL;
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwPropertyAnimation::updateCurrentValue(const DwVariant &v)
{
    if (m_targetItem)
    {
        m_targetItem->setProperty(m_property, v);
    }
}