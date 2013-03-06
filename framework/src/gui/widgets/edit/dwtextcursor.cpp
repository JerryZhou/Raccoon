#include "stable.h"
#include "dwtextcursor.h"
#include "dwtextdocument.h"

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwTextCursor, 'DTCR', DwRttiObject);

//------------------------------------------------------------------------------
/**
*/
DwTextCursor::DwTextCursor()
    : m_doc(NULL)
    , m_cur(0)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
void DwTextCursor::setDocument(DwTextDocument * doc)
{
    m_doc = doc;
    m_cur = 0;
}

//------------------------------------------------------------------------------
/**
*/
DwTextDocument *DwTextCursor::doc() const
{
    return m_doc;
}

//------------------------------------------------------------------------------
/**
*/
int DwTextCursor::cur() const
{
    return m_cur;
}

//------------------------------------------------------------------------------
/**
*/
int DwTextCursor::moveLeft(int num/* = 1 */)
{
    m_cur -= num;
    m_cur = dwClamp(m_cur, 0, m_doc->length());
    return m_cur;
}

//------------------------------------------------------------------------------
/**
*/
int DwTextCursor::moveRight(int num/* = 1 */)
{
    m_cur += num;
    m_cur = dwClamp(m_cur, 0, m_doc->length());
    return m_cur;
}

//------------------------------------------------------------------------------
/**
*/
int DwTextCursor::moveWorldLeft(int num/* = 1 */)
{
    for (int i=0; i < num; ++i)
    {
        int j = m_cur-1;
        // skip unknow things
        for (; j >= 0; --j)
        {
            if (j >= m_doc->length())
            {
                continue;
            }
            else
            {
                wchar_t c = m_doc->at(j).character();
                if (c == ' '|| c == '\t' || c == '\n')
                {
                    continue;
                }
                else
                {
                    break;
                }
            }
        }
        // look for the next world
        for (; j >= 0 && j < m_doc->length(); --j)
        {
            wchar_t c = m_doc->at(j).character();
            if (c == ' '|| c == '\t' || c == '\n')
            {
                break;
            }

        }
        // tick the cursor before the blank
        m_cur = dwClamp(j+1, 0, m_doc->length());
    }

    return m_cur;
}

//------------------------------------------------------------------------------
/**
*/
int DwTextCursor::moveWorldRight(int num/* = 1 */)
{
    for (int i=0; i < num; ++i)
    {
        int j = m_cur;
        // skip unknow things
        for (; j < m_doc->length(); ++j)
        {
            if (j < 0)
            {
                continue;
            }
            else
            {
                wchar_t c = m_doc->at(j).character();
                if (c == ' '|| c == '\t' || c == '\n')
                {
                    continue;;
                }
                else
                {
                    break;
                }
            }
        }
        // skip a world
        for (; j >= 0 && j < m_doc->length(); ++j)
        {
            wchar_t c = m_doc->at(j).character();
            if (c == ' '|| c == '\t' || c == '\n')
            {
                break;
            }

        }
        // skip unknow things util the next world begin
        for (; j < m_doc->length(); ++j)
        {
            if (j < 0)
            {
                continue;
            }
            else
            {
                wchar_t c = m_doc->at(j).character();
                if (c == ' '|| c == '\t' || c == '\n')
                {
                    continue;;
                }
                else
                {
                    break;
                }
            }
        }
        m_cur = dwClamp(j, 0, m_doc->length());
    }

    return m_cur;
}

//------------------------------------------------------------------------------
/**
*/
int DwTextCursor::moveUp(int num/* = 1 */)
{
    DW_UNUSED(num);
    int oldCur=m_cur;
    int newCur = moveLineBegin();
    int dis = oldCur - newCur;

    m_cur -= 2;
    moveLineBegin();
    moveRight(dis);

    return m_cur;
}

//------------------------------------------------------------------------------
/**
*/
int DwTextCursor::moveDown(int num/* = 1 */)
{
    DW_UNUSED(num);
    int oldCur = m_cur;
    int newCur = moveLineEnd();
    int dis = newCur - oldCur;

    m_cur += 2;
    moveLineEnd();
    moveLeft(dis);

    return m_cur;
}

//------------------------------------------------------------------------------
/**
*/
int DwTextCursor::moveLineEnd()
{
    int i=m_cur;
    for (; i >= 0 && i < m_doc->length(); ++i)
    {
        if (m_doc->at(i).character() == '\n')
        {
            break;
        }
    }

    m_cur = dwClamp(i, 0, m_doc->length());
    return m_cur;
}

//------------------------------------------------------------------------------
/**
*/
int DwTextCursor::moveLineBegin()
{
    int i = m_cur-1;
    for (; i >= 0 && i < m_doc->length(); --i)
    {
        if (m_doc->at(i).character() == '\n')
        {
            break;
        }
    }

    m_cur = dwClamp(i+1, 0, m_doc->length());
    return m_cur;
}

//------------------------------------------------------------------------------
/**
*/
int DwTextCursor::moveEnd()
{
    m_cur = m_doc->length();
    return m_cur;
}

//------------------------------------------------------------------------------
/**
*/
int DwTextCursor::moveBegin()
{
    m_cur = 0;
    return m_cur;
}

//------------------------------------------------------------------------------
/**
*/
int DwTextCursor::moveTo(int idx)
{
    m_cur = idx;
    m_cur = dwClamp(m_cur, 0, m_doc->length());
    return m_cur;
}