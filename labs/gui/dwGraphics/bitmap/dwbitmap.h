#pragma once
#include "ximage/dwimagedata.h"
class DwImageData;
class DwBitmapPrivate : public DwSharedData
{
public:
	DwBitmapPrivate();
	~DwBitmapPrivate();
	bool createBitmapFromFile(const DwString &filePath);
	bool createBitmapFromSize(int width,int height);
	bool createBitmapFromBuffer(const char *buffer, int size);

	bool loadFile(const DwString &filePathPath);
	void reset();
	DwBitmapPrivate *clone();

	DwImageData m_imageData;
	HBITMAP     m_hBitmap;
	uint8_t*    m_lpByte;
	BITMAPINFO  m_bitmapInfo;

	int   m_width;
	int   m_height;
};

class DwScopedPointer
{
public:
	typedef DwBitmapPrivate DwBitmapPrivateype;
	typedef DwBitmapPrivate *pointer;

	inline DwBitmapPrivate &operator*() const { return *d; }
	inline DwBitmapPrivate *operator->() { return d; }
	inline DwBitmapPrivate *operator->() const { return d; }
	inline DwBitmapPrivate *data() const { return d; }
	inline const DwBitmapPrivate *constData() const { return d; }

	inline void detach() { if (d && d->ref != 1) detach_helper(); }

	inline void reset()
	{
		if(d && !d->ref.deref())
			delete d;

		d = 0;
	}

	inline operator bool () const { return d != 0; }

	inline bool operator==(const DwScopedPointer &other) const { return d == other.d; }
	inline bool operator!=(const DwScopedPointer &other) const { return d != other.d; }
	inline bool operator==(const DwBitmapPrivate *ptr) const { return d == ptr; }
	inline bool operator!=(const DwBitmapPrivate *ptr) const { return d != ptr; }

	inline DwScopedPointer() { d = 0; }
	inline ~DwScopedPointer() { if (d && !d->ref.deref()) delete d; }

	explicit DwScopedPointer(DwBitmapPrivate *data);
	inline DwScopedPointer(const DwScopedPointer &o) : d(o.d) { if (d) d->ref.ref(); }
    inline DwScopedPointer & operator=(const DwScopedPointer &o) {
		if (o.d != d) {
			if (o.d)
				o.d->ref.ref();
			DwBitmapPrivate *old = d;
			d = o.d;
			if (old && !old->ref.deref())
				delete old;
		}
		return *this;
	}
	inline DwScopedPointer &operator=(DwBitmapPrivate *o) {
		if (o != d) {
			if (o)
				o->ref.ref();
			DwBitmapPrivate *old = d;
			d = o;
			if (old && !old->ref.deref())
				delete old;
		}
		return *this;
	}

	inline bool operator!() const { return !d; }

protected:
	DwBitmapPrivate *clone();

private:
	void detach_helper();

	DwBitmapPrivate *d;
};

class DwBitmap
{	
public:
	DwBitmap();
	~DwBitmap();

	bool createBitmapFromFile(const DwString &filePath);
	bool createBitmapFromSize(int width, int height);

	int width() const;
	int height() const;

	const HBITMAP handle() const;
	const uint8_t *bits() const;
	const BITMAPINFO bitmapinfo() const;
	void  reset();
private:
	DwScopedPointer d;
};