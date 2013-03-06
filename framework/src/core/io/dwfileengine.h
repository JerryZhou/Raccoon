#pragma once

#include <dwcore/dwstring.h>
#include <dwcore/dwiodevice.h>

class DwAbstractFileEngine
{
public:
    static DwAbstractFileEngine *create(const DwString &fileName);

    DwString fileName() const
    {
        return m_fileName;
    }
    void setFileName(const DwString &fileName)
    {
        m_fileName = fileName;
    }

    virtual bool atEnd() const = NULL;
    virtual int64_t pos() const = NULL;
    virtual int64_t size() const = NULL;
    virtual bool open(DwIODevice::OpenMode mode) = NULL;
    virtual bool seek(int64_t pos) = NULL;
    virtual int64_t read(char *data, int64_t size) = NULL;
    virtual int64_t write(const char *data, int64_t size) = NULL;
    virtual void close() = NULL;

private:
    DwString m_fileName;
};

class DwFileEngine : public DwAbstractFileEngine
{
public:
    DwFileEngine();

    static bool exist(const DwString &fileName);
    static bool remove(const DwString &fileName);

    virtual bool atEnd() const;
    virtual bool open(DwIODevice::OpenMode mode);
    virtual bool seek(int64_t pos);
    virtual int64_t pos() const;
    virtual int64_t size() const;
    virtual int64_t read(char *data, int64_t size);
    virtual int64_t write(const char *data, int64_t size);
    virtual void close();

private:
    bool seekToEnd();

private:
    HANDLE m_fileHandle;
};

class DW_CORE_EXPORT DwAbstractFileEngineHandler
{
public:
    DwAbstractFileEngineHandler();
    ~DwAbstractFileEngineHandler();
    virtual DwAbstractFileEngine *create(const DwString &fileName) const = 0;
};
