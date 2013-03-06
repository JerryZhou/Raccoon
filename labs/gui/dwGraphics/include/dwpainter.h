#pragma once
class DwPaintDevice;
class DwTexture;
class DwCanvas;
class DwBrush;
class DwTransform;
class DwPainterPrivate;
class DW_GRAPHICS_EXPORT DwPainter
{
	DW_DISABLE_COPY(DwPainter);
public:
	DwPainter();
	DwPainter(HWND hwnd, PaintType paintType);
	~DwPainter();

	void	begin(HWND hwnd, PaintType paintType);
	void	end(PaintType paintType);

	//DwCanvas& device() const;
	bool	  drawTexture(const DwPoint& point, const DwTexture& texture);
	bool	  drawTexture(const DwRect& rectangle, const DwTexture& texture);
	void      canvasToWindow();

	/*void	drawLine(const DwPoint& p1, const DwPoint& p2);
	void	drawLine(const DwPointF& p1, const DwPointF& p2);

	void	drawRect(const DwRect& rectangle );
	void	drawRect(const DwRectF& rectangle );

	/*void    setFont(/*TODO..);/**/
	/*void	drawText(const DwPointF& position, const DwString& text );
	void	drawText(const DwPoint& position, const DwString& text );

	void    setTexture(const DwTexture& texture);
	void	drawTexture(const DwPoint& point, const DwTexture& texture);


	const DwBrush &	brush() const;
	void	setBrush(const DwBrush & brush );
	void	setBrush(DwBrush::BrushStyle style );

	const DwBrush &	background() const;
	void	setBackground(const DwBrush & brush );
	void	setBackgroundMode(DwBrush::BGMode mode );

	CompositionMode compositionMode() const;
	void   setCompositionMode(CompositionMode mode );

	void	eraseRect (const DwRectF& rectangle );
	void	fillRect (const DwRectF& rectangle, const DwBrush& brush );

	void	restore();
	void	rotate(double_t  angle );
	void	shear(double_t sh, double_t sv);
	void	scale(double_t sx, double_t sy);
	void	translate ( const DwPointF & offset);

	void	resetTransform();
	const DwTransform &	transform () const;
	void	setTransform(const DwTransform & transform, bool combine = false);*/
private:
	DwExplicitlySharedDataPointer<DwPainterPrivate> d;
};