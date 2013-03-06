#pragma once
#include "dwnamespace.h"
#include "dwcore/dwsize.h"
#include "dwcore/dwpoint.h"
#include "dwcore/dwmap.h"
#include "dwgraphics/dwmatrix.h"
#include "dwgui/dwanchorline.h"
#include "dwgui/dwgraphicsitemlistener.h"

//------------------------------------------------------------------------------
/**
	ANCHORLINE AND ANCHORSSCHEMATIC DIAGRAM :
		top
	------------            |		|					|
	  verticalCenter		|		|					|
	------------			| left	| HorizontalCenter  | right
	  baeOffLine			|		|					|
	-------------			|		|					|
		bottom				|		|					|
	------------			|		|					|


				top
	|-------------------------------|-------------------|
	|		  verticalCenter		|					|
	|left--------------------------	| HorizontalCenter--| right
	|		  baeOffLine			|					|
	|------------------------------	|-------------------|
	|			bottom				|					|
	|------------------------------	|-------------------|

	NB! ANCHOR ONLY SUPPORT THE ORGIN POINT AT (0. , 0.)
	THAT'S MEAN THE BOUNDING RECT SHOULD BE (0., 0., width, height)
*/

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwAnchors : public DwGraphicsItemListener
{
public:
    // NB! DO NOT CHANGE THE ORDER
    enum AnchorIndex
    {
        Left = 0,
        Right,
        Top,
        Bottom,
        VCenter,
        HCenter,
        Baseline,
        IndexMax,
        Invalid,
        Fill,
        CenterIn,
    };

    DwAnchors(DwGraphicsItem *);
    ~DwAnchors();

    DwGraphicsItem* item() const;

    DwAnchorLine left() const;
    void setLeft(const DwAnchorLine &edge);
    inline void setLeft(const DwGraphicsItem *t, Dw::AnchorLine l);
    void resetLeft();

    DwAnchorLine right() const;
    void setRight(const DwAnchorLine &edge);
    inline void setRight(const DwGraphicsItem *t, Dw::AnchorLine l);
    void resetRight();

    DwAnchorLine horizontalCenter() const;
    void setHorizontalCenter(const DwAnchorLine &edge);
    inline void setHorizontalCenter(const DwGraphicsItem *t, Dw::AnchorLine l);
    void resetHorizontalCenter();

    DwAnchorLine top() const;
    void setTop(const DwAnchorLine &edge);
    inline void setTop(const DwGraphicsItem *t, Dw::AnchorLine l);
    void resetTop();

    DwAnchorLine bottom() const;
    void setBottom(const DwAnchorLine &edge);
    inline void setBottom(const DwGraphicsItem *t, Dw::AnchorLine l);
    void resetBottom();

    DwAnchorLine verticalCenter() const;
    void setVerticalCenter(const DwAnchorLine &edge);
    inline void setVerticalCenter(const DwGraphicsItem *t, Dw::AnchorLine l);
    void resetVerticalCenter();

    DwAnchorLine baseline() const;
    void setBaseline(const DwAnchorLine &edge);
    inline void setBaseline(const DwGraphicsItem *t, Dw::AnchorLine l);
    void resetBaseline();

    DwReal leftMargin() const;
    void setLeftMargin(DwReal);

    DwReal rightMargin() const;
    void setRightMargin(DwReal);

    DwReal horizontalCenterOffset() const;
    void setHorizontalCenterOffset(DwReal);

    DwReal topMargin() const;
    void setTopMargin(DwReal);

    DwReal bottomMargin() const;
    void setBottomMargin(DwReal);

    DwReal margins() const;
    void setMargins(DwReal);

    DwReal verticalCenterOffset() const;
    void setVerticalCenterOffset(DwReal);

    DwReal baselineOffset() const;
    void setBaselineOffset(DwReal);

    DwGraphicsItem *fill() const;
    void setFill(DwGraphicsItem *);
    void resetFill();

    DwGraphicsItem *centerIn() const;
    void setCenterIn(DwGraphicsItem *);
    void resetCenterIn();

    inline int usedAnchors() const;

    // Anchor setup Stuffs
    void setAnchor(Dw::Anchor anchor, const DwAnchorLine &edge);
    inline void setAnchor(Dw::Anchor anchor, const DwGraphicsItem *t, Dw::AnchorLine l);
    DwAnchorLine anchorOf(Dw::Anchor anchor) const;
    void setMargin(Dw::Anchor anchor, DwReal margin);
    DwReal marginOf(Dw::Anchor anchor) const;

    // Implement the listener's stuffs
    virtual void onChange(Dw::ChangeType change,
                          DwGraphicsItem* item, const DwVariant& newOne, const DwVariant& oldOne);
    virtual void onAdded(DwGraphicsItem* item);
    virtual void onRemoved(DwGraphicsItem* item);

protected:
    void addAnchorDepend(DwGraphicsItem* item, AnchorIndex index);
    void remAnchorDepend(DwGraphicsItem* item, AnchorIndex index);

    // Item Dependency Stuffs
    void clearItem(DwGraphicsItem *);
    void clearItem(DwGraphicsItem *, AnchorIndex index);
    bool isItemComplete() const;
    void classBegin();
    void componentComplete();

    // Item's Adaptor Stuffs
    void setItemHeight(DwReal);
    void setItemWidth(DwReal);
    void setItemX(DwReal);
    void setItemY(DwReal);
    void setItemPos(const DwPointF &);
    void setItemSize(const DwSizeF &);

    // Item's Change Listener Interface
    void itemGeometryChanged(DwGraphicsItem *, const DwRectF &newG, const DwRectF &oldG);

    // Check Stuffs
    bool checkHValid() const;
    bool checkVValid() const;
    bool checkHAnchorValid(DwAnchorLine anchor) const;
    bool checkVAnchorValid(DwAnchorLine anchor) const;
    bool calcStretch(const DwAnchorLine &edge1, const DwAnchorLine &edge2,
                     DwReal offset1, DwReal offset2, Dw::AnchorLine line, DwReal &stretch);

    // Update Stuffs
    void updateMe();
    void updateOnComplete();
    void updateHorizontalAnchors();
    void updateVerticalAnchors();
    void fillChanged();
    void centerInChanged();

    DwGraphicsItem *m_item;
    DwGraphicsItem *m_fill;
    DwGraphicsItem *m_centerIn;

    DwAnchorLine m_anchors[IndexMax];
    DwReal m_margins[IndexMax+1];
    int m_usedAnchors;

    bool m_componentComplete:1;
    bool m_updatingMe:1;
    // Packed 8 bit
    uint8_t m_updatingHorizontalAnchor:2;
    uint8_t m_updatingVerticalAnchor:2;
    uint8_t m_updatingFill:2;
    uint8_t m_updatingCenterIn:2;

    // Depend Map
    typedef DwMap<DwGraphicsItem *, int> DependsMap;
    DependsMap m_dependsMap;
private:
};// end of DwAnchors

//------------------------------------------------------------------------------
/**
*/
inline DwGraphicsItem* DwAnchors::item() const
{
    return m_item;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwAnchors::setLeft(const DwGraphicsItem *t, Dw::AnchorLine l)
{
    setLeft(DwAnchorLine((DwGraphicsItem *)t, l));
}

//------------------------------------------------------------------------------
/**
*/
inline void DwAnchors::setRight(const DwGraphicsItem *t, Dw::AnchorLine l)
{
    setRight(DwAnchorLine((DwGraphicsItem *)t, l));
}

//------------------------------------------------------------------------------
/**
*/
inline void DwAnchors::setHorizontalCenter(const DwGraphicsItem *t, Dw::AnchorLine l)
{
    setHorizontalCenter(DwAnchorLine((DwGraphicsItem *)t, l));
}

//------------------------------------------------------------------------------
/**
*/
inline void DwAnchors::setTop(const DwGraphicsItem *t, Dw::AnchorLine l)
{
    setTop(DwAnchorLine((DwGraphicsItem *)t, l));
}

//------------------------------------------------------------------------------
/**
*/
inline void DwAnchors::setBottom(const DwGraphicsItem *t, Dw::AnchorLine l)
{
    setBottom(DwAnchorLine((DwGraphicsItem *)t, l));
}

//------------------------------------------------------------------------------
/**
*/
inline void DwAnchors::setVerticalCenter(const DwGraphicsItem *t, Dw::AnchorLine l)
{
    setVerticalCenter(DwAnchorLine((DwGraphicsItem *)t, l));
}

//------------------------------------------------------------------------------
/**
*/
inline void DwAnchors::setBaseline(const DwGraphicsItem *t, Dw::AnchorLine l)
{
    setBaseline(DwAnchorLine((DwGraphicsItem *)t, l));
}

//------------------------------------------------------------------------------
/**
*/
inline int DwAnchors::usedAnchors() const
{
    return m_usedAnchors;
}

//------------------------------------------------------------------------------
/**
*/
inline void DwAnchors::setAnchor(Dw::Anchor anchor, const DwGraphicsItem *t, Dw::AnchorLine l)
{
    setAnchor(anchor, DwAnchorLine((DwGraphicsItem *)t, l));
}