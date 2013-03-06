#ifndef _PHYSICCONTROL_H_
#define _PHYSICCONTROL_H_
//by eagle

#include <Box2D/Box2D.h>

class CollisionContactListener;

class PhysicControl
{
public:
	PhysicControl();
	~PhysicControl();

	void initPhysicWorld();
	void createWall();

	void Step();

	b2World* getWorld(){return m_world;}

	//collision
	void createCollisionContactListener();
	CollisionContactListener* getContactListener(){return m_contactListener;}

protected:
	b2World* m_world;
	b2Body* m_groundBody;
	CollisionContactListener* m_contactListener;

};
#endif