#include "stable.h"
#include "dwgraphicsscenelistener.h"
#include "dwgraphicsscene.h"
#include "dwgraphicsitem.h"
#include "dwdummys.h"

//------------------------------------------------------------------------------
/**
*/
DwGraphicsSceneListener::DwGraphicsSceneListener()
{
    ::memset((void*)m_listers, 0, sizeof(m_listers));
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsSceneListener::~DwGraphicsSceneListener()
{
    ::memset((void*)m_listers, 0, sizeof(m_listers));
    DW_ASSERT(m_depends.size() == 0 && "SHOULD CALL remAllDepends() BEFORE DESTRUCTOR");
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSceneListener::addDepend(DwGraphicsScene *scene)
{
    DW_ASSERT(scene);
    DW_ASSERT(m_depends.indexOf(scene) == InvalidIndex);
    scene->attachListener(this);
    m_depends.append(scene);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSceneListener::remDepend(DwGraphicsScene *scene)
{
    DW_ASSERT(scene);
    DW_ASSERT(m_depends.indexOf(scene) != InvalidIndex);
    scene->detachListener(this);
    m_depends.removeOne(scene);
}

//------------------------------------------------------------------------------
/**
*/
bool DwGraphicsSceneListener::hasDepend(DwGraphicsScene *scene)
{
    return m_depends.indexOf(scene) != InvalidIndex;
}

//------------------------------------------------------------------------------
/**
*/
int DwGraphicsSceneListener::dependSize() const
{
    return m_depends.size();
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsScene *DwGraphicsSceneListener::dependAt(int index) const
{
    DW_ASSERT(index >=0 && index < m_depends.size());
    return m_depends.at(index);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSceneListener::remAllDepends()
{
    DwVector<DwGraphicsScene *> depends = m_depends;
    for (int i=0; i<depends.size(); ++i)
    {
        remDepend(depends.at(i));
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSceneListener::addNotifyLister(Dw::GraphicsSceneNotify notify, pfnSceneLister lister)
{
    DW_ASSERT(notify >= Dw::SceneNotifyMin && notify < Dw::SceneNotifyMax);
    m_listers[notify] = lister;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSceneListener::removeNotifyLister(Dw::GraphicsSceneNotify notify)
{
    addNotifyLister(notify,  NULL);
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsSceneListener::sceneNotify(Dw::GraphicsSceneNotify notify, DwGraphicsScene* scene, DwGraphicsItem* item)
{
    DW_ASSERT(notify >= Dw::SceneNotifyMin && notify < Dw::SceneNotifyMax);
    if (m_listers[notify])
    {
        (m_listers[notify])(scene, item);
    }

    onNotify(notify, scene, item);
}