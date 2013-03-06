#pragma once

#include "dwapp/dwstreamwriter.h"
#include "dwapp/dwhtmlelement.h"
#include "dwapp/dwxmlwriter.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwHtmlPageWriter : public DwStreamWriter
{
    DW_DECLARE_CLASS(DwHtmlPageWriter);
public:
    /// constructor
    DwHtmlPageWriter();
    /// destructor
    virtual ~DwHtmlPageWriter();
    /// set the title of the page
    void SetTitle(const DwString& title);
    /// begin writing the stream
    virtual bool open();
    /// end writing the stream
    virtual void close();

    /// add an attribute for the next element
    void AddAttr(const DwString& name, const DwString& value);
    /// begin a generic element
    void Begin(DwHtmlElement::Code e);
    /// end a generic element
    void End(DwHtmlElement::Code e);
    /// shortcut for Begin()/Text()/End()
    void Element(DwHtmlElement::Code e, const DwString& text);
    /// write a line break
    void LineBreak();
    /// write a horizontal rule
    void HorizontalRule();
    /// add inline text
    void Text(const DwString& t);

    /// write a 2-element table row
    void TableRow2(const DwString& col0, const DwString& col1);

private:
    DwString title;
    DwPtr<DwXmlWriter> xmlWriter;
    DwVector<std::pair<DwString,DwString> > attrs;
    DwVector<DwHtmlElement::Code> elementStack;
};// end of DwHtmlPageWriter

//------------------------------------------------------------------------------
/**
*/
inline void
DwHtmlPageWriter::SetTitle(const DwString& t)
{
    this->title = t;
}
    