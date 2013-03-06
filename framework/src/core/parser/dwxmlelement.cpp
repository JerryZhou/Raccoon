#include "stable.h"

#include <dwcore/dwdebug.h>
#include "dwxmlelement.h"
#include "dwxmlelementref.h"
#include "eastl/algorithm.h"

bool operator == (const DwXmlElement::KeyValue &left, const DwString &key)
{
    return left.m_key == key;
}

void DwXmlRefProxy::addRef(DwXmlElementRef *p)
{
    DW_ASSERT(p);
    if (p)
    {
        DW_ASSERT(m_refList.indexOf(p) == -1);
        m_refList.append(p);
    }
}

void DwXmlRefProxy::removeRef(DwXmlElementRef *p)
{
    DW_ASSERT(p);
    if (p)
    {
        m_refList.removeOne(p);
    }
}

DwXmlRefProxy::~DwXmlRefProxy()
{
    DwArrayList<DwXmlElementRef*>::iterator itEnd = m_refList.end();
    DwArrayList<DwXmlElementRef*>::iterator it = m_refList.begin();
    while(it != itEnd)
    {
        DwXmlElementRef* p = *it;
        DW_ASSERT(p);
        if (p)
        {
            p->clear();
        }
        ++it;
    }
}

DwXmlElement::DwXmlElement()
{

}

DwXmlElement::~DwXmlElement()
{

}

DwXmlElement::DwXmlElement( const DwString &name )
{
    m_tag = name;
}

DwString DwXmlElement::tag() const
{
    return m_tag;
}

void DwXmlElement::setTag( const DwString& tag )
{
    m_tag = tag;
}

DwString DwXmlElement::text() const
{
    return m_text;
}

void DwXmlElement::setText( const DwString& text )
{
    m_text = text;
}

int DwXmlElement::childCount() const
{
    return m_childList.size();
}

DwXmlElement * DwXmlElement::child( int index ) const
{
    if (index < 0 || index >= m_childList.size())
    {
        return NULL;
    }
    return m_childList[index];
}

void DwXmlElement::addChild( DwXmlElement *child )
{
    DW_ASSERT(child);
    m_childList.push_back(child);
}

int DwXmlElement::attributeCount() const
{
    return m_attributes.size();
}

DwString DwXmlElement::attribute( const DwString &attrName ) const
{
    StringPairVector::const_iterator it = eastl::find(m_attributes.begin(), m_attributes.end(), attrName);
    if (it == m_attributes.end())
    {
        dwDebug() << "DwXmlElement::attribute <" << m_tag << "> attribute [" << attrName << "] not found";
        return DwString();
    }

    return it->m_value;
}

DwString DwXmlElement::attribute( int index ) const
{
    if (index < 0 || index >= m_attributes.size())
    {
        DW_ASSERT(false);
        return DwString();
    }

    return m_attributes[index].m_value;
}

DwString DwXmlElement::attributeName( int index ) const
{
    if (index < 0 || index >= m_attributes.size())
    {
        DW_ASSERT(false);
        return DwString();
    }

    return m_attributes[index].m_key;
}

void DwXmlElement::setAttribute( const DwString &attrName, const DwString &attrValue )
{
    StringPairVector::iterator it = eastl::find(m_attributes.begin(), m_attributes.end(), attrName);
    if (it == m_attributes.end())
    {
        KeyValue attr;
        attr.m_key = attrName;
        attr.m_value = attrValue;
        m_attributes.push_back(attr);

        return;
    }

    it->m_value = attrValue;
}

void DwXmlElement::removeAttribute( const DwString &attrName )
{
    StringPairVector::iterator it = eastl::find(m_attributes.begin(), m_attributes.end(), attrName);
    if (it == m_attributes.end())
    {
        DW_ASSERT(false);
        return;
    }
    m_attributes.erase(it);
}

void DwXmlElement::deleteChild( const DwXmlElement *child )
{
    DW_ASSERT(child);
    XmlElements::iterator it = eastl::find(m_childList.begin(), m_childList.end(), child);
    if (it == m_childList.end())
    {
        DW_ASSERT(false);
        return;
    }
    m_childList.erase(it);
}

DwXmlElement* DwXmlElement::createChild()
{
    DwXmlElement* pe = new DwXmlElement();
    m_childList.append(pe);
    return pe;
}
