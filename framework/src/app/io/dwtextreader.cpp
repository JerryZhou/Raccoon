
#include "stable.h"
#include "dwtextreader.h"

DW_IMPLEMENT_CLASS(DwTextReader, 'TXTR', DwStreamReader);

//------------------------------------------------------------------------------
/**
*/
DwTextReader::DwTextReader()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
DwTextReader::~DwTextReader()
{
    if (this->isOpen())
    {
        this->close();
    }
}

//------------------------------------------------------------------------------
/**
*/
unsigned char DwTextReader::readChar()
{
    unsigned char c;
    this->m_stream->read(&c, sizeof(c));
    return c;
}

//------------------------------------------------------------------------------
/**
*/
DwString DwTextReader::readLine()
{
    // read chunk until newline found
    DwString result;
    char tmp[2];
    tmp[1] = '\0';
    while(!this->m_stream->eof())
    {
        if(1 != this->m_stream->read(tmp, 1)) break;
        if(tmp[0] == '\n') break;
        result.append(DwString(tmp));
    }
    return result;
}

//------------------------------------------------------------------------------
/**
*/
DwString DwTextReader::readAll()
{
    DwString result;
    if (this->m_stream->size() > 0)
    {
        this->m_stream->seek(0, DwStream::Begin);
        DwStream::Size size = this->m_stream->size();
        char* buf = (char*) DwMemoryAlloc(DW_SCRATCH_HEAP, size + 1);
        DwStream::Size bytesRead = this->m_stream->read(buf, size);
        buf[bytesRead] = 0;
        result = buf;
        DwMemoryFree(DW_SCRATCH_HEAP, buf);
    }
    return result;
}

//------------------------------------------------------------------------------
/**
*/
DwVector<DwString> DwTextReader::readAllLines()
{
    DwVector<DwString> result;
    while (!m_stream->eof())
    {
        result.append(this->readLine());
    }
    return result;
}
