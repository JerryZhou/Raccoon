#include "stable.h"

#include <dwcore/dwdebug.h>
#include "dwxmlelement.h"
#include "dwxmldocument.h"
#include "3rdparty/Markup.h"

static const wchar_t KXmlPathSeparator[] = L"/";
static const wchar_t KXmlPathAttrSeparator[] = L"@";

class DwXmlDocumentPrivate
{
public:
    DwXmlDocumentPrivate()
    {
    }

    ~DwXmlDocumentPrivate()
    {
        if (m_xmlRoot.isValid())
        {
            m_xmlRoot.destroy();
        }
    }
    DwString m_xmlFilePath;
    DwXmlElementRef m_xmlRoot;
};

DwXmlDocument::DwXmlDocument(DwObject *parent)
    : DwObject(parent)
    , d_ptr(new DwXmlDocumentPrivate)
{

}

DwXmlDocument::~DwXmlDocument( void )
{
    DW_SAFE_DELETE(d_ptr);
}

bool DwXmlDocument::loadFromFile( const DwString &xmlFile )
{
    if (isValid())
    {
        //this document object has already in use
        DW_ASSERT(false);
        return false;
    }
    //TODO: uses DwFileand DwTextSteam
    CMarkup xml;
    if (!xml.Load(xmlFile.constString()))
    {
        return false;
    }
    //TODO:do i need to save file name?
    d_ptr->m_xmlFilePath = xmlFile;
    return parseXml(xml);
}

bool DwXmlDocument::loadFromString( const DwString &xmlString )
{
    CMarkup xml;
    if (!xml.SetDoc(xmlString.constString()))
    {
        return false;
    }
    return parseXml(xml);
}

DwXmlElementRef DwXmlDocument::rootElement() const
{
    return d_ptr->m_xmlRoot;
}

DwXmlElementRefList DwXmlDocument::elementByPath( const DwString &elementPath ) const
{
    if (!isValid())
    {
        DW_ASSERT(false);
        return DwXmlElementRefList();
    }

    DwString ePath = elementPath;
    if (ePath.startsWith(KXmlPathSeparator))
    {
        ePath = ePath.remove(0, 1/*DwString(DwXmlxpathTag).length()*/);
    }

    DwXmlElementRefList elements;
    findElementByPath(d_ptr->m_xmlRoot, ePath, elements);
    return elements;
}

DwString DwXmlDocument::value( const DwString& valuePath )
{
    DwArrayList<DwString> pathList;
    //TODO:need to split string with ':'
    int pos = valuePath.lastIndexOf(KXmlPathAttrSeparator);
    if (pos == -1)
    {
        pathList.push_back(valuePath);
    }
    else
    {
        DwString pathName = valuePath.left(pos);
        DwString attrName = valuePath.right(valuePath.length() - pos - 1);
        pathList.push_back(pathName);
        pathList.push_back(attrName);
    }

    if (pathList.size() == 1)
    {
        DwXmlElementRefList elements = elementByPath(valuePath);
        //DW_ASSERT(elements.size() == 1);
        if (elements.isEmpty())
        {
            return DwString();
        }

        DwXmlElementRef& r = elements[0];
        if(r.isValid())
        {
            return r.text();
        }
        else
        {
            DW_ASSERT(false);
            return DwString();
        }
    }
    else if (pathList.size() == 2)
    {
        DwXmlElementRefList elements = elementByPath(pathList[0]);
        DW_ASSERT(elements.size() == 1);
        if (elements.isEmpty())
        {
            return DwString();
        }

        DwXmlElementRef& r = elements[0];
        if(r.isValid())
        {
            return r.attribute(pathList[1]);
        }
        else
        {
            DW_ASSERT(false);
            return DwString();
        }
    }
    else
    {
        DW_ASSERT(false);
        return DwString();
    }
}

void DwXmlDocument::setValue( const DwString& valuePath, const DwString& value )
{
    DwArrayList<DwString> pathList;
    //TODO:need to split string with ':'
    int pos = valuePath.lastIndexOf(KXmlPathAttrSeparator);
    if (pos == -1)
    {
        pathList.push_back(valuePath);
    }
    else
    {
        DwString pathName = valuePath.left(pos);
        DwString attrName = valuePath.right(valuePath.length() - pos - 1);
        pathList.push_back(pathName);
        pathList.push_back(attrName);
    }
    //TODO:if path is break in middle,create all element on path?
    if (pathList.size() == 1)
    {
        DwXmlElementRefList elements = elementByPath(valuePath);
        DW_ASSERT(elements.size() == 1);
        if (elements.size() == 1)
        {
            DwXmlElementRef& r = elements[0];
            r.setText(value);
        }
        /*
        DwXmlElementRefList::iterator itBegin = elements.begin();
        DwXmlElementRefList::iterator itEnd = elements.end();
        for (; itBegin != itEnd; )
        {
            if (itBegin->isValid())
            {
                itBegin->setText(value);
            }
            ++itBegin;
        }
        */
    }
    else if (pathList.size() == 2)
    {
        DwXmlElementRefList elements = elementByPath(pathList[0]);
        DW_ASSERT(elements.size() == 1);
        if (elements.size() == 1)
        {
            DwXmlElementRef& r = elements[0];
            r.setAttribute(pathList[1], value);
        }
        /*
        DwXmlElementRefList::iterator itBegin = elements.begin();
        DwXmlElementRefList::iterator itEnd = elements.end();
        for (; itBegin != itEnd; )
        {
            if (itBegin->isValid())
            {
                itBegin->setAttribute(pathList[1], value);
            }
            ++itBegin;
        }
        */
    }
    else
    {
        DW_ASSERT(false);
    }
}

bool DwXmlDocument::saveToFile( const DwString &xmlFile )
{
    CMarkup xml;
    xml.SetDoc(_T("<?xml version=\"1.0\"?>"));
    if (saveToElement(rootElement(), xml))
    {
        xml.Save(xmlFile.constString());
        return true;
    }
    return false;
}

bool DwXmlDocument::saveToStream( DwString &xmlStream )
{
    CMarkup xml;
    xml.SetDoc(_T("<?xml version=\"1.0\"?>"));
    if (saveToElement(rootElement(), xml))
    {
        xmlStream = DwString(xml.GetDoc().c_str());
        return true;
    }
    return false;
}

bool DwXmlDocument::isValid() const
{
    return d_ptr->m_xmlRoot.m_pElement != NULL;
}

DwXmlElementRefList DwXmlDocument::createElementByPath( const DwString &elementPath )
{
    int last = elementPath.lastIndexOf(KXmlPathSeparator);
    if (last <= 0)
    {
        DwString ePath = elementPath;
        if (last == 0)
        {
            ePath = ePath.remove(0, 1/*DwString(DwXmlxpathTag).length()*/);
        }

        if (isValid())
        {
            dwDebug() << "createElementByPath [" << elementPath << "] failed, already has root.";
            return DwXmlElementRefList();
        }
        else
        {
            //create root
            d_ptr->m_xmlRoot = DwXmlElementRef(new DwXmlElement);
            d_ptr->m_xmlRoot.setTag(ePath);
            DwXmlElementRefList ret;
            ret.push_back(d_ptr->m_xmlRoot);
            return ret;
        }
    }

    int len = elementPath.length();
    if (len == last)
    {
        DW_ASSERT(false);
        return DwXmlElementRefList();
    }

    DwString tag = elementPath.right(len - last - 1);
    DwString path = elementPath.left(last);
    DwXmlElementRefList elements =  elementByPath(path);

    DwXmlElementRefList::iterator itBegin = elements.begin();
    DwXmlElementRefList::iterator itEnd = elements.end();

    DwXmlElementRefList ret;
    for(; itBegin != itEnd; )
    {
        if (itBegin->isValid())
        {
            DwXmlElementRef r = itBegin->createChild();
            r.setTag(tag);
            ret.push_back(r);
        }
        ++itBegin ;
    }

    return ret;
}

/*
DwXmlElementRef DwXmlDocument::createFreeElement()
{
    //TODO:is this function needed?
    //too dangerous,who will delete DwXmlelment* ,if the element do not link to the tree

    return DwXmlElementRef(new DwXmlElement());
}
*/

DwXmlElementRefList DwXmlDocument::operator[]( const DwString& elementPath ) const
{
    return elementByPath(elementPath);
}

bool DwXmlDocument::parseXml(  CMarkup& xml )
{
    if (!parseRootElement(xml))
    {
        return false;
    }

    xml.IntoElem();
    parseElement(xml, d_ptr->m_xmlRoot);
    return true;
}

void DwXmlDocument::findElementByPath( const DwXmlElementRef& baseEle, const DwString& path, DwXmlElementRefList& elements ) const
{
    DwString eTag = baseEle.tag();
    if (eTag == path)
    {
        elements.push_back(baseEle);
    }

    int cn = baseEle.childCount();
    for (int i = 0; i < cn; i++)
    {
        DwXmlElementRef r = baseEle[i];
        if (!r.isValid())
        {
            DW_ASSERT(false);
            continue;
        }
        DwString incPath = eTag + DwString(KXmlPathSeparator) + r.tag();
        if (incPath == path)
        {
            elements.push_back(r);
        }
        else if (path.startsWith(incPath))
        {
            //DwString header(eTag);
            //header.append(DwXmlxpathTag);
            DwString s = path;
            findElementByPath(r, s.remove(0, eTag.length() + 1/*header.length()*/), elements);
        }
    }
}

bool DwXmlDocument::saveToElement( const DwXmlElementRef& element, CMarkup &xml )
{
    if (!element.isValid())
    {
        return false;
    }

    xml.AddElem(element.tag().constString(), element.text().constString());
    int count = element.attributeCount();
    for( int index = 0; index < count; ++index)
    {
        xml.AddAttrib(element.attributeName(index).constString(), element.attribute(index).constString());
    }
    xml.IntoElem();
    count = element.childCount();
    for( int index = 0; index < count; ++index)
    {
        saveToElement(element.child(index), xml);
    }

    xml.OutOfElem();
    return true;
}

void DwXmlDocument::parseElement( CMarkup &xml, DwXmlElementRef& parent )
{
    if( !xml.FindElem() )
    {
        return;
    }

    do
    {
        DwString tagName(xml.GetTagName().c_str());
        DwXmlElementRef element(new DwXmlElement(tagName));
        parent.addChild(element);

        int attrIndex = 0;
        do
        {
            DwString attrName(xml.GetAttribName(attrIndex).c_str());
            if(attrName.isEmpty())
            {
                break;
            }
            DwString attrValue(xml.GetAttrib(attrName.constString()).c_str());
            element.setAttribute(attrName, attrValue);
            attrIndex++;
        }
        while (true);

        element.setText( xml.GetData().c_str() );
        xml.IntoElem();
        parseElement(xml, element);
        xml.OutOfElem();
    }
    while (xml.FindElem());
}

bool DwXmlDocument::parseRootElement( CMarkup &xml )
{
    if( !xml.FindElem() )
    {
        DW_ASSERT(false);
        return false;
    }

    DwString tagName(xml.GetTagName().c_str());
    DwXmlElementRef element(new DwXmlElement(tagName));
    int attrIndex = 0;
    do
    {
        DwString attrName(xml.GetAttribName(attrIndex).c_str());
        if(attrName.isEmpty())
        {
            break;
        }
        DwString attrValue(xml.GetAttrib(attrName.constString()).c_str());
        element.setAttribute(attrName, attrValue);

        attrIndex++;
    }
    while (true);

    //this will detected at loadfromstring
    /*
    if (xml.FindElem())
    {
        //The xml have more than one root!!!!
        DW_ASSERT(false);
        return false;
    }
    */
    d_ptr->m_xmlRoot = element;
    return true;
}

