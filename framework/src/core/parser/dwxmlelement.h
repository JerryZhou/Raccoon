#pragma once

#include "dwcore/dwstring.h"
#include "dwcore/dwvector.h"
#include "dwcore/dwarraylist.h"
#include "dwcore/dwmalloc.h"

/*
enum eDwXmlValueType
{
    eDwXmlTypeNull,
    eDwXmlTypeCDATA,
};

struct DwXmlValueData
{
    DwXmlValueData() : eType(eDwXmlTypeNull)
    {

    }

    DwXmlValueData(const DwString& v) : eType(eDwXmlTypeNull), value(v)
    {

    }

    DwXmlValueData(eDwXmlValueType e, const DwString& v) : eType(e), value(v)
    {

    }

    operator DwString()
    {
        return value;
    }

    eDwXmlValueType    eType;
    DwString    value;
};
*/

class DwXmlElementRef;
class DwXmlRefProxy
{
public:
    void addRef(DwXmlElementRef *p);
    void removeRef(DwXmlElementRef *p);
    virtual ~DwXmlRefProxy();

private:
    DwArrayList<DwXmlElementRef*> m_refList;
};

#ifdef DW_SAFE_PARSER
class DwXmlElement : public DwXmlRefProxy
#else
class DwXmlElement
#endif
{
    friend class DwXmlElementRef;
    friend class DwXmlDocument;

private:
    explicit DwXmlElement(const DwString &name);
    DwXmlElement();
    ~DwXmlElement();

public:
    struct KeyValue
    {
        DwString    m_key;
        DwString    m_value;
    };

    void *operator new(size_t s)
    {
        return dwMalloc(s);
    }
    void operator delete(void *p)
    {
        return dwFree(p);
    }

public:
    DwString tag() const;
    void setTag(const DwString& tag);

    DwString text() const;
    void setText(const DwString& text);

    int childCount() const;
    DwXmlElement *child(int index) const;
    void addChild(DwXmlElement *child);
    int attributeCount() const;

    DwString attribute( const DwString &attrName ) const;
    DwString attribute( int index ) const;
    DwString attributeName( int index ) const;
    void setAttribute(const DwString &attrName, const DwString &attrValue);
    void removeAttribute(const DwString &attrName);

    void deleteChild(const DwXmlElement *child);
    DwXmlElement* createChild();

private:
    DwString	m_tag;
    DwString	m_text;
    typedef DwVector<KeyValue>	StringPairVector;
    StringPairVector	m_attributes;
    typedef DwVector<DwXmlElement*> XmlElements;
    XmlElements	m_childList;
};