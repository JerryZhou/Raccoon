#ifndef _ACTORITEM_H_
#define _ACTORITEM_H_
//by eagle

#include "dwgui/dwframewindow.h"
#include "dwgui/dwimagewidget.h"
#include "actor.h"
#include "gameState.h"
#include "physicControl.h"

class ActorItem
{
public:
	ActorItem(GameState::ActorType actorType, int mark);
	~ActorItem();

	/**Create imageItem(one imageItem corresponds to one physics body)**/
	void createGraphicItem(DwFrameWindow* parent);
	DwImageWidget* getGraphicItem(){ return m_widgetItem; }

	/**Create physics body(one imageItem corresponds to one physics body)**/
	void createPhysicsBody(PhysicControl* physicControl, b2Vec2 pos);
	void createRedBirdBody(b2World* physicWorld, b2Vec2 pos);
	void createPigBody(b2World* physicWorld, b2Vec2 pos);
	void createBlackBirdBody(b2World* physicWorld, b2Vec2 pos);
	void createCoinBody(b2World* physicWorld, b2Vec2 pos);

	/**Assign a name to imageItem and physics body(one imageItem corresponds to one physics body)**/
	void setGameActor(GameState::ActorType actorType, int mark);
	GameActor getGameActor() { return m_gameActor ;}

	/**define the item has been linked to physical body**/
	void setItemHasBeenPhysical(bool flag) { m_ItemHasBeenPhysical = flag; }
	bool getItemHasBeenPhysical() { return m_ItemHasBeenPhysical; }

protected:
	DwImageWidget* m_widgetItem;
	GameActor m_gameActor;

	bool     m_ItemHasBeenPhysical;
};



#endif