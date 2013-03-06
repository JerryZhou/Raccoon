#pragma once

class DwTexturePrivate;
class DW_GRAPHICS_EXPORT DwTexture
{
public:
	DwTexture();
	~DwTexture();

	bool createBitmapFromFile(const DwString &filePath);
	bool createBitmapFromSize(int width, int height);

	uint32_t handle() const;
	const uint8_t *bits() const;
	const BITMAPINFO bitmapInfo()const;

	int  width() const;
	int  height() const;

	const CompositionMode& compositionMode() const;
	void setCompositionMode(const CompositionMode& CompositionMode);

	DrawMode drawMode() const;
	void setDrawMode(DrawMode drawMode);

private:
	DwTexturePrivate *d;
};