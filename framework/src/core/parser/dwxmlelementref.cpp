#include "stable.h"
#include "dwxmlelementref.h"
#include "dwxmlelement.h"


DwXmlElementRef::DwXmlElementRef() : m_pElement(0)
{

}


DwXmlElementRef::~DwXmlElementRef()
{
#ifdef DW_SAFE_PARSER
    if (m_pElement)
    {
        m_pElement->removeRef(this);
    }
#endif
}

//TODO:how to be better,const_cast,too bad
DwXmlElementRef::DwXmlElementRef(DwXmlElement* pe)
    : m_pElement(pe)
{
    DW_ASSERT(m_pElement);
#ifdef DW_SAFE_PARSER
    if (m_pElement)
    {
        m_pElement->addRef(this);
    }
#endif
}

DwXmlElementRef::DwXmlElementRef( const DwXmlElementRef& rhs ) : m_pElement(NULL)
{
    *this = rhs;
}

DwXmlElementRef& DwXmlElementRef::operator = ( const DwXmlElementRef& rhs )
{
    if (this == &rhs)
    {
        return *this;
    }

#ifdef DW_SAFE_PARSER
    if (m_pElement)
    {
        m_pElement->removeRef(this);
    }
#endif

    m_pElement = rhs.m_pElement;

#ifdef DW_SAFE_PARSER
    if (m_pElement)
    {
        m_pElement->addRef(this);
    }
#endif
    return *this;
}

DwString DwXmlElementRef::tag() const
{
    DW_ASSERT(m_pElement);
    return m_pElement->tag();
}

void DwXmlElementRef::setTag( const DwString& tag )
{
    DW_ASSERT(m_pElement);
    m_pElement->setTag(tag);
}

DwString DwXmlElementRef::text() const
{
    DW_ASSERT(m_pElement);
    return m_pElement->text();
}

void DwXmlElementRef::setText( const DwString& text )
{
    DW_ASSERT(m_pElement);
    return m_pElement->setText(text);
}

int DwXmlElementRef::childCount() const
{
    DW_ASSERT(m_pElement);
    return m_pElement->childCount();
}

DwXmlElementRef DwXmlElementRef::child( int index ) const
{
    DW_ASSERT(m_pElement);
    return DwXmlElementRef(m_pElement->child(index));
}

void DwXmlElementRef::addChild( const DwXmlElementRef& child )
{
    DW_ASSERT(m_pElement);
    DW_ASSERT(child.m_pElement);
    if (child.m_pElement)
    {
        return m_pElement->addChild(child.m_pElement);
    }
}

int DwXmlElementRef::attributeCount() const
{
    DW_ASSERT(m_pElement);
    return m_pElement->attributeCount();
}

DwString DwXmlElementRef::attribute( const DwString &attrName ) const
{
    DW_ASSERT(m_pElement);
    return m_pElement->attribute(attrName);
}

DwString DwXmlElementRef::attribute( int index ) const
{
    DW_ASSERT(m_pElement);
    return m_pElement->attribute(index);
}

DwString DwXmlElementRef::attributeName( int index ) const
{
    DW_ASSERT(m_pElement);
    return m_pElement->attributeName(index);
}

void DwXmlElementRef::setAttribute( const DwString &attrName, const DwString &attrValue )
{
    DW_ASSERT(m_pElement);
    m_pElement->setAttribute(attrName, attrValue);
}

void DwXmlElementRef::removeAttribute( const DwString &attrName )
{
    DW_ASSERT(m_pElement);
    m_pElement->removeAttribute(attrName);
}

void DwXmlElementRef::deleteChild( DwXmlElementRef& child )
{
    DW_ASSERT(m_pElement);
    //remove from childlist first
    DW_ASSERT(child.m_pElement);
    if (child.m_pElement)
    {
        m_pElement->deleteChild(child.m_pElement);
    }

    //destroy elementRef
    child.destroy();
}

DwXmlElementRef DwXmlElementRef::createChild()
{
    DW_ASSERT(m_pElement);
    return DwXmlElementRef(m_pElement->createChild());
}

DwXmlElementRef DwXmlElementRef::operator[]( int index ) const
{
    return child(index);
}

DwString DwXmlElementRef::operator[]( const DwString &attrName ) const
{
    return attribute(attrName);
}

bool DwXmlElementRef::isValid() const
{
    return m_pElement != NULL;
}

void DwXmlElementRef::destroy()
{
    //destroy child first
    int cn = childCount();
    for (int i =0; i < cn; i++)
    {
        DwXmlElementRef c = child(i);
        c.destroy();
    }
    //then destroy self
    DW_SAFE_DELETE(m_pElement);
}

