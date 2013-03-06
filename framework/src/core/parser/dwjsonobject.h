#pragma once
#include "3rdparty/json.h"
#include "dwcore/dwmalloc.h"

class DwJsonDocument ;
class DwJsonObject
{
    friend class DwJsonObjectRef;
    friend class DwJsonDocument;
private:
    DwJsonObject();
    DwJsonObject(json_t* p, DwJsonDocument* pDoc);
    ~DwJsonObject();

    void *operator new(size_t s)
    {
        return dwMalloc(s);
    }
    void operator delete(void *p)
    {
        return dwFree(p);
    }

    void destroyJsonObj();
    json_t *m_jsonObj;
    DwJsonDocument * m_jsonDoc;
};
