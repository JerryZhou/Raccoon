#pragma once
#include "dwcore/dwstring.h"
#include "dwgui/dwrttiobject.h"
#include "dwgui/dwtextcursor.h"
#include "dwgui/dwrichchar.h"
#include "dwgui/dwcharformat.h"
#include "dwgui/dwtextformat.h"

//------------------------------------------------------------------------------
class DwDocumentValidater;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwTextDocument : public DwRttiObject
{
    DW_DECLARE_CLASS(DwTextDocument);
public:
    DwTextDocument();
    virtual ~DwTextDocument();

    DwTextCursor& cursor() ;
    DwTextCursor& selectCursor();
    int length() const;
    DwRichChar at(int idx) const;

    void beginBatchOp();
    void endBatchOp();

    bool insert(const DwRichChar &c, int pos = -1);
    int  insert(const DwString &s, int pos = -1);
    bool remove(int idx);

    bool append(const DwRichChar &c);
    int  append(const DwString &s);
    void appendFormat(DwCharFormat *format);

    bool setCurrentFormat(int idx);
    bool setCurrentFormat(DwCharFormat *format);

    int charFormatNum() const;
    DwCharFormat* charFormatAt(int idx) const;
    int charFormatIndexOf(DwCharFormat *format) const;
    int textFormatNum() const;
    DwTextFormat* textFormatAt(int idx) const;

    DwString plainText() const;
    void clear() ;

    int validaterNum() const;
    DwDocumentValidater *validaterAt(int idx) const;
    bool appendValidater(DwDocumentValidater *v);
    bool removeValidater(int idx);
    void clearValidaters();

protected:
    int findCharFormat(int pos);
    void onAdd(const DwRichChar *c, int pos);
    void onDel(const DwRichChar *c, int pos);
    void validateTheCharForamtArray();

    DwVector<DwRichChar> m_chars;

    DwVector<int> m_charFormatIndexs;
    DwVector<int> m_charContentsFormatIndexs;
    DwVector<DwCharFormat*> m_charFormats;

    DwVector<int> m_textFormatIndexs;
    DwVector<int> m_textContentsFormatIndexs;
    DwVector<DwTextFormat*> m_textFormats;

    DwTextCursor m_inputCursor;
    DwTextCursor m_selectCursor;

    int m_maxNumOfChar;
    DwVector<DwDocumentValidater*> m_validaters;
    bool m_bachOp;
private:
    friend class DwTextLayoutEngine;
    friend class DwMultiTextLayoutEngine;
};// end of DwTextDocument
DW_REGISTER_CLASS(DwTextDocument);