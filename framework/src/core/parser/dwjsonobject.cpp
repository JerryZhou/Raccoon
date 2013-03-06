#pragma once
#include "dwjsonobject.h"

DwJsonObject::DwJsonObject()
    : m_jsonObj(NULL)
    , m_jsonDoc(NULL)
{

}

DwJsonObject::DwJsonObject( json_t* p, DwJsonDocument* pDoc )
    : m_jsonObj(p)
    , m_jsonDoc(pDoc)
{

}

DwJsonObject::~DwJsonObject()
{
    //if (m_jsonObj)
    //{
    //    json_free_value(&m_jsonObj);
    //    m_jsonObj = NULL;
    //}
    m_jsonObj = NULL;
    m_jsonDoc = NULL;
}

void DwJsonObject::destroyJsonObj()
{
    if (m_jsonObj)
    {
        json_free_value(&m_jsonObj);
        m_jsonObj = NULL;
        m_jsonDoc = NULL;
    }
}