#include "stable.h"
#include "dwassignlib.h"
#include "dwfswrapper.h"
#include "graphicsview/dwdummys.h"

DW_IMPLEMENT_CLASS(DwAssignLib, 'ASLB', DwRttiObject);

//------------------------------------------------------------------------------
/**
*/
DwAssignLib::DwAssignLib()
    : m_isValid(false)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwAssignLib::~DwAssignLib()
{
    if (isValid())
    {
        discard();
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwAssignLib::setup()
{
    DwCSLocker locks(&m_criticalSection);

    DW_CHECK(!isValid());
    m_isValid = true;
    setupSystemAssigns();
    setupProjectAssigns();
}

//------------------------------------------------------------------------------
/**
*/
void DwAssignLib::discard()
{
    DwCSLocker locks(&m_criticalSection);

    DW_ASSERT(isValid());
    m_assignTable.clear();
    m_isValid = false;
}

//------------------------------------------------------------------------------
/**
*/
bool DwAssignLib::isValid() const
{
    return m_isValid;
}

//------------------------------------------------------------------------------
/**
*/
void DwAssignLib::setupSystemAssigns()
{
    DW_ASSERT(isValid());
    DwCSLocker locks(&m_criticalSection);

    DwString homeLocation = DwFsWrapper::homeDirectory();
    if (!homeLocation.isEmpty())
    {
        setAssign(DwAssign("home", homeLocation));
    }

    DwString binLocation = DwFsWrapper::binDirectory();
    if (!binLocation.isEmpty())
    {
        setAssign(DwAssign("bin", binLocation));
    }
    DwString tempLocation = DwFsWrapper::tempDirectory();
    if (!tempLocation.isEmpty())
    {
        setAssign(DwAssign("temp", tempLocation));
    }
    DwString userLocation = DwFsWrapper::userDirectory();
    if (!userLocation.isEmpty())
    {
        setAssign(DwAssign("user", userLocation));
    }

    DwString appDataLocation = DwFsWrapper::appDataDirectory();
    if (!appDataLocation.isEmpty())
    {
        setAssign(DwAssign("appdata", appDataLocation));
    }

    DwString programsLocation = DwFsWrapper::programsDirectory();
    if (!programsLocation.isEmpty())
    {
        setAssign(DwAssign("programs", programsLocation));
    }

    DwString currentLocation = DwFsWrapper::currentDirectory();
    if (!currentLocation.isEmpty())
    {
        setAssign(DwAssign("current", currentLocation));
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwAssignLib::setupProjectAssigns()
{
    DW_ASSERT(isValid());
    DwCSLocker locks(&m_criticalSection);

    DwString rootDir = DwFsWrapper::rootDirectory();
    setAssign(DwAssign("root", rootDir));

    // test data
    setAssign(DwAssign(L"dwgui", L"dll:///theme/gui"));
}

//------------------------------------------------------------------------------
/**
*/
void DwAssignLib::setAssign(const DwAssign& assign)
{
    DW_ASSERT(isValid());
    DwCSLocker locks(&m_criticalSection);

    if (hasAssign(assign.name()))
    {
        clearAssign(assign.name());
    }
    m_assignTable.insert(assign.name(), assign.path());
}

//------------------------------------------------------------------------------
/**
*/
bool DwAssignLib::hasAssign(const DwString& assignName) const
{
    DwCSLocker locks(&m_criticalSection);

    DW_ASSERT(!assignName.isEmpty());
    bool result = m_assignTable.contains(assignName);
    return result;
}

//------------------------------------------------------------------------------
/**
*/
DwString DwAssignLib::assignOf(const DwString& assignName) const
{
    DwCSLocker locks(&m_criticalSection);

    DW_ASSERT(!assignName.isEmpty());
    DW_ASSERT(hasAssign(assignName));
    DwString result = m_assignTable[assignName];
    return result;
}

//------------------------------------------------------------------------------
/**
*/
void DwAssignLib::clearAssign(const DwString& assignName)
{
    DwCSLocker locks(&m_criticalSection);

    DW_ASSERT(!assignName.isEmpty());
    DW_ASSERT(hasAssign(assignName));
    m_assignTable.remove(assignName);
}

//------------------------------------------------------------------------------
/**
*/
DwVector<DwAssign> DwAssignLib::assigns() const
{
    DwCSLocker locks(&m_criticalSection);

    DwVector<DwAssign> oassigns;
    DwMap<DwString, DwString>::const_iterator ite = m_assignTable.begin();
    while (ite != m_assignTable.end())
    {
        oassigns.append(DwAssign((*ite).first, (*ite).second));
        ++ite;
    }

    return oassigns;
}

//------------------------------------------------------------------------------
/**
    Resolves all assigns from an URI returning an URI. It is allowed to
    "stack" assigns, which means, defining an assign as pointing to
    another assign.
*/
DwUrl DwAssignLib::resolveAssigns(const DwUrl& uri) const
{
    return DwUrl(resolveAssignsInString(uri.asString()));
}

//------------------------------------------------------------------------------
/**
    Resolves all assigns from a URI. It is allowed to
    "stack" assigns, which means, defining an assign as pointing to
    another assign.s
*/
DwString DwAssignLib::resolveAssignsInString(const DwString& uriString) const
{
    if (!m_isValid)
    {
        const_cast<DwAssignLib*>(this)->setup();
    }

    DwString result = uriString;

    // check for assigns
    int colonIndex;
    while ((colonIndex = result.indexOf(':', 0)) > 0)
    {
        // special case: ignore one-caracter "assigns" because they are
        // really DOS drive letters
        if (colonIndex > 1)
        {
            DwString assignString = result.mid(0, colonIndex);

            // ignore unknown assigns, because these may be part of an URI
            if (hasAssign(assignString))
            {
                DwString postAssignString = result.mid(colonIndex + 1, result.length() - (colonIndex + 1));
                DwString replace = assignOf(assignString);
                if (!replace.isEmpty())
                {
                    if (replace[replace.length()-1] != L':'
                            && (replace[replace.length()-1] != L'/'
                                || replace[replace.length()-2] != L':'))
                    {
                        replace.append(L"/");
                    }
                    replace.append(postAssignString);
                }
                result = replace;
            }
            else break;
        }
        else break;
    }
    dwConvertBackslashes(result);
    dwTrimRight(result, L'/');
    return result;
}

//------------------------------------------------------------------------------
/**
*/
void DwAssignLib::initAssignLib()
{
    dwAssignLib()->setup();
}

//------------------------------------------------------------------------------
/**
*/
void DwAssignLib::unInitAssignLib()
{
    dwAssignLib()->discard();
}

//------------------------------------------------------------------------------
/**
*/
DW_GLOBAL_STATIC(DwAssignLib, _dw_assignLib)
DwAssignLib* dwAssignLib()
{
    return _dw_assignLib();
}