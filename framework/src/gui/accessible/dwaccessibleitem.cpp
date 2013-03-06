#include "stable.h"
#include "dwaccessibleitem.h"
#include "dwgui/dwgraphicsscene.h"
#include "dwgui/dwgraphicsview.h"
#include "dwgui/dwgraphicsitem.h"
#include "dwgui/dwgraphicssimpletextitem.h"
#include "dwgui/dwframewindow.h"
#include "dwgui/dwapplication.h"

DW_IMPLEMENT_CLASS(DwAccessibleItem, 'DAEM', DwAccessibleObject);

#define _DOBJ_(o) ( dwsafe_cast<DwGraphicsItem>((o)) )
#define _DOBJ_Parent(o) ( (o) ? dwsafe_cast<DwGraphicsItem>(o)->parentItem() : NULL )
#define _DOBJ_Focus(o)  ( ((o) && (o)->scene()) ? (o)->scene()->focusItem() : NULL )
typedef DwVector<DwGraphicsItem*> ItemList;

//------------------------------------------------------------------------------
/**
*/
static DwPoint mapToGlobal(const DwGraphicsItem *obj, const DwPoint &p)
{
    DW_ASSERT(obj);
    if (obj){
        DwGraphicsView * view = obj->scene()->views().at(0);
        if (view){
            return view->mapToGlobal(view->mapFromScene(obj->mapToScene(p)).toPoint());
        }
    }
    return p;
}

//------------------------------------------------------------------------------
/**
*/
static DwPoint mapToGlobal(const DwGraphicsItem *obj, int x, int y)
{
    return mapToGlobal(obj, DwPoint(x, y));
}

//------------------------------------------------------------------------------
/**
*/
static DwPoint mapFromGlobal(const DwGraphicsItem *obj, const DwPoint &p)
{
    DW_ASSERT(obj);
    if (obj){
        DwGraphicsView * view = obj->scene()->views().at(0);
        if (view){
            return obj->mapFromScene(view->mapToScene(view->mapFromGlobal(p)));
        }
    }
    return p;
}

//------------------------------------------------------------------------------
/**
*/
static DwPoint mapFromGlobal(const DwGraphicsItem *obj, int x, int y)
{
    return mapFromGlobal(obj, DwPoint(x, y));
}

//------------------------------------------------------------------------------
/**
*/
static DwVector<DwGraphicsItem*> childWidgetsImp(const DwGraphicsItem *widget)
{
    ItemList list = widget->children();
    ItemList widgets;
    for (int i = 0; i < list.size(); ++i) {
        DwGraphicsItem *w = list.at(i);
        if (w && !w->isA(DwFrameWindow::RTTI())
            && w->accessible()
            && w->isVisible())
            widgets.append(w);
    }
    return widgets;
}

//------------------------------------------------------------------------------
/**
*/
static DwString buddyString(const DwGraphicsItem *widget)
{
    if (!widget)
        return DwString();
    DwGraphicsItem *parent = widget->parentItem();
    if (!parent)
        return DwString();
    // TODO: buddy

    return DwString();
}

//------------------------------------------------------------------------------
/**
*/
DwString qt_accStripAmp(const DwString &text)
{
    return DwString(text).replace(DwString(wchar_t('&')), DwString());
}

//------------------------------------------------------------------------------
/**
*/
DwString qt_accHotKey(const DwString &text)
{
    if (text.isEmpty())
        return text;

    int fa = 0;
    wchar_t ac = 0;
    while ((fa = text.indexOf('&', fa)) != -1) {
        ++fa;
        if (fa < text.length()) {
            // ignore "&&"
            if (text.at(fa) == '&') {
                ++fa;
                continue;
            } else {
                ac = text.at(fa);
                break;
            }
        }
    }
    if (!ac)
        return DwString();
    return DwString(ac).toUpper();
}

//------------------------------------------------------------------------------
/**
*/
DwAccessibleItem::DwAccessibleItem(DwGraphicsItem *w, Role role, const DwString &name)
: DwAccessibleObject(w)
{
    DW_ASSERT(widget());
    m_role = role;
    m_name = name;
    m_asking = 0;
}

//------------------------------------------------------------------------------
/**
*/
DwAccessibleItem::~DwAccessibleItem()
{
    ;
}

//------------------------------------------------------------------------------
/**
*/
DwGraphicsItem *DwAccessibleItem::widget() const
{
    return dwsafe_cast<DwGraphicsItem>(object());
}

//------------------------------------------------------------------------------
/**
*/
DwRttiObject *DwAccessibleItem::parentObject() const
{
    DwRttiObject *parent = _DOBJ_Parent(object());
    if (!parent)
        parent = dwApp();
    return parent;
}

//------------------------------------------------------------------------------
/**
*/
int DwAccessibleItem::childAt(int x, int y) const
{
    DwGraphicsItem *w = widget();
    if (!w->isVisible())
        return -1;
    DwPoint gp = mapToGlobal(w, DwPoint(0, 0));
    if (!DwRect(gp.x(), gp.y(), int(w->width()), int(w->height())).contains(x, y))
        return -1;

    ItemList list = childWidgets();
    int ccount = childCount();

    // a complex child
    if (list.size() < ccount) {
        for (int i = 1; i <= ccount; ++i) {
            if (rect(i).contains(x, y))
                return i;
        }
        return 0;
    }

    DwPoint rp = mapFromGlobal(w, DwPoint(x, y));
    for (int i = 0; i<list.size(); ++i) {
        DwGraphicsItem *child = list.at(i);
        if (!child->isA(DwFrameWindow::RTTI()) 
            && child->isVisible() 
            && child->rect().contains(rp)) {
            return i + 1;
        }
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
DwRect DwAccessibleItem::rect(int child) const
{
    if (child)
    {
        DW_ASSERT("do not use it to child" && 0);
    }
    DwGraphicsItem *w = widget();
    if (!w->isVisible())
        return DwRect();
    DwPoint wpos = mapToGlobal(w, DwPoint(0, 0));

    return DwRect(wpos.x(), wpos.y(), (int)(w->width()), (int)(w->height()));
}

//------------------------------------------------------------------------------
/**
*/
void DwAccessibleItem::addControllingSignal(const DwString &signal)
{
    m_primarySignals.append(DwString(signal));
}

//------------------------------------------------------------------------------
/**
*/
void DwAccessibleItem::setValue(const DwString &value)
{
    m_value = value;
}

//------------------------------------------------------------------------------
/**
*/
void DwAccessibleItem::setDescription(const DwString &desc)
{
    m_description = desc;
}

//------------------------------------------------------------------------------
/**
*/
void DwAccessibleItem::setHelp(const DwString &help)
{
    m_help = help;
}

//------------------------------------------------------------------------------
/**
*/
void DwAccessibleItem::setAccelerator(const DwString &accel)
{
    m_accelerator = accel;
}

//------------------------------------------------------------------------------
/**
*/
static inline bool isAncestor(const DwRttiObject *obj, const DwRttiObject *child)
{
    while (child) {
        if (child == obj)
            return true;
        child = _DOBJ_Parent(child);
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
DwAccessible::Relation DwAccessibleItem::relationTo(int child,
            const DwAccessibleInterface *other, int otherChild) const
{
    Relation relation = Unrelated;
    if (m_asking == this) // recursive call
        return relation;

    DwRttiObject *o = other ? other->object() : 0;
    if (!o)
        return relation;

    DwRttiObject *focus = _DOBJ_Focus(widget());
    if (object() == focus && isAncestor(o, focus))
        relation |= FocusChild;

    // test for passive relationships.
    // m_asking protects from endless recursion.
    const_cast<DwAccessibleItem*>(this)->m_asking = this;
    int inverse = other->relationTo(otherChild, this, child);
    const_cast<DwAccessibleItem*>(this)->m_asking = 0;

    if (inverse & Controller)
        relation |= Controlled;
    if (inverse & Label)
        relation |= Labelled;

    if(o == object()) {
        if (child && !otherChild)
            return relation | Child;
        if (!child && otherChild)
            return relation | Ancestor;
        if (!child && !otherChild)
            return relation | Self;
    }

    DwRttiObject *parent = _DOBJ_Parent(object());
    if (o == parent)
        return relation | Child;

    if (_DOBJ_Parent(o) == parent) {
        relation |= Sibling;
        DwAccessibleInterface *sibIface = DwAccessible::queryAccessibleInterface(o);
        DW_ASSERT(sibIface);
        DwRect wg = rect(0);
        DwRect sg = sibIface->rect(0);
        if (wg.intersects(sg)) {
            DwAccessibleInterface *pIface = 0;
            sibIface->navigate(Ancestor, 1, &pIface);
            if (pIface && !((sibIface->state(0) | state(0)) & Invisible)) {
                int wi = pIface->indexOfChild(this);
                int si = pIface->indexOfChild(sibIface);

                if (wi > si)
                    relation |= DwAccessible::Covers;
                else
                    relation |= DwAccessible::Covered;
            }
            delete pIface;
        } else {
            DwPoint wc = wg.center();
            DwPoint sc = sg.center();
            if (wc.x() < sc.x())
                relation |= DwAccessible::Left;
            else if(wc.x() > sc.x())
                relation |= DwAccessible::Right;
            if (wc.y() < sc.y())
                relation |= DwAccessible::Up;
            else if (wc.y() > sc.y())
                relation |= DwAccessible::Down;
        }
        delete sibIface;

        return relation;
    }

    if (isAncestor(o, object()))
        return relation | Descendent;
    if (isAncestor(object(), o))
        return relation | Ancestor;

    return relation;
}

//------------------------------------------------------------------------------
/**
*/
int DwAccessibleItem::navigate(RelationFlag relation, int entry,
                                DwAccessibleInterface **target) const
{
    if (!target)
        return -1;

    *target = 0;
    DwRttiObject *targetObject = 0;

    ItemList childList = childWidgets();
    bool complexWidget = childList.size() < childCount();

    switch (relation) {
    // Hierarchical
    case Self:
        targetObject = object();
        break;
    case Child:
        if (complexWidget) {
            if (entry > 0 && entry <= childCount())
                return entry;
            return -1;
        }else {
            if (entry > 0 && childList.size() >= entry)
                targetObject = childList.at(entry - 1);
        }
        break;
    case Ancestor:
        {
            if (entry <= 0)
                return -1;
            targetObject = widget()->parentItem();
            int i;
            for (i = entry; i > 1 && targetObject; --i)
                targetObject = _DOBJ_Parent(targetObject);
            if (!targetObject && i == 1)
                targetObject = dwApp();
        }
        break;
    case Sibling:
        {
            DwAccessibleInterface *iface = DwAccessible::queryAccessibleInterface(parentObject());
            if (!iface)
                return -1;

            iface->navigate(Child, entry, target);
            delete iface;
            if (*target)
                return 0;
        }
        break;

    // Geometrical
    case DwAccessible::Left:
        if (complexWidget && entry) {
            if (entry < 2 || widget()->height() > widget()->width() + 20) // looks vertical
                return -1;
            return entry - 1;
        }
        // fall through
    case DwAccessible::Right:
        if (complexWidget && entry) {
            if (entry >= childCount() || widget()->height() > widget()->width() + 20) // looks vertical
                return -1;
            return entry + 1;
        }
        // fall through
    case DwAccessible::Up:
        if (complexWidget && entry) {
            if (entry < 2 || widget()->width() > widget()->height() + 20) // looks horizontal
                return - 1;
            return entry - 1;
        }
        // fall through
    case DwAccessible::Down:
        if (complexWidget && entry) {
            if (entry >= childCount() || widget()->width() > widget()->height()  + 20) // looks horizontal
                return - 1;
            return entry + 1;
        } else {
            DwAccessibleInterface *pIface = DwAccessible::queryAccessibleInterface(parentObject());
            if (!pIface)
                return -1;

            DwRect startg = rect(0);
            DwPoint startc = startg.center();
            DwAccessibleInterface *candidate = 0;
            int mindist = 100000;
            int sibCount = pIface->childCount();
            for (int i = 0; i < sibCount; ++i) {
                DwAccessibleInterface *sibling = 0;
                pIface->navigate(Child, i+1, &sibling);
                DW_ASSERT(sibling);
                if ((relationTo(0, sibling, 0) & Self) || (sibling->state(0) & DwAccessible::Invisible)) {
                    //ignore ourself and invisible siblings
                    delete sibling;
                    continue;
                }

                DwRect sibg = sibling->rect(0);
                DwPoint sibc = sibg.center();
                DwPoint sibp;
                DwPoint startp;
                DwPoint distp;
                switch (relation) {
                case DwAccessible::Left:
                    startp = DwPoint(startg.left(), startg.top() + startg.height() / 2);
                    sibp = DwPoint(sibg.right(), sibg.top() + sibg.height() / 2);
                    if (DwPoint(sibc - startc).x() >= 0) {
                        delete sibling;
                        continue;
                    }
                    distp = sibp - startp;
                    break;
                case DwAccessible::Right:
                    startp = DwPoint(startg.right(), startg.top() + startg.height() / 2);
                    sibp = DwPoint(sibg.left(), sibg.top() + sibg.height() / 2);
                    if (DwPoint(sibc - startc).x() <= 0) {
                        delete sibling;
                        continue;
                    }
                    distp = sibp - startp;
                    break;
                case DwAccessible::Up:
                    startp = DwPoint(startg.left() + startg.width() / 2, startg.top());
                    sibp = DwPoint(sibg.left() + sibg.width() / 2, sibg.bottom());
                    if (DwPoint(sibc - startc).y() >= 0) {
                        delete sibling;
                        continue;
                    }
                    distp = sibp - startp;
                    break;
                case DwAccessible::Down:
                    startp = DwPoint(startg.left() + startg.width() / 2, startg.bottom());
                    sibp = DwPoint(sibg.left() + sibg.width() / 2, sibg.top());
                    if (DwPoint(sibc - startc).y() <= 0) {
                        delete sibling;
                        continue;
                    }
                    distp = sibp - startp;
                    break;
		default:
		    break;
                }

                int dist = (int)dwSqrt((DwReal)distp.x() * distp.x() + distp.y() * distp.y());
                if (dist < mindist) {
                    delete candidate;
                    candidate = sibling;
                    mindist = dist;
                } else {
                    delete sibling;
                }
            }
            delete pIface;
            *target = candidate;
            if (*target)
                return 0;
        }
        break;
    case Covers:
        if (entry > 0) {
            DwAccessibleInterface *pIface = DwAccessible::queryAccessibleInterface(parentObject());
            if (!pIface)
                return -1;

            DwRect r = rect(0);
            int sibCount = pIface->childCount();
            DwAccessibleInterface *sibling = 0;
            for (int i = pIface->indexOfChild(this) + 1; i <= sibCount && entry; ++i) {
                pIface->navigate(Child, i, &sibling);
                if (!sibling || (sibling->state(0) & Invisible)) {
                    delete sibling;
                    sibling = 0;
                    continue;
                }
                if (sibling->rect(0).intersects(r))
                    --entry;
                if (!entry)
                    break;
                delete sibling;
                sibling = 0;
            }
            delete pIface;
            *target = sibling;
            if (*target)
                return 0;
        }
        break;
    case Covered:
        if (entry > 0) {
            DwAccessibleInterface *pIface = DwAccessible::queryAccessibleInterface(parentObject());
            if (!pIface)
                return -1;

            DwRect r = rect(0);
            int index = pIface->indexOfChild(this);
            DwAccessibleInterface *sibling = 0;
            for (int i = 1; i < index && entry; ++i) {
                pIface->navigate(Child, i, &sibling);
                DW_ASSERT(sibling);
                if (!sibling || (sibling->state(0) & Invisible)) {
                    delete sibling;
                    sibling = 0;
                    continue;
                }
                if (sibling->rect(0).intersects(r))
                    --entry;
                if (!entry)
                    break;
                delete sibling;
                sibling = 0;
            }
            delete pIface;
            *target = sibling;
            if (*target)
                return 0;
        }
        break;

    // Logical
    case FocusChild:
        {
            if (widget()->hasFocus()) {
                targetObject = object();
                break;
            }

            DwGraphicsItem *fw = _DOBJ_Focus(widget());
            if (!fw)
                return -1;

            if (isAncestor(widget(), fw) || fw == widget())
                targetObject = fw;
            /* ###
            DwGraphicsItem *parent = fw;
            while (parent && !targetObject) {
                parent = parent->parentWidget();
                if (parent == widget())
                    targetObject = fw;
            }
            */
        }
        break;
    case Label:
        if (entry > 0) {
            DwAccessibleInterface *pIface = DwAccessible::queryAccessibleInterface(parentObject());
            if (!pIface)
                return -1;

            // first check for all siblings that are labels to us
            // ideally we would go through all objects and check, but that
            // will be too expensive
            int sibCount = pIface->childCount();
            DwAccessibleInterface *candidate = 0;
            for (int i = 0; i < sibCount && entry; ++i) {
                pIface->navigate(Child, i+1, &candidate);
                DW_ASSERT(candidate);
                if (candidate->relationTo(0, this, 0) & Label)
                    --entry;
                if (!entry)
                    break;
                delete candidate;
                candidate = 0;
            }
            if (!candidate) {
                if (pIface->relationTo(0, this, 0) & Label)
                    --entry;
                if (!entry)
                    candidate = pIface;
            }
            if (pIface != candidate)
                delete pIface;

            *target = candidate;
            if (*target)
                return 0;
        }
        break;
    case Labelled: // only implemented in subclasses
        break;
    case Controller:
        //TODO:xxx
        break;
    case Controlled:
        //TODO:xxx
        break;
    default:
        break;
    }

    *target = DwAccessible::queryAccessibleInterface(targetObject);
    return *target ? 0 : -1;
}

//------------------------------------------------------------------------------
/**
*/
int DwAccessibleItem::childCount() const
{
    ItemList cl = childWidgets();
    return cl.size();
}

//------------------------------------------------------------------------------
/**
*/
int DwAccessibleItem::indexOfChild(const DwAccessibleInterface *child) const
{
    ItemList cl = childWidgets();
    int index = cl.indexOf(dwsafe_cast<DwGraphicsItem>(child->object()));
    if (index != -1)
        ++index;
    return index;
}

//------------------------------------------------------------------------------
/**
*/
DwString dw_setWindowTitle_helperHelper(const DwString &title, const DwGraphicsItem* item)
{
    DwFrameWindow *w = dwsafe_cast<DwFrameWindow>(item);
    if (w)
    {
        w->setCaption(title);
        return w->caption();
    }
    return DwString();
}

//------------------------------------------------------------------------------
/**
*/
DwString DwAccessibleItem::text(Text t, int child) const
{
    DwString str;

    switch (t) {
    case Name:
        if (!m_name.isEmpty()) {
            str = m_name;
        } else if (!widget()->accessibleName().isEmpty()) {
            str = widget()->accessibleName();
        } else if (!child && widget()->isA(DwFrameWindow::RTTI())) {
            str = dwsafe_cast<DwFrameWindow>(widget())->caption();
            /*if (widget()->isMinimized())
                str = dw_setWindowTitle_helperHelper(widget()->windowIconText(), widget());
            else
                str = dw_setWindowTitle_helperHelper(widget()->windowTitle(), widget());*/
        } else {
            str = qt_accStripAmp(buddyString(widget()));
        }
        break;
    case Description:
        if (!m_description.isEmpty())
            str = m_description;
        else if (!widget()->accessibleDescription().isEmpty())
            str = widget()->accessibleDescription();
        else
            str = widget()->toolTip();
        break;
    case Help:
        if (!m_help.isEmpty())
            str = m_help;
        else
            str = widget()->whatsThis();
        break;
    case Accelerator:
        if (!m_accelerator.isEmpty())
            str = m_accelerator;
        else
            str = qt_accHotKey(buddyString(widget()));
        break;
    case Value:
        str = m_value;
        break;
    default:
        break;
    }
    return str;
}

//------------------------------------------------------------------------------
/**
*/
int DwAccessibleItem::userActionCount(int child) const
{
    if (child)
        return 0;
    //return widget()->actions().count();
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
DwString DwAccessibleItem::actionText(int action, Text t, int child) const
{
    if (action == DefaultAction)
        action = SetFocus;

    if (action > 0 && !child) {
        /*QAction *act = widget()->actions().value(action - 1);
        if (act) {
            switch (t) {
            case Name:
                return act->text();
            case Description:
                return act->toolTip();
            case Accelerator:
                return act->shortcut().toString();
            default:
                break;
            }
        }*/
    }

    return DwAccessibleObject::actionText(action, t, child);
}

//------------------------------------------------------------------------------
/**
*/
bool DwAccessibleItem::doAction(int action, int child, const DwVariantList &params)
{
    if (action == SetFocus || action == DefaultAction) {
        if (child || !widget()->isEnabled())
            return false;
        if (widget()->focusPolicy() != Dw::NoFocus)
            widget()->setFocus();
        else if (widget()->isA(DwFrameWindow::RTTI()))
            //widget()->activateWindow();
            ::SetActiveWindow((HWND)(dwsafe_cast<DwFrameWindow>(widget())->winId()));
        else
            return false;
        return true;
    } else if (action > 0) {
        /*if (QAction *act = widget()->actions().value(action - 1)) {
            act->trigger();
            return true;
        }*/
    }
    return DwAccessibleObject::doAction(action, child, params);
}

//------------------------------------------------------------------------------
/**
*/
DwVector<DwGraphicsItem*> DwAccessibleItem::childWidgets()const
{
    return childWidgetsOf(widget());
}

//------------------------------------------------------------------------------
/**
*/
DwVector<DwGraphicsItem*> DwAccessibleItem::childWidgetsOf(const DwGraphicsItem* obj)const
{
    return childWidgetsImp(obj);
}

//------------------------------------------------------------------------------
/**
*/
DwAccessible::Role DwAccessibleItem::role(int child) const
{
    if (!child)
        return m_role;

    ItemList childList = childWidgets();
    if (childList.count() > 0 && child <= childList.count()) {
        DwGraphicsItem *targetWidget = childList.at(child - 1);
        DwAccessibleInterface *iface = DwAccessible::queryAccessibleInterface(targetWidget);
        if (iface) {
            DwAccessible::Role role = iface->role(0);
            delete iface;
            return role;
        }
    }

    return NoRole;
}

//------------------------------------------------------------------------------
/**
*/
DwAccessible::State DwAccessibleItem::state(int child) const
{
    if (child)
        return Normal;

    DwAccessible::State state = Normal;

    DwGraphicsItem *w = widget();
    if (!w->isVisible())
        state |= Invisible;
    if (w->focusPolicy() != Dw::NoFocus )//&& w->isActiveWindow())
        state |= Focusable;
    if (w->hasFocus())
        state |= Focused;
    if (!w->isEnabled())
        state |= Unavailable;
    if (w->isA(DwFrameWindow::RTTI())) {
        if (w->isMovable())
            state |= Movable;
        DwFrameWindow *ww = dwsafe_cast<DwFrameWindow>(w);
        if (ww->isAttribute(Dw::WA_ResizeAble))
            state |= Sizeable;
    }

    return state;
}