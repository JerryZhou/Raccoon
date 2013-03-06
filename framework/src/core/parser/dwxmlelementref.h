#pragma once
#include "dwcore/dwstring.h"
#include "dwcore/dwmalloc.h"

class DwXmlElement;
class DW_CORE_EXPORT DwXmlElementRef
{
    friend class DwXmlDocumentPrivate;
    friend class DwXmlDocument;
    friend class DwXmlRefProxy;
    explicit DwXmlElementRef(DwXmlElement* pe);

public:
    DwXmlElementRef();
    ~DwXmlElementRef();

    DwXmlElementRef(const DwXmlElementRef& rhs);
    DwXmlElementRef& operator = (const DwXmlElementRef& rhs);

    void *operator new(size_t s)
    {
        return dwMalloc(s);
    }
    void operator delete(void *p)
    {
        return dwFree(p);
    }

    DwString tag() const;
    void setTag(const DwString& tag);
    DwString text() const;
    void setText(const DwString& text);

    int childCount() const;
    DwXmlElementRef child(int index) const;
    void addChild(const DwXmlElementRef& child);
    int attributeCount() const;

    DwString attribute( const DwString &attrName ) const;
    DwString attribute( int index ) const;
    DwString attributeName( int index ) const;
    void setAttribute(const DwString &attrName, const DwString &attrValue);
    void removeAttribute(const DwString &attrName);

    void deleteChild( DwXmlElementRef& child);
    DwXmlElementRef createChild();

    DwXmlElementRef operator[](int index) const;
    DwString operator[](const DwString &attrName) const;

    bool isValid() const;

private:
    DwXmlElement* m_pElement;
    //if root was destroy,but ref in document has not set to NULL,then crash will happen
    //so root cant be destory by outside
    void destroy();
    void clear()
    {
        m_pElement = NULL;
    }
};