#pragma once

#include <dwcore/dwiodevice.h>

class DwAbstractFileEngine;
class DW_CORE_EXPORT DwFile : public DwIODevice
{
    DW_DISABLE_COPY(DwFile)

public:
    DwFile(const DwString &fileName);
    explicit DwFile(DwObject *parent);
    DwFile(const DwString &fileName, DwObject *parent);
    ~DwFile();

    virtual bool open(OpenMode mode);
    virtual void close();

    virtual bool atEnd() const;
    virtual int64_t bytesAvailable() const;
    virtual int64_t pos() const;
    virtual bool seek(int64_t pos);
    virtual int64_t size() const;

    static bool exist(const DwString &fileName);
    static bool remove(const DwString &fileName);

protected:
    int64_t readData(char *data, int64_t maxSize);
    int64_t writeData(const char *data, int64_t maxSize);

private:
    DwAbstractFileEngine *m_fileEngine;
};
