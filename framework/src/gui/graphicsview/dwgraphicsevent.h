#pragma once
#include "dwnamespace.h"
#include "dwgui/dwevent.h"
#include "dwgraphics/dwmatrix.h"
#include "dwcore/dwstring.h"
#include "dwcore/dwvariant.h"
#include "dwgui/dwmimedata.h"
#include "dwgui/dwrttiobjectptr.h"

//------------------------------------------------------------------------------
class DwGraphicsView;
class DwGraphicsItem;
class DwGraphicsScene;
class DwWindow;
class DwPainter;

//////////////////////////////////////////////////////////////////////////
class DW_GUI_EXPORT DwSceneEvent : public DwEvent
{
    DW_DECLARE_EVENTID(DwSceneEvent);
public:
    DwSceneEvent();
    virtual ~DwSceneEvent();

    // used to fetch vars in template way
    virtual DwVariant vars(const DwString& key) const;

    // event translated to target format
    virtual void translatedToScene(const DwGraphicsView *view);
    virtual void translatedToItem(const DwGraphicsItem *item);

    // event must tell the target item in scene
    virtual DwGraphicsItem* indexInterestItemFromScene(const DwGraphicsScene *scene);
    // event must tell the target item in view
    virtual DwGraphicsItem* indexInterestItemFromView(const DwGraphicsView *view);

    // if the item is reached the require
    virtual bool isInterestedBy(const DwGraphicsItem* item);
    // some times the handler should tell that which item will be truely interest
    void setInterestItem(const DwGraphicsItem* item) ;

    // result
    DwGraphicsItem* doneItem() const;
    void setDoneItem(const DwGraphicsItem* item) ;

    // keep view in event, if event was dispatched by view, will set
    DwGraphicsView* view() const ;
    void setView(const DwGraphicsView *v) ;

    // used to dispatch trace
    int& dispatchUpLevel();
    int& dispatchDownLevel();

    // clead trace information
    void clearLevels();
    void clearUpLevel();
    void clearDownLevel();

protected:
    // NB! SEE EACH EVENT TO KONW THE SPECIAL MEANING IN EVENT
    DwRttiObjectPtr<DwGraphicsItem> m_interestItem;
    DwRttiObjectPtr<DwGraphicsItem> m_doneItem;
    DwRttiObjectPtr<DwGraphicsView> m_view;

    int m_dispatchUpLevel;
    int m_dispatchDownLevel;
};
DW_REGISTER_CLASS(DwSceneEvent);

#if DW_ENABLE_EVENT_ITEM_INNER
//////////////////////////////////////////////////////////////////////////
class DW_GUI_EXPORT DwGraphicsItemEvent : public DwSceneEvent
{
    DW_DECLARE_EVENTID(DwGraphicsItemEvent);
public:
};
DW_REGISTER_CLASS(DwGraphicsItemEvent);

class DW_GUI_EXPORT DwItemCreateEvent : public DwGraphicsItemEvent
{
    DW_DECLARE_EVENTID(DwItemCreateEvent);
public:
};
DW_REGISTER_CLASS(DwItemCreateEvent);

class DW_GUI_EXPORT DwItemDestroyEvent : public DwGraphicsItemEvent
{
    DW_DECLARE_EVENTID(DwItemDestroyEvent);
public:
};
DW_REGISTER_CLASS(DwItemDestroyEvent);

class DW_GUI_EXPORT DwItemChangeEvent : public DwGraphicsItemEvent
{
    DW_DECLARE_EVENTID(DwItemChangeEvent);
public:
    Dw::GraphicsItemChange m_change;
    DwVariant m_oldOne;
    DwVariant m_newOne;
};
DW_REGISTER_CLASS(DwItemChangeEvent);
#endif

//////////////////////////////////////////////////////////////////////////
//NB! THIS EVENT IS THE SPECIAL EVENT, IN THE EVENT META SPACE,
//FORBID USED TO DILIDER ANY OTHER MESSAGE, FORBID USER TO INHERT IT
class DW_GUI_EXPORT DwEventAcceptedEvent : public DwSceneEvent
{
    DW_DECLARE_EVENTID(DwEventAcceptedEvent);
public:
    DwEventAcceptedEvent(DwSceneEvent* evt = NULL);
    virtual ~DwEventAcceptedEvent();

    // if the item is reached the require
    virtual bool isInterestedBy(const DwGraphicsItem* item);

    DwSceneEvent* m_acceptedEvent;
protected:
private:
};
DW_REGISTER_CLASS(DwEventAcceptedEvent);

//////////////////////////////////////////////////////////////////////////
class DW_GUI_EXPORT DwMouseEvent : public DwSceneEvent
{
    DW_DECLARE_EVENTID(DwMouseEvent);
public:
    DwMouseEvent();
    virtual ~DwMouseEvent();

    virtual void translatedToScene(const DwGraphicsView* view);
    virtual void translatedToItem(const DwGraphicsItem* item);

    virtual DwGraphicsItem* indexInterestItemFromScene(const DwGraphicsScene *scene);

    // if the item is reached the require
    virtual bool isInterestedBy(const DwGraphicsItem* item);

    //DwPoint m_mousePoint;
    DwPoint m_screenPoint;  // screen coordinate
    DwPoint m_devicePoint;  // device coordinate
    DwPoint m_viewPoint;    // view coordinate
    DwPoint m_scenePoint;   // scene coordinate
    DwPoint m_itemPoint;    // item coordinate

    Dw::MouseButton m_button;

    bool m_searched;
    bool m_isNC;
    unsigned int m_nc;
protected:
private:
};
DW_REGISTER_CLASS(DwMouseEvent);

class DW_GUI_EXPORT DwSceneMouseEvent : public DwMouseEvent
{
    DW_DECLARE_EVENTID(DwSceneMouseEvent);
public:
protected:
private:
};
DW_REGISTER_CLASS(DwSceneMouseEvent);

class DW_GUI_EXPORT DwMouseDownEvent : public DwSceneMouseEvent
{
    DW_DECLARE_EVENTID(DwMouseDownEvent);
public:
protected:
private:
};
DW_REGISTER_CLASS(DwMouseDownEvent);

class DW_GUI_EXPORT DwMouseUpEvent : public DwSceneMouseEvent
{
    DW_DECLARE_EVENTID(DwMouseUpEvent);
public:
protected:
private:
};
DW_REGISTER_CLASS(DwMouseUpEvent);

class DW_GUI_EXPORT DwMouseWheelEvent : public DwSceneMouseEvent
{
    DW_DECLARE_EVENTID(DwMouseWheelEvent);
public:
    DwMouseWheelEvent();

    virtual DwGraphicsItem* indexInterestItemFromScene(const DwGraphicsScene *scene);

    // if the item is reached the require
    virtual bool isInterestedBy(const DwGraphicsItem* item);

    // forward the user
    bool isRollUp();
    // toward the user
    bool isRollDown();
    // abs of the delta value
    DwReal absDelta();

    Dw::MouseVirtualKeyState m_fwKeys;
    DwReal m_delta;

    // m_delta expressed in multiples or divisions of WheelData
    static const int WheelData = 120; // WHEEL_DELTA
protected:
private:
};
DW_REGISTER_CLASS(DwMouseWheelEvent);

// NB: DISPATCH PRIORITY : GRABER, HOVER, UNDERITEM
class DW_GUI_EXPORT DwMouseMoveEvent : public DwSceneMouseEvent
{
    DW_DECLARE_EVENTID(DwMouseMoveEvent);
public:
    DwMouseMoveEvent();

    virtual DwGraphicsItem* indexInterestItemFromScene(const DwGraphicsScene *scene);

    DwPoint m_delta;
protected:
private:
};
DW_REGISTER_CLASS(DwMouseMoveEvent);

class DW_GUI_EXPORT DwMouseDbClickEvent : public DwSceneMouseEvent
{
    DW_DECLARE_EVENTID(DwMouseDbClickEvent);
public:
protected:
private:
};
DW_REGISTER_CLASS(DwMouseDbClickEvent);

class DW_GUI_EXPORT DwMouseClickedEvent : public DwMouseEvent
{
    DW_DECLARE_EVENTID(DwMouseClickedEvent);
public:
    DwMouseClickedEvent(DwGraphicsItem* clickItem = NULL);
    virtual ~DwMouseClickedEvent();

    // event must tell the target item in scene
    virtual DwGraphicsItem* indexInterestItemFromScene(const DwGraphicsScene *scene);
    // event must tell the target item in view
    virtual DwGraphicsItem* indexInterestItemFromView(const DwGraphicsView *view);

    // TODO: item->isAccpetClicked()
    // if the item is reached the require
    // virtual bool isInterestedBy(DwGraphicsItem* item);

    DwGraphicsItem* m_clickedItem;
protected:
private:
};
DW_REGISTER_CLASS(DwMouseClickedEvent);

//////////////////////////////////////////////////////////////////////////
class DW_GUI_EXPORT DwKeyEvent : public DwSceneEvent
{
    DW_DECLARE_EVENTID(DwKeyEvent);
public:
    DwKeyEvent();
    virtual ~DwKeyEvent();

    virtual DwGraphicsItem* indexInterestItemFromScene(const DwGraphicsScene *scene);

    virtual bool isInterestedBy(const DwGraphicsItem* item);

    bool isExtendKey() const;

    unsigned int keyCode () const
    {
        return m_virtualKey;
    }
    unsigned int modifier() const
    {
        return m_modifier;
    }

    bool isShfitDown() const
    {
        return m_isShiftDown;
    }
    bool isCtrlDown() const
    {
        return m_isControlDown;
    }
    bool isAltDown() const
    {
        return m_isAltDown;
    }

    static bool isKeyModifierDown(Dw::Modifier modifier) ;

    static bool isAnyKeyModifier() ;

    // NB! DONOT CHANGE THE ORDER OF MEMBER
    unsigned int m_virtualKey;
    union
    {
        struct
        {
            uint32_t m_repeatCount : 15;	//0-15
            uint32_t m_scanCode : 7;		//16-23
            uint32_t m_extendCode : 1;		//24
            uint32_t m_reserved : 3;		//25-28
            uint32_t m_contextCode : 1;		//29
            uint32_t m_preState : 1;		//30
            uint32_t m_transitionState : 1;	//31
        };
        uint32_t m_param;
    };

    bool m_isShiftDown : 1;
    bool m_isControlDown : 1;
    bool m_isAltDown : 1;
    unsigned int m_modifier;

protected:
private:
};
DW_REGISTER_CLASS(DwKeyEvent);

class DW_GUI_EXPORT DwKeyDownEvent : public DwKeyEvent
{
    DW_DECLARE_EVENTID(DwKeyDownEvent);
public:
protected:
private:
};
DW_REGISTER_CLASS(DwKeyDownEvent);

class DW_GUI_EXPORT DwKeyUpEvent : public DwKeyEvent
{
    DW_DECLARE_EVENTID(DwKeyUpEvent);
public:
protected:
private:
};
DW_REGISTER_CLASS(DwKeyUpEvent);

//////////////////////////////////////////////////////////////////////////
class DW_GUI_EXPORT DwCharEvent : public DwSceneEvent
{
    DW_DECLARE_EVENTID(DwCharEvent);
public:
    DwCharEvent();
    virtual ~DwCharEvent();

    virtual DwGraphicsItem* indexInterestItemFromScene(const DwGraphicsScene *scene);

    virtual bool isInterestedBy(const DwGraphicsItem* item);

    // NB! DONOT CHANGE THE ORDER OF MEMBER
    union
    {
        unsigned int m_virtualKey;
        unsigned int m_characterCode; // in utf-16 format
    };

    union
    {
        struct
        {
            uint32_t m_repeatCount : 15;	//0-15
            uint32_t m_scanCode : 7;		//16-23
            uint32_t m_extendCode : 1;		//24
            uint32_t m_reserved : 3;		//25-28
            uint32_t m_contextCode : 1;		//29
            uint32_t m_preState : 1;		//30
            uint32_t m_transitionState : 1;	//31
        };
        uint32_t m_param;
    };
protected:
private:
};
DW_REGISTER_CLASS(DwCharEvent);

#if DW_ENABLE_EVENT_IME
//TODO: TO SUPPORT IME APPLICATION
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class DW_GUI_EXPORT DwIMEEvent : public DwSceneEvent
{
    DW_DECLARE_EVENTID(DwIMEEvent);
public:
protected:
private:
};
DW_REGISTER_CLASS(DwIMEEvent);

class DW_GUI_EXPORT DwIMECharEvent : public DwIMEEvent
{
    DW_DECLARE_EVENTID(DwIMECharEvent);
public:
protected:
private:
};
DW_REGISTER_CLASS(DwIMECharEvent);

class DW_GUI_EXPORT DwIMEKeyEvent : public DwIMEEvent
{
    DW_DECLARE_EVENTID(DwIMEKeyEvent);
public:
protected:
private:
};
DW_REGISTER_CLASS(DwIMEKeyEvent);

class DW_GUI_EXPORT DwIMEKeyDownEvent : public DwIMEKeyEvent
{
    DW_DECLARE_EVENTID(DwIMEKeyDownEvent);
public:
protected:
private:
};
DW_REGISTER_CLASS(DwIMEKeyDownEvent);

class DW_GUI_EXPORT DwIMEKeyUpEvent : public DwIMEKeyEvent
{
    DW_DECLARE_EVENTID(DwIMEKeyUpEvent);
public:
protected:
private:
};
DW_REGISTER_CLASS(DwIMEKeyUpEvent);

class DW_GUI_EXPORT DwIMESetConTextEvent : public DwIMEEvent
{
    DW_DECLARE_EVENTID(DwIMESetConTextEvent);
public:
protected:
private:
};
DW_REGISTER_CLASS(DwIMESetConTextEvent);

class DW_GUI_EXPORT DwIMENotifyEvent : public DwIMEEvent
{
    DW_DECLARE_EVENTID(DwIMENotifyEvent);
public:
protected:
private:
};
DW_REGISTER_CLASS(DwIMENotifyEvent);

class DW_GUI_EXPORT DwIMEControlEvent : public DwIMEEvent
{
    DW_DECLARE_EVENTID(DwIMEControlEvent);
public:
protected:
private:
};
DW_REGISTER_CLASS(DwIMEControlEvent);

class DW_GUI_EXPORT DwIMECompositionFullEvent : public DwIMEEvent
{
    DW_DECLARE_EVENTID(DwIMECompositionFullEvent);
public:
protected:
private:
};
DW_REGISTER_CLASS(DwIMECompositionFullEvent);

class DW_GUI_EXPORT DwIMESelectEvent : public DwIMEEvent
{
    DW_DECLARE_EVENTID(DwIMESelectEvent);
public:
protected:
private:
};
DW_REGISTER_CLASS(DwIMESelectEvent);

class DW_GUI_EXPORT DwIMERequestEvent : public DwIMEEvent
{
    DW_DECLARE_EVENTID(DwIMERequestEvent);
public:
protected:
private:
};
DW_REGISTER_CLASS(DwIMERequestEvent);
#endif

#if DW_ENABLE_EVENT_HOVER
//////////////////////////////////////////////////////////////////////////
class DW_GUI_EXPORT DwHoverEvent : public DwSceneEvent
{
    DW_DECLARE_EVENTID(DwHoverEvent);
public:
    virtual bool isInterestedBy(const DwGraphicsItem* item);
protected:
private:
};
DW_REGISTER_CLASS(DwHoverEvent);

class DW_GUI_EXPORT DwHoverEnterEvent : public DwHoverEvent
{
    DW_DECLARE_EVENTID(DwHoverEnterEvent);
public:
protected:
private:
};
DW_REGISTER_CLASS(DwHoverEnterEvent);

class DW_GUI_EXPORT DwHoverGainEvent : public DwHoverEvent
{
    DW_DECLARE_EVENTID(DwHoverGainEvent);
public:
    DwHoverGainEvent(DwGraphicsItem* item = NULL);
protected:
private:
};
DW_REGISTER_CLASS(DwHoverGainEvent);

class DW_GUI_EXPORT DwHoverLeaveEvent : public DwHoverEvent
{
    DW_DECLARE_EVENTID(DwHoverLeaveEvent);
public:
protected:
private:
};
DW_REGISTER_CLASS(DwHoverLeaveEvent);
#endif

#if DW_ENABLE_EVENT_FOCUS
//////////////////////////////////////////////////////////////////////////
class DW_GUI_EXPORT DwFocusEvent : public DwSceneEvent
{
    DW_DECLARE_EVENTID(DwFocusEvent);
public:
    DwFocusEvent();

    virtual bool isInterestedBy(const DwGraphicsItem* item);

    Dw::FocusReason m_reason;
protected:
private:
};
DW_REGISTER_CLASS(DwFocusEvent);

class DW_GUI_EXPORT  DwFocusInEvent : public DwFocusEvent
{
    DW_DECLARE_EVENTID(DwFocusInEvent);
public:
protected:
private:
};
DW_REGISTER_CLASS(DwFocusInEvent);

class DW_GUI_EXPORT DwFocusOutEvent : public DwFocusEvent
{
    DW_DECLARE_EVENTID(DwFocusOutEvent);
public:
protected:
private:
};
DW_REGISTER_CLASS(DwFocusOutEvent);

class DW_GUI_EXPORT DwFocusGainEvent : public DwFocusEvent
{
    DW_DECLARE_EVENTID(DwFocusGainEvent);
public:
protected:
private:
};
DW_REGISTER_CLASS(DwFocusGainEvent);
#endif

#if DW_ENABLE_EVENT_DRAGDROP
//////////////////////////////////////////////////////////////////////////
class DW_GUI_EXPORT DwSceneDragEvent : public DwSceneEvent
{
    DW_DECLARE_EVENTID(DwSceneDragEvent);
public:
    DwSceneDragEvent(DwGraphicsItem* item = NULL);
    virtual ~DwSceneDragEvent();

    virtual void constructFromMouseEvent(DwMouseEvent* evt);

    virtual DwGraphicsItem* indexInterestItemFromScene(const DwGraphicsScene *scene);

    virtual bool isInterestedBy(const DwGraphicsItem* item);

    //DwPoint
    DwPoint m_screenPoint;  // screen coordinate
    DwPoint m_devicePoint;  // device coordinate
    DwPoint m_viewPoint;    // view coordinate
    DwPoint m_scenePoint;   // scene coordinate

    Dw::MouseButton m_button;
protected:
};
DW_REGISTER_CLASS(DwSceneDragEvent);

class DW_GUI_EXPORT DwDragStartEvent : public DwSceneDragEvent
{
    DW_DECLARE_EVENTID(DwDragStartEvent);
public:
    DwDragStartEvent(DwGraphicsItem* item = NULL): DwSceneDragEvent(item) {}
protected:
private:
};
DW_REGISTER_CLASS(DwDragStartEvent);

class DW_GUI_EXPORT DwDragMoveEvent : public DwSceneDragEvent
{
    DW_DECLARE_EVENTID(DwDragMoveEvent);
public:
    DwDragMoveEvent(DwGraphicsItem* item = NULL, const DwPoint& curPos = DwPoint(), const DwPoint& delta = DwPoint())
        : DwSceneDragEvent(item)
        , m_curMousePoint(curPos)
        , m_delta(delta)
    {
        ;
    }

    DwPoint m_delta;
    DwPoint m_curMousePoint;
private:
};
DW_REGISTER_CLASS(DwDragMoveEvent);

class DW_GUI_EXPORT DwDragEndEvent : public DwSceneDragEvent
{
    DW_DECLARE_EVENTID(DwDragEndEvent);
public:
    DwDragEndEvent(DwGraphicsItem* item = NULL): DwSceneDragEvent(item) {}
protected:
private:
};
DW_REGISTER_CLASS(DwDragEndEvent);

//////////////////////////////////////////////////////////////////////////
class DW_GUI_EXPORT DwDragDropEvent : public DwMouseEvent
{
    DW_DECLARE_EVENTID(DwDragDropEvent);
public:
    DwDragDropEvent();
    virtual ~DwDragDropEvent();

    virtual void constructFromMouseEvent(DwMouseEvent* evt);
    virtual void constructFromDragEvent(DwSceneDragEvent* evt);

    virtual DwGraphicsItem* indexInterestItemFromScene(const DwGraphicsScene *scene);

    virtual bool isInterestedBy(const DwGraphicsItem* item);

    const DwMimeData *mimeData() const;

    DwMimeData m_mimeData;

    // exclude this drop event
    DwRttiObjectPtr<DwGraphicsItem> m_excludeItem;
protected:
private:
};
DW_REGISTER_CLASS(DwDragDropEvent);
#endif

#if DW_ENABLE_EVENT_GRABER
//////////////////////////////////////////////////////////////////////////
class DW_GUI_EXPORT DwGraberEvent : public DwSceneEvent
{
    DW_DECLARE_EVENTID(DwGraberEvent)
public:
protected:
private:
};
DW_REGISTER_CLASS(DwGraberEvent);

class DW_GUI_EXPORT DwMouseGraberBaseEvent : public DwGraberEvent
{
    DW_DECLARE_EVENTID(DwMouseGraberBaseEvent)
public:
protected:
private:
};
DW_REGISTER_CLASS(DwMouseGraberBaseEvent);

class DW_GUI_EXPORT DwGrabMouseEvent : public DwMouseGraberBaseEvent
{
    DW_DECLARE_EVENTID(DwGrabMouseEvent)
public:
protected:
private:
};
DW_REGISTER_CLASS(DwGrabMouseEvent);

class DW_GUI_EXPORT DwUngrabMouseEvent : public DwMouseGraberBaseEvent
{
    DW_DECLARE_EVENTID(DwUngrabMouseEvent)
public:
protected:
private:
};
DW_REGISTER_CLASS(DwUngrabMouseEvent);

class DW_GUI_EXPORT DwMouseGainEvent : public DwMouseGraberBaseEvent
{
    DW_DECLARE_EVENTID(DwMouseGainEvent)
public:
protected:
private:
};
DW_REGISTER_CLASS(DwMouseGainEvent);

class DW_GUI_EXPORT DwKeyboardGraberBaseEvent : public DwGraberEvent
{
    DW_DECLARE_EVENTID(DwKeyboardGraberBaseEvent)
public:
protected:
private:
};
DW_REGISTER_CLASS(DwKeyboardGraberBaseEvent);

class DW_GUI_EXPORT DwGrabKeyboardEvent : public DwKeyboardGraberBaseEvent
{
    DW_DECLARE_EVENTID(DwGrabKeyboardEvent)
public:
protected:
private:
};
DW_REGISTER_CLASS(DwGrabKeyboardEvent);

class DW_GUI_EXPORT DwUngrabKeyboardEvent : public DwKeyboardGraberBaseEvent
{
    DW_DECLARE_EVENTID(DwUngrabKeyboardEvent)
public:
protected:
private:
};
DW_REGISTER_CLASS(DwUngrabKeyboardEvent);


class DW_GUI_EXPORT DwKeyboardGainEvent : public DwKeyboardGraberBaseEvent
{
    DW_DECLARE_EVENTID(DwKeyboardGainEvent)
public:
protected:
private:
};
DW_REGISTER_CLASS(DwKeyboardGainEvent);
#endif

//////////////////////////////////////////////////////////////////////////TODO////////////////////////////////////
//////////////////////////////////////////////////////////////////////////TODO////////////////////////////////////
//////////////////////////////////////////////////////////////////////////TODO////////////////////////////////////
class DW_GUI_EXPORT DwTimerEvent : public DwSceneEvent
{
    DW_DECLARE_EVENTID(DwTimerEvent);
public:
    DwTimerEvent(unsigned int timerId = 0, DwWindow *window = NULL);
    virtual ~DwTimerEvent();

    unsigned int timeId() const;
    void setTimerId(unsigned int timerId);

    unsigned int m_timerId;
    DwRttiObjectPtr<DwWindow> m_window;
protected:
private:
};
DW_REGISTER_CLASS(DwTimerEvent);

//////////////////////////////////////////////////////////////////////////
class DW_GUI_EXPORT DwWinEventBase : public DwSceneEvent
{
    DW_DECLARE_EVENTID(DwWinEventBase);
public:
    virtual DwGraphicsItem* indexInterestItemFromScene(const DwGraphicsScene *scene);
};
DW_REGISTER_CLASS(DwWinEventBase);

// NB! THIS EVENT DILIVER DOWN
class DW_GUI_EXPORT DwResizeEvent : public DwWinEventBase
{
    DW_DECLARE_EVENTID(DwResizeEvent);
public:
    // NB! DO NOT CHANGE THE ORDER OF RESIZEPATTERN
    enum ResizePattern
    {
        Restored,	// Mapping to SIZE_RESTORED
        Minimized,	// Mapping to SIZE_MINIMIZED
        Maximized,	// Mapping to SIZE_MAXIMIZED
        MaxShow,	// Mapping to SIZE_MAXSHOW
        MaxHide,	// Mapping to SIZE_MAXHIDE
    };

    DwResizeEvent(ResizePattern pattern = Restored, const DwSizeF& oldOne = DwSizeF(), const DwSizeF& newOne = DwSizeF());
    virtual ~DwResizeEvent();

    virtual bool isInterestedBy(const DwGraphicsItem* item);

    int m_hwnd;
    ResizePattern m_pattern;
    DwSizeF m_oldSize;
    DwSizeF m_newSize;
    DwRect m_clientRect;
protected:
private:
};
DW_REGISTER_CLASS(DwResizeEvent);

//////////////////////////////////////////////////////////////////////////
class DW_GUI_EXPORT DwMoveEvent : public DwWinEventBase
{
    DW_DECLARE_EVENTID(DwMoveEvent);
public:

    DwRect m_clientRect;
protected:
private:
};
DW_REGISTER_CLASS(DwMoveEvent);

//////////////////////////////////////////////////////////////////////////
class DW_GUI_EXPORT DwWinOnShowEvent : public DwWinEventBase
{
    DW_DECLARE_EVENTID(DwWinOnShowEvent);
public:
    DwWinOnShowEvent();
    int m_winId;
    DwRttiObjectPtr<DwWindow> m_window;
    Dw::ShowCommand m_showCommand;
protected:
private:
};
DW_REGISTER_CLASS(DwWinOnShowEvent);

#if DW_ENABLE_EVENT_VISIBLE
//////////////////////////////////////////////////////////////////////////
// NB! THIS EVENT DILIVER DOWN
class DW_GUI_EXPORT DwVisibleEvent : public DwSceneEvent
{
    DW_DECLARE_EVENTID(DwVisibleEvent);
public:
    DwVisibleEvent();
    virtual ~DwVisibleEvent();

    virtual bool isInterestedBy(const DwGraphicsItem* item);
};
DW_REGISTER_CLASS(DwVisibleEvent);

class DW_GUI_EXPORT DwShowEvent : public DwVisibleEvent
{
    DW_DECLARE_EVENTID(DwShowEvent);
public:
protected:
private:
};
DW_REGISTER_CLASS(DwShowEvent);

class DW_GUI_EXPORT DwHideEvent : public DwVisibleEvent
{
    DW_DECLARE_EVENTID(DwHideEvent);
public:
protected:
private:
};
DW_REGISTER_CLASS(DwHideEvent);
#endif

//////////////////////////////////////////////////////////////////////////
class DW_GUI_EXPORT DwWinEvent : public DwWinEventBase
{
    DW_DECLARE_EVENTID(DwWinEvent);
public:
    DwWinEvent();

    long m_winId;
protected:
private:
};
DW_REGISTER_CLASS(DwWinEvent);

class DW_GUI_EXPORT DwNcHitTestEvent : public DwWinEvent
{
    DW_DECLARE_EVENTID(DwNcHitTestEvent);

public:
    DwNcHitTestEvent();

    virtual void translatedToScene(const DwGraphicsView* view);
    virtual void translatedToItem(const DwGraphicsItem* item);

    virtual DwGraphicsItem* indexInterestItemFromScene(const DwGraphicsScene *scene);

    // if the item is reached the require
    virtual bool isInterestedBy(const DwGraphicsItem* item);

    //DwPoint m_mousePoint;
    DwRect m_clientRect;    // client rect
    DwPoint m_screenPoint;  // screen coordinate
    DwPoint m_devicePoint;  // device coordinate
    DwPoint m_viewPoint;    // view coordinate
    DwPoint m_scenePoint;   // scene coordinate
    DwPoint m_itemPoint;    // item coordinate

    long m_result;
    bool m_searched;
protected:
private:
};
DW_REGISTER_CLASS(DwNcHitTestEvent);

#if DW_ENABLE_EVENT_COMMAND
//////////////////////////////////////////////////////////////////////////
class DW_GUI_EXPORT DwCommandEvent : public DwWinEventBase
{
    DW_DECLARE_EVENTID(DwCommandEvent);
public:
    DwCommandEvent();

    int m_winId;
protected:
private:
};
DW_REGISTER_CLASS(DwCommandEvent);

class DW_GUI_EXPORT DwWindowCreateEvent : public DwCommandEvent
{
    DW_DECLARE_EVENTID(DwWindowCreateEvent);
public:
    DwWindowCreateEvent(DwWindow* window = NULL);
    virtual ~DwWindowCreateEvent();

    DwRttiObjectPtr<DwWindow> m_window;
protected:
private:
};
DW_REGISTER_CLASS(DwWindowCreateEvent);

class DW_GUI_EXPORT DwWindowDestroyEvent : public DwCommandEvent
{
    DW_DECLARE_EVENTID(DwWindowDestroyEvent);
public:
    DwWindowDestroyEvent(DwWindow* window = NULL);
    virtual ~DwWindowDestroyEvent();

    DwRttiObjectPtr<DwWindow> m_window;
protected:
private:
};
DW_REGISTER_CLASS(DwWindowDestroyEvent);

class DW_GUI_EXPORT DwWindowClosedEvent : public DwCommandEvent
{
    DW_DECLARE_EVENTID(DwWindowClosedEvent);
public:
    DwWindowClosedEvent(DwWindow* window = NULL);
    virtual ~DwWindowClosedEvent();

    DwRttiObjectPtr<DwWindow> m_window;
protected:
private:
};
DW_REGISTER_CLASS(DwWindowClosedEvent);

class DW_GUI_EXPORT DwCursorChangeEvent : public DwCommandEvent
{
    DW_DECLARE_EVENTID(DwCursorChangeEvent);
public:
protected:
private:
};
DW_REGISTER_CLASS(DwCursorChangeEvent);

class DW_GUI_EXPORT DwContexMenuEvent : public DwCommandEvent
{
    DW_DECLARE_EVENTID(DwContexMenuEvent);
public:
protected:
private:
};
DW_REGISTER_CLASS(DwContexMenuEvent);

class DW_GUI_EXPORT DwQuitEvent : public DwCommandEvent
{
    DW_DECLARE_EVENTID(DwQuitEvent);
public:
protected:
private:
};
DW_REGISTER_CLASS(DwQuitEvent);
#endif

#if DW_ENABLE_EVENT_WIN_COMMAND
//////////////////////////////////////////////////////////////////////////
class DW_GUI_EXPORT DwWinCommandEventBase : public DwWinEventBase
{
    DW_DECLARE_EVENTID(DwWinCommandEventBase);
public:
    DwWinCommandEventBase();

    HWND m_hWnd;
    WPARAM m_wparam;
    LPARAM m_lparam;
    LRESULT m_lResult;
    BOOL m_handled;
protected:
private:
};
DW_REGISTER_CLASS(DwWinCommandEventBase);

class DW_GUI_EXPORT DwWinCommandEvent : public DwWinCommandEventBase
{
    DW_DECLARE_EVENTID(DwWinCommandEvent);
public:
protected:
private:
};
DW_REGISTER_CLASS(DwWinCommandEvent);

class DW_GUI_EXPORT DwWinSysCommandEvent : public DwWinCommandEventBase
{
    DW_DECLARE_EVENTID(DwWinSysCommandEvent);
public:

protected:
private:
};
DW_REGISTER_CLASS(DwWinSysCommandEvent);
#endif

#if DW_ENABLE_EVENT_TIPS
//////////////////////////////////////////////////////////////////////////
class DW_GUI_EXPORT DwTipsEvent : public DwSceneEvent
{
    DW_DECLARE_EVENTID(DwTipsEvent);
public:
    virtual bool isInterestedBy(const DwGraphicsItem* item);

    DwString m_tips;
protected:
private:
};
DW_REGISTER_CLASS(DwTipsEvent);

class DW_GUI_EXPORT DwToolTipEvent : public DwTipsEvent
{
    DW_DECLARE_EVENTID(DwToolTipEvent);
public:
protected:
private:
};
DW_REGISTER_CLASS(DwToolTipEvent);

class DW_GUI_EXPORT DwWhatsThisEvent : public DwTipsEvent
{
    DW_DECLARE_EVENTID(DwWhatsThisEvent);
public:
protected:
private:
};
DW_REGISTER_CLASS(DwWhatsThisEvent);

class DW_GUI_EXPORT DwStatusTipEvent : public DwTipsEvent
{
    DW_DECLARE_EVENTID(DwStatusTipEvent);
public:
protected:
private:
};
DW_REGISTER_CLASS(DwStatusTipEvent);

//////////////////////////////////////////////////////////////////////////
class DW_GUI_EXPORT DwVisibleTipsEvent : public DwTipsEvent
{
    DW_DECLARE_EVENTID(DwVisibleTipsEvent);
public:
    DwVisibleTipsEvent(DwGraphicsItem* targetItem = NULL);
    virtual ~DwVisibleTipsEvent();

    DwRttiObjectPtr<DwGraphicsItem> m_targetItem;
protected:
private:
};
DW_REGISTER_CLASS(DwVisibleTipsEvent);

class DW_GUI_EXPORT DwShowToolTipEvent : public DwVisibleTipsEvent
{
    DW_DECLARE_EVENTID(DwShowToolTipEvent);
public:
    DwShowToolTipEvent(DwGraphicsItem* targetItem = NULL) : DwVisibleTipsEvent(targetItem) { }
protected:
private:
};
DW_REGISTER_CLASS(DwShowToolTipEvent);

class DW_GUI_EXPORT DwHideToolTipEvent : public DwVisibleTipsEvent
{
    DW_DECLARE_EVENTID(DwHideToolTipEvent);
public:
    DwHideToolTipEvent(DwGraphicsItem* targetItem = NULL) : DwVisibleTipsEvent(targetItem) { }
protected:
private:
};
DW_REGISTER_CLASS(DwHideToolTipEvent);
#endif

//////////////////////////////////////////////////////////////////////////
class DW_GUI_EXPORT DwPaintEvent : public DwSceneEvent
{
    DW_DECLARE_EVENTID(DwPaintEvent);
public:
    DwPaintEvent();
    virtual ~DwPaintEvent();

    DwRttiObjectPtr<DwWindow> m_window;
    DwRect m_deviceRect;
protected:
private:
};
DW_REGISTER_CLASS(DwPaintEvent);

//////////////////////////////////////////////////////////////////////////
class DW_GUI_EXPORT DwEraseBkgndEvent : public DwSceneEvent
{
    DW_DECLARE_EVENTID(DwEraseBkgndEvent);
public:
    DwEraseBkgndEvent();
    virtual ~DwEraseBkgndEvent();

    DwRttiObjectPtr<DwWindow> m_window;
    DwRectF m_deviceRect;
protected:
private:
};
DW_REGISTER_CLASS(DwEraseBkgndEvent);

//////////////////////////////////////////////////////////////////////////
class DW_GUI_EXPORT DwMouseNcToClientEvent : public DwSceneEvent
{
    DW_DECLARE_EVENTID(DwMouseNcToClientEvent);
public:
protected:
private:
};
DW_REGISTER_CLASS(DwMouseNcToClientEvent);

//////////////////////////////////////////////////////////////////////////
class DW_GUI_EXPORT DwWindowEvent : public DwWinEventBase
{
    DW_DECLARE_EVENTID(DwWindowEvent);
public:
    DwWindowEvent();

    void assignWin(int winId, unsigned int msg, unsigned int wp, unsigned int lp);

    int m_winId;
    unsigned int m_msg;
    unsigned int m_wparam;
    unsigned long m_lparam;
    long m_result;
};
DW_REGISTER_CLASS(DwWindowEvent);

//////////////////////////////////////////////////////////////////////////
class DW_GUI_EXPORT DwWindowActiveEvent : public DwWindowEvent
{
    DW_DECLARE_EVENTID(DwWindowActiveEvent);
public:
    DwWindowActiveEvent();

    bool m_actived;
};
DW_REGISTER_CLASS(DwWindowActiveEvent);

//////////////////////////////////////////////////////////////////////////
class DW_GUI_EXPORT DwUpdateHoverEvent : public DwSceneEvent
{
    DW_DECLARE_EVENTID(DwUpdateHoverEvent);
public:
    DwUpdateHoverEvent() : m_mousEvt(NULL)
    {
        ;
    }

    DwMouseEvent *m_mousEvt;
};
DW_REGISTER_CLASS(DwUpdateHoverEvent);