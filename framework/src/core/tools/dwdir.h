#pragma once
#include "dwstring.h"

class DwDirPrivate;
class DW_CORE_EXPORT DwDir
{
public:
    DwDir(const DwString& path);
    ~DwDir();

    void setPath(const DwString& path);
    DwString path() const;
    DwString absolutePath() const;

    bool cdUp();
    bool cd(const DwString& path);

    static DwString applicationDir() ;
    static DwString pathAppend(const DwString &path, const DwString &filename) ;
    static DwString dirFromFilePath(const DwString &filePath);

private:
    static DwString cleanPath(const DwString& path);
    static bool isRoot(const DwString& pathName) ;
    static bool isRelativePath(const DwString& pathName) ;

    DwDirPrivate* d_ptr;
};