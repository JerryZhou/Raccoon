#pragma once

class  DwPaintDevice
{
	DW_DISABLE_COPY(DwPaintDevice);
public:
	DwPaintDevice();
	virtual ~DwPaintDevice();
    
	virtual HDC getDC() const;
	virtual void releaseDC();

};