#include "actorItem.h"

//-----------------------------------------------------------------
ActorItem::ActorItem(GameState::ActorType actorType, int mark)
: m_widgetItem(NULL)
{
	setGameActor(actorType,mark);
	setItemHasBeenPhysical(false);
}
//-----------------------------------------------------------------
ActorItem::~ActorItem()
{
	if (m_widgetItem)
		delete m_widgetItem;

}
//-----------------------------------------------------------------
void ActorItem::createGraphicItem(DwFrameWindow* parent)
{
	//Create an image item
	 m_widgetItem = new DwImageWidget(parent);
	 switch(m_gameActor.m_actorType)
	 {
	 case GameState::Pig :
		 m_widgetItem->setImage(DwString(L"home:testdata/space/pig1.jpg"));
		 break;
	 case GameState::RedBird :
		 m_widgetItem->setImage(DwString(L"home:testdata/space/bird1.jpg"));
		 break;
	 case GameState::Coin :
		 m_widgetItem->setImage(DwString(L"home:testdata/space/coin1.jpg"));
		 break;
	 default:
		 break;
	 }
	 m_widgetItem->setMovable(true);

}
//-----------------------------------------------------------------
void ActorItem::setGameActor(GameState::ActorType actorType, int mark)
{
	m_gameActor.m_actorType = actorType;
	m_gameActor.m_mark      = mark;
}
//-----------------------------------------------------------------
void ActorItem::createPhysicsBody(PhysicControl* physicControl,b2Vec2 pos)
{
	b2World* world = physicControl->getWorld();

	switch(m_gameActor.m_actorType)
	{
		case GameState::Pig :
			createPigBody(world,pos);
			break;
		case GameState::RedBird :
			createRedBirdBody(world,pos);
			break;
		case GameState::BlackBird :
			createBlackBirdBody(world,pos);
			break;
		case GameState::Coin :
			createCoinBody(world,pos);
			break;
		default:
			break;
	}
}
//-----------------------------------------------------------------
void ActorItem::createRedBirdBody(b2World* physicWorld, b2Vec2 pos)
{
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set(pos.x,pos.y);
	bd.gravityScale = 1.0f;

	GameActor* name = new GameActor;
	name->m_actorType = m_gameActor.m_actorType;
	name->m_mark = m_gameActor.m_mark;
	bd.userData = name;
	b2Body* body = physicWorld->CreateBody(&bd);

	b2PolygonShape shape;
	shape.SetAsBox(1.5f,1.5f);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.restitution = .5f;
	fixture.filter.categoryBits = 0x002;
	fixture.filter.maskBits = 0x002;
	body->CreateFixture(&fixture);

	b2Vec2 vel(-10,40);
	body->SetLinearVelocity(vel);
	body->SetAngularVelocity(0.5);

	setItemHasBeenPhysical(true);
}
//-----------------------------------------------------------------
void ActorItem::createPigBody(b2World* physicWorld, b2Vec2 pos)
{
	b2BodyDef bd;
	bd.type = b2_kinematicBody;
	bd.position.Set(pos.x,pos.y);

	GameActor* name = new GameActor;
	name->m_actorType = m_gameActor.m_actorType;
	name->m_mark = m_gameActor.m_mark;
	bd.userData = name;
	b2Body* body = physicWorld->CreateBody(&bd);

	b2PolygonShape shape;
	shape.SetAsBox(2.0f,2.0f);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 10.0f;
	fixture.restitution = 0.01f;
	fixture.filter.categoryBits = 0x002;
	fixture.filter.maskBits = 0x002;
	body->CreateFixture(&fixture);

	setItemHasBeenPhysical(true);
}
//-----------------------------------------------------------------
void ActorItem::createBlackBirdBody(b2World* physicWorld, b2Vec2 pos)
{
	b2BodyDef buttonBodyDef;
	buttonBodyDef.type = b2_kinematicBody;
	buttonBodyDef.position.Set(pos.x,pos.y);
	GameActor* name = new GameActor;
	name->m_actorType = m_gameActor.m_actorType;
	name->m_mark = m_gameActor.m_mark;
	buttonBodyDef.userData = name;
	buttonBodyDef.bullet = true;
	b2Body* body = physicWorld->CreateBody(&buttonBodyDef);
	b2PolygonShape shape;
	shape.SetAsBox(12.5f,2.15f);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 100.0f;
	fixture.restitution = 1.0f;
	fixture.filter.categoryBits = 0x022;
	fixture.filter.maskBits = 0x022;
	body->CreateFixture(&fixture);

	setItemHasBeenPhysical(true);
}
//-----------------------------------------------------------------
void ActorItem::createCoinBody(b2World* physicWorld, b2Vec2 pos)
{
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.gravityScale = 0.5f;
	bd.position.Set(pos.x, pos.y);

	GameActor* name = new GameActor;
	name->m_actorType = m_gameActor.m_actorType;
	name->m_mark = m_gameActor.m_mark;
	bd.userData = name;
	b2Body* body = physicWorld->CreateBody(&bd);

	b2PolygonShape shape;
	shape.SetAsBox(2.0f,2.0f);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 10.0f;
	fixture.restitution = 0.1f;
	fixture.filter.categoryBits = 0x020;
	fixture.filter.maskBits = 0x020;
	body->CreateFixture(&fixture);
	body->SetAngularVelocity(2);

	setItemHasBeenPhysical(true);
}