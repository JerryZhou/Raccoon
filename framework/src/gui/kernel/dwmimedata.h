#pragma once

#include "dwcore/dwvariant.h"
#include "dwcore/dwstring.h"
#include "dwcore/dwvector.h"
#include "dwgui/dwrttiobject.h"
#include "dwgui/dwrttiobjectptr.h"
#include "dwgui/dwmediatype.h"
#include "dwgui/dwurl.h"
#include "dwgraphics/dwimage.h"

// Multipurpose Internet Mail Extensions  http://en.wikipedia.org/wiki/MIME
//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwMimeData : public DwRttiObject
{
    DW_DECLARE_CLASS(DwMimeData);

public:
    typedef DwVector<DwString> MimeTexts;
    typedef DwVector<DwUrl> MimeUrls;
    typedef DwVector<DwRttiObjectPtr<DwRttiObject> > MimeObjects;
    typedef DwVector<DwImage> MimeImages;

public:
    DwMimeData();
    virtual ~DwMimeData();

    bool isValid() const;
    bool hasFormat(const DwString &mainType) const;
    bool hasFormat(const DwString &mainType, const DwString &subType) const;
    const DwMediaType& format() const;
    void setFormat(const DwString &mainType, const DwString &subType);
    void setFormat(const DwMediaType &);

    const MimeTexts *texts() const;
    void appendText(const DwString &text);

    const MimeUrls *urls() const;
    void appendUrl(const DwUrl &url);

    const MimeObjects *objects() const;
    void appendObject(DwRttiObject *obj);

    const MimeImages *images()const;
    void appendImage(const DwImage &image);

    void clear();

protected:
    DwMediaType m_format;

    MimeTexts m_texts;
    MimeUrls m_urls;
    MimeObjects m_objects;
    MimeImages m_images;
};// end of DwMimeData
DW_REGISTER_CLASS(DwMimeData);