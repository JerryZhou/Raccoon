#pragma once

#include "dwgui/dwrttiobject.h"
#include "dwgui/dwurl.h"

// NB ! THE LOG IS NOT THREAD SAFE
// TODO: BE MORE INFRASTUCTURE, NOT USE URL STRING AND OTHER SUPERSTRATUM STRUCT
//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwRingLog : public DwRttiObject
{
    DW_DECLARE_CLASS(DwRingLog);
    DW_DISABLE_COPY(DwRingLog);
public:
    // default size is 4K
    DwRingLog(int len = 4096);
    // default destruct
    virtual ~DwRingLog();

    // return false, if the length is max than size of buffer
    bool __cdecl format(const wchar_t *fmt, ...);
    // return false, if the length is max than size of buffer
    bool __cdecl format(const char *fmt, ...);

    // dump all of information to a file
    void dump(const DwUrl &url);
    // dump at the end of app
    void dump(const char *);
    // dump to file
    void dump(DwString file);
    // dump to string
    void dumpToString(DwString &str);

    // is uniocde text
    bool isUnicode() const;
    // set the dump will start with 0xfffe
    void setUnicode(bool unicode);

    // final data interface, will return a null-terminated buffer
    const char* data() const;
    // empty the data
    void clear();

    // user should implement the own object format
    template<typename T>
    inline DwRingLog& operator << (const T& v);

protected:
    // copy the null-terminated str into buff
    bool copyToBuffer(const wchar_t *str);
    // copy the null-terminated str into buff
    bool copyToBuffer(const char *str);
    // copy the buff in len
    bool copyToBuffer(void *p, int len);

    bool m_unicode;
    int m_cur;
    int m_bufLen;
    char *m_buffer;
};// end of DwRingLog