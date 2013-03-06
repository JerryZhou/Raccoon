#pragma once
#include "dwcore/dwstring.h"
#include "dwnamespace.h"

//------------------------------------------------------------------------------
class DwGraphicsScene;
class DwGraphicsItem;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwGraphicsSceneListener
{
public:
    typedef void (*pfnSceneLister)(DwGraphicsScene* scene, DwGraphicsItem* item);

    DwGraphicsSceneListener();
    virtual ~DwGraphicsSceneListener();

    // virtual declarative interface
    virtual void onAdded(DwGraphicsScene *scene) = 0;
    virtual void onRemoved(DwGraphicsScene *scene) = 0;

    virtual void onNotify(Dw::GraphicsSceneNotify notify, DwGraphicsScene *scene, DwGraphicsItem* item)  = 0;

    // depends
    void addDepend(DwGraphicsScene *scene);
    void remDepend(DwGraphicsScene *scene);
    bool hasDepend(DwGraphicsScene *scene);
    int dependSize() const;
    DwGraphicsScene *dependAt(int index) const;
    void remAllDepends();

    // responds
    void addNotifyLister(Dw::GraphicsSceneNotify notify, pfnSceneLister lister);
    void removeNotifyLister(Dw::GraphicsSceneNotify notify);

protected:
    void sceneNotify(Dw::GraphicsSceneNotify notify, DwGraphicsScene *scene, DwGraphicsItem* item) ;

private:
    friend class DwGraphicsScene;
    pfnSceneLister m_listers[Dw::SceneNotifyMax];
    DwVector<DwGraphicsScene *> m_depends;
};// end of DwGraphicsSceneListener
