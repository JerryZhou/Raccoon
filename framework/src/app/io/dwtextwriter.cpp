
#include "stable.h"
#include "dwtextwriter.h"
#include "dwapp/dwutil.h"

DW_IMPLEMENT_CLASS(DwTextWriter, 'TXTW', DwStreamWriter);

//------------------------------------------------------------------------------
/**
*/
DwTextWriter::DwTextWriter()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
DwTextWriter::~DwTextWriter()
{
    if (this->isOpen())
    {
        this->close();
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwTextWriter::writeChar(unsigned char c)
{
    this->m_stream->write(&c, sizeof(c));
}

//------------------------------------------------------------------------------
/**
*/
void DwTextWriter::writeString(const DwString& str)
{
    DwByteArray datas = str.toUtf8();
    this->m_stream->write(datas.data(), datas.size());
}

//------------------------------------------------------------------------------
/**
*/
void __cdecl DwTextWriter::writeFormatted(const char* fmtString, ...)
{
    va_list argList;
    va_start(argList, fmtString);
    DwString str;
    DwStringUtil::stringFormatFromArgList(str, fmtString, argList);
    writeString(str);
    va_end(argList);
}

//------------------------------------------------------------------------------
/**
*/
void DwTextWriter::writeLine(const DwString& line)
{
    this->writeString(line);
    this->writeChar('\n');
}

//------------------------------------------------------------------------------
/**
*/
void DwTextWriter::writeLines(const DwVector<DwString>& lines)
{
    int i;
    for (i = 0; i < lines.size(); i++)
    {
        this->writeLine(lines[i]);
    }
}