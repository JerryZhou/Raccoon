#pragma once
#include "dwgraphicsobject.h"

//------------------------------------------------------------------------------
class DwGeometryBufferData;
class DwTexture;
class DwGraphicsEffect;

//------------------------------------------------------------------------------
class DW_GRAPHICS_EXPORT DwGeometryBuffer : public DwGraphicsObject
{
    _declare_data(DwGeometryBuffer);
public:
    DwGeometryBuffer();
    virtual ~DwGeometryBuffer();

    // geometry stuffs
    void setTexture(const DwTexture *tex);
    void addGeometry(const DwVertex* v, int count);
    void clear();

    // translation stuffs
    void setTranslation(const DwVector3& t);
    void setRotation(const DwVector3& r);
    void setPivot(const DwVector3& p);

    // effect
    void setEffect(const DwGraphicsEffect *effect);

    // the real draw stuff
    void draw();

protected:
    void updateMatrix() const;
};// end of DwGeomtryBuffer