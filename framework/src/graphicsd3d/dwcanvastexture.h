#pragma once
#include "dwcanvas.h"
#include "dwtexture.h"

//------------------------------------------------------------------------------
class DwCanvasWindowTexture;

class DW_GRAPHICS_EXPORT DwCanvasTexture : public DwCanvas
{
    _declare_data(DwCanvasTexture);
public:
    DwCanvasTexture();
    explicit DwCanvasTexture(const DwSize& s);
    virtual ~DwCanvasTexture();

    virtual void beginPaint(const DwRect &s, const DColor& c);
    virtual void endPaint();

    // return the texture
    DwTexture   texture() const;

protected:
    virtual void resize(const DwSize& s);
private:
};// end of DwCanvasTexture