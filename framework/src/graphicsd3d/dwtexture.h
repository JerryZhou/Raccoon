#pragma once
#include "dwcore/dwstring.h"
#include "dwcore/dwbytearray.h"
#include "dwcore/dwsize.h"
#include "dwgraphicsobject.h"

//------------------------------------------------------------------------------
class DwTextureData;

//------------------------------------------------------------------------------
class DW_GRAPHICS_EXPORT DwTexture : public DwGraphicsObject
{
    _declare_data(DwTexture)
public:
    DwTexture();
    explicit DwTexture(DwTextureData* dat);
    virtual ~DwTexture();

    bool loadFromFile(const DwString &);
    bool loadFromMemory(const DwByteArray& data);

    DwSize size() const;
    DwSizeF scale() const;
    int width() const;
    int height() const;
protected:
    void updateScales();
};// end of DwTexture