#pragma once
#include "dwgui/dwrttiobject.h"

//------------------------------------------------------------------------------
class DwTextDocument;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwTextCursor : public DwRttiObject
{
    DW_DECLARE_CLASS(DwTextCursor);
public:
    DwTextCursor();

    void setDocument(DwTextDocument * );
    DwTextDocument *doc() const;

    int cur() const;

    // left
    int moveLeft(int num = 1);
    // right
    int moveRight(int num = 1);
    // ctrl left
    int moveWorldLeft(int num = 1);
    // ctrl right
    int moveWorldRight(int num = 1);
    // up
    int moveUp(int num = 1);
    // down
    int moveDown(int num = 1);
    // end
    int moveLineEnd();
    // home
    int moveLineBegin();
    // ctrl end
    int moveEnd();
    // ctrl home
    int moveBegin();

    // move to idx in document
    int moveTo(int idx);

protected:
    DwTextDocument *m_doc;
    int m_cur;
};// end of DwTextCursor
DW_REGISTER_CLASS(DwTextCursor);