#ifndef _MAINFRAMEWINDOW_H_
#define _MAINFRAMEWINDOW_H_
//by eagle

#include "dwgui/dwmainwindow.h"

#include "physicControl.h"
#include "menuStart.h"
#include "actorItem.h"
//#include "testGoldCoins.h"

class MainFrameWindow : public DwMainWindow
{
	DW_DECLARE_CLASS(MainFrameWindow);
public:
	MainFrameWindow(DwObject *parent = NULL);
	~MainFrameWindow();

	void initScene();
	bool createScene();
	void updateScene();

	void createCoin(const b2Vec2 pos);
	
	void updateActorItem(ActorItem *actorItem,b2Body* actorBody);
	void updateMenuStart(b2Body* actorBody);

	void timeStep();

    void onKeyDownEvent(DwRttiObject *item, DwEvent *evt);
    void onKeyUpEvent(DwRttiObject *item, DwEvent *evt);
    void onMouseClickedEvent(DwRttiObject *item, DwEvent *evt);

protected:
	DwVector<ActorItem *> m_pigsActorItem;
	DwVector<ActorItem *> m_coinsActorItem;
	ActorItem*            m_redBirdActorItem;
//	DwVector<TestGoldCoins> m_coins;

	PhysicControl* m_physicControl;
	MenuStart*     m_MenuStart;
	bool           m_hasCreatedScene;

	int m_coinScore;
};


#endif