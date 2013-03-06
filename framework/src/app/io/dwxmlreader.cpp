
#include "stable.h"
#include "dwxmlreader.h"
#include "dwapp/dwutil.h"
#include "tinyxml/tinyxml.h"

DW_IMPLEMENT_CLASS(DwXmlReader, 'XMLR', DwStreamReader);

//------------------------------------------------------------------------------
class DwXmlReaderHelper
{
public:
    /// find a node by path, handles relativ paths as well
    TiXmlNode* FindNode(const DwString& path) const;

    /// static helper object for TinyXml-Initialization at application startup
    class TinyXmlInitHelper
    {
    public:
        /// constructor
        TinyXmlInitHelper()
        {
            TiXmlBase::SetCondenseWhiteSpace(true);
        }
    };

    DwXmlReaderHelper(DwXmlReader* _this): 
        m_xmlDocument(0),
        m_curNode(0),
        m_this(_this)
    {
        ;
    }

    static TinyXmlInitHelper initTinyXml;
    TiXmlDocument* m_xmlDocument;
    TiXmlElement* m_curNode;
    DwXmlReader* m_this;
protected:
private:
};

// This static object setsup TinyXml at application startup
// (set condense white space to false). There seems to be no easy,
// alternative way (see TinyXml docs for details)
DwXmlReaderHelper::TinyXmlInitHelper DwXmlReaderHelper::initTinyXml;

    
//------------------------------------------------------------------------------
/**
*/
DwXmlReader::DwXmlReader() :
    m_helper(0)
{
    m_helper = new DwXmlReaderHelper(this);// empty
}

//------------------------------------------------------------------------------
/**
*/
DwXmlReader::~DwXmlReader()
{
    if (this->isOpen())
    {
        this->close();
    }
    delete m_helper;
    m_helper = NULL;
}

//------------------------------------------------------------------------------
/**
    Opens the stream and reads the content of the stream into
    TinyXML.
*/
bool
DwXmlReader::open()
{
    DW_ASSERT(0 == m_helper->m_xmlDocument);
    DW_ASSERT(0 == m_helper->m_curNode);

    if (DwStreamReader::open())
    {
        // create an XML document object
        m_helper->m_xmlDocument = new (TiXmlDocument);
        if (!m_helper->m_xmlDocument->LoadStream(this->m_stream))
        {
            // parsing XML structure failed, provide feedback
            const DwUrl& uri = this->m_stream->getURI();
            if (uri.isValid())
            {
                DW_ASSERT(false && "XmlReader::Open(): failed to open stream as XML '%s'\nTinyXML error: %s!");
            }
            else
            {
                DW_ASSERT(false && "XmlReader::Open(): failed to open stream as XML (URI not valid)!\nTinyXML error: %s");
            }
            delete (m_helper->m_xmlDocument);
            m_helper->m_xmlDocument = 0;
            return false;
        }

        // since the XML document is now loaded, we can close the original stream
        if (!this->m_streamWasOpen)
        {
            this->m_stream->close();
        }

        // set the current node to the root node
        m_helper->m_curNode = m_helper->m_xmlDocument->RootElement();
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
DwXmlReader::close()
{
    DW_ASSERT(0 != m_helper->m_xmlDocument);

    // delete the xml document
    delete (m_helper->m_xmlDocument);
    m_helper->m_xmlDocument = 0;
    m_helper->m_curNode = 0;

    DwStreamReader::close();
}

//------------------------------------------------------------------------------
/**
    This method returns the line number of the current node.
*/
int
DwXmlReader::GetCurrentNodeLineNumber() const
{
    DW_ASSERT(m_helper->m_curNode);
    return m_helper->m_curNode->Row();
}

//------------------------------------------------------------------------------
/**
    This method finds an xml node by path name. It can handle absolute
    paths and paths relative to the current node. All the usual file system
    path conventions are valid: "/" is the path separator, "." is the
    current directory, ".." the parent directory.
*/
TiXmlNode*
DwXmlReaderHelper::FindNode(const DwString& path) const
{
    DW_ASSERT(m_this->isOpen());
    DW_ASSERT(!path.isEmpty());

    bool absPath = (path[0] == '/');
    DwVector<DwString> tokens = DwStringUtil::tokenize(path, "/");

    // get starting node (either root or current node)
    TiXmlNode* node;
    if (absPath)
    {
        node = m_xmlDocument;
    }
    else
    {
        DW_ASSERT(0 != m_curNode);
        node = m_curNode;
    }

    // iterate through path components
    int i;
    int num = tokens.size();
    for (i = 0; i < num; i++)
    {
        const DwString& cur = tokens[i];
        if (DwString(".") == cur)
        {
            // do nothing
        }
        else if (DwString("..") == cur)
        {
            // go to parent directory
            node = node->Parent();
            if (node == m_xmlDocument)
            {
                DW_ASSERT(false && "XmlStream::FindNode(%s): path points above root node!");
                return 0;
            }
        }
        else
        {
            // find child node
            node = node->FirstChild(cur.toUtf8().data());
            if (0 == node)
            {
                return 0;
            }
        }
    }
    return node;
}

//------------------------------------------------------------------------------
/**
    This method returns true if the node identified by path exists. Path
    follows the normal filesystem path conventions, "/" is the separator,
    ".." is the parent node, "." is the current node. An absolute path
    starts with a "/", a relative path doesn't.
*/
bool
DwXmlReader::hasNode(const DwString& n) const
{
    return (m_helper->FindNode(n) != 0);
}

//------------------------------------------------------------------------------
/**
    Get the short name (without path) of the current node.
*/
DwString
DwXmlReader::GetCurrentNodeName() const
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(0 != m_helper->m_curNode);
    return DwString(m_helper->m_curNode->Value());
}

//------------------------------------------------------------------------------
/**
    This returns the full absolute path of the current node. Path components
    are separated by slashes.
*/
DwString
DwXmlReader::GetCurrentNodePath() const
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(0 != m_helper->m_curNode);

    // build bottom-up array of node names
    DwVector<DwString> components;
    TiXmlNode* node = m_helper->m_curNode;
    while (node != m_helper->m_xmlDocument)
    {
        components.append(node->Value());
        node = node->Parent();
    }

    // build top down path
    DwString path = "/";
    int i;
    for (i = components.size() - 1; i >= 0; --i)
    {
        path.append(components[i]);
        if (i > 0)
        {
            path.append("/");
        }
    }
    return path;
}

//------------------------------------------------------------------------------
/**
    Set the node pointed to by the path string as current node. The path
    may be absolute or relative, following the usual filesystem path 
    conventions. Separator is a slash.
*/
void
DwXmlReader::SetToNode(const DwString& path)
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(!path.isEmpty());
    TiXmlNode* n = m_helper->FindNode(path);
    if (n)
    {
        m_helper->m_curNode = n->ToElement();
    }
    else
    {
        DW_ASSERT(false && "XmlReader::SetToNode(%s): node to found!");
    }
}

//------------------------------------------------------------------------------
/**
    Sets the current node to the first child node. If no child node exists,
    the current node will remain unchanged and the method will return false.
    If name is a valid string, only child element matching the name will 
    be returned. If name is empty, all child nodes will be considered.
*/
bool
DwXmlReader::SetToFirstChild(const DwString& name)
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(0 != m_helper->m_curNode);
    TiXmlElement* child = 0;
    if (name.isEmpty())
    {
        child = m_helper->m_curNode->FirstChildElement();
    }
    else
    {
        child = m_helper->m_curNode->FirstChildElement(name.toUtf8().data());
    }
    if (child)
    {
        m_helper->m_curNode = child;
        return true;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
    Sets the current node to the next sibling. If no more children exist,
    the current node will be reset to the parent node and the method will 
    return false. If name is a valid string, only child element matching the 
    name will be returned. If name is empty, all child nodes will be considered.
*/
bool
DwXmlReader::SetToNextChild(const DwString& name)
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(0 != m_helper->m_curNode);

    TiXmlElement* sib = 0;
    if (name.isEmpty())
    {
        sib = m_helper->m_curNode->NextSiblingElement();
    }
    else
    {
        sib = m_helper->m_curNode->NextSiblingElement(name.toUtf8().data());
    }
    if (sib)
    {
        m_helper->m_curNode = sib;
        return true;
    }
    else
    {
        this->SetToParent();
        return false;
    }
}

//------------------------------------------------------------------------------
/**
    Sets the current node to its parent. If no parent exists, the
    current node will remain unchanged and the method will return false.
*/
bool
DwXmlReader::SetToParent()
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(0 != m_helper->m_curNode);
    TiXmlNode* parent = m_helper->m_curNode->Parent();
    if (parent)
    {
        m_helper->m_curNode = parent->ToElement();
        return true;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
    Return true if an attribute of the given name exists on the current node.
*/
bool
DwXmlReader::HasAttr(const char* name) const
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(0 != m_helper->m_curNode);
    DW_ASSERT(0 != name);
    return (0 != m_helper->m_curNode->Attribute(name));
}

//------------------------------------------------------------------------------
/**
    Return array with names of all attrs on current node
*/
DwVector<DwString>
DwXmlReader::GetAttrs() const
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(0 != m_helper->m_curNode);
    DwVector<DwString> res;
    const TiXmlAttribute * attr = m_helper->m_curNode->FirstAttribute();
    while(0 != attr)
    {
        res.append(attr->Name());
        attr = attr->Next();
    }
    return res;
}

//------------------------------------------------------------------------------
/**
    Return the provided attribute as string. If the attribute does not exist
    the method will fail hard (use HasAttr() to check for its existance).
*/
DwString
DwXmlReader::GetString(const char* name) const
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(0 != m_helper->m_curNode);
    DW_ASSERT(0 != name);
    DwString str;
    const char* val = m_helper->m_curNode->Attribute(name);    
    if (0 == val)
    {
        DW_ASSERT("XmlReader: attribute '%s' doesn't exist on node '%s'!" && false);
    }
    else
    {
        str = val;
    }
    return str;
}

//------------------------------------------------------------------------------
/**
    Return the provided attribute as a bool. If the attribute does not exist
    the method will fail hard (use HasAttr() to check for its existance).
*/
bool
DwXmlReader::GetBool(const char* name) const
{
    DwString s = this->GetString(name);
    if (s == "true" || s == "1" || s=="TRUE" || s=="True")
    {
        return true;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
    Return the provided attribute as int. If the attribute does not exist
    the method will fail hard (use HasAttr() to check for its existance).
*/
int
DwXmlReader::GetInt(const char* name) const
{
    return this->GetString(name).toInt32();
}

//------------------------------------------------------------------------------
/**
    Return the provided attribute as float. If the attribute does not exist
    the method will fail hard (use HasAttr() to check for its existance).
*/
float
DwXmlReader::GetFloat(const char* name) const
{
    DwString s = this->GetString(name);
    return float(atof(s.toUtf8().data()));
}
    
//------------------------------------------------------------------------------
/**
    Return the provided optional attribute as string. If the attribute doesn't
    exist, the default value will be returned.
*/
DwString
DwXmlReader::GetOptString(const char* name, const DwString& defaultValue) const
{
    if (this->HasAttr(name))
    {
        return this->GetString(name);
    }
    else
    {
        return defaultValue;
    }
}

//------------------------------------------------------------------------------
/**
    Return the provided optional attribute as bool. If the attribute doesn't
    exist, the default value will be returned.
*/
bool
DwXmlReader::GetOptBool(const char* name, bool defaultValue) const
{
    if (this->HasAttr(name))
    {
        return this->GetBool(name);
    }
    else
    {
        return defaultValue;
    }
}

//------------------------------------------------------------------------------
/**
    Return the provided optional attribute as int. If the attribute doesn't
    exist, the default value will be returned.
*/
int
DwXmlReader::GetOptInt(const char* name, int defaultValue) const
{
    if (this->HasAttr(name))
    {
        return this->GetInt(name);
    }
    else
    {
        return defaultValue;
    }
}

//------------------------------------------------------------------------------
/**
    Return the provided optional attribute as float. If the attribute doesn't
    exist, the default value will be returned.
*/
float
DwXmlReader::GetOptFloat(const char* name, float defaultValue) const
{
    if (this->HasAttr(name))
    {
        return this->GetFloat(name);
    }
    else
    {
        return defaultValue;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
DwXmlReader::HasContent() const
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(0 != m_helper->m_curNode);
    TiXmlNode* child = m_helper->m_curNode->FirstChild();
    return child && (child->Type() == TiXmlNode::TEXT);
}

//------------------------------------------------------------------------------
/**
*/
DwString
DwXmlReader::GetContent() const
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(m_helper->m_curNode);
    TiXmlNode* child = m_helper->m_curNode->FirstChild();
    DW_ASSERT(child->Type() == TiXmlNode::TEXT);
    return child->Value();
}