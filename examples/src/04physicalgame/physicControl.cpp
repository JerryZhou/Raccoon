#include "physicControl.h"
#include <string>
#include "dwcore/dwstring.h"

#include "actor.h"
#include "gameState.h"
#include "physicCollisionContactListener.h"

//-----------------------------------------------------------------
PhysicControl::PhysicControl()
{
	//create the world
	initPhysicWorld();
	//create the collision contact listener
	createCollisionContactListener();
	//create the wall
	createWall();

}
//-----------------------------------------------------------------
PhysicControl::~PhysicControl()
{
	//delete the world
	delete m_world;
	m_world = NULL;
}



//-----------------------------------------------------------------
void PhysicControl::initPhysicWorld()
{
	//1: create the world.
	b2Vec2  gravity(0.0f,-9.8f);
	m_world = new b2World(gravity);
	//2: define the ground body.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(32.0,-50.0f);
	//user data, we assign name to it
	GameActor* name = new GameActor;
	name->m_actorType = GameState::Ground;
	name->m_mark = 0;
	groundBodyDef.userData = name;
	m_groundBody = m_world->CreateBody(&groundBodyDef);
	// Define the ground box shape.
	b2PolygonShape groundBox;
	groundBox.SetAsBox(30.0f, 2.0f);
	m_groundBody->CreateFixture(&groundBox, 0.0f);
}

//-----------------------------------------------------------------
void PhysicControl::createWall()
{
	//create the wall
	b2Vec2 v[4];
	v[0].Set(-51.2f,-38.4f);
	v[1].Set(51.2f,-38.4f);
	v[2].Set(51.2f,38.4f);
	v[3].Set(-51.2f,38.4f);
	b2ChainShape chain;
	chain.CreateLoop(v,4);

	b2BodyDef wallBodyDef;
	wallBodyDef.position.Set(51.2f,38.4f);
	GameActor* name = new GameActor;
	name->m_actorType = GameState::Wall;
	name->m_mark = 0;
	wallBodyDef.userData = name;

	b2Body* wallBody;
	wallBody = m_world->CreateBody(&wallBodyDef);

	b2FixtureDef fixture;
	fixture.shape = &chain;
	fixture.density = 0.0f;
	fixture.restitution = 0.6f;
	//define the collision filter
	fixture.filter.categoryBits = 0x002;
	fixture.filter.maskBits = 0x002;
	wallBody->CreateFixture(&fixture);
}
//-----------------------------------------------------------------
void PhysicControl::createCollisionContactListener()
{
	//3: create the contact listener
	m_contactListener = new CollisionContactListener();
	m_contactListener->m_isAccelerate = false;
	m_world->SetContactListener(m_contactListener);
}
//-----------------------------------------------------------------
void PhysicControl::Step()
{
	//simulate
	float32 timeStep = 1.0f / 30.f;
	int32   velocityIterations = 10;
	int32   positioniterations = 8;
	m_world->Step(timeStep,velocityIterations,positioniterations);
	m_world->ClearForces();
}