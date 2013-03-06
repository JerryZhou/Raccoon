#pragma once
#include "dwgui/dwwidgetitem.h"

//------------------------------------------------------------------------------
class DwScrollBar;
class DwGraphicsFrameItem;
class DwContentGemoryListener;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwScrollArea : public DwWidgetItem
{
    DW_DECLARE_CLASS(DwScrollArea);

public:
    explicit DwScrollArea(DwGraphicsItem *parent = NULL);
    virtual ~DwScrollArea();

    void setContent(DwGraphicsItem *);
    inline DwGraphicsItem* content() const;

    inline DwScrollBar* horizontalScroll() const;
    inline DwScrollBar* verticalScroll() const;

    virtual DwRectF clipRect() const;

protected:
    friend class DwScrollBar;
    friend class DwContentGemoryListener;

    // NB! ONLY CAN BE CALLED BY SCROLLBAR
    void scrollHorizialTo(DwReal percent);
    void scrollVerticalTo(DwReal percent);

    DwReal contentWidth() const;
    DwReal contentHeight() const;

    void updateScrollBar();
    void updateContentClipRect();

    void updateScrollContent();

    DwGraphicsItem *m_content;
    DwScrollBar* m_horizontalScroll;
    DwScrollBar* m_verticalScroll;

    DwContentGemoryListener* m_listener;
    bool m_updateMe;
private:
};// end of DwScrollArea
DW_REGISTER_CLASS(DwScrollArea);

//------------------------------------------------------------------------------
/**
*/
inline DwGraphicsItem* DwScrollArea::content() const
{
    return m_content;
}

//------------------------------------------------------------------------------
/**
*/
inline DwScrollBar* DwScrollArea::horizontalScroll() const
{
    return m_horizontalScroll;
}

//------------------------------------------------------------------------------
/**
*/
inline DwScrollBar* DwScrollArea::verticalScroll() const
{
    return m_verticalScroll;
}