#include "stable.h"
#include "dwi18n.h"
#include "dwcore/dwxmlparser.h"
#include "dwgui/dwresourceinterface.h"
#include "graphicsview/dwdummys.h"

static const wchar_t KXmlPathSeparator[] = L"/";
static const wchar_t KXmlPathAttrSeparator[] = L"@";

// a special xml op class
//------------------------------------------------------------------------------
class DwXmlDocumentContainer : public DwXmlDocument
{
public:
    // the element path in DwXmlDocumentContainer is a unique path for a  unique node
    // next three interface will always create node in the element path
    DwXmlElementRefList elementByPath( const DwString &elementPath ) const
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

        DwVector<DwString> ePaths = dwTokenize(ePath, KXmlPathSeparator);
        DwXmlElementRefList elements;
        findAndCreateElementByPath(rootElement(), ePaths, 0, elements);
        return elements;
    }

    void findAndCreateElementByPath(const DwXmlElementRef& baseEle, const DwVector<DwString>& paths, int cur,
        DwXmlElementRefList& elements) const
    {
        if (cur >= paths.size())
        {
            return;
        }

        DW_ASSERT(baseEle.isValid());
        DwString eTag = paths[cur];
        DwXmlElementRef nextEle;
        int cn = baseEle.childCount();
        for (int i = 0; i < cn; i++)
        {
            DwXmlElementRef r = baseEle[i];
            DW_ASSERT(r.isValid());

            if (r.tag() == eTag)
            {
                // find break
                nextEle = r;
                break;
            }
        }

        if (!nextEle.isValid())
        {
            // not find , so we create it and it's child
            nextEle = baseEle;
            for (int i=cur; i<paths.size(); ++i)
            {
                DwXmlElementRef child = nextEle.createChild();
                child.setTag(paths.at(i));
                elements.push_back(child);
                nextEle = child;
            }
        }
        else
        {
            // find it
            elements.push_back(nextEle);
            ++cur;
            // keep from unnecessary function call
            if (cur < paths.size())
            {
                findAndCreateElementByPath(nextEle, paths, cur, elements);
            }
        }
    }
};

//------------------------------------------------------------------------------
/**
*/
DwI18n::DwI18n()
{
    m_doc = new DwXmlDocumentContainer();

    // we need a empty doc
    m_doc->loadFromString(DwString::fromUtf8("<?xml version=\"1.0\" encoding=\"utf8\"?><DwI18n/>"));
}

//------------------------------------------------------------------------------
/**
*/
DwI18n::~DwI18n()
{
    DW_SAFE_DELETE(m_doc);
}

//------------------------------------------------------------------------------
/**
*/
bool	DwI18n::isEmpty () const
{
    return m_doc->isValid() ;//|| m_contents.isEmpty();
}

//------------------------------------------------------------------------------
/**
*/
bool DwI18n::load( const DwUrl & url )
{
    DwByteArray data = DwResourceInterface::acquireData(url);
    return load(data.data(), data.size());
}

//------------------------------------------------------------------------------
/**
*/
bool DwI18n::load( const char * data, int len )
{
    DW_ASSERT(data);
    return m_doc->loadFromString(DwString::fromUtf8(data, len));
}

//------------------------------------------------------------------------------
/**
*/
bool DwI18n::save( const DwUrl & url )
{
    return m_doc->saveToFile(url.localPath());
}

//------------------------------------------------------------------------------
/**
    TODO: optimize , use element ref
*/
DwString DwI18n::translate( const char *context, const char * sourceText, const char * disambiguation /*= 0 */ )
{
    DW_ASSERT(sourceText);
    DwString path;
    // context/disambiguation/sourceText
    dwStringFormat(path, "%s/%s/%s", context ? context : "DwI18n", disambiguation ? disambiguation: "DwI18n", sourceText ? sourceText: "DwI18n");
    // just remove the space
    path.replace(" ", "");
    path.replace("'", "");

    DwXmlElementRefList elements = m_doc->elementByPath(path);
    DW_ASSERT(!elements.isEmpty());

    DwString result;
    if (!elements.isEmpty())
    {
        result = elements.back().text();
        if (result.isEmpty())
        {
            result = DwString::fromUtf8(sourceText);
            elements.back().setText(result);
        }
    }
    return result;
}
