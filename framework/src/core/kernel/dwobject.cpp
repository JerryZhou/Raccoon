#include "stable.h"

#include "dwglobalstatic.h"
#include "eastl/hash_map.h"
#include <dwcore/dwmap.h>
#include "dwobject.h"

typedef eastl::vector<DwObject**> DwGuardList;
typedef eastl::hash_map< DwObject*, DwGuardList* > DwGuardHash;
DW_GLOBAL_STATIC(DwGuardHash, guardHash);

static DwGuardList *getGuardList(DwObject *p)
{
    DwGuardHash *hash = guardHash();
    DwGuardHash::iterator it = hash->find(p);
    DwGuardList *guardList = NULL;
    if(it == hash->end())
    {
        guardList = new DwGuardList();
        (*hash)[p] = guardList;
    }
    else
    {
        guardList = it->second;
    }

    return guardList;
}

static void insertGuardList(DwGuardList *guardList, DwObject **ptr)
{
    int count = guardList->size();
    for(int i = 0; i < count; i++)
    {
        if((*guardList)[i] == NULL)
        {
            (*guardList)[i] = ptr;
            return;
        }
    }

    guardList->push_back(ptr);
}

static void clearGuardList(DwGuardList *guardList)
{
    int count = guardList->size();
    for(int i = 0; i < count; i++)
    {
        DwObject **p = (*guardList)[i];
        if(p)
        {
            *p = NULL;
        }
        (*guardList)[i] = NULL;
    }
}

static void removeGuardList(DwGuardList *guardList, DwObject **ptr)
{
    int count = guardList->size();
    for(int i = 0; i < count; i++)
    {
        if(ptr == (*guardList)[i])
        {
            (*guardList)[i] = NULL;
            return;
        }
    }
}

DwObject::DwObject(DwObject *parent)
    : m_parent(NULL)
{
    setParent(parent);

    //keep destruct dependence is right
    guardHash();
}

DwObject::~DwObject()
{
    clearGuard(this);
    int count = childCount();
    for(int i = 0; i < count; i++)
    {
        DwObject *p = m_children[i];
        p->m_parent = NULL;
        m_children[i] = NULL;
        delete p;
    }
    m_children.clear();

    setParent(NULL);
}

DwString DwObject::objectName() const
{
    return m_objectName;
}

void DwObject::setObjectName( const DwString &name )
{
    m_objectName = name;
}

bool DwObject::operator==( const DwObject &other ) const
{
    return &other == this;
}

void DwObject::setParent(DwObject *p)
{
    if(p == this) return;
    if(p == m_parent) return;
    if (m_parent)
    {
        m_parent->m_children.removeOne(this);
        m_parent = NULL;
    }

    if (p)
    {
        m_parent = p;
        p->m_children.push_back(this);
    }
}

int DwObject::childCount() const
{
    return m_children.size();
}

DwObject * DwObject::child( int i ) const
{
    return m_children[i];
}

void DwObject::addGuard( DwObject **ptr )
{
    DwGuardList *guardList = getGuardList(*ptr);
    insertGuardList(guardList, ptr);
}

void DwObject::removeGuard( DwObject **ptr )
{
    DwGuardList *guardList = getGuardList(*ptr);

    removeGuardList(guardList, ptr);
}

void DwObject::clearGuard(DwObject *ptr)
{
    DwGuardHash *hash = guardHash();
    DwGuardHash::iterator it = hash->find(ptr);
    if(it != hash->end())
    {
        DwGuardList *guardList = it->second;
        clearGuardList(guardList);
        delete guardList;
        guardHash()->erase(it);
    }
}

void DwObject::changeGuard( DwObject **ptr, DwObject *o )
{
    if(o)
    {
        DwGuardList *guardList = getGuardList(o);
        insertGuardList(guardList, ptr);
    }

    if(*ptr)
    {
        DwGuardList *guardList = getGuardList(*ptr);
        removeGuardList(guardList, ptr);
    }
}
