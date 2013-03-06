#include "stable.h"
#include "menuStart.h"
#include "dwgui/dwgraphicspixmapitem.h"
#include "dwgui/dwgraphicssimpletextitem.h"
#include "dwgui/dwgridpositioner.h"
#include "dwgui/dwanchors.h"
#include "dwgraphics/dwimage.h"
#include "dwgui/dwgraphicsframeitem.h"
#include "dwgui/dwwidgeteventhandler.h"

const char KNormalImage[] = "home:testdata/space/start.jpg";
const char KSpaceImage[] = "home:testdata/space/space.jpg";
const char KPressdownImage[] = "dwgui:btn_4word_pushed.png";
const char KHoverImage[]  = "dwgui:btn_4word_hover.png";

DW_IMPLEMENT_CLASS(MenuStart, 'MEST', DwWidgetItem)

//-----------------------------------------------------------------
class MenuPushButtonEventHandler : public DwWidgetEventHandler
{
    DW_DECLARE_CLASS(MenuPushButtonEventHandler);
public:
    MenuPushButtonEventHandler()
    {
        DW_ASSERT(false);
    }

	MenuPushButtonEventHandler(DwGraphicsItem* item, DwGraphicsBackgroundItem* back)
		: DwWidgetEventHandler(item, back)
	{
	}

	virtual void onMouseClicked(DwMouseClickedEvent* evt)
	{
		MenuStart* button = (MenuStart*)(m_item);
		button->setBackground(KSpaceImage);
		button->setButtonState(GameState::HasClickedState);
		button->sigClicked.emit();
		evt->accept();

		std::wstring debugMessage(L"mouse event!\n");
		OutputDebugString(debugMessage.c_str());
	}

	//virtual void onCharEvent(DwCharEvent* evt)
	//{	
	//	if (evt->m_characterCode)// utf16
	//	{	
	//		MenuStart* button = (MenuStart*)(m_item);
	//		//debug information
	//		std::wstring debugMessage;
	//		switch(evt->m_virtualKey)
	//		{
	//			case 97://VK_A
	//				button->setButtonState(GameState::LeftState);
	//				debugMessage = L"VK_A event!\n";
	//				break;
	//			case 100://VK_D
	//				button->setButtonState(GameState::RightState);
	//				debugMessage = L"VK_D event!\n";
	//				break;
	//			default:
	//				debugMessage = L"keyBoard event!\n";
	//		}
	//		OutputDebugString(debugMessage.c_str());
	//	}
	//}
};
DW_IMPLEMENT_CLASS(MenuPushButtonEventHandler, 'PBER', DwWidgetEventHandler);

//-----------------------------------------------------------------
MenuStart::MenuStart(DwGraphicsItem *parent)
: DwWidgetItem(parent)
, m_menuStartState(GameState::IdleState)
{
	setName(L"MenuStart");

	m_background = new DwGraphicsFrameItem(this);
	m_background->setMargins(DwMargins(3, 3, 3, 3));
	m_background->anchors()->setFill(this);
	m_background->setName(L"background");
	m_background->setImage(DwWidget::SNormal, KNormalImage);
	m_background->setImage(DwWidget::SHover, KHoverImage);
	m_background->setImage(DwWidget::SPressDown, KPressdownImage);
	setBackground(KNormalImage);

	m_icon = new DwGraphicsPixmapItem(this);
	m_text = new DwGraphicsSimpleTextItem(this);
	m_icon->setVisible(false);
	m_icon->setName(L"icon");
	m_text->setVisible(false);
	m_text->setName(L"text");

	setEventHandler(new MenuPushButtonEventHandler(this, NULL));
	setHorizable(true);
}
//-----------------------------------------------------------------
MenuStart::~MenuStart()
{
}
//-----------------------------------------------------------------
void MenuStart::setImage(DwWidget::FrameState state, const DwUrl& image)
{
	m_background->setImage(state, image);
}
//-----------------------------------------------------------------
void MenuStart::setBackground(const DwUrl& backImage)
{
	DwImage background(backImage.localPath());
	setBoundingRect(DwRectF(DwPointF(), DwSizeF(background.size())));
	setImage(DwWidget::SNormal, backImage);
}
//-----------------------------------------------------------------
void MenuStart::setIcon(const DwString& icon)
{
	m_icon->setPixmap(DwImage(icon));
	m_icon->setVisible(true);
}
//-----------------------------------------------------------------
void MenuStart::setText(const DwString& text)
{
	m_text->setText(text);
	m_text->setVisible(true);
}
//-----------------------------------------------------------------
void MenuStart::setHorizable(bool h)
{
	if (h)
	{
		m_icon->clearAnchors();
		m_icon->anchors()->setLeftMargin(10.0);
		m_icon->anchors()->setLeft(DwAnchorLine(this, Dw::Left));
		m_icon->anchors()->setVerticalCenter(DwAnchorLine(this, Dw::VCenter));

		m_text->clearAnchors();
		m_text->anchors()->setLeftMargin(5.0);
		m_text->anchors()->setLeft(DwAnchorLine(m_icon, Dw::Right));
		m_text->anchors()->setVerticalCenter(DwAnchorLine(this, Dw::VCenter));	
	}
	else
	{
		m_icon->clearAnchors();
		m_icon->anchors()->setTopMargin(10.0);
		m_icon->anchors()->setTop(DwAnchorLine(this, Dw::Top));
		m_icon->anchors()->setHorizontalCenter(DwAnchorLine(this, Dw::HCenter));

		m_text->clearAnchors();
		m_text->anchors()->setTopMargin(3.0);
		m_text->anchors()->setTop(DwAnchorLine(m_icon, Dw::Bottom));
		m_text->anchors()->setHorizontalCenter(DwAnchorLine(this, Dw::HCenter));
	}
}
//-----------------------------------------------------------------
void MenuStart::setTextOnly()
{
	m_icon->setVisible(false);
	m_text->clearAnchors();
	m_text->anchors()->setCenterIn(this);
}
//-----------------------------------------------------------------
void MenuStart::setIconOnly()
{
	m_text->setVisible(false);
	m_icon->clearAnchors();
	m_icon->anchors()->setCenterIn(this);
}
//-----------------------------------------------------------------
void MenuStart::setButtonState(GameState::MenuStartState state)
{
	m_menuStartState = state;
}
