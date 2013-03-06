#include "stable.h"
#include "dwaccessible.h"
#include "dwgui/dwframewindow.h"
#include "dwgui/dwgraphicsview.h"
#include "dwgui/dwapplication.h"
#include "dwgui/dwaccessibleitem.h"
#include "dwgui/dwaccessibleapplication.h"
#include "dwgui/dwaccessiblewindow.h"
#include "dwcore/dwmap.h"

typedef DwMap<const DwRtti*, DwAccessible::InterfaceFactory > InterfaceFactoryContainer;
DW_GLOBAL_STATIC(InterfaceFactoryContainer, dwAccessibleFactories);
DW_IMPLEMENT_CLASS(DwAccessible, 'MSAA', DwRttiObject);

static bool accessibility_active = false;
static bool cleanupAdded = false;
static void dwAccessibleCleanup()
{
    dwAccessibleFactories()->clear();
}

//------------------------------------------------------------------------------
DwAccessible::UpdateHandler DwAccessible::updateHandler = 0;
DwAccessible::RootObjectHandler DwAccessible::rootObjectHandler = 0;

//------------------------------------------------------------------------------
/**
*/
void DwAccessible::installFactory(const DwRtti* key, InterfaceFactory factory)
{
    if (!factory)
        return;

    if (!cleanupAdded) {
        //qAddPostRoutine(dwAccessibleCleanup);
        cleanupAdded = true;
    }
    if (dwAccessibleFactories()->contains(key))
        return;
    dwAccessibleFactories()->insert(key, factory);
}

//------------------------------------------------------------------------------
/**
*/
void DwAccessible::removeFactory(const DwRtti* key)
{
    dwAccessibleFactories()->remove(key);
}

//------------------------------------------------------------------------------
/**
*/
DwAccessible::UpdateHandler DwAccessible::installUpdateHandler(UpdateHandler handler)
{
    UpdateHandler old = updateHandler;
    updateHandler = handler;
    return old;
}

//------------------------------------------------------------------------------
/**
*/
DwAccessible::RootObjectHandler DwAccessible::installRootObjectHandler(RootObjectHandler handler)
{
    RootObjectHandler old = rootObjectHandler;
    rootObjectHandler = handler;
    return old;
}

//------------------------------------------------------------------------------
/**
*/
DwAccessibleInterface *DwAccessible::queryAccessibleInterface(DwRttiObject *object)
{
    accessibility_active = true;
    DwAccessibleInterface *iface = 0;
    if (!object)
        return 0;

    /*QEvent e(QEvent::AccessibilityPrepare);
    QApplication::sendEvent(object, &e);*/

    if (!dwAccessibleFactories()->isEmpty()){
        const DwRtti *mo = object->rtti();
        while (mo) {
            InterfaceFactory factory = dwAccessibleFactories()->value(mo, NULL);
            if (factory){
                iface = factory(object);
                if (iface)
                    return iface;
            }

            mo = mo->parent();
        }
    }

    DwGraphicsItem *widget = dwsafe_cast<DwGraphicsItem>(object);
    if (widget){
        if (widget->isA(DwFrameWindow::RTTI())){
            return new DwAccessibleWindow(widget);
        }else
        return new DwAccessibleItem(widget);
    }
    else if (object == dwApp()){
        return new DwAccessibleApplication();
    }

    return 0;
}

//------------------------------------------------------------------------------
/**
*/
bool DwAccessible::isActive()
{
    return accessibility_active;
}

//------------------------------------------------------------------------------
/**
*/
void DwAccessible::initialize()
{

}

//------------------------------------------------------------------------------
/**
*/
void DwAccessible::cleanup()
{

}

//------------------------------------------------------------------------------
/**
*/
void DwAccessible::updateAccessibility(DwRttiObject *o, int who, Event reason)
{
    DW_ASSERT(o);

    if (updateHandler) {
        updateHandler(o, who, reason);
        return;
    }

    DwString soundName;
    switch (reason) {
    case PopupMenuStart:
        soundName = DwString("MenuPopup");
        break;

    case MenuCommand:
        soundName = DwString("MenuCommand");
        break;

    case Alert:
        soundName = DwString("SystemAsterisk");
        break;
    default:
        break;
    }

    if (soundName.size()) {
        //TODO: xx
        DwString file;
        if (!file.isEmpty()) {
            //::PlaySound(soundName.constString(), 0, SND_ALIAS | SND_ASYNC | SND_NODEFAULT | SND_NOWAIT);
        }
    }

    if (!isActive())
        return;

    // TODO: notify
    return;
}

void DwAccessible::setRootObject(DwRttiObject *o)
{
    if (rootObjectHandler) {
        rootObjectHandler(o);
    }
}

//------------------------------------------------------------------------------
class DwWindowsEnumerate : public IEnumVARIANT
{
public:
    DwWindowsEnumerate(const DwVector<int> &a)
        : ref(0), current(0),array(a)
    {
    }

    virtual ~DwWindowsEnumerate() {}

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID, LPVOID *);
    ULONG STDMETHODCALLTYPE AddRef();
    ULONG STDMETHODCALLTYPE Release();

    HRESULT STDMETHODCALLTYPE Clone(IEnumVARIANT **ppEnum);
    HRESULT STDMETHODCALLTYPE Next(unsigned long  celt, VARIANT FAR*  rgVar, unsigned long FAR*  pCeltFetched);
    HRESULT STDMETHODCALLTYPE Reset();
    HRESULT STDMETHODCALLTYPE Skip(unsigned long celt);

private:
    ULONG ref;
    ULONG current;
    DwVector<int> array;
};

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsEnumerate::QueryInterface(REFIID id, LPVOID *iface)
{
    *iface = 0;
    if (id == IID_IUnknown)
        *iface = (IUnknown*)this;
    else if (id == IID_IEnumVARIANT)
        *iface = (IEnumVARIANT*)this;

    if (*iface) {
        AddRef();
        return S_OK;
    }

    return E_NOINTERFACE;
}

//------------------------------------------------------------------------------
/**
*/
ULONG STDMETHODCALLTYPE DwWindowsEnumerate::AddRef()
{
    return ++ref;
}

//------------------------------------------------------------------------------
/**
*/
ULONG STDMETHODCALLTYPE DwWindowsEnumerate::Release()
{
    if (!--ref) {
        delete this;
        return 0;
    }
    return ref;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsEnumerate::Clone(IEnumVARIANT **ppEnum)
{
    DwWindowsEnumerate *penum = 0;
    *ppEnum = 0;

    penum = new DwWindowsEnumerate(array);
    if (!penum)
        return E_OUTOFMEMORY;
    penum->current = current;
    penum->array = array;
    penum->AddRef();
    *ppEnum = penum;

    return S_OK;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsEnumerate::Next(unsigned long  celt, VARIANT FAR*  rgVar, unsigned long FAR*  pCeltFetched)
{
    if (pCeltFetched)
        *pCeltFetched = 0;

    ULONG l;
    for (l = 0; l < celt; l++) {
        VariantInit(&rgVar[l]);
        if ((current+1) > (ULONG)array.size()) {
            *pCeltFetched = l;
            return S_FALSE;
        }

        rgVar[l].vt = VT_I4;
        rgVar[l].lVal = array[(int)current];
        ++current;
    }
    *pCeltFetched = l;
    return S_OK;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsEnumerate::Reset()
{
    current = 0;
    return S_OK;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsEnumerate::Skip(unsigned long celt)
{
    current += celt;
    if (current > (ULONG)array.size()) {
        current = array.size();
        return S_FALSE;
    }
    return S_OK;
}

//------------------------------------------------------------------------------
class DwWindowsAccessible : public IAccessible, IOleWindow, DwAccessible
{
    DW_DECLARE_CLASS(DwWindowsAccessible);
public:
    DwWindowsAccessible(DwAccessibleInterface *a=NULL)
        : ref(0), accessible(a)
    {
    }

    virtual ~DwWindowsAccessible()
    {
        delete accessible;
    }

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID, LPVOID *);
    ULONG STDMETHODCALLTYPE AddRef();
    ULONG STDMETHODCALLTYPE Release();

    HRESULT STDMETHODCALLTYPE GetTypeInfoCount(unsigned int *);
    HRESULT STDMETHODCALLTYPE GetTypeInfo(unsigned int, unsigned long, ITypeInfo **);
    HRESULT STDMETHODCALLTYPE GetIDsOfNames(const _GUID &, wchar_t **, unsigned int, unsigned long, long *);
    HRESULT STDMETHODCALLTYPE Invoke(long, const _GUID &, unsigned long, unsigned short, tagDISPPARAMS *, tagVARIANT *, tagEXCEPINFO *, unsigned int *);

    HRESULT STDMETHODCALLTYPE accHitTest(long xLeft, long yTop, VARIANT *pvarID);
    HRESULT STDMETHODCALLTYPE accLocation(long *pxLeft, long *pyTop, long *pcxWidth, long *pcyHeight, VARIANT varID);
    HRESULT STDMETHODCALLTYPE accNavigate(long navDir, VARIANT varStart, VARIANT *pvarEnd);
    HRESULT STDMETHODCALLTYPE get_accChild(VARIANT varChildID, IDispatch** ppdispChild);
    HRESULT STDMETHODCALLTYPE get_accChildCount(long* pcountChildren);
    HRESULT STDMETHODCALLTYPE get_accParent(IDispatch** ppdispParent);

    HRESULT STDMETHODCALLTYPE accDoDefaultAction(VARIANT varID);
    HRESULT STDMETHODCALLTYPE get_accDefaultAction(VARIANT varID, BSTR* pszDefaultAction);
    HRESULT STDMETHODCALLTYPE get_accDescription(VARIANT varID, BSTR* pszDescription);
    HRESULT STDMETHODCALLTYPE get_accHelp(VARIANT varID, BSTR *pszHelp);
    HRESULT STDMETHODCALLTYPE get_accHelpTopic(BSTR *pszHelpFile, VARIANT varChild, long *pidTopic);
    HRESULT STDMETHODCALLTYPE get_accKeyboardShortcut(VARIANT varID, BSTR *pszKeyboardShortcut);
    HRESULT STDMETHODCALLTYPE get_accName(VARIANT varID, BSTR* pszName);
    HRESULT STDMETHODCALLTYPE put_accName(VARIANT varChild, BSTR szName);
    HRESULT STDMETHODCALLTYPE get_accRole(VARIANT varID, VARIANT *pvarRole);
    HRESULT STDMETHODCALLTYPE get_accState(VARIANT varID, VARIANT *pvarState);
    HRESULT STDMETHODCALLTYPE get_accValue(VARIANT varID, BSTR* pszValue);
    HRESULT STDMETHODCALLTYPE put_accValue(VARIANT varChild, BSTR szValue);

    HRESULT STDMETHODCALLTYPE accSelect(long flagsSelect, VARIANT varID);
    HRESULT STDMETHODCALLTYPE get_accFocus(VARIANT *pvarID);
    HRESULT STDMETHODCALLTYPE get_accSelection(VARIANT *pvarChildren);

    HRESULT STDMETHODCALLTYPE GetWindow(HWND *phwnd);
    HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(BOOL fEnterMode);

private:
    ULONG ref;
    DwAccessibleInterface *accessible;
};
DW_IMPLEMENT_CLASS(DwWindowsAccessible, 'WALE', DwAccessible);

//------------------------------------------------------------------------------
/**
*/
static inline BSTR DwStringToBSTR(const DwString &str)
{
    BSTR bstrVal;

    int wlen = str.length()+1;
    bstrVal = SysAllocStringByteLen(0, wlen*2);
    memcpy(bstrVal, str.constString(), sizeof(wchar_t)*(wlen));
    bstrVal[wlen] = 0;

    return bstrVal;
}

//------------------------------------------------------------------------------
/**
*/
IAccessible *dw_createWindowsAccessible(DwAccessibleInterface *access)
{
    DwWindowsAccessible *acc = new DwWindowsAccessible(access);
    IAccessible *iface;
    acc->QueryInterface(IID_IAccessible, (void**)&iface);

    return iface;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsAccessible::QueryInterface(REFIID id, LPVOID *iface)
{
    *iface = 0;
    if (id == IID_IUnknown)
        *iface = (IUnknown*)(IDispatch*)this;
    else if (id == IID_IDispatch)
        *iface = (IDispatch*)this;
    else if (id == IID_IAccessible)
        *iface = (IAccessible*)this;
    else if (id == IID_IOleWindow)
        *iface = (IOleWindow*)this;
    else
        return E_NOINTERFACE;

    AddRef();
    return S_OK;
}

//------------------------------------------------------------------------------
/**
*/
ULONG STDMETHODCALLTYPE DwWindowsAccessible::AddRef()
{
    return ++ref;
}

//------------------------------------------------------------------------------
/**
*/
ULONG STDMETHODCALLTYPE DwWindowsAccessible::Release()
{
    if (!--ref) {
        delete this;
        return 0;
    }
    return ref;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsAccessible::GetTypeInfoCount(unsigned int * pctinfo)
{
    // We don't use a type library
    *pctinfo = 0;
    return S_OK;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsAccessible::GetTypeInfo(unsigned int, unsigned long, ITypeInfo **pptinfo)
{
    // We don't use a type library
    *pptinfo = 0;
    return S_OK;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsAccessible::GetIDsOfNames(const _GUID &, wchar_t **rgszNames, unsigned int, unsigned long, long *rgdispid)
{
    // PROPERTIES:  Hierarchical
    if (_bstr_t(rgszNames[0]) == _bstr_t(L"accParent"))
        rgdispid[0] = DISPID_ACC_PARENT;
    else if(_bstr_t(rgszNames[0]) == _bstr_t(L"accChildCount"))
        rgdispid[0] = DISPID_ACC_CHILDCOUNT;
    else if(_bstr_t(rgszNames[0]) == _bstr_t(L"accChild"))
        rgdispid[0] = DISPID_ACC_CHILD;

    // PROPERTIES:  Descriptional
    else if(_bstr_t(rgszNames[0]) == _bstr_t(L"accName("))
        rgdispid[0] = DISPID_ACC_NAME;
    else if(_bstr_t(rgszNames[0]) == _bstr_t(L"accValue"))
        rgdispid[0] = DISPID_ACC_VALUE;
    else if(_bstr_t(rgszNames[0]) == _bstr_t(L"accDescription"))
        rgdispid[0] = DISPID_ACC_DESCRIPTION;
    else if(_bstr_t(rgszNames[0]) == _bstr_t(L"accRole"))
        rgdispid[0] = DISPID_ACC_ROLE;
    else if(_bstr_t(rgszNames[0]) == _bstr_t(L"accState"))
        rgdispid[0] = DISPID_ACC_STATE;
    else if(_bstr_t(rgszNames[0]) == _bstr_t(L"accHelp"))
        rgdispid[0] = DISPID_ACC_HELP;
    else if(_bstr_t(rgszNames[0]) == _bstr_t(L"accHelpTopic"))
        rgdispid[0] = DISPID_ACC_HELPTOPIC;
    else if(_bstr_t(rgszNames[0]) == _bstr_t(L"accKeyboardShortcut"))
        rgdispid[0] = DISPID_ACC_KEYBOARDSHORTCUT;
    else if(_bstr_t(rgszNames[0]) == _bstr_t(L"accFocus"))
        rgdispid[0] = DISPID_ACC_FOCUS;
    else if(_bstr_t(rgszNames[0]) == _bstr_t(L"accSelection"))
        rgdispid[0] = DISPID_ACC_SELECTION;
    else if(_bstr_t(rgszNames[0]) == _bstr_t(L"accDefaultAction"))
        rgdispid[0] = DISPID_ACC_DEFAULTACTION;

    // METHODS
    else if(_bstr_t(rgszNames[0]) == _bstr_t(L"accSelect"))
        rgdispid[0] = DISPID_ACC_SELECT;
    else if(_bstr_t(rgszNames[0]) == _bstr_t(L"accLocation"))
        rgdispid[0] = DISPID_ACC_LOCATION;
    else if(_bstr_t(rgszNames[0]) == _bstr_t(L"accNavigate"))
        rgdispid[0] = DISPID_ACC_NAVIGATE;
    else if(_bstr_t(rgszNames[0]) == _bstr_t(L"accHitTest"))
        rgdispid[0] = DISPID_ACC_HITTEST;
    else if(_bstr_t(rgszNames[0]) == _bstr_t(L"accDoDefaultAction"))
        rgdispid[0] = DISPID_ACC_DODEFAULTACTION;
    else
        return DISP_E_UNKNOWNINTERFACE;

    return S_OK;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsAccessible::Invoke(long dispIdMember, const _GUID &, unsigned long, unsigned short wFlags, tagDISPPARAMS *pDispParams, tagVARIANT *pVarResult, tagEXCEPINFO *, unsigned int *)
{
    HRESULT hr = DISP_E_MEMBERNOTFOUND;

    switch(dispIdMember)
    {
        case DISPID_ACC_PARENT:
            if (wFlags == DISPATCH_PROPERTYGET) {
                if (!pVarResult)
                    return E_INVALIDARG;
                hr = get_accParent(&pVarResult->pdispVal);
            } else {
                hr = DISP_E_MEMBERNOTFOUND;
            }
            break;

        case DISPID_ACC_CHILDCOUNT:
            if (wFlags == DISPATCH_PROPERTYGET) {
                if (!pVarResult)
                    return E_INVALIDARG;
                hr = get_accChildCount(&pVarResult->lVal);
            } else {
                hr = DISP_E_MEMBERNOTFOUND;
            }
            break;

        case DISPID_ACC_CHILD:
            if (wFlags == DISPATCH_PROPERTYGET)
                hr = get_accChild(pDispParams->rgvarg[0], &pVarResult->pdispVal);
            else
                hr = DISP_E_MEMBERNOTFOUND;
            break;

        case DISPID_ACC_NAME:
            if (wFlags == DISPATCH_PROPERTYGET)
                hr = get_accName(pDispParams->rgvarg[0], &pVarResult->bstrVal);
            else if (wFlags == DISPATCH_PROPERTYPUT)
                hr = put_accName(pDispParams->rgvarg[0], pVarResult->bstrVal);
            else
                hr = DISP_E_MEMBERNOTFOUND;
            break;

        case DISPID_ACC_VALUE:
            if (wFlags == DISPATCH_PROPERTYGET)
                hr = get_accValue(pDispParams->rgvarg[0], &pVarResult->bstrVal);
            else if (wFlags == DISPATCH_PROPERTYPUT)
                hr = put_accValue(pDispParams->rgvarg[0], pVarResult->bstrVal);
            else
                hr = DISP_E_MEMBERNOTFOUND;
            break;

        case DISPID_ACC_DESCRIPTION:
            if (wFlags == DISPATCH_PROPERTYGET)
                hr = get_accDescription(pDispParams->rgvarg[0], &pVarResult->bstrVal);
            else
                hr = DISP_E_MEMBERNOTFOUND;
            break;

        case DISPID_ACC_ROLE:
            if (wFlags == DISPATCH_PROPERTYGET)
                hr = get_accRole(pDispParams->rgvarg[0], pVarResult);
            else
                hr = DISP_E_MEMBERNOTFOUND;
            break;

        case DISPID_ACC_STATE:
            if (wFlags == DISPATCH_PROPERTYGET)
                hr = get_accState(pDispParams->rgvarg[0], pVarResult);
            else
                hr = DISP_E_MEMBERNOTFOUND;
            break;

        case DISPID_ACC_HELP:
            if (wFlags == DISPATCH_PROPERTYGET)
                hr = get_accHelp(pDispParams->rgvarg[0], &pVarResult->bstrVal);
            else
                hr = DISP_E_MEMBERNOTFOUND;
            break;

        case DISPID_ACC_HELPTOPIC:
            if (wFlags == DISPATCH_PROPERTYGET)
                hr = get_accHelpTopic(&pDispParams->rgvarg[2].bstrVal, pDispParams->rgvarg[1], &pDispParams->rgvarg[0].lVal);
            else
                hr = DISP_E_MEMBERNOTFOUND;
            break;

        case DISPID_ACC_KEYBOARDSHORTCUT:
            if (wFlags == DISPATCH_PROPERTYGET)
                hr = get_accKeyboardShortcut(pDispParams->rgvarg[0], &pVarResult->bstrVal);
            else
                hr = DISP_E_MEMBERNOTFOUND;
            break;

        case DISPID_ACC_FOCUS:
            if (wFlags == DISPATCH_PROPERTYGET)
                hr = get_accFocus(pVarResult);
            else
                hr = DISP_E_MEMBERNOTFOUND;
            break;

        case DISPID_ACC_SELECTION:
            if (wFlags == DISPATCH_PROPERTYGET)
                hr = get_accSelection(pVarResult);
            else
                hr = DISP_E_MEMBERNOTFOUND;
            break;

        case DISPID_ACC_DEFAULTACTION:
            if (wFlags == DISPATCH_PROPERTYGET)
                hr = get_accDefaultAction(pDispParams->rgvarg[0], &pVarResult->bstrVal);
            else
                hr = DISP_E_MEMBERNOTFOUND;
            break;

        case DISPID_ACC_SELECT:
            if (wFlags == DISPATCH_METHOD)
                hr = accSelect(pDispParams->rgvarg[1].lVal, pDispParams->rgvarg[0]);
            else
                hr = DISP_E_MEMBERNOTFOUND;
            break;

        case DISPID_ACC_LOCATION:
            if (wFlags == DISPATCH_METHOD)
                hr = accLocation(&pDispParams->rgvarg[4].lVal, &pDispParams->rgvarg[3].lVal, &pDispParams->rgvarg[2].lVal, &pDispParams->rgvarg[1].lVal, pDispParams->rgvarg[0]);
            else
                hr = DISP_E_MEMBERNOTFOUND;
            break;

        case DISPID_ACC_NAVIGATE:
            if (wFlags == DISPATCH_METHOD)
                hr = accNavigate(pDispParams->rgvarg[1].lVal, pDispParams->rgvarg[0], pVarResult);
            else
                hr = DISP_E_MEMBERNOTFOUND;
            break;

        case DISPID_ACC_HITTEST:
            if (wFlags == DISPATCH_METHOD)
                hr = accHitTest(pDispParams->rgvarg[1].lVal, pDispParams->rgvarg[0].lVal, pVarResult);
            else
                hr = DISP_E_MEMBERNOTFOUND;
            break;

        case DISPID_ACC_DODEFAULTACTION:
            if (wFlags == DISPATCH_METHOD)
                hr = accDoDefaultAction(pDispParams->rgvarg[0]);
            else
                hr = DISP_E_MEMBERNOTFOUND;
            break;

        default:
            hr = DISP_E_MEMBERNOTFOUND;
            break;
    }

    if (!SUCCEEDED(hr)) {
        return hr;
    }
    return hr;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsAccessible::accHitTest(long xLeft, long yTop, VARIANT *pvarID)
{
    if (!accessible->isValid())
        return E_FAIL;

    int control = accessible->childAt(xLeft, yTop);
    if (control == -1) {
        (*pvarID).vt = VT_EMPTY;
        return S_FALSE;
    }
    DwAccessibleInterface *acc = 0;
    if (control)
        accessible->navigate(Child, control, &acc);
    if (!acc) {
        (*pvarID).vt = VT_I4;
        (*pvarID).lVal = control;
        return S_OK;
    }

    DwWindowsAccessible* wacc = new DwWindowsAccessible(acc);
    IDispatch *iface = 0;
    wacc->QueryInterface(IID_IDispatch, (void**)&iface);
    if (iface) {
        (*pvarID).vt = VT_DISPATCH;
        (*pvarID).pdispVal = iface;
        return S_OK;
    } else {
        delete wacc;
    }

    (*pvarID).vt = VT_EMPTY;
    return S_FALSE;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsAccessible::accLocation(long *pxLeft, long *pyTop, long *pcxWidth, long *pcyHeight, VARIANT varID)
{
    if (!accessible->isValid())
        return E_FAIL;

    DwRect rect = accessible->rect(varID.lVal);
    if (rect.isValid()) {
        *pxLeft = rect.x();
        *pyTop = rect.y();
        *pcxWidth = rect.width();
        *pcyHeight = rect.height();
    } else {
        *pxLeft = 0;
        *pyTop = 0;
        *pcxWidth = 0;
        *pcyHeight = 0;
    }
    return S_OK;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsAccessible::accNavigate(long navDir, VARIANT varStart, VARIANT *pvarEnd)
{
    if (!accessible->isValid())
        return E_FAIL;

    DwAccessibleInterface *acc = 0;
    int control = -1;
    switch(navDir) {
    case NAVDIR_FIRSTCHILD:
        control = accessible->navigate(Child, 1, &acc);
        break;
    case NAVDIR_LASTCHILD:
        control = accessible->navigate(Child, accessible->childCount(), &acc);
        break;
    case NAVDIR_NEXT:
    case NAVDIR_PREVIOUS:
        if (!varStart.lVal){
            DwAccessibleInterface *parent = 0;
            accessible->navigate(Ancestor, 1, &parent);
            if (parent) {
                int index = parent->indexOfChild(accessible);
                index += (navDir == NAVDIR_NEXT) ? 1 : -1;
                if (index > 0 && index <= parent->childCount())
                    control = parent->navigate(Child, index, &acc);
                delete parent;
            }
        } else {
            int index = varStart.lVal;
            index += (navDir == NAVDIR_NEXT) ? 1 : -1;
            if (index > 0 && index <= accessible->childCount())
                control = accessible->navigate(Child, index, &acc);
        }
        break;
    case NAVDIR_UP:
        control = accessible->navigate(Up, varStart.lVal, &acc);
        break;
    case NAVDIR_DOWN:
        control = accessible->navigate(Down, varStart.lVal, &acc);
        break;
    case NAVDIR_LEFT:
        control = accessible->navigate(Left, varStart.lVal, &acc);
        break;
    case NAVDIR_RIGHT:
        control = accessible->navigate(Right, varStart.lVal, &acc);
        break;
    default:
        break;
    }
    if (control == -1) {
        (*pvarEnd).vt = VT_EMPTY;
        return S_FALSE;
    }
    if (!acc) {
        (*pvarEnd).vt = VT_I4;
        (*pvarEnd).lVal = control;
        return S_OK;
    }

    DwWindowsAccessible* wacc = new DwWindowsAccessible(acc);

    IDispatch *iface = 0;
    wacc->QueryInterface(IID_IDispatch, (void**)&iface);
    if (iface) {
        (*pvarEnd).vt = VT_DISPATCH;
        (*pvarEnd).pdispVal = iface;
        return S_OK;
    } else {
        delete wacc;
    }

    (*pvarEnd).vt = VT_EMPTY;
    return S_FALSE;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsAccessible::get_accChild(VARIANT varChildID, IDispatch** ppdispChild)
{
    if (!accessible->isValid())
        return E_FAIL;

    if (varChildID.vt == VT_EMPTY)
        return E_INVALIDARG;

    DwAccessibleInterface *acc = 0;
    RelationFlag rel = varChildID.lVal ? Child : Self;
    accessible->navigate(rel, varChildID.lVal, &acc);

    if (acc) {
        DwWindowsAccessible* wacc = new DwWindowsAccessible(acc);
        wacc->QueryInterface(IID_IDispatch, (void**)ppdispChild);
        return S_OK;
    }

    *ppdispChild = 0;
    return S_FALSE;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsAccessible::get_accChildCount(long* pcountChildren)
{
    if (!accessible->isValid())
        return E_FAIL;

    *pcountChildren = accessible->childCount();
    return S_OK;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsAccessible::get_accParent(IDispatch** ppdispParent)
{
    if (!accessible->isValid())
        return E_FAIL;

    DwAccessibleInterface *acc = 0;
    accessible->navigate(Ancestor, 1, &acc);
    if (acc) {
        DwWindowsAccessible* wacc = new DwWindowsAccessible(acc);
        wacc->QueryInterface(IID_IDispatch, (void**)ppdispParent);

        if (*ppdispParent)
            return S_OK;
    }

    *ppdispParent = 0;
    return S_FALSE;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsAccessible::accDoDefaultAction(VARIANT varID)
{
    if (!accessible->isValid())
        return E_FAIL;

    return accessible->doAction(DefaultAction, varID.lVal, DwVariantList()) ? S_OK : S_FALSE;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsAccessible::get_accDefaultAction(VARIANT varID, BSTR* pszDefaultAction)
{
    if (!accessible->isValid())
        return E_FAIL;

    DwString def = accessible->actionText(DefaultAction, Name, varID.lVal);
    if (def.isEmpty()) {
        *pszDefaultAction = 0;
        return S_FALSE;
    }

    *pszDefaultAction = DwStringToBSTR(def);
    return S_OK;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsAccessible::get_accDescription(VARIANT varID, BSTR* pszDescription)
{
    if (!accessible->isValid())
        return E_FAIL;

    DwString descr = accessible->text(Description, varID.lVal);
    if (descr.size()) {
        *pszDescription = DwStringToBSTR(descr);
        return S_OK;
    }

    *pszDescription = 0;
    return S_FALSE;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsAccessible::get_accHelp(VARIANT varID, BSTR *pszHelp)
{
    if (!accessible->isValid())
        return E_FAIL;

    DwString help = accessible->text(Help, varID.lVal);
    if (help.size()) {
        *pszHelp = DwStringToBSTR(help);
        return S_OK;
    }

    *pszHelp = 0;
    return S_FALSE;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsAccessible::get_accHelpTopic(BSTR *, VARIANT, long *)
{
    return DISP_E_MEMBERNOTFOUND;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsAccessible::get_accKeyboardShortcut(VARIANT varID, BSTR *pszKeyboardShortcut)
{
    if (!accessible->isValid())
        return E_FAIL;

    DwString sc = accessible->text(Accelerator, varID.lVal);
    if (sc.size()) {
        *pszKeyboardShortcut = DwStringToBSTR(sc);
        return S_OK;
    }

    *pszKeyboardShortcut = 0;
    return S_FALSE;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsAccessible::get_accName(VARIANT varID, BSTR* pszName)
{
    if (!accessible->isValid())
        return E_FAIL;

    DwString n = accessible->text(Name, varID.lVal);
    if (n.size()) {
        *pszName = DwStringToBSTR(n);
        return S_OK;
    }

    *pszName = 0;
    return S_FALSE;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsAccessible::put_accName(VARIANT, BSTR)
{
    return DISP_E_MEMBERNOTFOUND;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsAccessible::get_accRole(VARIANT varID, VARIANT *pvarRole)
{
    if (!accessible->isValid())
        return E_FAIL;

    Role role = accessible->role(varID.lVal);
    if (role != NoRole) {
        if (role == LayeredPane)
            role = DwAccessible::Pane;
        (*pvarRole).vt = VT_I4;
        (*pvarRole).lVal = role;
    } else {
        (*pvarRole).vt = VT_EMPTY;
    }
    return S_OK;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsAccessible::get_accState(VARIANT varID, VARIANT *pvarState)
{
    if (!accessible->isValid())
        return E_FAIL;

    (*pvarState).vt = VT_I4;
    (*pvarState).lVal = accessible->state(varID.lVal);
    return S_OK;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsAccessible::get_accValue(VARIANT varID, BSTR* pszValue)
{
    if (!accessible->isValid())
        return E_FAIL;

    DwString value = accessible->text(Value, varID.lVal);
    if (!value.isNull()) {
        *pszValue = DwStringToBSTR(value);
        return S_OK;
    }

    *pszValue = 0;
    return S_FALSE;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsAccessible::put_accValue(VARIANT, BSTR)
{
    return DISP_E_MEMBERNOTFOUND;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsAccessible::accSelect(long flagsSelect, VARIANT varID)
{
    if (!accessible->isValid())
        return E_FAIL;

    bool res = false;

    if (flagsSelect & SELFLAG_TAKEFOCUS)
        res = accessible->doAction(SetFocus, varID.lVal, DwVariantList());
    if (flagsSelect & SELFLAG_TAKESELECTION) {
        accessible->doAction(ClearSelection, 0, DwVariantList());
        res = accessible->doAction(AddToSelection, varID.lVal, DwVariantList());
    }
    if (flagsSelect & SELFLAG_EXTENDSELECTION)
        res = accessible->doAction(ExtendSelection, varID.lVal, DwVariantList());
    if (flagsSelect & SELFLAG_ADDSELECTION)
        res = accessible->doAction(AddToSelection, varID.lVal, DwVariantList());
    if (flagsSelect & SELFLAG_REMOVESELECTION)
        res = accessible->doAction(RemoveSelection, varID.lVal, DwVariantList());

    return res ? S_OK : S_FALSE;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsAccessible::get_accFocus(VARIANT *pvarID)
{
    if (!accessible->isValid())
        return E_FAIL;

    DwAccessibleInterface *acc = 0;
    int control = accessible->navigate(FocusChild, 1, &acc);
    if (control == -1) {
        (*pvarID).vt = VT_EMPTY;
        return S_FALSE;
    }
    if (!acc || control == 0) {
        (*pvarID).vt = VT_I4;
        (*pvarID).lVal = control ? control : CHILDID_SELF;
        return S_OK;
    }

    DwWindowsAccessible* wacc = new DwWindowsAccessible(acc);
    IDispatch *iface = 0;
    wacc->QueryInterface(IID_IDispatch, (void**)&iface);
    if (iface) {
        (*pvarID).vt = VT_DISPATCH;
        (*pvarID).pdispVal = iface;
        return S_OK;
    } else {
        delete wacc;
    }

    (*pvarID).vt = VT_EMPTY;
    return S_FALSE;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsAccessible::get_accSelection(VARIANT *pvarChildren)
{
    if (!accessible->isValid())
        return E_FAIL;

    int cc = accessible->childCount();
    DwVector<int> sel;
    sel.resize(cc);
    int selIndex = 0;
    for (int i = 1; i <= cc; ++i) {
        DwAccessibleInterface *child = 0;
        int i2 = accessible->navigate(Child, i, &child);
        bool isSelected = false;
        if (child) {
            isSelected = !!(child->state(0) & Selected);
            delete child;
            child = 0;
        } else {
            isSelected = !!(accessible->state(i2) & Selected);
        }
        if (isSelected)
            sel[selIndex++] = i;
    }
    sel.resize(selIndex);
    if (sel.isEmpty()) {
        (*pvarChildren).vt = VT_EMPTY;
        return S_FALSE;
    }
    if (sel.size() == 1) {
        (*pvarChildren).vt = VT_I4;
        (*pvarChildren).lVal = sel[0];
        return S_OK;
    }
    IEnumVARIANT *iface = new DwWindowsEnumerate(sel);
    IUnknown *uiface;
    iface->QueryInterface(IID_IUnknown, (void**)&uiface);
    (*pvarChildren).vt = VT_UNKNOWN;
    (*pvarChildren).punkVal = uiface;

    return S_OK;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsAccessible::GetWindow(HWND *phwnd)
{
    *phwnd = 0;
    if (!accessible->isValid())
        return E_UNEXPECTED;

    DwFrameWindow *o = dwsafe_cast<DwFrameWindow>(accessible->object());
    if (!o)
        return E_FAIL;

    *phwnd = (HWND)(o->winId());
    return S_OK;
}

//------------------------------------------------------------------------------
/**
*/
HRESULT STDMETHODCALLTYPE DwWindowsAccessible::ContextSensitiveHelp(BOOL)
{
    return S_OK;
}

DW_IMPLEMENT_ABSTRACT_CLASS(DwAccessibleInterface, 'AICE', DwAccessible);