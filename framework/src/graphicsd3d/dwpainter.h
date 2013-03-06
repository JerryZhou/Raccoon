#pragma once

//------------------------------------------------------------------------------
class DW_GRAPHICS_EXPORT DwPainter
{
public:
    void setTransform(const DwTransform &);
    void setClip(const DwRect &);
    void drawGeometry(const DwGeometryBuffer &);
protected:
private:
};// end of DwPainter