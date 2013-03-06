#pragma once
#include "dwgui/dwrttiobject.h"

//------------------------------------------------------------------------------
class DwTextDocument;
class DwRichChar;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwDocumentValidater : public DwRttiObject
{
    DW_DECLARE_CLASS(DwDocumentValidater);
public:
    enum ValidaterFlag { ValidaterAdd = 0x01, ValidaterDel = 0x02 };
    enum ValidaterTime { ValidaterBefore = 0x01, ValidaterAfter = 0x02 };

    explicit DwDocumentValidater(DwTextDocument *doc = NULL);
    virtual ~DwDocumentValidater();

    void open();
    void close();
    bool isOpen() const;

    bool isValidater(ValidaterFlag) const;
    void setValidater(ValidaterFlag, bool b = true);

    bool isValidaterWhen(ValidaterTime) const;
    void setValidaterWhen(ValidaterTime, bool b = true);

    virtual bool validateDoc();
    virtual bool onAdd(DwRichChar *c, int pos = -1);
    virtual bool onDel(DwRichChar *c, int pos = -1);

protected:
    DwTextDocument *m_doc;
    int m_validaterFlags;
    int m_validateTime;
    bool m_open;
};// end of DwDocumentValidater
DW_REGISTER_CLASS(DwDocumentValidater);