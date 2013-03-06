#pragma once

class DwImageDataPrivate;
class DwImageData
{
	
public:
	DwImageData();
	~DwImageData();

	bool loadFile(const DwString& filePath);
	
	int width()  const;
	int height() const;
	int depth()  const;

	uint8_t *bits() const;
	const BITMAPINFO bmi() const;
	void reset();
private:
	DwExplicitlySharedDataPointer<DwImageDataPrivate> d;
};