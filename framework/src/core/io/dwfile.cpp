#include "stable.h"

#include "dwfileengine.h"
#include "dwresourcefileengine.h"
#include "dwfile.h"

DwFile::DwFile(const DwString &fileName)
{
    m_fileEngine = DwAbstractFileEngine::create(fileName);
    m_fileEngine->setFileName(fileName);
}

DwFile::DwFile( DwObject *parent )
    : DwIODevice(parent)
    , m_fileEngine(NULL)
{
}

DwFile::DwFile(const DwString &fileName, DwObject *parent)
    : DwIODevice(parent)
{
    m_fileEngine = DwAbstractFileEngine::create(fileName);
    m_fileEngine->setFileName(fileName);
}

int64_t DwFile::readData( char *data, int64_t maxSize )
{
    return m_fileEngine->read(data, maxSize);
}

int64_t DwFile::writeData( const char *data, int64_t maxSize )
{
    return m_fileEngine->write(data, maxSize);
}

bool DwFile::open( OpenMode mode )
{
    if(m_fileEngine->open(mode))
    {
        return DwIODevice::open(mode);
    }

    return false;
}

void DwFile::close()
{
    m_fileEngine->close();
}

int64_t DwFile::bytesAvailable() const
{
    return m_fileEngine->size() - m_fileEngine->pos();
}

int64_t DwFile::pos() const
{
    return m_fileEngine->pos();
}

bool DwFile::seek(int64_t pos)
{
    return m_fileEngine->seek(pos);
}

int64_t DwFile::size() const
{
    return m_fileEngine->size();
}

bool DwFile::exist( const DwString &fileName )
{
    if(fileName.startsWith(":"))
    {
        return DwResourceFileEngine::exist(fileName);
    }

    return DwFileEngine::exist(fileName);
}

bool DwFile::remove( const DwString &fileName )
{
    if(fileName.startsWith(L":"))
    {
        DW_ASSERT(false);
        return false;
    }

    return DwFileEngine::remove(fileName);
}

DwFile::~DwFile()
{
    DW_SAFE_DELETE(m_fileEngine);
}

bool DwFile::atEnd() const
{
    return m_fileEngine->atEnd();
}
