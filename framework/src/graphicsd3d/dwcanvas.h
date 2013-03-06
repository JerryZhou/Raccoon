#pragma once
#include "dwcore/dwsize.h"
#include "dwcore/dwrect.h"
#include "dwgraphicsobject.h"

//------------------------------------------------------------------------------
class DwCanvasData;
typedef uint32_t DColor;

//------------------------------------------------------------------------------
class DW_GRAPHICS_EXPORT DwCanvas : public DwGraphicsObject
{
    _declare_data(DwCanvas);
public:
    DwCanvas();
    virtual ~DwCanvas();

    virtual void beginPaint(const DwRect &s, const DColor& c) = 0 ;
    virtual void endPaint() = 0 ;

    void setSize(int w, int h);
    void setSize(const DwSize& s);
    DwSize size() const;
    int width() const;
    int height() const;

    // set blend mode
    void setBlendMode(BlendMode mode);
    // return the belnd mode
    BlendMode blendMode()const;

protected:
    void setupMatrix();
    void setupBlend();

    DwCanvas(DwCanvasData *d);
    virtual void resize(const DwSize& s) = 0;
};// end of DwCanvas

//------------------------------------------------------------------------------
/**
*/
inline void DwCanvas::setSize(int w, int h)
{
    setSize(DwSize(w, h));
}