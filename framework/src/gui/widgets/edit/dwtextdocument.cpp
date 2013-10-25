#include "stable.h"
#include "dwtextdocument.h"
#include "dwdocumentvalidater.h"

//------------------------------------------------------------------------------
DW_IMPLEMENT_CLASS(DwTextDocument, 'TDNT', DwRttiObject);

//------------------------------------------------------------------------------
/**
*/
DwTextDocument::DwTextDocument()
    : m_maxNumOfChar(65536)
    , m_bachOp(false)
{
    m_inputCursor.setDocument(this);
    m_selectCursor.setDocument(this);

    m_textFormatIndexs.append(0);
    m_textContentsFormatIndexs.append(0);
    m_textFormats.append(new DwTextFormat());

    m_charFormatIndexs.append(0);
    m_charContentsFormatIndexs.append(0);
    m_charFormats.append(new DwCharFormat());
}

//------------------------------------------------------------------------------
/**
*/
DwTextDocument::~DwTextDocument()
{
    for (int i=0; i<m_charFormats.size(); ++i)
    {
        delete m_charFormats.at(i);
    }

    for (int i=0; i<m_textFormats.size(); ++i)
    {
        delete m_textFormats.at(i);
    }

    for (int i=0; i<m_validaters.size(); ++i)
    {
        delete m_validaters.at(i);
    }
}

//------------------------------------------------------------------------------
/**
*/
DwTextCursor& DwTextDocument::cursor()
{
    return m_inputCursor;
}

//------------------------------------------------------------------------------
/**
*/
DwTextCursor& DwTextDocument::selectCursor()
{
    return m_selectCursor;
}

//------------------------------------------------------------------------------
/**
*/
int DwTextDocument::length() const
{
    return m_chars.size();
}

//------------------------------------------------------------------------------
/**
*/
DwRichChar DwTextDocument::at(int idx) const
{
    DW_ASSERT(idx >= 0 && idx < m_chars.size());
    return m_chars.at(idx);
}

//------------------------------------------------------------------------------
/**
*/
void DwTextDocument::beginBatchOp()
{
    m_bachOp = true;
}


//------------------------------------------------------------------------------
/**
*/
void DwTextDocument::endBatchOp()
{
    m_bachOp = false;
    for (int i=0; i<m_validaters.size(); ++i)
    {
        DwDocumentValidater *v = m_validaters.at(i);
        if (v->isOpen())
        {
            v->validateDoc();
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwTextDocument::appendFormat(DwCharFormat *format)
{
    m_charFormats.append(format);
}

//------------------------------------------------------------------------------
/**
*/
bool DwTextDocument::replaceFormat(int idx, DwCharFormat *format)
{
    if(idx < m_charFormats.size())
    {
	DwCharFormat* oldFormat = m_charFormats.at(idx);
	DW_SAFE_DELETE(oldFormat);
	m_charFormats[idx] = format;

	return true;
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
int DwTextDocument::charFormatNum() const
{
    return m_charFormats.size();
}

//------------------------------------------------------------------------------
/**
*/
DwCharFormat* DwTextDocument::charFormatAt(int idx) const
{
    return m_charFormats.at(idx);
}

//------------------------------------------------------------------------------
/**
*/
int DwTextDocument::charFormatIndexOf(DwCharFormat *format) const
{
    for (int i=0; i<m_charFormats.size(); ++i)
    {
        if (m_charFormats.at(i) == format)
        {
            return i;
        }
    }
    return -1;
}

//------------------------------------------------------------------------------
/**
*/
int DwTextDocument::textFormatNum() const
{
    return m_textFormats.size();
}

//------------------------------------------------------------------------------
/**
*/
DwTextFormat* DwTextDocument::textFormatAt(int idx) const
{
    return m_textFormats.at(idx);
}

//------------------------------------------------------------------------------
/**
*/
DwString DwTextDocument::plainText() const
{
    DwString s;
    for (int i=0; i<m_chars.size(); ++i)
    {
        s.append(wchar_t(m_chars.at(i).character()));
    }
    return s;
}

//------------------------------------------------------------------------------
/**
*/
void DwTextDocument::clear()
{
    m_chars.clear();
    m_inputCursor.moveEnd();
    m_selectCursor.moveEnd();
	for(DwVector<DwDocumentValidater*>::iterator i = m_validaters.begin();i != m_validaters.end();++i)
	{
		if(*i)
		{
			(*i)->refresh();
		}
	}

	for(DwVector<DwCharFormat*>::iterator i = m_charFormats.begin();i != m_charFormats.end();++i)
	{
		if(*i)
		{
			(*i)->refresh();
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
int DwTextDocument::validaterNum() const
{
    return m_validaters.size();
}

//------------------------------------------------------------------------------
/**
*/
DwDocumentValidater *DwTextDocument::validaterAt(int idx) const
{
    return m_validaters.at(idx);
}

//------------------------------------------------------------------------------
/**
*/
bool DwTextDocument::appendValidater(DwDocumentValidater *v)
{
    DW_ASSERT(v);
    m_validaters.append(v);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwTextDocument::removeValidater(int idx)
{
    DW_ASSERT(idx >= 0 && idx < m_validaters.size());
    m_validaters.remove(idx);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void DwTextDocument::clearValidaters()
{
    for (int i=m_chars.size(); i>0; --i)
    {
        remove(i-1);
    }
}

//------------------------------------------------------------------------------
/**
*/
int DwTextDocument::findCharFormat(int pos)
{
    int i=0;
    for (; i < m_charFormatIndexs.size(); ++i)
    {
        if (pos < m_charFormatIndexs.at(i))
        {
            return i - 1;
        }
    }
    return i - 1;
}

//------------------------------------------------------------------------------
/**
*/
void DwTextDocument::onAdd(const DwRichChar *c, int pos)
{
    DW_UNUSED(c);
    int i=0;
    bool justDoIt = false;
    for (; i < m_charFormatIndexs.size(); ++i)
    {
        if (pos == m_charFormatIndexs.at(i))
        {
            justDoIt = true;
            continue;
        }
        // move after
        if (justDoIt || pos < m_charFormatIndexs.at(i))
        {
            m_charFormatIndexs[i] = m_charFormatIndexs.at(i) + 1;
        }
    }

    int formatIndex = findCharFormat(pos);
    m_charFormats.at(formatIndex)->onAdd(c);
}

//------------------------------------------------------------------------------
/**
*/
void DwTextDocument::onDel(const DwRichChar *c, int pos)
{
    DW_UNUSED(c);
    int formatIndex = findCharFormat(pos);
    m_charFormats.at(formatIndex)->onDel(c);

    int i=0;
    bool justDoIt = false;
    for (; i < m_charFormatIndexs.size(); ++i)
    {
        if (pos == m_charFormatIndexs.at(i))
        {
            justDoIt = true;
            continue;
        }
        // move before
        if (justDoIt || pos < m_charFormatIndexs.at(i))
        {
            m_charFormatIndexs[i] = m_charFormatIndexs.at(i) - 1;
        }
    }

    validateTheCharForamtArray();
}

//------------------------------------------------------------------------------
/**
*/
void DwTextDocument::validateTheCharForamtArray()
{
    int i = m_charFormatIndexs.size()-1;
    for (; i>0; --i)
    {
        int n = m_charFormatIndexs.at(i);
        int m = m_charFormatIndexs.at(i-1);
        int l = m_chars.size();
        if (n == m || n >= l || n < 0)
        {
            m_charFormatIndexs.remove(i);
            m_charContentsFormatIndexs.remove(i);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
bool DwTextDocument::insert(const DwRichChar &c, int pos/* = -1*/)
{
    // inner quick validater
    if (m_chars.size() > m_maxNumOfChar && m_maxNumOfChar > 0)
    {
        return false;
    }

    if (pos == -1)
    {
        pos = m_chars.size();
    }

    // common validater
    if (!m_bachOp)
    {
        for (int i=0; i<m_validaters.size(); ++i)
        {
            DwDocumentValidater *v = m_validaters.at(i);
            if (v->isOpen() &&
                    v->isValidater(DwDocumentValidater::ValidaterAdd) &&
                    v->isValidaterWhen(DwDocumentValidater::ValidaterBefore))
            {
                bool b = v->onAdd(const_cast<DwRichChar*>(&c), pos);
                if (!b)
                {
                    return false;
                }
            }
        }
    }

    m_chars.insert(pos, c);
    onAdd(&c, pos);

    // common validater
    if (!m_bachOp)
    {
        for (int i=0; i<m_validaters.size(); ++i)
        {
            DwDocumentValidater *v = m_validaters.at(i);
            if (v->isOpen() &&
                    v->isValidater(DwDocumentValidater::ValidaterAdd) &&
                    v->isValidaterWhen(DwDocumentValidater::ValidaterAfter))
            {
                bool b = v->onAdd(const_cast<DwRichChar*>(&c), pos);
                if (!b)
                {
                    return false;
                }
            }
        }
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
int DwTextDocument::insert(const DwString &s, int pos /*= -1*/)
{
    if (pos == -1)
    {
        return append(s);
    }
    else
    {
        int num = 0;
        for (int i=0; i<s.length(); ++i)
        {
            bool isOk = insert(DwRichChar(s.at(i)), pos);
            if (isOk)
            {
                ++num;
                pos += 1;
            }
        }
        return num;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool DwTextDocument::remove(int idx)
{
    DW_ASSERT(idx >= 0 && idx < m_chars.size());
    // common validater
    DwRichChar c = m_chars.at(idx);
    if (!m_bachOp)
    {
        for (int i=0; i<m_validaters.size(); ++i)
        {
            DwDocumentValidater *v = m_validaters.at(i);
            if (v->isOpen() &&
                    v->isValidater(DwDocumentValidater::ValidaterDel) &&
                    v->isValidaterWhen(DwDocumentValidater::ValidaterBefore))
            {
                bool b = v->onDel(&c, idx);
                if (!b)
                {
                    return false;
                }
            }
        }
    }

    m_chars.remove(idx);
    onDel(&c, idx);

    // common validater
    if (!m_bachOp)
    {
        for (int i=0; i<m_validaters.size(); ++i)
        {
            DwDocumentValidater *v = m_validaters.at(i);
            if (v->isOpen() &&
                    v->isValidater(DwDocumentValidater::ValidaterDel) &&
                    v->isValidaterWhen(DwDocumentValidater::ValidaterAfter))
            {
                bool b = v->onDel(const_cast<DwRichChar*>(&c), idx);
                if (!b)
                {
                    return false;
                }
            }
        }
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool DwTextDocument::append(const DwRichChar &c)
{
    return insert(c, m_chars.size());
}

//------------------------------------------------------------------------------
/**
*/
int DwTextDocument::append(const DwString &s)
{
    int num = 0;
    bool isOk = false;
    for (int i=0; i<s.length(); ++i)
    {
        isOk = append(DwRichChar(s.at(i)));
        if (isOk)
        {
            ++num;
        }
    }
    return num;
}

//------------------------------------------------------------------------------
/**
*/
bool DwTextDocument::setCurrentFormat(int idx)
{
    int cur = m_inputCursor.cur();
    if (idx >=0 && idx < m_charFormats.size())
    {
        int pos = findCharFormat(cur);
        DW_ASSERT(pos >= 0);
        m_charFormatIndexs.insert(pos+1, cur);
        m_charContentsFormatIndexs.insert(pos+1, idx);
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwTextDocument::setCurrentFormat(DwCharFormat *format)
{
    int idx = charFormatIndexOf(format);
    return setCurrentFormat(idx);
}
