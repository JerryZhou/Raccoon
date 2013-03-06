#include "dwdir.h"
#include "io/dwfileengine.h"
#include "kernel/dwappdata.h"

DW_GLOBAL_STATIC(DwApplicationData, _appData);

class DwDirPrivate
{
public:
    DwDirPrivate(const DwString& path);
    ~DwDirPrivate();

    DwString m_path;
};

DwDirPrivate::DwDirPrivate(const DwString& path)
//: m_path(path)
{
    if (path.endsWith(L'\\') || path.endsWith(L'/'))
    {
        m_path = path.left(path.length() - 1);
    }
    else
    {
        m_path = path;
    }
}

DwDir::DwDir(const DwString& path)
    : d_ptr(new DwDirPrivate(path))
{

}

DwDir::~DwDir()
{
}

DwString DwDir::applicationDir()
{
    return dirFromFilePath(_appData()->m_applicationFilePath);
}

DwString DwDir::pathAppend( const DwString& path, const DwString& filename )
{
    DwString newPath = path;
    if (path.endsWith(L'/') || path.endsWith(L'\\'))
    {
        newPath.append(filename);
    }
    else
    {
        newPath.append(L"\\");
        newPath.append(filename);
    }

    return newPath;
}

DwString DwDir::dirFromFilePath(const DwString &filePath)
{
    int forwardSlashPos = filePath.lastIndexOf(L'/');
    int backSlashPos = filePath.lastIndexOf(L'\\');
    if(forwardSlashPos == -1 && backSlashPos == -1)
    {
        return filePath;
    }

    return filePath.left(dwMax(forwardSlashPos, backSlashPos));
}

void DwDir::setPath( const DwString& path )
{
    if (path.endsWith(L'\\') || path.endsWith(L'/'))
    {
        d_ptr->m_path = path.left(path.length() - 1);
    }
    else
    {
        d_ptr->m_path = path;
    }
}

DwString DwDir::path() const
{
    return d_ptr->m_path;
}

DwString DwDir::absolutePath() const
{
    DwString ret;
    if (!isRelativePath(d_ptr->m_path))
    {
        ret = d_ptr->m_path;
    }
    ret = pathAppend(applicationDir(), d_ptr->m_path);
    return cleanPath(ret);
}

bool DwDir::cdUp()
{
    return cd("..");
}

bool DwDir::cd( const DwString& path )
{
    if (path.isEmpty() || path == DwString(L'.'))
    {
        return false;
    }

    DwString newPath = d_ptr->m_path;
    if (!isRelativePath(path))
    {
        newPath = cleanPath(path);
    }
    else
    {
        if (isRoot(d_ptr->m_path))
        {
            if (path == "..")
                return false;
        }
        else
        {
            newPath += L'/';
        }

        newPath += path;
        if (path.indexOf(L'/') >= 0
                || d_ptr->m_path == "."
                || path == "..")
        {
            newPath = cleanPath(newPath);

            if (newPath.startsWith(".."))
            {
                DwDir dir(newPath);
                newPath = dir.absolutePath();
            }
        }
    }

    setPath(newPath);

    if (!DwFileEngine::exist(newPath))
        return false;

    return true;
}

DwString DwDir::cleanPath(const DwString& path)
{
    if (path.isEmpty())
        return path;
    DwString name = path;
    wchar_t dir_separator = L'\\';
    if (dir_separator != L'/')
        name.replace(dir_separator, L'/');

    int used = 0, levels = 0;
    const int len = name.length();
    DwVector<wchar_t> outVector;
    outVector.resize(len);
    wchar_t *out = &outVector[0];

    const wchar_t *p = name.constString();
    for (int i = 0, last = -1, iwrite = 0; i < len; ++i)
    {
        if (p[i] == L'/')
        {
            while (i < len-1 && p[i+1] == L'/')
            {
                //allow unc paths
                if (!i)
                    break;

                i++;
            }
            bool eaten = false;
            if (i < len - 1 && p[i+1] == L'.')
            {
                int dotcount = 1;
                if (i < len - 2 && p[i+2] == L'.')
                    dotcount++;
                if (i == len - dotcount - 1)
                {
                    if (dotcount == 1)
                    {
                        break;
                    }
                    else if (levels)
                    {
                        if (last == -1)
                        {
                            for (int i2 = iwrite-1; i2 >= 0; i2--)
                            {
                                if (out[i2] == L'/')
                                {
                                    last = i2;
                                    break;
                                }
                            }
                        }
                        used -= iwrite - last - 1;
                        break;
                    }
                }
                else if (p[i+dotcount+1] == L'/')
                {
                    if (dotcount == 2 && levels)
                    {
                        if (last == -1 || iwrite - last == 1)
                        {
                            for (int i2 = (last == -1) ? (iwrite-1) : (last-1); i2 >= 0; i2--)
                            {
                                if (out[i2] == L'/')
                                {
                                    eaten = true;
                                    last = i2;
                                    break;
                                }
                            }
                        }
                        else
                        {
                            eaten = true;
                        }
                        if (eaten)
                        {
                            levels--;
                            used -= iwrite - last;
                            iwrite = last;
                            last = -1;
                        }
                    }
                    else if (dotcount == 2 && i > 0 && p[i - 1] != L'.')
                    {
                        eaten = true;
                        int tmp = 0 < last ? last : 0;
                        used -= iwrite - tmp;
                        iwrite = tmp;
                        last = -1;
                        ++i;
                    }
                    else if (dotcount == 1)
                    {
                        eaten = true;
                    }
                    if (eaten)
                        i += dotcount;
                }
                else
                {
                    levels++;
                }
            }
            else if (last != -1 && iwrite - last == 1)
            {
                eaten = (iwrite > 2);
                last = -1;
            }
            else if (last != -1 && i == len-1)
            {
                eaten = true;
            }
            else
            {
                levels++;
            }
            if (!eaten)
                last = i - (i - iwrite);
            else
                continue;
        }
        else if (!i && p[i] == L'.')
        {
            int dotcount = 1;
            if (len >= 1 && p[1] == L'.')
                dotcount++;
            if (len >= dotcount && p[dotcount] == L'/')
            {
                if (dotcount == 1)
                {
                    i++;
                    while (i+1 < len-1 && p[i+1] == L'/')
                        i++;
                    continue;
                }
            }
        }
        out[iwrite++] = p[i];
        used++;
    }

    DwString ret = (used == len ? name : DwString(out, used));
    // Strip away last slash except for root directories
    if (ret.length() > 1 && ret.endsWith(L'/'))
    {
        if (!(ret.length() == 3 && ret.at(1) == L':'))
        {
            ret.resize(ret.length() - 1);
        }
    }

    return ret;
}

bool DwDir::isRoot( const DwString& pathName )
{
    if (!isRelativePath(pathName) && pathName.length() == 2)
    {
        return true;
    }
    return false;
}

bool DwDir::isRelativePath(const DwString& pathName)
{
    //unc path
    if (pathName.startsWith(L"\\\\?\\"))
    {
        return false;
    }
    //normal path
    if (pathName.length() >= 2)
    {
        wchar_t ch0 = pathName.at(0);
        wchar_t ch1 = pathName.at(1);
        bool isChar = false;
        if ((ch0 >= L'a' && ch0 <= L'z') || (ch0 >= L'A' && ch0 <= L'Z'))
        {
            isChar = true;
        }
        if (isChar && ch1 == L':')
        {
            return false;
        }
    }

    return true;
}