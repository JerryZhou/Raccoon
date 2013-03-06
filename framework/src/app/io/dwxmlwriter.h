#pragma once

#include "dwapp/dwstreamwriter.h"

//------------------------------------------------------------------------------
class DwXmlWriterHelper;

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwXmlWriter : public DwStreamWriter
{
    DW_DECLARE_CLASS(DwXmlWriter);
public:
    /// constructor
    DwXmlWriter();
    /// destructor
    virtual ~DwXmlWriter();
    /// begin writing the stream
    virtual bool open();
    /// end writing the stream
    virtual void close();

    /// begin a new node under the current node
    bool beginNode(const DwString& nodeName);
    /// end current node, set current node to parent
    void endNode();
    /// write content text
    void writeContent(const DwString& text);
    /// write a comment
    void writeComment(const DwString& comment);

    /// set string attribute on current node
    void setString(const DwString& name, const DwString& value);
    /// set bool attribute on current node
    void setBool(const DwString& name, bool value);
    /// set int attribute on current node
    void setInt(const DwString& name, int value);
    /// set float attribute on current node
    void setFloat(const DwString& name, float value);
    /// generic setter, 
    template<typename T> void set(const DwString& name, const T &value);

private:
    DwXmlWriterHelper *m_helper;
};// end of DwXmlWriter