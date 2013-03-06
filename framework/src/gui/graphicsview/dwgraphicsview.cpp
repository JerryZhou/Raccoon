#include "stable.h"

#include "dwdummys.h"
#include "dwgraphicsview.h"
#include "dwgraphicsscene.h"
#include "dwgraphicsitem.h"
#include "dwgraphicseventdispatch.h"
#include "dwgraphicsevent.h"
#include "dwgraphicseventhandler.h"
#include "dwgraphicsutil.h"
#include "dwgraphics/dwpainter.h"
#include "dwgraphics/dwregion.h"
#include "dwgui/dwwindowadapter.h"
#include "dwgui/dwwindow.h"

DW_IMPLEMENT_CLASS(DwGraphicsView, 'DGVW', DwRttiObject);

//------------------------------------------------------------------------------
/**
*/
DwGraphicsView::DwGraphicsView(DwObject * parent/* = NULL*/)
    : DwObject(parent)
    , m_scene(NULL)
    , m_identitySceneMatrix(1)
    , m_identityDeviceMatrix(1)
    , m_selfCreateWindow(0)
    , m_requireRawMessage(0)
    , m_aspectRatio(Dw::KeepAspectRatio)
    , m_window(NULL)
    , m_backgroundColor(0, 0, 0, 0)
    , m_eventHandler(NULL)
    , m_viewportUpdateMode(Dw::SmartViewportUpdate)
    , m_fullUpdatePending(0)
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsView::DwGraphicsView(DwGraphicsScene* scene, DwObject * parent/* = NULL*/, DwWindow* w/* = NULL*/)
    : DwObject(parent)
    , m_scene(NULL)
    , m_identitySceneMatrix(1)
    , m_identityDeviceMatrix(1)
    , m_selfCreateWindow(0)
    , m_aspectRatio(Dw::KeepAspectRatio)
    , m_window(NULL)
    , m_backgroundColor(0, 0, 0, 0)
    , m_eventHandler(NULL)
    , m_viewportUpdateMode(Dw::SmartViewportUpdate)
    , m_fullUpdatePending(0)
{
    setScene(scene);
    setWindow(w);
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsView::~DwGraphicsView()
{
    setEventHandler(NULL);
    setScene(NULL);
    setWindow(NULL);
}

//------------------------------------------------------------------------------
/**
*/
DwColor DwGraphicsView::backgroundColor() const
{
    return m_backgroundColor;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsView::setBackgroundColor(const DwColor& color)
{
    m_backgroundColor = color;
}

//------------------------------------------------------------------------------
/**
*/
DwWindow* DwGraphicsView::window() const
{
    return m_window;
}

//------------------------------------------------------------------------------
/**
	TODO: multiple device support
*/
void DwGraphicsView::setWindow(DwWindow* w)
{
    if (m_window == w)
    {
        return;
    }

    if (m_window)
    {
        m_window->setView(NULL);
        if (m_selfCreateWindow)
        {
            delete m_window;
            m_selfCreateWindow = 0;
        }
    }

    m_window = w;
    if (w)
    {
        w->setView(this);
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsView::show(Dw::ShowCommand nCmd/* = Dw::SW_Normal*/, const DwRect& r/* = DwRect(0, 0, 640, 480)*/)
{
    if (m_window)
    {
        m_window->show(nCmd, r);
    }
    else
    {
        setWindow(new DwWindow());
        m_selfCreateWindow = 1;
        m_window->show(nCmd, r);
    }

    adjustPortFromWindow();
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsScene *DwGraphicsView::scene() const
{
    return m_scene;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsView::setScene(DwGraphicsScene *scene)
{
    if (m_scene == scene)
    {
        return;
    }
    if (m_scene)
    {
        m_scene->m_views.removeOne(this);
    }

    m_scene = scene;
    if (scene)
    {
        scene->m_views.append(this);
    }
}

//------------------------------------------------------------------------------
/**
*/
DwRectF DwGraphicsView::sceneRect() const
{
    if (m_sceneRect.isNull())
    {
        const_cast<DwGraphicsView*>(this)->setSceneRect(m_scene->sceneRect());
    }
    return m_sceneRect;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsView::setSceneRect(const DwRectF &rect)
{
    if (m_sceneRect != rect)
    {
        m_sceneRect = rect;
        updateTransform();
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsView::scrollHeight(DwReal h)
{
    setSceneRect(sceneRect().adjusted(0, h, 0, h));
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsView::scrollWidth(DwReal w)
{
    setSceneRect(sceneRect().adjusted(w, 0, w, 0));
}

//------------------------------------------------------------------------------
/**
*/
DwRectF DwGraphicsView::viewPortRect() const
{
    return m_viewPortRect;
}

//------------------------------------------------------------------------------
/**
	set the viewport as the rect : do clip
*/
void DwGraphicsView::setViewPortRect(const DwRectF &rect)
{
    if (m_viewPortRect != rect)
    {
        m_viewPortRect = rect;
        updateTransform();
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsView::updateTransform()
{
    // size safe check
    DwRectF targetRect = m_viewPortRect; // viewport
    DwRectF sourceRect = m_sceneRect;  // scenerect

    if (sourceRect.isNull() || targetRect.isNull())
    {
        return;
    }

    m_identityDeviceMatrix = 0;
    m_identitySceneMatrix = 0;

    // Find the ideal x / y scaling ratio to fit \a source into \a target.
    // The same algothrim as in render
    DwReal xratio = targetRect.width() / sourceRect.width();
    DwReal yratio = targetRect.height() / sourceRect.height();

    // Scale according to the aspect ratio mode.
    switch (m_aspectRatio)
    {
    case Dw::KeepAspectRatio:
        xratio = yratio = dwMin(xratio, yratio);
        break;
    case Dw::KeepAspectRatioByExpanding:
        xratio = yratio = dwMax(xratio, yratio);
        break;
    case Dw::IgnoreAspectRatio:
        break;
    }

    // Setup painter matrix.
    DwTransform painterMatrix;
    painterMatrix *= DwTransform()
                     .translate(targetRect.left(), targetRect.top()) // view to device
                     .scale(xratio, yratio)
                     .translate(-sourceRect.left(), -sourceRect.top()); // scene to view

    m_deviceTransform = DwTransform::fromTranslate(targetRect.left(), targetRect.top()); // view to device
    m_sceneTransform = DwTransform().translate(sourceRect.left(), sourceRect.top()).scale(1.0 / xratio, 1.0 / yratio);
}

//------------------------------------------------------------------------------
/**
	transform the coord from view to scene
*/
const DwTransform& DwGraphicsView::sceneTransform() const
{
    return m_sceneTransform;
}

//------------------------------------------------------------------------------
/**
	transform the coord from the view to device
*/
const DwTransform& DwGraphicsView::deviceTransform() const
{
    return m_deviceTransform;
}

//------------------------------------------------------------------------------
/**
	transform the point from the view to scene
*/
DwPointF DwGraphicsView::mapToScene(const DwPointF &point) const
{
    if (m_identitySceneMatrix)
    {
        return point;
    }
    else
    {
        return m_sceneTransform.map(point);
    }
}

//------------------------------------------------------------------------------
/**
	transform the point from the view to scene
*/
DwPoint DwGraphicsView::mapToScene(const DwPoint &point) const
{
    if (m_identitySceneMatrix)
    {
        return point;
    }
    else
    {
        return m_sceneTransform.map(point);
    }
}

//------------------------------------------------------------------------------
/**
	transform the rect from the view to scene
*/
DwRectF DwGraphicsView::mapToScene(const DwRectF &rect) const
{
    if (m_identitySceneMatrix)
    {
        return rect;
    }
    else
    {
        return m_sceneTransform.mapRect(rect);
    }
}

//------------------------------------------------------------------------------
/**
	transform the rect from the view to scene
*/
DwRect DwGraphicsView::mapToScene(const DwRect &rect)const
{
    if (m_identitySceneMatrix)
    {
        return rect;
    }
    else
    {
        return m_sceneTransform.mapRect(rect);
    }
}

//------------------------------------------------------------------------------
/**
	transform the point from the scene to view
*/
DwPointF DwGraphicsView::mapFromScene(const DwPointF &point) const
{
    if (m_identitySceneMatrix)
    {
        return point;
    }
    else
    {
        return m_sceneTransform.inverted().map(point);
    }
}

//------------------------------------------------------------------------------
/**
	transform the rect from the scene to view
*/
DwRectF DwGraphicsView::mapFromScene(const DwRectF &rect) const
{
    if (m_identitySceneMatrix)
    {
        return rect;
    }
    else
    {
        return m_sceneTransform.inverted().mapRect(rect);
    }
}

//------------------------------------------------------------------------------
/**
	transform the rect from the scene to view
*/
DwRect DwGraphicsView::mapFromScene(const DwRect &rect) const
{
    if (m_identitySceneMatrix)
    {
        return rect;
    }
    else
    {
        return m_sceneTransform.inverted().mapRect(rect);
    }
}

//------------------------------------------------------------------------------
/**
	transform the point from the view to the device
*/
DwPointF DwGraphicsView::mapToDevice(const DwPointF &point) const
{
    if (m_identityDeviceMatrix)
    {
        return point;
    }
    else
    {
        return m_deviceTransform.map(point);
    }
}

//------------------------------------------------------------------------------
/**
	transform the point from the view to the device
*/
DwPoint DwGraphicsView::mapToDevice(const DwPoint &point) const
{
    if (m_identityDeviceMatrix)
    {
        return point;
    }
    else
    {
        return m_deviceTransform.map(point);
    }
}

//------------------------------------------------------------------------------
/**
	transform the rect from the view to the device
*/
DwRectF DwGraphicsView::mapToDevice(const DwRectF &rect) const
{
    if (m_identityDeviceMatrix)
    {
        return rect;
    }
    else
    {
        return m_deviceTransform.mapRect(rect);
    }
}

//------------------------------------------------------------------------------
/**
	transform the rect from the view to the device
*/
DwRect DwGraphicsView::mapToDevice(const DwRect &rect) const
{
    if (m_identityDeviceMatrix)
    {
        return rect;
    }
    else
    {
        return m_deviceTransform.mapRect(rect);
    }
}

//------------------------------------------------------------------------------
/**
	transform the point from the device to the view
*/
DwPointF DwGraphicsView::mapFromDevice(const DwPointF &point) const
{
    if (m_identityDeviceMatrix)
    {
        return point;
    }
    else
    {
        return m_deviceTransform.inverted().map(point);
    }
}

//------------------------------------------------------------------------------
/**
	transform the point from the device to the view
*/
DwPoint DwGraphicsView::mapFromDevice(const DwPoint &point) const
{
    if (m_identityDeviceMatrix)
    {
        return point;
    }
    else
    {
        return m_deviceTransform.inverted().map(point);
    }
}

//------------------------------------------------------------------------------
/**
	transform the rect from the device to the view
*/
DwRectF DwGraphicsView::mapFromDevice(const DwRectF &rect) const
{
    if (m_identityDeviceMatrix)
    {
        return rect;
    }
    else
    {
        return m_deviceTransform.inverted().mapRect(rect);
    }
}

//------------------------------------------------------------------------------
/**
	transform the rect from the device to the view
*/
DwRect DwGraphicsView::mapFromDevice(const DwRect &rect) const
{
    if (m_identityDeviceMatrix)
    {
        return rect;
    }
    else
    {
        return m_deviceTransform.inverted().mapRect(rect);
    }
}

//------------------------------------------------------------------------------
/**
*/
DwPoint deviceToGlobal(const DwGraphicsView* view, const DwPoint& p)
{
    if (view && view->window())
    {
        HWND hWnd = (HWND)(view->window()->winId());
        if (hWnd)
        {
            tagPOINT pp = { p.x(), p.y() };
            ::ClientToScreen(hWnd, &pp);
            return DwPoint(pp.x, pp.y);
        }
    }
    return p;
}

//------------------------------------------------------------------------------
/**
*/
DwPointF deviceToGlobal(const DwGraphicsView* view, const DwPointF& p)
{
    return DwPointF(deviceToGlobal(view, p.toPoint()));
}

//------------------------------------------------------------------------------
/**
*/
DwPoint deviceFromGlobal(const DwGraphicsView* view, const DwPoint& p)
{
    if (view && view->window())
    {
        HWND hWnd = (HWND)(view->window()->winId());
        if (hWnd)
        {
            tagPOINT pp = { p.x(), p.y() };
            ::ScreenToClient(hWnd, &pp);
            return DwPoint(pp.x, pp.y);
        }
    }
    return p;
}

//------------------------------------------------------------------------------
/**
*/
DwPointF deviceFromGlobal(const DwGraphicsView* view, const DwPointF& p)
{
    return DwPointF(deviceFromGlobal(view, p.toPoint()));
}

//------------------------------------------------------------------------------
/**
*/
DwPointF DwGraphicsView::mapToGlobal(const DwPointF &point) const
{
    return deviceToGlobal(this, mapToDevice(point));
}

//------------------------------------------------------------------------------
/**
*/
DwPoint DwGraphicsView::mapToGlobal(const DwPoint &point) const
{
    return deviceToGlobal(this, mapToDevice(point));
}

//------------------------------------------------------------------------------
/**
*/
DwRectF DwGraphicsView::mapToGlobal(const DwRectF &rect) const
{
    DwRectF r = mapToDevice(rect);
    return DwRectF(deviceToGlobal(this, r.topLeft()), r.size());
}

//------------------------------------------------------------------------------
/**
*/
DwRect DwGraphicsView::mapToGlobal(const DwRect &rect) const
{
    DwRect r = mapToDevice(rect);
    return DwRect(deviceToGlobal(this, r.topLeft()), r.size());
}

//------------------------------------------------------------------------------
/**
*/
DwPointF DwGraphicsView::mapFromGlobal(const DwPointF &point) const
{
    return mapFromDevice(deviceFromGlobal(this, point));
}

//------------------------------------------------------------------------------
/**
*/
DwPoint DwGraphicsView::mapFromGlobal(const DwPoint &point) const
{
    return mapFromDevice(deviceFromGlobal(this, point));
}

//------------------------------------------------------------------------------
/**
*/
DwRectF DwGraphicsView::mapFromGlobal(const DwRectF &rect) const
{
    DwRectF r = DwRectF(deviceFromGlobal(this, rect.topLeft()), rect.size());
    return mapFromDevice(r);
}

//------------------------------------------------------------------------------
/**
*/
DwRect DwGraphicsView::mapFromGlobal(const DwRect &rect) const
{
    DwRect r = DwRect(deviceFromGlobal(this, rect.topLeft()), rect.size());
    return mapFromDevice(r);
}

//------------------------------------------------------------------------------
/**
	@target : in the painter device coordinate system
	@source : in the scene coordinate system
	NB! : no device transform
*/
void DwGraphicsView::render(DwPainter *painter, const DwRectF &target/* = DwRectF()*/, const DwRectF &source/* = DwRectF()*/,
                            Dw::AspectRatioMode aspectRatioMode/* = Dw::KeepAspectRatio*/)
{
    DW_CHECK(m_scene);
    // Default source rect = viewport rect
    DwRectF sourceRect = source;

    if (source.isNull())
        sourceRect = sceneRect();

    DwRectF targetRect = target;
    if (targetRect.isNull())
    {
        targetRect = viewportRectWithPainter(painter);
        if (targetRect.isNull())
        {
            targetRect = sourceRect;
        }
    }

    if (sourceRect.isNull())
    {
        return;
    }

    // Find the ideal x / y scaling ratio to fit \a source into \a target.
    DwReal xratio = targetRect.width() / sourceRect.width();
    DwReal yratio = targetRect.height() / sourceRect.height();

    // Scale according to the aspect ratio mode.
    switch (aspectRatioMode)
    {
    case Dw::KeepAspectRatio:
        xratio = yratio = dwMin(xratio, yratio);
        break;
    case Dw::KeepAspectRatioByExpanding:
        xratio = yratio = dwMax(xratio, yratio);
        break;
    case Dw::IgnoreAspectRatio:
        break;
    }

    // Setup painter matrix.
    DwTransform painterMatrix;
    painterMatrix *= DwTransform()
                     .translate(targetRect.left(), targetRect.top()) // view to device
                     .scale(xratio, yratio)
                     .translate(-sourceRect.left(), -sourceRect.top()); // scene to view

    if (painterMatrix.isIdentity())
    {
        m_scene->drawItems(painter, NULL, DwRegion(sourceRect.toRect()));
    }
    else
    {
        m_scene->drawItems(painter, &painterMatrix, DwRegion(sourceRect.toRect()));
    }
}

//------------------------------------------------------------------------------
/**
	painter the scene to the viewport
	NB! : no device transform
*/
void DwGraphicsView::drawItems(DwPainter *painter)
{
    render(painter, viewPortRect(), sceneRect(), m_aspectRatio);
}

//------------------------------------------------------------------------------
/**
	NB! ONLY USED TO CALLED BY PAINT EVENT
	painter the scene(which in the viewport) to the viewport
*/
void DwGraphicsView::drawViewPort(DwPainter *painter, const DwRectF &rect)
{
    DW_ASSERT(painter);
    DW_CHECK(m_scene);
    bool savedClip = false;
    DwRectF updateViewRect = rect.isNull() ? viewPortRect() : rect;

    // NB! THERE IS A PROBLEM WHEN AFTER INVALIDATE THE WINDOW, BUT THE GetUpdateRect() DO
    // NOT BE SAME AS THE INVALIDATE RECT, SO AT HERE WILL HIJACK THE UPDATE RECT
    if (!m_dirtyBoundingRect.isEmpty() && updateViewRect != viewPortRect()
            && m_dirtyBoundingRect != updateViewRect.toRect()
            && m_dirtyBoundingRect.contains(updateViewRect.toRect()))
    {
        DW_UI_LOGER() << "DwGraphicsView::drawViewPort hi jack upateRect: ";
        DW_UI_LOGER() << updateViewRect.toRect() << " to " << m_dirtyBoundingRect << "\n";
        updateViewRect = m_dirtyBoundingRect;
    }
    m_fullUpdatePending = false;
    m_dirtyBoundingRect.setRect(0, 0, 0, 0);

    if (updateViewRect != viewPortRect())
    {
        savedClip = true;
        painter->save();
        painter->setClipRect(updateViewRect.toRect());
    }

    // background
    if (m_backgroundColor.alpha() != 0)
    {
        painter->fillRect(updateViewRect.toRect(), m_backgroundColor);
    }

    // scene
    DwRectF exposedSceneRect = mapToScene(mapFromDevice(updateViewRect));

    if (!m_identitySceneMatrix || !m_identityDeviceMatrix)
    {
        DwTransform sceneToDeviceTransform = sceneTransform().inverted() * deviceTransform();
        m_scene->drawItems(painter, &sceneToDeviceTransform, DwRegion(exposedSceneRect.toRect()));
    }
    else
    {
        m_scene->drawItems(painter, NULL, DwRegion(exposedSceneRect.toRect()));
    }

    if (savedClip)
    {
        painter->restrore();
    }
}

void DwGraphicsView::drawSceneRegion(const DwRegion& region)
{
    DwWindowAdapter* ada = adapter();
    if (ada && m_scene)
    {
        DwCanvas* canvas = ada->canvas();
        bool savedClip = false;

        DwPainter painter(canvas);

        // begin paint
        // TODO: need to transform region from scene to view(canvas)
        canvas->beginePaint(region);

        DwRect boundingRect = region.boundingRect();
        if (boundingRect != sceneRect())
        {
            savedClip = true;
            painter.save();
            painter.setClip(&region);
        }

        // background
        if (m_backgroundColor.alpha() != 0)
        {
            painter.fillRegion(region, m_backgroundColor);
        }

        // draw scene
        if (!m_identitySceneMatrix || !m_identityDeviceMatrix)
        {
            DwTransform sceneToDeviceTransform = sceneTransform().inverted() * deviceTransform();
            m_scene->drawItems(&painter, &sceneToDeviceTransform, region);
        }
        else
        {
            m_scene->drawItems(&painter, NULL, region);
        }

        if (savedClip)
        {
            painter.restrore();
        }

        // end paint
        canvas->endPaint();
    }
}

//------------------------------------------------------------------------------
/**
	@pos : in view coordinate system
*/
DwGraphicsItem *DwGraphicsView::itemAt(const DwPoint &pos) const
{
    DW_ASSERT(m_scene);
    return m_scene->itemAt(mapToScene(pos));
}

//------------------------------------------------------------------------------
/**
	@pos : in view coordinate system
*/
DwVector<DwGraphicsItem *> DwGraphicsView::items(const DwPoint &pos) const
{
    DW_ASSERT(m_scene);
    return m_scene->items(mapToScene(pos), Dw::IntersectsItemShape, Dw::DescendingOrder);
}

//------------------------------------------------------------------------------
/**
	@rect : in view coordinate system
*/
DwVector<DwGraphicsItem *> DwGraphicsView::items(const DwRect &rect, Dw::ItemSelectionMode mode /*= Dw::IntersectsItemShape*/) const
{
    DW_ASSERT(m_scene);
    return m_scene->items(mapToScene(rect), mode, Dw::DescendingOrder);
}

//------------------------------------------------------------------------------
/**
*/
bool DwGraphicsView::sendEvent(DwSceneEvent *evt)
{
    dwGraphicsEventDispatch()->dispatchEvent(this, evt);

    return evt->isAccepted();
}

//------------------------------------------------------------------------------
/**
*/
bool DwGraphicsView::sendEvent(DwGraphicsItem* item, DwSceneEvent *evt)
{
    evt->setInterestItem(item);

    return sendEvent(evt);
}

//------------------------------------------------------------------------------
/**
*/
DwViewEventHandler *DwGraphicsView::eventHandler() const
{
    return m_eventHandler;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsView::setEventHandler(DwViewEventHandler *handler)
{
    if (m_eventHandler == handler)
    {
        return;
    }
    if (m_eventHandler)
    {
        delete m_eventHandler;
    }
    m_eventHandler = handler;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem* DwGraphicsView::lastHoverItem() const
{
    if (m_scene)
    {
        return m_scene->lastHoverItem();
    }
    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem* DwGraphicsView::lastFocusItem() const
{
    if (m_scene)
    {
        return m_scene->lastFocusItem();
    }
    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem* DwGraphicsView::mouseGrabItem() const
{
    if (m_scene)
    {
        return m_scene->mouseGrabItem();
    }
    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsView::setMouseGrabItem(DwGraphicsItem* item)
{
    if (m_scene)
    {
        m_scene->grabMouse(item);
    }
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem* DwGraphicsView::keyBoardGrabItem() const
{
    if (m_scene)
    {
        return m_scene->keyBoardGrabItem();
    }
    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsView::setKeyBoardGrabItem(DwGraphicsItem* item)
{
    if (m_scene)
    {
        m_scene->grabKeyboard(item);
    }
}

//------------------------------------------------------------------------------
/**
*/
static const int DWGRAPHICSVIEW_REGION_RECT_THRESHOLD = 50;
void DwGraphicsView::updateScene(const DwVector<DwRectF> &rects)
{
    if (m_fullUpdatePending || m_viewportUpdateMode == Dw::NoViewportUpdate)
        return;

    // Extract and reset dirty scene rect info.
    DwVector<DwRect> dirtySceneRects = m_dirtyRects;
    m_dirtyRects.clear();

    bool fullUpdate = m_viewportUpdateMode == Dw::FullViewportUpdate;
    bool boundingRectUpdate = (m_viewportUpdateMode == Dw::BoundingRectViewportUpdate)
                              || (m_viewportUpdateMode == Dw::SmartViewportUpdate
                                  && ((dirtySceneRects.size() + rects.size()) >= DWGRAPHICSVIEW_REGION_RECT_THRESHOLD));

    DwRect vsRect = sceneRect().toAlignedRect();
    bool redraw = false;

    // Convert scene rects to viewport rects.
    for (int i = 0; i < rects.size(); ++i)
    {
        DwRect xrect = rects.at(i).toAlignedRect();
        if (!vsRect.intersects(xrect))
        {
            continue;
        }
        dirtySceneRects.append(xrect);
    }

    for (int i = 0; i < dirtySceneRects.size(); ++i)
    {
        const DwRect &rect = dirtySceneRects.at(i);
        if (!boundingRectUpdate)
        {
            m_dirtyRegion.uniond(rect);
        }
        else
        {
            m_dirtyBoundingRect |= rect;
        }
        redraw = true;
        if (fullUpdate)
        {
            // If fullUpdate is true and we found a visible dirty rect,
            // we're done.
            break;
        }
    }

    if (!redraw)
        return;

    if (m_window)
    {
        if (fullUpdate)
        {
            m_dirtyRegion = DwRegion(sceneRect().toRect());
        }
        else if (boundingRectUpdate)
        {
            m_dirtyRegion = DwRegion(m_dirtyBoundingRect);
        }

        m_fullUpdatePending = false;
        m_dirtyBoundingRect.setRect(0, 0, 0, 0);
        DwRegion sceneRegion = m_dirtyRegion;
        drawSceneRegion(sceneRegion);
        m_dirtyRegion.clear();
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsView::invalidateScene(const DwRectF &rect/* = DwRectF()*/, Dw::SceneLayer layers/* = Dw::AllLayers*/)
{
    DW_UNUSED(layers);
    DW_UNUSED(rect);

    if (m_window)
    {
        if (sceneRect().intersects(rect))
        {
            m_scene->update(rect);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsView::updateSceneRect(const DwRectF &rect)
{
    DW_UNUSED(rect);
}

//------------------------------------------------------------------------------
/**
	NB! THE rect is in the device coordinate system
*/
bool DwGraphicsView::updateRect(const DwRect &rect)
{
    if (m_fullUpdatePending || m_viewportUpdateMode == Dw::SmartViewportUpdate
            || !viewPortRect().toRect().intersects(rect) )
    {
        return false;
    }

    DwRect sRect = mapToScene(mapFromDevice(rect));
    switch (m_viewportUpdateMode)
    {
    case Dw::FullViewportUpdate:
        m_fullUpdatePending = true;
        break;
    case Dw::BoundingRectViewportUpdate:
        m_dirtyBoundingRect = m_dirtyBoundingRect.united(sRect);
        if (m_dirtyBoundingRect.contains(sceneRect().toRect()))
        {
            m_fullUpdatePending = true;
        }
        break;
    case Dw::SmartViewportUpdate: 
        m_dirtyRects.append(rect);
    default:
        break;
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
void DwGraphicsView::setIndicator(Dw::ViewIndicator indi, const DwGraphicsItem* item)
{
    m_indicators[indi] = item;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem *DwGraphicsView::indicator(Dw::ViewIndicator indi) const
{
    return m_indicators[indi];
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem *DwGraphicsView::windowItem() const
{
    return m_indicators[Dw::WindowItem];
}

//------------------------------------------------------------------------------
/**
*/
DwWindowAdapter* DwGraphicsView::adapter() const
{
    if (m_window)
    {
        return m_window->adapter();
    }
    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
bool DwGraphicsView::hitTest(const DwPoint &p) const
{
    DwWindowAdapter *ada = adapter();
    if (ada)
    {
        return ( ada->canvas()->pixel(p) == DwColor() );
    }
    return false;
}

//------------------------------------------------------------------------------
/**
	TODO:
*/
bool DwGraphicsView::viewportEvent(DwEvent *evt)
{
    DW_UNUSED(evt);
    return false;
}

//------------------------------------------------------------------------------
/**
	return the result in device coordinate system
*/
DwRectF DwGraphicsView::viewportRectWithPainter(DwPainter *painter) const
{
    if (!m_viewPortRect.isNull())
    {
        return m_viewPortRect;
    }
    else
    {
        return DwRectF(0., 0., painter->width(), painter->height());
    }
}

//------------------------------------------------------------------------------
/**
	TODO: implement more mode
*/
void DwGraphicsView::adjustPortFromWindow()
{
    if (m_window)
    {
        if (m_viewPortRect.isNull())
        {
            setViewPortRect(DwRectF(0., 0., m_window->width(), m_window->height()));
        }
        if (m_sceneRect.isNull())
        {
            setSceneRect(DwRectF(0., 0., m_window->width(), m_window->height()));
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsView* DwGraphicsView::viewFromPoint(const DwPoint &p)
{
    DwWindow * window = DwWindow::windowFromPoint(p);
    if (window)
    {
        return window->view();
    }
    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsView* DwGraphicsView::viewFromPoint(int x, int y)
{
    DwWindow * window = DwWindow::windowFromPoint(x, y);
    if (window)
    {
        return window->view();
    }
    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsView* DwGraphicsView::viewFromWinId(int winId)
{
    DwWindow * window = DwWindow::windowFromWinId((WinId)winId);
    if (window)
    {
        return window->view();
    }
    return NULL;
}