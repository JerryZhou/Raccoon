#include "stable.h"

#include <dwcore/dwdebug.h>

#ifdef DW_SAFE_PARSER
#include <dwcore/dwset.h>
#endif

#include "dwjsondocument.h"
#include "dwjsonobjectref.h"
#include "dwjsonobject.h"
#include "dwcore/dwfile.h"
#include "dwcore/dwbytearray.h"

class DwJsonDocumentPrivate
{
public:
    DwJsonDocumentPrivate();
    ~DwJsonDocumentPrivate();
    DwJsonObjectRef root;

#ifdef DW_SAFE_PARSER
    bool m_isChanged;
    DwSet<json_t*> m_deleteObjs;
#endif
};

DwJsonDocumentPrivate::DwJsonDocumentPrivate()
#ifdef DW_SAFE_PARSER
    : m_isChanged(false)
#endif
{

}

DwJsonDocumentPrivate::~DwJsonDocumentPrivate()
{
    if (root.isValid())
    {
        root.deleteObject(true);
    }
}

#ifdef DW_SAFE_PARSER
void DwJsonDocument::appendToDelObjs( const DwJsonObjectRef& jr)
{
    DW_ASSERT(jr.isValid() && isValid());
    if (jr.isValid() && isValid())
    {
        d_ptr->m_isChanged = true;
        d_ptr->m_deleteObjs.insert(jr.m_pObj->m_jsonObj);
    }
}

bool DwJsonDocument::isDeleted(const DwJsonObjectRef& jr)
{
    if (!d_ptr->m_isChanged)
    {
        return false;
    }
    return d_ptr->m_deleteObjs.contains(jr.m_pObj->m_jsonObj);
}
#endif

DwJsonDocument::DwJsonDocument( DwObject *parent /*= NULL*/ )
    : DwObject(parent)
    , d_ptr(new DwJsonDocumentPrivate)
{

}

DwJsonDocument::~DwJsonDocument()
{
    DW_SAFE_DELETE(d_ptr);
}

bool DwJsonDocument::loadFromData( const DwString& data )
{
    DwByteArray cdata = d_ptr->root.toNarrowString(data.constString(), data.length(), CP_UTF8);

    return loadFromData(cdata.data());
}

bool DwJsonDocument::loadFromData( const char* data )
{
    if (!data)
    {
        dwDebug() << "DwJsonDocument::loadFromData, " << " load failed.";
        return false;
    }
    json_t* pj = NULL;
    if(json_parse_document(&pj, (char*)data) == JSON_OK)
    {
        d_ptr->root.m_pObj->m_jsonObj = pj;
        d_ptr->root.m_pObj->m_jsonDoc = this;
        return true;
    }

    return false;
}

bool DwJsonDocument::saveToStream( DwString& jstring )
{
    if (!isValid())
    {
        dwDebug() << "DwJsonDocument::saveToStream, " <<jstring<<" is not valid.";
        return false;
    }

    char* data = NULL;
    if (json_tree_to_string(d_ptr->root.m_pObj->m_jsonObj, &data) == JSON_OK)
    {
        jstring = DwString(data);
        ::free(data);
        return true;
    }

    return false;
}

bool DwJsonDocument::saveToFile( const DwString& fileName )
{
    if (!isValid())
    {
        dwDebug() << "DwJsonDocument::saveToFile, " <<fileName<<" is not valid.";
        return false;
    }
    if (fileName.isEmpty())
    {
        dwDebug() << "DwJsonDocument::saveToFile, "<<" file name is empty.";
        return false;
    }

    DwByteArray cdata = d_ptr->root.toNarrowString(fileName.constString(), fileName.length(), CP_UTF8);

    FILE *fp = fopen(cdata.data(), "w");
    if (!fp)
        return false;

    enum json_error error = json_stream_output(fp, d_ptr->root.m_pObj->m_jsonObj);
    fclose(fp);
    fp = NULL;

    return (JSON_OK == error) ? true : false;
}

DwJsonObjectRef DwJsonDocument::rootObject() const
{
    if (!isValid())
    {
        dwDebug() << "DwJsonDocument::rootObject, " << " is not valid.";
        return DwJsonObjectRef();
    }

    return d_ptr->root;
}

bool DwJsonDocument::loadFromFile( const DwString& fileName )
{
    if (!DwFile::exist(fileName))
    {
        dwDebug() << "DwJsonDocument::loadFromFile, " << fileName << " not found.";
        return false;
    }

    DwFile f(fileName);
    if (!f.open(DwFile::ReadOnly))
    {
        dwDebug() << "DwJsonDocument::loadFromFile, " << fileName << " read file failed.";
        return false;
    }

    DwByteArray data = f.readAll();
    bool ret = loadFromData(data.data());
    f.close();

    return ret;
}

bool DwJsonDocument::isValid() const
{
    return d_ptr && d_ptr->root.m_pObj->m_jsonObj;
}

DwJsonObjectRef DwJsonDocument::createRoot(DwJsonValueType eType)
{
    d_ptr->root = DwJsonObjectRef::createRoot(eType, this);
    return d_ptr->root;
}

