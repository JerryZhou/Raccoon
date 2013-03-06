#ifndef _ACTOR_H_
#define _ACTOR_H_
//eagle

#include "gameState.h"


class GameActor
{
public:
	GameState::ActorType m_actorType;
	int                  m_mark;
};

#endif