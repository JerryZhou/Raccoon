
#include "stable.h"
#include "dwstreamreader.h"

DW_IMPLEMENT_CLASS(DwStreamReader, 'STRR', DwRefCounted);

//------------------------------------------------------------------------------
/**
*/
DwStreamReader::DwStreamReader() :
    m_isOpen(false),
    m_streamWasOpen(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
DwStreamReader::~DwStreamReader()
{
    DW_ASSERT(!this->isOpen());
}

//------------------------------------------------------------------------------
/**
    Attaches the reader to a stream. This will imcrement the refcount
    of the stream.
*/
void DwStreamReader::setStream(const DwPtr<DwStream>& s)
{
    this->m_stream = s;
}

//------------------------------------------------------------------------------
/**
    Get pointer to the attached stream. If there is no stream attached,
    an assertion will be thrown. Use HasStream() to determine if a stream
    is attached.
*/
const DwPtr<DwStream>& DwStreamReader::stream() const
{
    return this->m_stream;
}

//------------------------------------------------------------------------------
/**
    Returns true if a stream is attached to the reader.
*/
bool DwStreamReader::hasStream() const
{
    return this->m_stream.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
bool DwStreamReader::eof() const
{
    DW_ASSERT(this->isOpen());
    return this->m_stream->eof();
}

//------------------------------------------------------------------------------
/**
*/
bool DwStreamReader::open()
{
    DW_ASSERT(!this->m_isOpen);
    DW_ASSERT(this->m_stream.isvalid());
    DW_ASSERT(this->m_stream->canRead());
    if (this->m_stream->isOpen())
    {
        DW_ASSERT((this->m_stream->accessMode() == DwStream::ReadAccess) || (this->m_stream->accessMode() == DwStream::ReadWriteAccess));
        this->m_streamWasOpen = true;
        this->m_isOpen = true;
    }
    else
    {
        this->m_streamWasOpen = false;
        this->m_stream->setAccessMode(DwStream::ReadAccess);
        this->m_isOpen = this->m_stream->open();
    }
    return this->m_isOpen;
}

//------------------------------------------------------------------------------
/**
*/
void DwStreamReader::close()
{
    DW_ASSERT(this->m_isOpen);
    if ((!this->m_streamWasOpen) && m_stream->isOpen())
    {
        this->m_stream->close();
    }
    this->m_isOpen = false;
}
