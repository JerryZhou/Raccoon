#include "stable.h"
#include "dwdocumentvalidater.h"
#include "dwtextdocument.h"
#include "dwrichchar.h"

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwDocumentValidater, 'DVER', DwRttiObject);

//------------------------------------------------------------------------------
/**
*/
DwDocumentValidater::DwDocumentValidater(DwTextDocument *doc/* = NULL*/)
    : m_doc(doc)
    , m_open(false)
    , m_validaterFlags(0)
    , m_validateTime(ValidaterBefore)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwDocumentValidater::~DwDocumentValidater()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwDocumentValidater::open()
{
    m_open = true;
}

//------------------------------------------------------------------------------
/**
*/
void DwDocumentValidater::close()
{
    m_open = false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwDocumentValidater::isOpen() const
{
    return m_open;
}

//------------------------------------------------------------------------------
/**
*/
bool DwDocumentValidater::isValidater(ValidaterFlag f) const
{
    return !!(m_validaterFlags & f);
}

//------------------------------------------------------------------------------
/**
*/
void DwDocumentValidater::setValidater(ValidaterFlag f, bool b/* = true*/)
{
    if (b)
    {
        m_validaterFlags |= f;
    }
    else
    {
        m_validaterFlags &= (~f);
    }
}

//------------------------------------------------------------------------------
/**
*/
bool DwDocumentValidater::isValidaterWhen(ValidaterTime f) const
{
    return !!(m_validateTime & f);
}

//------------------------------------------------------------------------------
/**
*/
void DwDocumentValidater::setValidaterWhen(ValidaterTime f, bool b/* = true*/)
{
    if (b)
    {
        m_validateTime |= f;
    }
    else
    {
        m_validateTime &= (~f);
    }
}

//------------------------------------------------------------------------------
/**
*/
bool DwDocumentValidater::validateDoc()
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwDocumentValidater::onAdd(DwRichChar *, int /* = -1*/)
{
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwDocumentValidater::onDel(DwRichChar *, int /* = -1*/)
{
    return true;
}