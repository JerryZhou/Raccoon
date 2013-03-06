
//////////////////////////////////////////////////////////////////////////
class SceneEventTracer
{
public:
    SceneEventTracer()
    {
        ;
    }

    void begin(const DwRttiObject *route, DwEvent * evt)
    {
#ifdef _DEBUG
        if (route && evt)
        {
            {
                DwDebug() << L"Begin Event : " << evt->eventId()->name() << route->rtti()->name() <<L" Dispatch Trace Information";
            }
        }
#else
        DW_UNUSED(route);
        DW_UNUSED(evt);
#endif
    }

    void append(const DwGraphicsItem * item, DwEvent * evt)
    {
#ifdef _DEBUG
        if (item && evt && evt->isTraced() && evt->isA(DwSceneEvent::EventId()))
        {
            DwSceneEvent * sceneEvt = (DwSceneEvent*)(evt);
            int level = 0;
            DwDebug stream;
            if (evt->isUp())
            {
                level = sceneEvt->dispatchUpLevel();
            }
            else
            {
                level = sceneEvt->dispatchDownLevel();
            }

            for (int i= 0; i<level; ++i)
            {
                stream << L'\t';
            }
            item->debugString(stream);
        }
#else
        DW_UNUSED(item);
        DW_UNUSED(evt);
#endif
    }

    void append(const DwGraphicsScene * scene, DwEvent * evt)
    {
#ifdef _DEBUG
        if (scene && evt )
        {
            DwDebug stream;
            stream << L"In Scene Handler --> ";
            scene->debugString(stream);
        }
#else
        DW_UNUSED(scene);
        DW_UNUSED(evt);
#endif
    }

    void append(const DwGraphicsView * view, DwEvent * evt)
    {
#ifdef _DEBUG
        if (view && evt)
        {
            DwDebug stream;
            stream << L"In View Handler --> ";
            view->debugString(stream);
        }
#else
        DW_UNUSED(view);
        DW_UNUSED(evt);
#endif
    }

    void end(const DwRttiObject *route, DwEvent * evt)
    {
#ifdef _DEBUG
        if (evt && route)
        {
            {
                DwDebug() << L"End Event : " << evt->eventId()->name() << route->rtti()->name() <<L" Dispatch Trace Information";
            }
        }
#else
        DW_UNUSED(route);
        DW_UNUSED(evt);
#endif
    }
};
DW_GLOBAL_STATIC(SceneEventTracer, _dw_SceneEventTracer);

class RouteTracer
{
public:
    RouteTracer(const DwRttiObject *route, DwEvent * evt)
        : m_evt(NULL)
    {
        m_route = (DwRttiObject *)route;
        m_evt = evt;

        if (m_evt && m_evt->isTraced() )
        {
            if ( m_route->isA(DwGraphicsView::RTTI())
                    || m_route->isA(DwGraphicsScene::RTTI()) )
            {
                _dw_SceneEventTracer()->begin(m_route, m_evt);
            }

            _dw_SceneEventTracer()->append(dwsafe_cast<DwGraphicsView>(m_route), evt);
            _dw_SceneEventTracer()->append(dwsafe_cast<DwGraphicsScene>(m_route), evt);
            _dw_SceneEventTracer()->append(dwsafe_cast<DwGraphicsItem>(m_route), evt);
        }
    }
    ~RouteTracer()
    {
        if (m_evt && m_evt->isTraced() )
        {
            if ( m_route->isA(DwGraphicsView::RTTI())
                    || m_route->isA(DwGraphicsScene::RTTI()) )
            {
                _dw_SceneEventTracer()->end(m_route, m_evt);

                if (m_evt->isAccepted())
                {
                    ;
                }
            }
        }
    }
protected:
    DwEvent * m_evt;
    DwRttiObject *m_route;
};

#ifdef _DEBUG
#define DW_EVENT_TRACE(route, evt) \
        RouteTracer router(route, evt)
#else
#define DW_EVENT_TRACE(route, evt) evt
#endif

//////////////////////////////////////////////////////////////////////////