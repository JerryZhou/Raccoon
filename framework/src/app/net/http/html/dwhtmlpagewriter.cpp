
#include "stable.h"
#include "dwhtmlpagewriter.h"
#include "dwapp/dwutil.h"

DW_IMPLEMENT_CLASS(DwHtmlPageWriter, 'HMPW', DwStreamWriter);

//------------------------------------------------------------------------------
/**
*/
DwHtmlPageWriter::DwHtmlPageWriter()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
DwHtmlPageWriter::~DwHtmlPageWriter()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
DwHtmlPageWriter::open()
{
    if (DwStreamWriter::open())
    {
        // create an XmlWriter and attach it to our stream
        this->xmlWriter = DwXmlWriter::create();
        this->xmlWriter->setStream(this->m_stream);
        this->xmlWriter->open();
        
        this->Begin(DwHtmlElement::Html);
        this->Begin(DwHtmlElement::Head);
        this->Begin(DwHtmlElement::Title);
        this->Text(this->title);
        this->End(DwHtmlElement::Title);
        this->End(DwHtmlElement::Head);
        this->Begin(DwHtmlElement::Body);

        return true;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
DwHtmlPageWriter::close()
{
    DW_ASSERT(this->isOpen());

    // close HTML page
    this->End(DwHtmlElement::Body);
    this->End(DwHtmlElement::Html);
    DW_ASSERT(elementStack.isEmpty());

    // close XML writer
    this->xmlWriter->close();
    this->xmlWriter = 0;

    // set MIME type on our stream
    this->m_stream->setMediaType(DwMediaType("text/html"));    

    // call parent class
    DwStreamWriter::close();
}

//------------------------------------------------------------------------------
/**
    Adds an attribute for the next HTML element. Any number of attributes
    can be defined before calling Begin(). The attribute stack will be cleared
    as Begin() returns.
*/
void
DwHtmlPageWriter::AddAttr(const DwString& name, const DwString& value)
{
    DW_ASSERT(this->isOpen());
    DW_ASSERT(!name.isNull());
    DW_ASSERT(!value.isNull());
    this->attrs.append(std::make_pair<DwString,DwString>(name, value));
}

//------------------------------------------------------------------------------
/**
    Begin a generic HTML element. Elements can be nested.
*/
void
DwHtmlPageWriter::Begin(DwHtmlElement::Code element)
{
    DW_ASSERT(this->isOpen());
    this->elementStack.push_back(element);
    this->xmlWriter->beginNode(DwHtmlElement::toHtml(element));
    int i;
    for (i = 0; i < this->attrs.size(); i++)
    {
        this->xmlWriter->setString(this->attrs[i].first, this->attrs[i].second);
    }
    this->attrs.clear();
}

//------------------------------------------------------------------------------
/**
    End a generic HTML element. The element must match the last Begin() call!
*/
void
DwHtmlPageWriter::End(DwHtmlElement::Code element)
{
    DW_UNUSED(element);
    DW_ASSERT(this->isOpen());
    DW_ASSERT(!this->elementStack.isEmpty());
    DW_ASSERT(this->elementStack.back() == element);
    this->elementStack.pop_back();
    this->xmlWriter->endNode();
}

//------------------------------------------------------------------------------
/**
*/
void
DwHtmlPageWriter::LineBreak()
{
    DW_ASSERT(this->isOpen());
    this->xmlWriter->beginNode("br");
    this->xmlWriter->endNode();
}

//------------------------------------------------------------------------------
/**
*/
void
DwHtmlPageWriter::HorizontalRule()
{
    DW_ASSERT(this->isOpen());
    this->xmlWriter->beginNode("hr");
    this->xmlWriter->endNode();
}

//------------------------------------------------------------------------------
/**
*/
void
DwHtmlPageWriter::Text(const DwString& str)
{
    this->xmlWriter->writeContent(str);
}

//------------------------------------------------------------------------------
/**
*/
void
DwHtmlPageWriter::Element(DwHtmlElement::Code e, const DwString& text)
{
    this->Begin(e);
    this->Text(text);
    this->End(e);
}

//------------------------------------------------------------------------------
/**
*/
void
DwHtmlPageWriter::TableRow2(const DwString& col0, const DwString& col1)
{
    this->Begin(DwHtmlElement::TableRow);
    this->Element(DwHtmlElement::TableData, col0);
    this->Element(DwHtmlElement::TableData, col1);
    this->End(DwHtmlElement::TableRow);   
}