#pragma once

class DwBmpFilePrivate;
class DwBmpFile
{
public:
	DwBmpFile();
	~DwBmpFile();

	bool loadFile(const DwString & file);
	int  width(void)	const;	
	int  height(void)const;
	int  depth(void) const; 
	BITMAPINFO bmi(void) const;
	uint8_t * bits(void) const;	  
	void    clear();
private:
	DwExplicitlySharedDataPointer<DwBmpFilePrivate> d;
};