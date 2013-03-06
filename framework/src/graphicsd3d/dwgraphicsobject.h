#pragma once
#include "dwgraphicsdata.h"

//------------------------------------------------------------------------------
class DW_GRAPHICS_EXPORT DwGraphicsObject
{
public:
    DwGraphicsObject();
    DwGraphicsObject(const DwGraphicsObject& other);
    DwGraphicsObject& operator=(const DwGraphicsObject &other);
    DwGraphicsObject& operator=(DwGraphicsObjectData* d);
    virtual  ~DwGraphicsObject();

    DwGraphicsObjectData * data() const;

protected:
    DwGraphicsObject(DwGraphicsObjectData* d);
    DwGraphicsObjectData *m_data;
};// end of DwGraphicsObject