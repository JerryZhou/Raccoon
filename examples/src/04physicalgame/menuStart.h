#ifndef _MENUSTART_H_
#define _MENUSTART_H_
//by eagle

#include "dwgui/dwwidgetitem.h"
#include "actorItem.h"
#include "gameState.h"

class DwGraphicsSimpleTextItem;
class DwGraphicsFrameItem;
class DwGraphicsPixmapItem;

//-----------------------------------------------------------------
class MenuStart : public DwWidgetItem
{
	DW_DECLARE_CLASS(MenuStart);

	DW_WIDGET_PROPERTY_BEGIN(MenuStart, DwWidgetItem);
	DW_WIDGET_PROPERTY_END();
public:
	explicit MenuStart(DwGraphicsItem *parent = NULL);
	virtual ~MenuStart();

	void setBackground(const DwUrl& );
	void setIcon(const DwString& );
	void setText(const DwString& );
	void setImage(DwWidget::FrameState state, const DwUrl& image);

	DwGraphicsFrameItem* background() const { return m_background; }
	DwGraphicsPixmapItem* icon() const { return m_icon; }
	DwGraphicsSimpleTextItem* text() const { return m_text; }

	void setHorizable(bool h);

	void setTextOnly();
	void setIconOnly();

	void setButtonState(GameState::MenuStartState state);
	GameState::MenuStartState getButtonState(){ return m_menuStartState ;}
	void setActorItem(ActorItem* actorItem) {m_actorItem =  actorItem;}
	ActorItem*  getActorItem() {return m_actorItem ;}

	sigslot::signal0<> sigClicked;
protected:

	DwGraphicsFrameItem *m_background;
	DwGraphicsPixmapItem *m_icon;
	DwGraphicsSimpleTextItem *m_text;

	GameState::MenuStartState m_menuStartState;
	ActorItem*  m_actorItem;
};

#endif
