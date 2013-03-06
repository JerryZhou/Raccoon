#pragma once
#include "dwgui/dwwidgetitem.h"
#include "dwgui/dwtextdocument.h"
#include "dwgui/dwtextlayoutengine.h"

//------------------------------------------------------------------------------
class DwGraphicsSimpleTextItem;
class DwGraphicsFrameItem;

// TODO: cache and dirty rect update
//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwGraphicsEditText : public DwGraphicsItem
{
    DW_DECLARE_CLASS(DwGraphicsEditText);
public:
    explicit DwGraphicsEditText(DwGraphicsItem *parent = NULL);
    virtual ~DwGraphicsEditText();

    DwTextDocument &doc() ;
    DwTextLayoutEngine &layoutEngine();

    virtual void paint(DwPainter *painter, const DwStyleOptionGraphicsItem *option);

protected:
    virtual void notifyItemChange(Dw::GraphicsItemChange change, const DwVariant& newOne, const DwVariant& oldOne);

    DwPoint offset() const;
    DwReal offsetX() const;
    DwReal offsetY() const;
    void setDisplayOffsetX(DwReal x);
    void setDisplayOffsetY(DwReal y);

    void setDocument(DwTextDocument *d);
    void setLayoutEngine(DwTextLayoutEngine *engine);

    DwTextDocument *m_doc;
    DwTextLayoutEngine *m_layoutEngine;
    DwReal m_xDisplayOffset;
    DwReal m_yDisplayOffset;

private:
    friend class DwSimpleEditEventHandler;
    friend class DwLineEdit;
    friend class DwMultiEdit;
};// end of DwSimpleEdit
DW_REGISTER_CLASS(DwGraphicsEditText);