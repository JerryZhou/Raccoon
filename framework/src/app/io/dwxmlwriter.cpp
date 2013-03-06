
#include "stable.h"
#include "dwxmlwriter.h"
#include "dwapp/dwutil.h"
#include "tinyxml/tinyxml.h"

DW_IMPLEMENT_CLASS(DwXmlWriter, 'XMLW', DwStreamWriter);

//------------------------------------------------------------------------------
class DwXmlWriterHelper
{
public:
    DwXmlWriterHelper() :
        m_xmlDocument(0),
        m_curNode(0)
    {
        ;
    }

    TiXmlDocument* m_xmlDocument;
    TiXmlElement* m_curNode;
protected:
private:
};

//------------------------------------------------------------------------------
/**
*/
DwXmlWriter::DwXmlWriter() :
    m_helper(0)
{
    // empty
    m_helper = new DwXmlWriterHelper();
}

//------------------------------------------------------------------------------
/**
*/
DwXmlWriter::~DwXmlWriter()
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
    Open the XML stream for writing. This will create a new TiXmlDocument
    object which will be written to the stream in Close().
*/
bool
DwXmlWriter::open()
{
    DW_ASSERT(0 == m_helper->m_xmlDocument);
    DW_ASSERT(0 == m_helper->m_curNode);

    if (DwStreamWriter::open())
    {
        // create xml document object
        m_helper->m_xmlDocument = new (TiXmlDocument);

        // add declaration (<?xml version="1.0" encoding="UTF-8"?>)
        m_helper->m_xmlDocument->InsertEndChild(TiXmlDeclaration("1.0", "UTF-8", ""));
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
    Close the XML stream.
*/
void
DwXmlWriter::close()
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(0 != m_helper->m_xmlDocument);

    // write XML data to stream
    m_helper->m_xmlDocument->SaveStream(this->m_stream);
    
    // delete the XML document object
    delete(m_helper->m_xmlDocument);
    m_helper->m_xmlDocument = 0;
    m_helper->m_curNode = 0;
        
    // close the stream
    DwStreamWriter::close();
}

//------------------------------------------------------------------------------
/**
    Begin a new node. The new node will be set as the current
    node. Nodes may form a hierarchy. Make sure to finalize a node
    with a corresponding call to EndNode()!
*/
bool
DwXmlWriter::beginNode(const DwString& name)
{
    DW_ASSERT(this->isOpen());
    if (0 == m_helper->m_curNode)
    {
        // create the root node
        m_helper->m_curNode = m_helper->m_xmlDocument->InsertEndChild(TiXmlElement(name.toUtf8().data()))->ToElement();
    }
    else
    {
        // create a child node
        m_helper->m_curNode = m_helper->m_curNode->InsertEndChild(TiXmlElement(name.toUtf8().data()))->ToElement();
    }
    return true;
}

//------------------------------------------------------------------------------
/**
    Finalize current node. This will set the parent of the current node as 
    new current node so that correct hierarchical behaviour is implemented.
*/
void
DwXmlWriter::endNode()
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(0 != m_helper->m_curNode);

    TiXmlNode* parent = m_helper->m_curNode->Parent();
    DW_ASSERT(parent);
    if (parent == m_helper->m_xmlDocument)
    {
        // we're back at the root
        m_helper->m_curNode = 0;
    }
    else
    {
        m_helper->m_curNode = parent->ToElement();
    }
}

//------------------------------------------------------------------------------
/**
    Write inline text at current position.
*/
void
DwXmlWriter::writeContent(const DwString& text)
{
    DW_ASSERT(0 != m_helper->m_curNode);
    m_helper->m_curNode->InsertEndChild(TiXmlText(text.toUtf8().data()));
}

//------------------------------------------------------------------------------
/**
    Write a comment into the XML file.
*/
void
DwXmlWriter::writeComment(const DwString& comment)
{
    DW_ASSERT(0 != m_helper->m_curNode);
    TiXmlComment tiXmlComment;
    tiXmlComment.SetValue(comment.toUtf8().data());
    m_helper->m_curNode->InsertEndChild(tiXmlComment);
}

//------------------------------------------------------------------------------
/**
    Set the provided attribute to a string value.
*/
void
DwXmlWriter::setString(const DwString& name, const DwString& value)
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(0 != m_helper->m_curNode);
    DW_ASSERT(!name.isNull());
    m_helper->m_curNode->SetAttribute(name.toUtf8().data(), value.toUtf8().data());
}

//------------------------------------------------------------------------------
/**
    Set the provided attribute to a bool value.
*/
void
DwXmlWriter::setBool(const DwString& name, bool value)
{
    if (value)
    {
        this->setString(name, "true");
    }
    else
    {
        this->setString(name, "false");
    }
    
}

//------------------------------------------------------------------------------
/**
    Set the provided attribute to an int value.
*/
void
DwXmlWriter::setInt(const DwString& name, int value)
{
    this->setString(name, DwString::number(value));
}

//------------------------------------------------------------------------------
/**
    Set the provided attribute to a float value.
*/
void
DwXmlWriter::setFloat(const DwString& name, float value)
{
    DwString s;
    DwStringUtil::stringFormat(s, "%.6f", value);
    this->setString(name, s);
}