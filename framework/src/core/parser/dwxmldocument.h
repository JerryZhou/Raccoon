#pragma once

#include "dwxmlelementref.h"
#include "dwcore/dwobject.h"
#include "dwcore/dwarraylist.h"

class CMarkup;
class DwXmlDocumentPrivate;
typedef DwArrayList<DwXmlElementRef> DwXmlElementRefList;

class DW_CORE_EXPORT DwXmlDocument : public DwObject
{
    DW_DISABLE_COPY(DwXmlDocument);

public:
    explicit DwXmlDocument(DwObject *parent = NULL);
    ~DwXmlDocument(void);

public:
    bool loadFromFile( const DwString &xmlFile);
    bool loadFromString( const DwString &xmlString );

    bool isValid() const;
    DwXmlElementRef rootElement() const;
    DwXmlElementRefList elementByPath( const DwString &elementPath ) const;

    DwString value(const DwString& valuePath);
    void setValue(const DwString& valuePath, const DwString& value);

    bool saveToFile(const DwString &xmlFile);
    bool saveToStream(DwString &xmlStream);

    DwXmlElementRefList createElementByPath( const DwString &elementPath );
    DwXmlElementRefList operator[](const DwString& elementPath) const;

private:
    bool parseXml( CMarkup& xml);
    void findElementByPath(const DwXmlElementRef& baseEle, const DwString& path, DwXmlElementRefList& elements) const;
    bool saveToElement(const DwXmlElementRef& element, CMarkup &xml);
    void parseElement( CMarkup &xml, DwXmlElementRef& parent );
    bool parseRootElement( CMarkup &xml );

private:
    DwXmlDocumentPrivate* d_ptr;
};
