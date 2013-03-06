
#include "stable.h"
#include "dwstreamwriter.h"

DW_IMPLEMENT_CLASS(DwStreamWriter, 'STWR', DwRefCounted);

//------------------------------------------------------------------------------
/**
*/
DwStreamWriter::DwStreamWriter() :
    m_isOpen(false),
    m_streamWasOpen(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
DwStreamWriter::~DwStreamWriter()
{
    DW_ASSERT(!this->isOpen());
}

//------------------------------------------------------------------------------
/**
    Attaches the writer to a stream. This will imcrement the refcount
    of the stream.
*/
void DwStreamWriter::setStream(const DwPtr<DwStream>& s)
{
    this->m_stream = s;
}

//------------------------------------------------------------------------------
/**
    Get pointer to the attached stream. If there is no stream attached,
    an assertion will be thrown. Use HasStream() to determine if a stream
    is attached.
*/
const DwPtr<DwStream>& DwStreamWriter::stream() const
{
    return this->m_stream;
}

//------------------------------------------------------------------------------
/**
    Returns true if a stream is attached to the writer.
*/
bool DwStreamWriter::hasStream() const
{
    return this->m_stream.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
bool DwStreamWriter::open()
{
    DW_ASSERT(!this->m_isOpen);
    DW_ASSERT(this->m_stream.isvalid());
    DW_ASSERT(this->m_stream->canWrite());
    if (this->m_stream->isOpen())
    {
        DW_ASSERT((this->m_stream->accessMode() == DwStream::WriteAccess) 
            || (this->m_stream->accessMode() == DwStream::ReadWriteAccess) 
            || (this->m_stream->accessMode() == DwStream::AppendAccess));
        this->m_streamWasOpen = true;
        this->m_isOpen = true;
    }
    else
    {
        this->m_streamWasOpen = false;
        this->m_stream->setAccessMode(DwStream::WriteAccess);
        this->m_isOpen = this->m_stream->open();
    }
    return this->m_isOpen;
}

//------------------------------------------------------------------------------
/**
*/
void
DwStreamWriter::close()
{
    DW_ASSERT(this->m_isOpen);
    if ((!this->m_streamWasOpen) && m_stream->isOpen())
    {
        m_stream->close();
    }
    this->m_isOpen = false;
}
