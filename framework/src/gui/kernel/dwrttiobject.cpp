#include "stable.h"
#include "dwrttiobject.h"
#include "dwrttiobjectptr.h"
#include "dwcore/dwobject.h"
#include "dwgui/dwtimer.h"
#include "dwcore/dwglobalstatic.h"
#include "dwcore/dwlocks.h"
#include "dwfactory.h"

// Used to delete later
//------------------------------------------------------------------------------
struct DwDeleteLaterLib : public sigslot::has_slots
{
    DwVector<DwRttiObjectPtr<DwRttiObject> > deleteLaterObjects;
    DwTimer deleteLaterTimer;
    DwCriticalSection criticalSection;

    DwDeleteLaterLib()
    {
        deleteLaterTimer.sigTimeout.connect(this, &DwDeleteLaterLib::onDeleteEvent);
        deleteLaterTimer.start(0);
    }

    ~DwDeleteLaterLib()
    {
        DwCSLocker locks(&criticalSection);
        if( deleteLaterObjects.size() > 0)
        {
            onDeleteEvent();
        }
        deleteLaterTimer.sigTimeout.disconnect_all();
        deleteLaterTimer.stop();
    }

    void deleteThis(DwRttiObject* obj)
    {
        DwCSLocker locks(&criticalSection);

        deleteLaterObjects.append(DwRttiObjectPtr<DwRttiObject>(obj));
        if (!deleteLaterTimer.isActive())
        {
            deleteLaterTimer.start(16);
        }
    }

    void onDeleteEvent()
    {
        DwCSLocker locks(&criticalSection);

        DwVector<DwRttiObjectPtr<DwRttiObject> > toDeleteObjects = deleteLaterObjects;
        deleteLaterObjects.clear();
        for (int i=0; i<toDeleteObjects.size(); ++i)
        {
            DwRttiObjectPtr<DwRttiObject> obj = toDeleteObjects.at(i);
            if (!obj.isNull())
            {
                delete obj;
            }
        }
        toDeleteObjects.clear();

        if (deleteLaterObjects.size() == 0)
        {
            deleteLaterTimer.stop();
        }
    }

    void onAppQuit()
    {
        DwCSLocker locks(&criticalSection);

        if(deleteLaterTimer.isActive())
        {
            deleteLaterTimer.stop();
        }

        // need to execute the roll deleteLater
        while (deleteLaterObjects.size() > 0)
        {
            onDeleteEvent();
        }

        DW_ASSERT(deleteLaterObjects.size() == 0 && "THERE IS DELTET LATER EVENT IN APP QUIT");
    }
};
DW_GLOBAL_STATIC(DwDeleteLaterLib, _dw_deleteLaterLib);

// used to dump current objects info(check objects memory leak....)
//------------------------------------------------------------------------------
struct DFourCC
{
    DFourCC(int c)
    {
        fourcc = c;
    }

    union
    {
        int fourcc;
        struct
        {
            char revision;
            char build;
            char minor;
            char major;
        };
    };
};

//------------------------------------------------------------------------------
/**
*/
template<>
DwDebug &DwDebug::operator<< <DFourCC>(const DFourCC &t)
{
    if (t.fourcc)
    {
        m_stream << t.major << t.minor << t.build << t.revision << KSpace;
    }
    else
    {
        m_stream << "NULL";
    }
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
static  uint32_t dwRoundUp4(uint32_t val)
{
    return ((uint32_t)val + (4 - 1)) & ~(4 - 1);
}

//------------------------------------------------------------------------------
struct DwDumpObjectsLib
{
    DwCriticalSection criticalSection;
    DwMap<DwRttiObject *, bool> dumpObjets;

    DwDumpObjectsLib()
    {
        ;
    }
    ~DwDumpObjectsLib()
    {
        DW_ASSERT(dumpObjets.count() == 0);
    }

    void addObj(DwRttiObject *obj)
    {
        DwCSLocker locks(&criticalSection);

        dumpObjets.insert(obj, true);
    }

    void removeObj(DwRttiObject *obj)
    {
        DwCSLocker locks(&criticalSection);

        dumpObjets.remove(obj);
    }

    void dumpObjs()const
    {
        DwCSLocker locks((DwCriticalSection*)&criticalSection);

        {
            DwDebug() << L"*************************************************************************";
            DwDebug() << L"*******************************DUMP OBJECTS******************************";
            DwDebug() << L"RttiObjects Total Num : " << dumpObjets.count();
        }

        uint32_t memorySize = 0;
        DwMap<DwRtti *, DwVector<DwRttiObject *> > objClassLib;
        DwMap<DwRttiObject *, bool>::const_iterator ite = dumpObjets.begin();
        while (ite != dumpObjets.end())
        {
            DwRttiObject *obj = ((*ite).first);
            DwRtti *rtti = obj->rtti();
            objClassLib[rtti].append(obj);
            ++ite;

            memorySize += dwRoundUp4(rtti->instanceSize());
        }
        {
            DwDebug() << L"RttiObjects Total Memory Size : " << memorySize << "Byte" << DwReal(1.0 * memorySize / 1024) << "KB";
        }

        DwMap<DwRtti *, DwVector<DwRttiObject *> >::const_iterator iteObjs = objClassLib.begin();
        while (iteObjs != objClassLib.end())
        {
            DwRtti *rtti = ((*iteObjs).first);
            const DwVector<DwRttiObject *>& objs = ((*iteObjs).second);

            {
                DwDebug() << "ClassName: " << rtti->name() <<" Class Fourcc: " << DFourCC(rtti->fourCC()) << "\nInstance Size" << rtti->instanceSize()
                          << " Objects Num: " << objs.size();
            }
            for (int i = 0; i<objs.size(); ++i)
            {
                objs.at(i)->debugString(DwDebug());
            }
            ++iteObjs;
        }

        {
            DwDebug() << L"****************************END DUMP OBJECTS****************************";
            DwDebug() << L"************************************************************************\n";
        }
    }

    DwVector<DwRttiObject *> dumpObjsOf(DwRtti *genre)
    {
        DwCSLocker locks(&criticalSection);

        if (!genre)
        {
            dumpObjs();
            return DwVector<DwRttiObject *>();
        }

        {
            DwDebug() << L"*********************************************************************";
            DwDebug() << L"*****************************DUMP OBJECTS****************************";
            DwDebug() << L"RttiObjects Total Num : " << dumpObjets.count();
        }

        uint32_t memorySize = 0;
        uint32_t instanceSize = dwRoundUp4(genre->instanceSize());

        DwVector<DwRttiObject *> objs;
        DwMap<DwRttiObject *, bool>::const_iterator ite = dumpObjets.begin();
        while (ite != dumpObjets.end())
        {
            DwRttiObject *obj = ((*ite).first);
            DwRtti *rtti = obj->rtti();
            if (rtti == genre)
            {
                memorySize += instanceSize;
                objs.append(obj);
            }
            ++ite;
        }

        DwRtti *rtti = genre;
        {
            DwDebug() << "ClassName: " << rtti->name() <<" Class Fourcc: " << DFourCC(rtti->fourCC()) << "\nInstance Size" << rtti->instanceSize()
                      << " Objects Num: " << objs.size();
        }
        {
            DwDebug() << L"RttiObjects Total Memory Size : " << memorySize << "Byte" << DwReal(1.0 * memorySize / 1024) << "KB";
        }
        for (int i = 0; i<objs.size(); ++i)
        {
            objs.at(i)->debugString(DwDebug());
        }

        {
            DwDebug() << L"****************************END DUMP OBJECTS****************************";
            DwDebug() << L"************************************************************************\n";
        }
        return objs;
    }
};
DW_GLOBAL_STATIC(DwDumpObjectsLib, _dw_dumpObjectsLib);

//------------------------------------------------------------------------------
DW_IMPLEMENT_ROOT_CLASS(DwRttiObject, 'DROT');

//------------------------------------------------------------------------------
/**
*/
DwRttiObject::DwRttiObject()
{
    // to to order the destruction sequence
    DwTHolderCenter<DwRttiObject>::Instance();

#ifdef _DEBUG
    _dw_dumpObjectsLib()->addObj(this);
#endif
}

//------------------------------------------------------------------------------
/**
*/
DwRttiObject::DwRttiObject(const DwRttiObject& other)
{
    DW_UNUSED(other);
#ifdef _DEBUG
    _dw_dumpObjectsLib()->addObj(this);
#endif
}

//------------------------------------------------------------------------------
/**
*/
DwRttiObject::~DwRttiObject()
{
    DwTHolderCenter<DwRttiObject>::Instance()->destroyHolder(this);

#ifdef _DEBUG
    _dw_dumpObjectsLib()->removeObj(this);
#endif
}

//------------------------------------------------------------------------------
/**
*/
void DwRttiObject::debugString(DwDebug &stream)const
{
    stream <<" " <<this->rtti()->name() << ": " << (int)(this);
}

//------------------------------------------------------------------------------
/**
*/
void DwRttiObject::deleteLater()
{
    _dw_deleteLaterLib()->deleteThis(this);
}

//------------------------------------------------------------------------------
/**
*/
bool DwRttiObject::isA(const DwRtti *genre) const
{
    return this->rtti()->isDerivedFrom(genre);
}

//------------------------------------------------------------------------------
/**
*/
bool DwRttiObject::isA(const DwRtti &genre) const
{
    return isA(&genre);
}

//------------------------------------------------------------------------------
/**
*/
bool DwRttiObject::isA(const DwString &genre) const
{
    const DwRtti *rtti = dwFactory()->classRttiOf(genre);
    return isA(rtti);
}

//------------------------------------------------------------------------------
/**
*/
bool DwRttiObject::isA(const DwFourCC &genre) const
{
    const DwRtti *rtti = dwFactory()->classRttiOf(genre);
    return isA(rtti);
}

//------------------------------------------------------------------------------
/**
*/
void DwRttiObject::dumpObjects()
{
#ifdef _DEBUG
    _dw_dumpObjectsLib()->dumpObjs();
#endif
}

//------------------------------------------------------------------------------
/**
*/
DwVector<DwRttiObject *> DwRttiObject::dumpObjectsOf(DwRtti *genre)
{
#ifdef _DEBUG
    return _dw_dumpObjectsLib()->dumpObjsOf(genre);
#else
    DW_UNUSED(genre);
    return DwVector<DwRttiObject *>();
#endif
}

//------------------------------------------------------------------------------
/**
*/
void DwRttiObject::initRttiObjects()
{
    static bool dw_holder_exits = false;
    //------------------------------------------------------------------------------
    // ensure the destruct order
    if (!dw_holder_exits)
    {
        dw_holder_exits = DwTHolderCenter<DwRttiObject>::Instance() != NULL;
        bool dw_deleteLater_exits = _dw_deleteLaterLib() != NULL;
        DW_UNUSED(dw_holder_exits);
        DW_UNUSED(dw_deleteLater_exits);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwRttiObject::unInitRttiObjects()
{
    _dw_deleteLaterLib()->onAppQuit();
}
