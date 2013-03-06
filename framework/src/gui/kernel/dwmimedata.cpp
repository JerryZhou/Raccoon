#include "stable.h"
#include "dwmimedata.h"

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwMimeData, 'MIME', DwRttiObject);

//------------------------------------------------------------------------------
/**
*/
DwMimeData::DwMimeData()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwMimeData::~DwMimeData()
{
    clear();
}

//------------------------------------------------------------------------------
/**
*/
bool DwMimeData::isValid() const
{
    return m_format.isValid();
}

//------------------------------------------------------------------------------
/**
*/
bool DwMimeData::hasFormat(const DwString &mainType) const
{
    return m_format.type() == mainType;
}

//------------------------------------------------------------------------------
/**
*/
bool DwMimeData::hasFormat(const DwString &mainType, const DwString &subType) const
{
    return m_format.type() == mainType && m_format.subType() == subType;
}

//------------------------------------------------------------------------------
/**
*/
const DwMediaType& DwMimeData::format() const
{
    return m_format;
}

//------------------------------------------------------------------------------
/**
*/
void DwMimeData::setFormat(const DwString &mainType, const DwString &subType)
{
    setFormat(DwMediaType(mainType, subType));
}

//------------------------------------------------------------------------------
/**
*/
void DwMimeData::setFormat(const DwMediaType &f)
{
    m_format = f;
}

//------------------------------------------------------------------------------
/**
*/
const DwMimeData::MimeTexts *DwMimeData::texts() const
{
    return &m_texts;
}

//------------------------------------------------------------------------------
/**
*/
void DwMimeData::appendText(const DwString &text)
{
    m_texts.append(text);
}

//------------------------------------------------------------------------------
/**
*/
const DwMimeData::MimeUrls *DwMimeData::urls() const
{
    return &m_urls;
}

//------------------------------------------------------------------------------
/**
*/
void DwMimeData::appendUrl(const DwUrl &url)
{
    m_urls.append(url);
}

//------------------------------------------------------------------------------
/**
*/
const DwMimeData::MimeObjects *DwMimeData::objects() const
{
    return &m_objects;
}

//------------------------------------------------------------------------------
/**
*/
void DwMimeData::appendObject(DwRttiObject *obj)
{
    DW_ASSERT(obj);
    m_objects.append(obj);
}

//------------------------------------------------------------------------------
/**
*/
const DwMimeData::MimeImages *DwMimeData::images()const
{
    return &m_images;
}

//------------------------------------------------------------------------------
/**
*/
void DwMimeData::appendImage(const DwImage &image)
{
    return m_images.append(image);
}

//------------------------------------------------------------------------------
/**
*/
void DwMimeData::clear()
{
    m_format.clear();
    m_texts.clear();
    m_urls.clear();
    m_objects.clear();
    m_images.clear();
}