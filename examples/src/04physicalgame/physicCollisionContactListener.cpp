#include "stable.h"
#include <xstring>
#include "physicCollisionContactListener.h"

#include "actor.h"

//-----------------------------------------------------------------
CollisionContactListener::CollisionContactListener()
{
	for(int i = 0 ;i < 3; i++)
		m_visibleCoinMark[i] = GameState::InvisibleCoin;
}
//-----------------------------------------------------------------
void CollisionContactListener::BeginContact(b2Contact *)
{

}
//-----------------------------------------------------------------
void CollisionContactListener::EndContact(b2Contact* contact)
{
	//get the name of two fixtures
	GameActor* BodyA((GameActor*)contact->GetFixtureA()->GetBody()->GetUserData());
	GameActor* BodyB((GameActor*)contact->GetFixtureB()->GetBody()->GetUserData());

	//collision between pigs and bird
	if (BodyA->m_actorType == GameState::Pig && BodyB->m_actorType == GameState::RedBird)
	{
		m_visiblePigMark[BodyA->m_mark] = GameState::Invisible;

	}else if (BodyA->m_actorType == GameState::RedBird && BodyB->m_actorType == GameState::Pig)
	{
		m_visiblePigMark[BodyB->m_mark] = GameState::Invisible;
	}


	//reevaluate the bird
	if ( ( BodyA->m_actorType == GameState::RedBird && BodyB->m_actorType == GameState::BlackBird ) || 
		 ( BodyA->m_actorType == GameState::BlackBird && BodyB->m_actorType == GameState::RedBird ) )
	{
		m_isAccelerate = true;
	}

	//add score
	if ( BodyA->m_actorType == GameState::Coin &&  BodyB->m_actorType == GameState::BlackBird )
	{
		m_visibleCoinMark[BodyA->m_mark] = GameState::InvisibleCoin;

	}else if ( BodyA->m_actorType == GameState::BlackBird &&  BodyB->m_actorType == GameState::Coin )
	{
		m_visibleCoinMark[BodyB->m_mark] = GameState::InvisibleCoin;
	}

	OutputDebugString(std::wstring(L"BodyA Type:").c_str());
	ouputDebugInformation(BodyA->m_actorType);
	OutputDebugString(std::wstring(L"BodyB Type:").c_str());
	ouputDebugInformation(BodyB->m_actorType);
}

//----------------------------------------------------------
void CollisionContactListener::ouputDebugInformation(const GameState::ActorType bodyActor)
{
	std::wstring name;
	switch(bodyActor)
	{
		case GameState::Ground :
			 name = L"Ground";
			 break;
		case GameState::Wall :
			name = L"Wall";
			break;
		case GameState::Coin :
			name = L"Coin";
			break;
		case GameState::Pig :
			name = L"Pig";
			break;
		case GameState::RedBird :
			name = L"RedBird";
			break;
		case GameState::BlackBird :
			name = L"BlackBird";
			break;
		default:
			name = L"No collision";
			break;
	}
	OutputDebugString(name.c_str());
	OutputDebugString(std::wstring(L"\n").c_str());
}
//----------------------------------------------------------
void CollisionContactListener::PostSolve(b2Contact* , const b2ContactImpulse* )
{

}

//----------------------------------------------------------
void CollisionContactListener::PreSolve(b2Contact* , const b2Manifold* )
{

}