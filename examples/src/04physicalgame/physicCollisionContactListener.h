#ifndef _COLLISIONCONTACTLISTENER_H_
#define _COLLISIONCONTACTLISTENER_H_
//by eagle

#include <Box2D/Box2D.h>
#include "dwcore/dwvector.h"

#include "gameState.h"

class CollisionContactListener : public b2ContactListener
{
public:
	CollisionContactListener();
	~CollisionContactListener(){};

	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);

	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

	void ouputDebugInformation(const GameState::ActorType bodyActor);

	//the pig is visible or invisible.
	DwVector<GameState::PigState> m_visiblePigMark;
	//the coin is visible or invisible.
	GameState::CoinState m_visibleCoinMark[3];
	//accelerate the redBird.
	bool m_isAccelerate;
};



#endif