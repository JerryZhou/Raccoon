#pragma once

#include <dwcore/dwstring.h>
#include <dwcore/dwiodevice.h>
#include <dwcore/dwfileengine.h>

class DwResourceFileEngine : public DwAbstractFileEngine
{
public:
    static bool exist(const DwString &fileName);

    virtual bool atEnd() const;
    virtual int64_t pos() const;
    virtual int64_t size() const;
    virtual bool open(DwIODevice::OpenMode mode);
    virtual bool seek(int64_t pos);
    virtual int64_t read(char *data, int64_t size);
    virtual int64_t write(const char *data, int64_t size);
    virtual void close();
};
