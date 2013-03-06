#include "mainFrameWindow.h"
#include "stable.h"
#include "dwcore/sigslot.h"
#include "dwgui/dwanchors.h"
#include "dwgui/dweventsignaldispatch.h"
#include "dwgui/dwringlogcase.h"

#include "gameState.h"
#include "dwcore\dwarraylist.h"
#include "physicCollisionContactListener.h"



DwArrayList<int> t;
double fps()
{
	t.push_back(GetTickCount());

	if(t.back() - t.front() > 1000)
	{
		t.pop_front();
	}

	int begin = t.front();
	int end = t.back();

	return t.size() * 1000.0 / (end - begin + 1);
}


//-----------------------------------------------------------------
DW_IMPLEMENT_CLASS(MainFrameWindow, 'EAGL', DwMainWindow)
//-----------------------------------------------------------------
MainFrameWindow::MainFrameWindow(DwObject *parent /* = NULL */)
: DwMainWindow(parent)
, m_hasCreatedScene(false)
{
	initScene();
	//m_background->setVisible(false);
}
//-----------------------------------------------------------------
MainFrameWindow::~MainFrameWindow()
{
	if (m_physicControl != NULL)
		delete m_physicControl;
}
//-----------------------------------------------------------------
void MainFrameWindow::initScene()
{
	//Create and init the physical world
	m_physicControl = new PhysicControl();
	//Create the menu start
	m_MenuStart = new MenuStart(this);
	m_MenuStart->setTextOnly();
	m_MenuStart->setText(L"start");
	m_MenuStart->anchors()->setBottom(DwAnchorLine(this, Dw::Bottom));
	m_MenuStart->setActorItem(new ActorItem(GameState::BlackBird,0));
	m_MenuStart->getActorItem()->createPhysicsBody(m_physicControl,b2Vec2(12.5f,2.15f));
    m_MenuStart->signalDispatch()->connect(DwKeyDownEvent::EventId(), this, &MainFrameWindow::onKeyDownEvent);
    m_MenuStart->signalDispatch()->connect(DwKeyUpEvent::EventId(), this, &MainFrameWindow::onKeyUpEvent);
    m_MenuStart->signalDispatch()->connect(DwMouseClickedEvent::EventId(), this, &MainFrameWindow::onMouseClickedEvent);
	m_coinScore = 0;;
}

//------------------------------------------------------------------------------
/**
*/
void MainFrameWindow::onKeyDownEvent(DwRttiObject *item, DwEvent *evtt)
{
    DW_UNUSED(item);
    DW_UNUSED(evtt);
    DwKeyDownEvent *evt = dwsafe_cast<DwKeyDownEvent>(evtt);
    MenuStart *button = dwsafe_cast<MenuStart>(item);
    if (evt->m_virtualKey)// utf16
    {
        //debug information
        std::wstring debugMessage;
        switch(evt->m_virtualKey)
        {
        case 'a':
        case 'A'://VK_A
            button->setButtonState(GameState::LeftState);
            debugMessage = L"VK_A event!\n";
            break;
        case 'd':
        case 'D'://VK_D
            button->setButtonState(GameState::RightState);
            debugMessage = L"VK_D event!\n";
            break;
        default:
            debugMessage = L"keyBoard event!\n";
        }
    }
    updateScene();
}

//------------------------------------------------------------------------------
/**
*/
void MainFrameWindow::onKeyUpEvent(DwRttiObject *item, DwEvent *evtt)
{
    DW_UNUSED(item);
    DW_UNUSED(evtt);
    DwKeyUpEvent *evt = dwsafe_cast<DwKeyUpEvent>(evtt);
    MenuStart *button = dwsafe_cast<MenuStart>(item);
    if (evt->m_virtualKey)// utf16
    {
        //debug information
        std::wstring debugMessage;
        switch(evt->m_virtualKey)
        {
        case 'a':
        case 'A'://VK_A
            button->setButtonState(GameState::IdleState);
            debugMessage = L"VK_A event!\n";
            break;
        case 'd':
        case 'D'://VK_D
            button->setButtonState(GameState::IdleState);
            debugMessage = L"VK_D event!\n";
            break;
        default:
            debugMessage = L"keyBoard event!\n";
        }
    }
    updateScene();
}

//------------------------------------------------------------------------------
/**
*/
void MainFrameWindow::onMouseClickedEvent(DwRttiObject *item, DwEvent *evtt)
{
    DwMouseClickedEvent *evt = dwsafe_cast<DwMouseClickedEvent>(evtt);
    MenuStart *button = dwsafe_cast<MenuStart>(item);
    if (evt && button)
    {
        m_hasCreatedScene = createScene();
        m_MenuStart->setButtonState(GameState::IdleState);
    }
}

//-----------------------------------------------------------------
void MainFrameWindow::timeStep()
{
	m_physicControl->Step();
	
	//Update the Scene
	if (m_hasCreatedScene)
		updateScene();

	//fps
	printf("fps: %f\n",fps());
}
//-----------------------------------------------------------------
bool MainFrameWindow::createScene()
{
	//Create the pigs
	//flag defines that the column
	int flag = 0;
	for (int i = 0 ; i < 23 ; i++)
	{
		for(int j =0 ; j < 3 ; j++)
		{
			ActorItem* newActorItem = new ActorItem(GameState::Pig,i*2+j+flag);
			newActorItem->createPhysicsBody(m_physicControl,b2Vec2(6.0f+i*4, 60.0f-j*4));
			newActorItem->createGraphicItem(this);
			m_physicControl->getContactListener()->m_visiblePigMark.append(GameState::Visible);
			m_pigsActorItem.append(newActorItem);
		}
		flag++;
	}

	//Create the redBird
	m_redBirdActorItem= new ActorItem(GameState::RedBird,0);
	m_redBirdActorItem->createPhysicsBody(m_physicControl,b2Vec2(6.0f, 5.5f));
	m_redBirdActorItem->createGraphicItem(this);

	//Create the coins Item
	int coinNumber = 3;
	for (int i = 0; i < coinNumber; i++)
	{
		ActorItem* newActorItem = new ActorItem(GameState::Coin,i);
		newActorItem->createGraphicItem(this);
		newActorItem->getGraphicItem()->setVisible(false);
		m_coinsActorItem.append(newActorItem);
	}

	return true;
}
//-----------------------------------------------------------------
void MainFrameWindow::updateScene()
{
	b2World* world = m_physicControl->getWorld();
	b2Body* bodyList = world->GetBodyList();
	for(b2Body* actorBody = bodyList; actorBody; actorBody = actorBody->GetNext())
	{
		GameActor* BodyA((GameActor*)actorBody->GetUserData());
		switch( BodyA->m_actorType )
		{
		case GameState::Pig:
			if (m_physicControl->getContactListener()->m_visiblePigMark.at(BodyA->m_mark) == GameState::Invisible)
			{
				m_pigsActorItem[BodyA->m_mark]->getGraphicItem()->setVisible(false);
				//create the coin physical body
				createCoin(actorBody->GetPosition());
				//delete the pig physical body
				b2Body* bodyDelete = actorBody;
				actorBody = actorBody->GetNext();
				m_physicControl->getWorld()->DestroyBody(bodyDelete);
				bodyDelete = NULL;
			}else
				updateActorItem(m_pigsActorItem.at(BodyA->m_mark),actorBody);
			break;
		case GameState::RedBird:
			{
				if (m_physicControl->getContactListener()->m_isAccelerate)
				{
					//get the velocity
					b2Vec2 preVelocity = actorBody->GetLinearVelocity();
					b2Vec2 newVelocity = preVelocity;
					if (preVelocity.Length() < 40)
					{
						preVelocity.Normalize();
						newVelocity.x = preVelocity.x * 40;
						newVelocity.y = preVelocity.y * 40;
					}
					actorBody->SetLinearVelocity(newVelocity);
					m_physicControl->getContactListener()->m_isAccelerate = false;
				}else
				{
					//the bird are active?
					if (actorBody->GetLinearVelocity().Length() == 0)
					{
						actorBody->SetLinearVelocity(b2Vec2(0,40));
						actorBody->SetAngularVelocity(0.5);
					}
				}
				updateActorItem(m_redBirdActorItem,actorBody);
					
			}
			break;
		case GameState::BlackBird:
				updateMenuStart(actorBody);
			break;
		case GameState::Coin:
			{
				if ( actorBody->GetPosition().y < 0 
					|| m_physicControl->getContactListener()->m_visibleCoinMark[BodyA->m_mark] == GameState::InvisibleCoin)
				{
					b2Body* bodyDelete = actorBody;
					actorBody = actorBody->GetNext();
					m_physicControl->getWorld()->DestroyBody(bodyDelete);
					bodyDelete = NULL;
					m_coinsActorItem.at(BodyA->m_mark)->setItemHasBeenPhysical(false);
					m_coinsActorItem.at(BodyA->m_mark)->getGraphicItem()->setVisible(false);
					if (m_physicControl->getContactListener()->m_visibleCoinMark[BodyA->m_mark] == GameState::InvisibleCoin)
					{
						m_coinScore += 100;
						this->setCaption(DwString(L"Raccoon Engine ตรทึ: ") + DwString::number(m_coinScore));
					}
					continue;
				}
				updateActorItem(m_coinsActorItem.at(BodyA->m_mark),actorBody);
			}
			break;
		default:
			break;			
		}
	}
}
//-----------------------------------------------------------------
void MainFrameWindow::updateActorItem(ActorItem *actorItem,b2Body* actorBody)
{
	const b2Transform& xf = actorBody->GetTransform();
	for (b2Fixture* f = actorBody->GetFixtureList(); f; f = f->GetNext())
	{			
		if (f->GetType() == b2Shape::e_polygon)
		{
			b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();
			int32 vertexCount = poly->m_vertexCount;
			b2Assert(vertexCount <= b2_maxPolygonVertices);
			b2Vec2 vertices[b2_maxPolygonVertices];

			for (int32 i = 0; i < vertexCount; ++i)
			{
				vertices[i] = b2Mul(xf, poly->m_vertices[i]);
			}
			//calculate the pos
			DwPointF pos((vertices[0].x+vertices[2].x)*10/2,-(vertices[0].y+vertices[2].y)*10/2 + 768);
			//calculate the angle
			b2Vec2 center1((vertices[0].x+vertices[2].x)/2,(vertices[0].y+vertices[2].y)/2);
			b2Vec2 center2((vertices[1].x+vertices[2].x)/2,(vertices[1].y+vertices[2].y)/2);
			b2Vec2 vector1 = center2 - center1;
			vector1.Normalize();
			float Angle = b2Dot(vector1,b2Vec2(1.0,0.0));
			Angle = std::acos(Angle); 
			Angle = (float)(Angle*180.0f)/3.14f;
			if ((vertices[1].y+vertices[2].y)/2 < (vertices[0].y+vertices[2].y)/2)
				Angle = 360-Angle;
			//calculate the transform
			b2Rot rot(Angle/180.0f*3.1415926f);
			DwTransform transform;							
			transform.setMatrix(rot.c , -rot.s ,   0,
				rot.s ,  rot.c ,   0,
				pos.x(),  pos.y(),      1);
			DwTransform transform2;
			if (actorItem->getGameActor().m_actorType == GameState::RedBird)
				transform2.setTranslated(-15,-15);
			else
				transform2.setTranslated(-20,-20); 
			

			actorItem->getGraphicItem()->setTransform(transform2*transform);
		}//if (f->GetType() == b2Shape::e_polygon)
	}//for
}
//-----------------------------------------------------------------
void MainFrameWindow::updateMenuStart(b2Body* actorBody)
{
	if(m_MenuStart->getButtonState() == GameState::RightState)
	{
		DwPointF prePos = m_MenuStart->pos();
		m_MenuStart->setPos(prePos.x()+20,prePos.y());

		b2Vec2 befPos = actorBody->GetPosition(); 
		actorBody->SetTransform(b2Vec2(befPos.x+2.0f,befPos.y),0);
	}else if (m_MenuStart->getButtonState() == GameState::LeftState)
	{
		DwPointF prePos =m_MenuStart->pos();
		m_MenuStart->setPos(prePos.x()-20,prePos.y());

		b2Vec2 befPos = actorBody->GetPosition(); 
		actorBody->SetTransform(b2Vec2(befPos.x-2.0f,befPos.y),0);
	}
}
//-----------------------------------------------------------------
void MainFrameWindow::createCoin(const b2Vec2 pos)
{
	b2World* world = m_physicControl->getWorld();
	
	//1/2 rate to create the coin physical body
	if ((rand() % 4 == 1) || (rand() % 4 == 2))
	{
		for(int i = 0 ; i < m_coinsActorItem.size();i++)
		{
			if (m_coinsActorItem.at(i)->getItemHasBeenPhysical())
				continue;
			else
			{
				m_coinsActorItem.at(i)->createCoinBody(world,pos);
				m_coinsActorItem.at(i)->getGraphicItem()->setVisible(true);
				m_physicControl->getContactListener()->m_visibleCoinMark[i] = GameState::VisibleCoin;
				break;
			}
		}
	}
}