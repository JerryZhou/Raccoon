#pragma once

class DwBitmapPrivate;
class DwBitmap
{
public:
	DwBitmap();
	~DwBitmap();
	bool createBitmap(const DwString &file);
	bool createBitmap(int nWidth, int nHeight);

	unsigned int handle() const;
	int     width() const;
	int     height() const;
	const uint8_t *bits() const;
	void    clear();
private:
	DwExplicitlySharedDataPointer<DwBitmapPrivate> d;
};