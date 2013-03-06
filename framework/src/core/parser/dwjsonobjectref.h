#pragma once
#include "dwcore/dwstring.h"
#include "dwcore/dwbytearray.h"

enum DwJsonValueType
{
    DwJsonStringValue = 0,
    DwJsonNumberValue,
    DwJsonObjectValue,
    DwJsonArrayValue,
    DwJsonTrueValue,
    DwJsonFalseValue,
    DwJsonNULLValue
};

class DwJsonObject;
class DW_CORE_EXPORT DwJsonObjectRef
{
    friend class DwJsonDocument;
public:
    DwJsonObjectRef();
    ~DwJsonObjectRef();

    explicit DwJsonObjectRef(DwJsonObject* pObj);
    DwJsonObjectRef(DwJsonObjectRef& jObj);

    void *operator new(size_t s)
    {
        return dwMalloc(s);
    }
    void operator delete(void *p)
    {
        return dwFree(p);
    }

    const DwJsonObjectRef& operator = (const DwJsonObjectRef& jObj);

    bool isValid() const;
    DwJsonValueType type() const;

    DwString value() const;
    bool boolValue() const;
    DwJsonObjectRef parent() const;
    //DwJsonObjectRef object();

    void setValue(const DwString& value);
    void setBoolValue(bool b);
    //void setObject(const DwJsonObjectRef& obj);

    DwJsonObjectRef child(const DwString& name) const;
    DwJsonObjectRef child(int index) const;
    int childCount() const;

    bool deleteObject(bool noCheck = false);

    DwJsonObjectRef operator [] (const DwString& name) const;
    DwJsonObjectRef operator [] (int index) const;

    DwJsonObjectRef insert(const DwString& name);
    DwJsonObjectRef insert(int index);
private:
    DwJsonObjectRef createObject(DwJsonValueType eType);
    static DwJsonObjectRef createRoot(DwJsonValueType eType, DwJsonDocument* pDoc);
    DwJsonObjectRef createArrayObject(DwJsonValueType eType, DwJsonObjectRef& pre);
    //TODO:remove later
    DwByteArray toNarrowString( const wchar_t* pStr , int len, UINT CodePage) const;

    mutable DwJsonObject* m_pObj;
};
