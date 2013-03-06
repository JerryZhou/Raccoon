#pragma once

//TODO...
class DwBrush
{	
public:
	enum BGMode
	{
		TransparentMode = 0x00,
		OpaqueMode ,
	};
	
	enum BrushStyle
	{
		NoBrush = 0x00,
		//TODO...

	};
public:
	DwBrush();
	~DwBrush();
};