#include "stable.h"
#include <dwcore/dwdebug.h>
#include "dwjsonobjectref.h"
#include "dwcore/dwarraylist.h"
#include "dwjsondocument.h"
#include "dwjsonobject.h"

DwByteArray DwJsonObjectRef::toNarrowString( const wchar_t* pStr , int len, UINT CodePage) const
{
    if(pStr == NULL)
    {
        dwDebug() << "DwJsonObjectRef::toNarrowString, " << " pStr is null.";
        return DwByteArray();
    }

    if (len < 0 && len != -1)
    {
        dwDebug() << "DwJsonObjectRef::toNarrowString, " << " pStr length " <<len;
        return DwByteArray();
    }

    // figure out how many narrow characters we are going to get
    int nChars = WideCharToMultiByte( CodePage , 0 ,
                                      pStr , len , NULL , 0 , NULL , NULL ) ;
    if ( len == -1 )
        -- nChars ;
    if ( nChars == 0 )
        return DwByteArray() ;

    // convert the wide string to a narrow string
    // nb: slightly naughty to write directly into the string like this
    char* cVal = (char*)dwMalloc(nChars + 1);
    memset(cVal, 0, nChars + 1);
    WideCharToMultiByte( CodePage , 0 , pStr , len ,  cVal , nChars , NULL , NULL ) ;

    DwByteArray ret(cVal);
    dwFree(cVal);

    return ret;
}

DwJsonObjectRef::DwJsonObjectRef()
    : m_pObj(new DwJsonObject())
{

}

DwJsonObjectRef::DwJsonObjectRef( DwJsonObject* pObj )
{
    DW_ASSERT(pObj);
    m_pObj = new DwJsonObject;
    m_pObj->m_jsonObj = pObj->m_jsonObj;
    m_pObj->m_jsonDoc = pObj->m_jsonDoc;
}

DwJsonObjectRef::DwJsonObjectRef( DwJsonObjectRef& jObj )
{
    m_pObj = new DwJsonObject;
    m_pObj->m_jsonObj = jObj.m_pObj->m_jsonObj;
    m_pObj->m_jsonDoc = jObj.m_pObj->m_jsonDoc;
}

DwJsonObjectRef::~DwJsonObjectRef()
{
    DW_SAFE_DELETE(m_pObj);
}

const DwJsonObjectRef& DwJsonObjectRef::operator = ( const DwJsonObjectRef& jObj )
{
    if (!jObj.isValid())
    {
        dwDebug() << "DwJsonObjectRef::operator =, " <<" is not valid ";
    }
    m_pObj->m_jsonObj = jObj.m_pObj->m_jsonObj;
    m_pObj->m_jsonDoc = jObj.m_pObj->m_jsonDoc;

    return *this;
}

bool DwJsonObjectRef::isValid() const
{
#ifdef DW_SAFE_PARSER
    if (m_pObj && m_pObj->m_jsonObj && m_pObj->m_jsonDoc)
    {
        if (!m_pObj->m_jsonDoc->isDeleted(*this))
        {
            return true;
        }

        m_pObj->m_jsonDoc = NULL;
        m_pObj->m_jsonObj = NULL;
        m_pObj = NULL;
    }
    return false;
#else
    return m_pObj && m_pObj->m_jsonObj;
#endif
}

DwJsonValueType DwJsonObjectRef::type() const
{
    if (!isValid())
    {
        dwDebug() << "DwJsonObjectRef::type, " << " is not valid ";
        return DwJsonNULLValue;
    }
    return static_cast<DwJsonValueType>(m_pObj->m_jsonObj->type);
}

DwString DwJsonObjectRef::value() const
{
    if (!isValid())
    {
        dwDebug() << "DwJsonObjectRef::value, " << " is not valid ";
        return DwString();
    }

    if (m_pObj->m_jsonObj->text)
    {
        return m_pObj->m_jsonObj->text;
    }
    else
    {
        dwDebug() << "DwJsonObjectRef::value, " << " value is not valid ";
        return DwString();
    }
}

bool DwJsonObjectRef::boolValue() const
{
    if (!isValid())
    {
        dwDebug() << "DwJsonObjectRef::boolValue, " << " is not valid ";
        return false;
    }

    if (m_pObj->m_jsonObj->type == JSON_TRUE)
    {
        return true;
    }
    else if (m_pObj->m_jsonObj->type == JSON_FALSE)
    {
        return false;
    }
    else
    {
        dwDebug() << "DwJsonObjectRef::boolValue, " << " type is wrong ";
        return false;
    }
}


DwJsonObjectRef DwJsonObjectRef::parent() const
{
    if (isValid() && m_pObj->m_jsonObj->parent)
    {
        DwJsonObject obj(m_pObj->m_jsonObj->parent, m_pObj->m_jsonDoc);
        return DwJsonObjectRef(&obj);
    }
    else
    {
        dwDebug() << "DwJsonObjectRef::parent, " << " no parent ";
        return DwJsonObjectRef();
    }
}

DwJsonObjectRef DwJsonObjectRef::child(const DwString& name) const
{
    if (!isValid())
    {
        dwDebug() << "DwJsonObjectRef::child, " <<name<< " is not valid ";
        return DwJsonObjectRef();
    }

    if (m_pObj->m_jsonObj->type != JSON_OBJECT)
    {
        dwDebug() << "DwJsonObjectRef::child, " <<name<< " is not JSON_OBJECT ";
        return DwJsonObjectRef();
    }

    json_t *child = m_pObj->m_jsonObj->child;
    while(child)
    {
        if(child->text && name == child->text)
        {
            DwJsonObject obj(child->child, m_pObj->m_jsonDoc);
            return DwJsonObjectRef(&obj);
        }
        child = child->next;
    }
    return DwJsonObjectRef();
}

DwJsonObjectRef DwJsonObjectRef::child( int index ) const
{
    if (!isValid())
    {
        dwDebug() << "DwJsonObjectRef::child, " <<index<< " is not valid ";
        return DwJsonObjectRef();
    }
    //if this json_object is new,set to json_array
    if (m_pObj->m_jsonObj->type == JSON_OBJECT && !m_pObj->m_jsonObj->child)
    {
        m_pObj->m_jsonObj->type = JSON_ARRAY;
    }

    if (m_pObj->m_jsonObj->type != JSON_ARRAY)
    {
        dwDebug() << "DwJsonObjectRef::child, " <<index<< " is not JSON_ARRAY ";
        return DwJsonObjectRef();
    }

    json_t* cursor = dw_json_tree_find_array_helper(m_pObj->m_jsonObj, index);
    if (!cursor)
    {
        return DwJsonObjectRef();
    }
    DwJsonObject obj(cursor, m_pObj->m_jsonDoc);
    return DwJsonObjectRef(&obj);
}

int DwJsonObjectRef::childCount() const
{
    if (!isValid())
    {
        dwDebug() << "DwJsonObjectRef::childCount, " << " is not valid ";
        return 0;
    }

    if (m_pObj->m_jsonObj->type != JSON_OBJECT && m_pObj->m_jsonObj->type != JSON_ARRAY)
    {
        dwDebug() << "DwJsonObjectRef::childCount, " << " is not JSON_OBJECT or JSON_ARRAY ";
        return 0;
    }

    int count = 0;
    json_t *child = m_pObj->m_jsonObj->child;
    while(child)
    {
        count++;
        child = child->next;
    }
    return count;
}

bool DwJsonObjectRef::deleteObject(bool noCheck)
{
    if (!isValid())
    {
        dwDebug() << "DwJsonObjectRef::deleteObject, " << " is not valid ";
        return false;
    }

#ifdef DW_SAFE_PARSER
    if (!noCheck)
    {
        DwArrayList<json_t*> children;
        children.append(m_pObj->m_jsonObj);
        while (!children.isEmpty())
        {
            json_t *cursor = children.front();
            children.pop_front();
            DwJsonObject obj(cursor, m_pObj->m_jsonDoc);
            DwJsonObjectRef jr(&obj);
            m_pObj->m_jsonDoc->appendToDelObjs(jr);

            json_t *child = cursor->child;
            while (child)
            {
                children.append(child);
                child = child->next;
            }
        }
    }
#endif

    m_pObj->destroyJsonObj();
    return true;
}

void DwJsonObjectRef::setValue( const DwString& value )
{
    if (!isValid())
    {
        dwDebug() << "DwJsonObjectRef::setValue, " <<value<< " is not valid ";
        return;
    }

    DwByteArray cVal = toNarrowString(value.constString(), value.length(), CP_UTF8);
    int length = strlen (cVal.data()) + 1;
    if (m_pObj->m_jsonObj->text)
    {
        //DOTO:dwFree
        free(m_pObj->m_jsonObj->text);
    }
    if (m_pObj->m_jsonObj->type == JSON_OBJECT)
    {
        m_pObj->m_jsonObj->type = JSON_STRING;
    }
    m_pObj->m_jsonObj->text = (char*)malloc (length * sizeof (char));
    strncpy (m_pObj->m_jsonObj->text, cVal.data(), length);
}

void DwJsonObjectRef::setBoolValue( bool b )
{
    if (!isValid())
    {
        dwDebug() << "DwJsonObjectRef::setBoolValue, " <<b<< " is not valid ";
        return;
    }

    if (m_pObj->m_jsonObj->child)
    {
        dwDebug() << "DwJsonObjectRef::setBoolValue, " <<b<< " is error,this jsonobject has child ";
        return;
    }

    json_value_type v = b ? JSON_TRUE : JSON_FALSE;
    m_pObj->m_jsonObj->type = v;
}

DwJsonObjectRef DwJsonObjectRef::createObject( DwJsonValueType eType)
{
    if (!isValid())
    {
        dwDebug() << "DwJsonObjectRef::createObject, " <<eType<< " is not valid ";
        return DwJsonObjectRef();
    }

    json_t* newVal = json_new_value(static_cast<json_value_type>(eType));

    if (JSON_OK == json_insert_child(m_pObj->m_jsonObj, newVal))
    {
        DwJsonObject obj(newVal, m_pObj->m_jsonDoc);
        return DwJsonObjectRef(&obj);
    }

    return DwJsonObjectRef();
}

DwJsonObjectRef DwJsonObjectRef::operator[]( const DwString& name ) const
{
    if (!isValid())
    {
        dwDebug() << "DwJsonObjectRef::operator[], " <<name<< " is not valid ";
        return DwJsonObjectRef();
    }

    return child(name);
}

DwJsonObjectRef DwJsonObjectRef::operator[]( int index ) const
{
    if (!isValid())
    {
        dwDebug() << "DwJsonObjectRef::operator[], " <<index<< " is not valid ";
        return DwJsonObjectRef();
    }

    return child(index);
}

DwJsonObjectRef DwJsonObjectRef::createRoot(DwJsonValueType eType, DwJsonDocument* pDoc)
{
    if (eType != DwJsonArrayValue && eType != DwJsonObjectValue)
    {
        dwDebug() << "DwJsonObjectRef::createRoot, " << eType <<" this type can not be root ";
        return DwJsonObjectRef();
    }

    json_t* newVal = json_new_value(static_cast<json_value_type>(eType));
    DwJsonObject obj(newVal, pDoc);
    return DwJsonObjectRef(&obj);
}

DwJsonObjectRef DwJsonObjectRef::insert( const DwString& name )
{
    if (!isValid() || m_pObj->m_jsonObj->type != JSON_OBJECT)
    {
        dwDebug() << "DwJsonObjectRef::insert, " << name <<" is not valid or type is not JSON_OBJECT ";
        return DwJsonObjectRef();
    }

    DwJsonObjectRef val = child(name);
    if (val.isValid())
    {
        dwDebug() << "DwJsonObjectRef::insert, " << name <<" already has ";
        return val;
    }

    DwJsonObjectRef key = createObject(DwJsonStringValue);
    key.setValue(name);
    return key.createObject(DwJsonObjectValue);
}

DwJsonObjectRef DwJsonObjectRef::insert( int index )
{
    if (!isValid())
    {
        dwDebug() << "DwJsonObjectRef::insert, " <<index<<" is not valid ";
        return DwJsonObjectRef();
    }
    //if this json_object is new,set to json_array
    if (m_pObj->m_jsonObj->type == JSON_OBJECT && !m_pObj->m_jsonObj->child)
    {
        m_pObj->m_jsonObj->type = JSON_ARRAY;
    }

    DwJsonObjectRef val = child(index);
    return createArrayObject(DwJsonObjectValue, val);
}

DwJsonObjectRef DwJsonObjectRef::createArrayObject( DwJsonValueType eType, DwJsonObjectRef& next )
{
    json_t* newVal = json_new_value(static_cast<json_value_type>(eType));

    if (JSON_OK == json_array_insert_child(m_pObj->m_jsonObj, newVal, next.m_pObj->m_jsonObj))
    {
        DwJsonObject obj(newVal, m_pObj->m_jsonDoc);
        return DwJsonObjectRef(&obj);
    }

    return DwJsonObjectRef();
}


