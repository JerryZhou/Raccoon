#pragma once

#include "dwapp/dwstreamreader.h"

//------------------------------------------------------------------------------
class DwXmlReaderHelper;

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwXmlReader : public DwStreamReader
{
    DW_DECLARE_CLASS(DwXmlReader);
public:
    /// constructor
    DwXmlReader();
    /// destructor
    virtual ~DwXmlReader();

    /// begin reading from the stream
    virtual bool open();
    /// end reading from the stream
    virtual void close();

    /// return true if node exists 
    bool hasNode(const DwString& path) const;
    /// get short name of current node
    DwString GetCurrentNodeName() const;
    /// get path to current node
    DwString GetCurrentNodePath() const;
    /// returns the line number of the current node
    int GetCurrentNodeLineNumber() const;
    
    /// set current node as path
    void SetToNode(const DwString& path);
    /// set current node to first child node, return false if no child exists
    bool SetToFirstChild(const DwString& name = "");
    /// set current node to next sibling node, return false if no more sibling exists
    bool SetToNextChild(const DwString& name = "");
    /// set current node to parent, return false if no parent exists
    bool SetToParent();

    /// return true if matching attribute exists on current node
    bool HasAttr(const char* attr) const;
    /// return names of all attrs on current node
    DwVector<DwString> GetAttrs() const;

    /// return true if current node has embedded content
    bool HasContent() const;
    /// return embedded content of current node
    DwString GetContent() const;

    /// get string attribute value from current node
    DwString GetString(const char* attr) const;
    /// get bool attribute value from current node
    bool GetBool(const char* attr) const;
    /// get int attribute value from current node
    int GetInt(const char* attr) const;
    /// get float attribute value from current node
    float GetFloat(const char* attr) const;
    /// generic getter for extension types
    template<typename T> T Get(const char* attr) const;

    /// get optional string attribute value from current node
    DwString GetOptString(const char* attr, const DwString& defaultValue) const;
    /// get optional bool attribute value from current node
    bool GetOptBool(const char* attr, bool defaultValue) const;
    /// get optional int attribute value from current node
    int GetOptInt(const char* attr, int defaultValue) const;
    /// get optional float attribute value from current node
    float GetOptFloat(const char* attr, float defaultValue) const;
    
private:
    DwXmlReaderHelper *m_helper;
};// end of DwXmlReader
