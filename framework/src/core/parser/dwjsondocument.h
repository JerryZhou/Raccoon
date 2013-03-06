#pragma once
#include "dwcore/dwobject.h"
#include "dwjsonobjectref.h"

class DwJsonDocumentPrivate;
class DW_CORE_EXPORT DwJsonDocument : public DwObject
{
    DW_DISABLE_COPY(DwJsonDocument);
#ifdef DW_SAFE_PARSER
    friend class DwJsonObjectRef;
#endif
public:
    DwJsonDocument(DwObject *parent = NULL);
    ~DwJsonDocument();

public:
    bool loadFromFile(const DwString& fileName);
    bool loadFromData(const DwString& data);
    bool loadFromData(const char* data);

    bool saveToStream(DwString& jstring);
    bool saveToFile(const DwString& fileName);

    bool isValid() const;
    DwJsonObjectRef rootObject() const;
    DwJsonObjectRef createRoot(DwJsonValueType eType);

private:
    DwJsonDocumentPrivate* d_ptr;
#ifdef DW_SAFE_PARSER
    void appendToDelObjs(const DwJsonObjectRef& jr);
    bool isDeleted(const DwJsonObjectRef& jr);
#endif
};