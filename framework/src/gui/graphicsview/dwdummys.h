#pragma once
//NB! DO NOT INCLUDE THIS IN ANY HEADER FILES
/// dummy dependency stuffs
//////////////////////////////////////////////////////////////////////////
#ifndef DW_CHECK
#define DW_CHECK(con) if (!(con)) {return;}
#define DW_CHECK_RET(con, ret) if(!(con)) {return (ret);}
#define DW_CHECK_ASSERT(con) if(!(con)) {assert(false); return;}
#define DW_CHECK_ASSERT_RET(con, ret) if(!(con)) {assert(false); return (ret);}
#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
const int  InvalidIndex = -1;
const unsigned int InvalidTimerId = (unsigned int)(-1);
typedef int IndexT;
//////////////////////////////////////////////////////////////////////////

// Used to imp refcounted out side the class in execution flow
//////////////////////////////////////////////////////////////////////////
class DwLevelRefCounted
{
public:
    DwLevelRefCounted(int& count)
        : m_counted(count)
    {
        ++m_counted;
    }

    ~DwLevelRefCounted()
    {
        --m_counted;
    }
protected:
private:
    int &m_counted;
};
const int CountedLevelOne = 0;
//////////////////////////////////////////////////////////////////////////

// Used to save the temp variable state
//////////////////////////////////////////////////////////////////////////
typedef void* RefHandler;
class DwRefPtr
{
public:
    DwRefPtr(RefHandler& h, void* to)
        : m_h(h)
    {
        m_h = to;
    }

    ~DwRefPtr()
    {
        m_h = NULL;
    }
protected:
private:
    RefHandler &m_h;
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
#define DW_EVENT_CHECK(evt) if(evt->isAccepted()) {return;}
#define DW_EVENT_CHECK_RET(evt, ret) if(evt->isAccepted()) {return ret;}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
#define DW_EVENT_CAST(evt, type) (dwsafe_cast<type>(evt))
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
#define DW_EVENT_BEGIN() if(0) {
#define DW_EVENT_END() }
#define DW_EVENT_DISPATCH(evt, type, handler) } else if( evt->eventId() == type::EventId() ) { handler(DW_EVENT_CAST(evt, type)); DW_EVENT_CHECK(evt);
#define DW_EVENT_DISPATCH_AS(evt, type, handler) }  else if( evt->isA( type::EventId() ) ) { handler(DW_EVENT_CAST(evt, type)); DW_EVENT_CHECK(evt);
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
#define DW_OUT DW_UI_LOGER
#define DW_ENDL "\n"
#define DW_TR(s) s << DW_ENDL
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define DW_DEBUG_EVENTINFO(evt) DW_UI_LOGER() << (evt)->eventId()->name() << DW_ENDL;
#else
#define DW_DEBUG_EVENTINFO(evt)
#endif
//////////////////////////////////////////////////////////////////////////

// The Default scheme is local file
//////////////////////////////////////////////////////////////////////////
#define DEFAULT_IO_SCHEME "file"
//////////////////////////////////////////////////////////////////////////

// The Max Path Length
//////////////////////////////////////////////////////////////////////////
#define DW_GUI_MAX_PATH 512
//////////////////////////////////////////////////////////////////////////

// Max Event Dispatch Deep
//////////////////////////////////////////////////////////////////////////
#define DW_GUI_MAX_EVENT_LEVEL 32
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
class DwSceneEvent;
void dwDebugEventInfo(DwSceneEvent* evt);
//////////////////////////////////////////////////////////////////////////

// Used to expand the dwstring and the path
//////////////////////////////////////////////////////////////////////////
DwString dwExtractToEnd(const DwString &str, IndexT pos);

void dwTerminateAtIndex(DwString &str, IndexT pos);

void dwTrimRight(DwString &str, wchar_t c);

void dwConvertBackslashes(DwString &str);

int dwLastSlashIndex(DwString &str, int from = -1);

DwString dwExtractDirName(const DwString &str);

DwString dwExtractLastDirName(const DwString &str);

DwVector<DwString> dwTokenize(const DwString& inStr, const DwString& whiteSpace, wchar_t fence);

DwVector<DwString> dwTokenize(const DwString& inStr, const DwString& whiteSpace);

bool dwCheckValidCharSet(const DwString &str, const DwString &charset);

bool dwMatchPattern(const DwString &str, const DwString &pattern);

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
void __cdecl dwStringFormat(DwString &str, const char* fmtString, ...);
void __cdecl dwStringFormatFromArgList(DwString &str, const char* fmtString, va_list argList);
void __cdecl dwStringFormat(DwString &str, const wchar_t* fmtString, ...);
void __cdecl dwStringFormatFromArgList(DwString &str, const wchar_t* fmtString, va_list argList);
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
int dwRandom();
int dwRandom(int from, int to);
float dwRandomF(float from, float to);

//////////////////////////////////////////////////////////////////////////
#define DW_MESSAGE_HANDLER(func) \
if(true) \
{ \
    bHandled = TRUE; \
    lResult = func(uMsg, wParam, lParam, bHandled); \
    if(bHandled) \
        return TRUE; \
}