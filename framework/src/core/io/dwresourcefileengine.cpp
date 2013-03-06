#include "stable.h"

#include <dwcore/dwdebug.h>

#include "dwresourcefileengine.h"

bool DwResourceFileEngine::exist(const DwString &fileName)
{
    DW_UNUSED(fileName);
    dwDebug() << "DwResourceFileEngine Not implemented";
    return false;
}

int64_t DwResourceFileEngine::pos() const
{
    dwDebug() << "DwResourceFileEngine Not implemented";
    return 0;
}

int64_t DwResourceFileEngine::size() const
{
    dwDebug() << "DwResourceFileEngine Not implemented";
    return 0;
}

bool DwResourceFileEngine::open( DwIODevice::OpenMode mode )
{
    DW_UNUSED(mode);
    dwDebug() << "DwResourceFileEngine Not implemented";
    return false;
}

bool DwResourceFileEngine::seek( int64_t pos )
{
    DW_UNUSED(pos);
    dwDebug() << "DwResourceFileEngine Not implemented";
    return false;
}

int64_t DwResourceFileEngine::read( char *data, int64_t size )
{
    DW_UNUSED(data);
    DW_UNUSED(size);

    dwDebug() << "DwResourceFileEngine Not implemented";
    return 0;
}

int64_t DwResourceFileEngine::write( const char *data, int64_t size )
{
    DW_UNUSED(data);
    DW_UNUSED(size);
    dwDebug() << "DwResourceFileEngine Not implemented";
    return 0;
}

void DwResourceFileEngine::close()
{
    dwDebug() << "DwResourceFileEngine Not implemented";
}

bool DwResourceFileEngine::atEnd() const
{
    dwDebug() << "DwResourceFileEngine Not implemented";
    return false;
}
