#pragma once
#include "dwcore/dwobject.h"
#include "dwcore/dwvector.h"
#include "dwgraphics/dwtransform.h"
#include "dwgraphics/dwcolor.h"
#include "dwgraphics/dwregion.h"
#include "dwgui/dwrttiobject.h"
#include "dwgui/dwrttiobjectptr.h"

//------------------------------------------------------------------------------
class DwGraphicsItem;
class DwGraphicsScene;
class DwPainter;
class DwEvent;
class DwSceneEvent;
class DwWindow;
class DwViewEventHandler;
class DwWindowAdapter;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwGraphicsView : public DwObject , public DwRttiObject
{
    DW_DECLARE_CLASS(DwGraphicsView);
public:
    DwGraphicsView(DwObject * parent = NULL);
    explicit DwGraphicsView(DwGraphicsScene* scene, DwObject * parent = NULL, DwWindow* w = NULL);
    virtual ~DwGraphicsView();

    // Window Stuffs
    DwWindow* window() const;
    void setWindow(DwWindow* window);
    inline bool isOwnerdWindow() const;

    DwColor backgroundColor() const;
    void setBackgroundColor(const DwColor& color);

    void show(Dw::ShowCommand nCmd = Dw::SW_Normal, const DwRect& r = DwRect(0, 0, 640, 480));

    // Scene Stuffs
    DwGraphicsScene *scene() const;
    void setScene(DwGraphicsScene *scene);

    DwRectF sceneRect() const;
    void setSceneRect(const DwRectF &rect);
    inline void setSceneRect(DwReal x, DwReal y, DwReal w, DwReal h);

    void scrollHeight(DwReal h);
    void scrollWidth(DwReal w);

    // View port Stuffs
    DwRectF viewPortRect() const;
    void setViewPortRect(const DwRectF &rect);
    inline void setViewPortRect(DwReal x, DwReal y, DwReal w, DwReal h);

    const DwTransform& sceneTransform() const;
    const DwTransform& deviceTransform() const;

    DwPointF mapToScene(const DwPointF &point) const;
    DwPoint mapToScene(const DwPoint &point) const;
    DwRectF mapToScene(const DwRectF &rect) const;
    DwRect mapToScene(const DwRect &rect)const;

    DwPointF mapFromScene(const DwPointF &point) const;
    DwRectF mapFromScene(const DwRectF &rect) const;
    DwRect mapFromScene(const DwRect &rect)const;

    DwPointF mapToDevice(const DwPointF &point) const;
    DwPoint mapToDevice(const DwPoint &point) const;
    DwRectF mapToDevice(const DwRectF &rect) const;
    DwRect mapToDevice(const DwRect &rect) const;

    DwPointF mapFromDevice(const DwPointF &point) const;
    DwPoint mapFromDevice(const DwPoint &point) const;
    DwRectF mapFromDevice(const DwRectF &rect) const;
    DwRect mapFromDevice(const DwRect &rect) const;

    DwPointF mapToGlobal(const DwPointF &point) const;
    DwPoint mapToGlobal(const DwPoint &point) const;
    DwRectF mapToGlobal(const DwRectF &rect) const;
    DwRect mapToGlobal(const DwRect &rect) const;

    DwPointF mapFromGlobal(const DwPointF &point) const;
    DwPoint mapFromGlobal(const DwPoint &point) const;
    DwRectF mapFromGlobal(const DwRectF &rect) const;
    DwRect mapFromGlobal(const DwRect &rect) const;

    // Render Stuffs
    void render(DwPainter *painter, const DwRectF &target = DwRectF(), const DwRectF &source = DwRectF(),
                Dw::AspectRatioMode aspectRatioMode = Dw::KeepAspectRatio);

    void drawItems(DwPainter *painter);

    void drawViewPort(DwPainter *painter, const DwRectF &rect);
    void drawSceneRegion(const DwRegion& region);

    // Item Index Stuffs
    DwGraphicsItem *itemAt(const DwPoint &pos) const;
    DwVector<DwGraphicsItem *> items(const DwPoint &pos) const;
    DwVector<DwGraphicsItem *> items(const DwRect &rect, Dw::ItemSelectionMode mode = Dw::IntersectsItemShape) const;
    inline DwGraphicsItem *itemAt(int x, int y) const;

    // Event Stuffs
    bool sendEvent(DwSceneEvent *evt);
    bool sendEvent(DwGraphicsItem* item, DwSceneEvent *evt);
    DwViewEventHandler *eventHandler() const;
    void setEventHandler(DwViewEventHandler *handler);

    DwGraphicsItem* lastHoverItem() const;
    DwGraphicsItem* lastFocusItem() const;
    DwGraphicsItem* mouseGrabItem() const;
    void setMouseGrabItem(DwGraphicsItem* item);
    DwGraphicsItem* keyBoardGrabItem() const;
    void setKeyBoardGrabItem(DwGraphicsItem* item);

    // Dirty Rectange Stuffs
    void updateScene(const DwVector<DwRectF> &rects); // in scene coordinate
    void invalidateScene(const DwRectF &rect = DwRectF(), Dw::SceneLayer layers = Dw::AllLayers);// in scene coordinate
    void updateSceneRect(const DwRectF &rect); // in scene coordinate
    bool updateRect(const DwRect &rect);// in device coordinate

    // Level Relations
    void setIndicator(Dw::ViewIndicator indi, const DwGraphicsItem* item);
    DwGraphicsItem *indicator(Dw::ViewIndicator indi) const;
    DwGraphicsItem *windowItem() const;

    // Window Adapter
    DwWindowAdapter* adapter() const;

    // Hit Test
    bool hitTest(const DwPoint &p) const;

    // control flags
    bool requireRawMessage() const;
    void setRequireRawMessage(bool b);

    // Look up view
    static DwGraphicsView* viewFromPoint(const DwPoint &p);
    static DwGraphicsView* viewFromPoint(int x, int y);
    static DwGraphicsView* viewFromWinId(int winId);

protected:
    // Event Stuffs
    bool viewportEvent(DwEvent *evt);

    // Tools
    DwRectF viewportRectWithPainter(DwPainter *painter) const;

private:
    // calcute the transform
    void updateTransform();
    // auto resize the viewport and the scene rect to the window
    void adjustPortFromWindow();

    // Packed 32 bits
    uint32_t m_identitySceneMatrix : 1;
    uint32_t m_identityDeviceMatrix : 1;
    uint32_t m_selfCreateWindow : 1;
    uint32_t m_fullUpdatePending : 1;
    uint32_t m_requireRawMessage : 1;

    DwGraphicsScene* m_scene;
    DwWindow* m_window;
    DwViewEventHandler* m_eventHandler;

    DwRectF m_viewPortRect;	// in device coordinate system
    DwRectF m_sceneRect;    // in scene coordinate system;
    DwTransform m_sceneTransform;	// transform from the view to scene
    DwTransform m_deviceTransform;	// transform from the view to device
    Dw::AspectRatioMode m_aspectRatio;

    // used for dirty rectanges [opt: scene coordinate]
    DwColor m_backgroundColor;
    Dw::ViewportUpdateMode m_viewportUpdateMode;
    DwRect m_dirtyBoundingRect;
    DwRegion m_dirtyRegion;
    DwVector<DwRect> m_dirtyRects;

    // used for build relation in item's level
    DwRttiObjectPtr<DwGraphicsItem> m_indicators[Dw::ViewIndicatorMax];
};// end of DwGraphicsView

//------------------------------------------------------------------------------
/**
*/
inline bool DwGraphicsView::isOwnerdWindow() const
{
    return m_selfCreateWindow;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwGraphicsView::setSceneRect(DwReal x, DwReal y, DwReal w, DwReal h)
{
    setSceneRect(DwRectF(x, y, w, h));
}

//------------------------------------------------------------------------------
/**
*/
inline void DwGraphicsView::setViewPortRect(DwReal x, DwReal y, DwReal w, DwReal h)
{
    setViewPortRect(DwRectF(x, y, w, h));
}

//------------------------------------------------------------------------------
/**
*/
inline DwGraphicsItem *DwGraphicsView::itemAt(int x, int y) const
{
    return itemAt(DwPoint(x, y));
}

//------------------------------------------------------------------------------
/**
*/
inline bool DwGraphicsView::requireRawMessage() const
{
    return m_requireRawMessage;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwGraphicsView::setRequireRawMessage(bool b)
{
    m_requireRawMessage = b;
}