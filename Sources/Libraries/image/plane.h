#pragma once

#include "newmath/library.include.h"

#include "line.h"
#include "color.h"

#pragma pack(push,1)

namespace image
{

enum WrapMode
{
	CLAMP,
	REPEAT,
	MIRROR,
};

namespace Private
{
	static inline int iround(float f) { return int(f); }
	static inline int ifloor(float f) { return int(floor(f)); }
	static inline float frac(float f) { return f - floor(f); }

	static inline int mirror(int x, int w)
	{
		x = abs(x);
		while (x >= w) 
		{
			x = 2 * w - x - 2;
		}
		return x;
	}
}

template<typename ColorT> 
struct Plane
{
public:

	typedef ColorT ColorType;

	ColorType* bmp;
	int sizeX, sizeY;
	unsigned int pitchInBytes;

	inline Plane()
	{
		defaults(); 
	}

	inline Plane(void* ptr, unsigned int pitchInBytes, int sx, int sy) 
	{ 
		defaults();
		init(ptr, pitchInBytes, sx, sy); 
	}

	inline ~Plane()
	{
		clear();
	}

	inline void defaults() 
	{
		bmp = 0;
		pitchInBytes = 0;
		sizeX = 0;
		sizeY = 0;
		memoryOwner = false;
	}

	bool empty() const { return sizeY == 0 && sizeX == 0; }

	void clear()
	{
		if(memoryOwner)
		{
			free(bmp);
		}

		defaults();
	}

	inline void init(void* ptr, unsigned int pitchInBytes_, int sx, int sy)
	{
		bmp = (ColorType*)ptr;
		sizeX = sx;
		sizeY = sy;
		pitchInBytes = pitchInBytes_;
	}

	inline unsigned int memory() const { return pitchInBytes * sizeY; }
	inline bool hasPitchOverhead() const { return sizeof(ColorType) * sizeX != pitchInBytes; } 

	inline void fill(const ColorType& c)
	{
		for (int y = 0; y < sizeY; ++y)
		{
			for (int x = 0; x < sizeX; ++x)
			{
				line_ptr(y)[x] = c;
			}
		}
	}

	template<class Function>
	inline void fill_fn(Function&& fn)
	{
		for (int y = 0; y < sizeY; ++y)
		{
			for (int x = 0; x < sizeX; ++x)
			{
				line_ptr(y)[x] = fn(x, y);
			}
		}
	}

	inline void copy(const void* ptr, int sx, int sy, unsigned int pitchInBytes_)
	{
		if (sx != sizeX || sy != sizeY)
		{
			allocate(sx, sy);
		}

		//ENFORCE_EQUAL(pitchInBytes, pitchInBytes_ )
		mem::memcpy(bmp, ptr, sy * pitchInBytes);
	}

	template<class ColorType1>
	inline void copy(const Plane<ColorType1>& plane)
	{
		if(plane.sizeX != sizeX || plane.sizeY != sizeY)
		{
			allocate(plane.sizeX, plane.sizeY);
		}

		for (int y = 0; y < sizeY; ++y)
		{
			for (int x = 0; x < sizeX; ++x)
			{
				operator()(x, y) = plane(x, y);
			}
		}
	}

	template<class ColorType1>
	inline void copy_lt(int szX, int szY, const Plane<ColorType1>& plane)
	{
		if (sizeX < szX || sizeY < szY)	{ allocate(szX, szY); }
		copy_lt(plane);
	}

	template<class ColorType1>
	inline void copy_lt(int szX, int szY, const Plane<ColorType1>& plane, int destChannel, int srcChannel)
	{
		if (sizeX < szX || sizeY < szY)	{ allocate(szX, szY); }
		copy_lt(plane, destChannel, srcChannel);
	}

	template<class ColorType1>
	inline void copy_lt(const Plane<ColorType1>& plane)
	{
		for (int y = 0; y < sizeY; ++y)
		{
			if (plane.sizeY <= y) break;
			for (int x = 0; x < sizeX; ++x)
			{
				if (plane.sizeX <= x) break;
				operator()(x, y) = plane(x, y);
			}
		}
	}

	template<class ColorType1>
	inline void copy_lt(const Plane<ColorType1>& plane, int destChannel, int srcChannel)
	{
		for (int y = 0; y < sizeY; ++y)
		{
			if (plane.sizeY <= y) break;
			for (int x = 0; x < sizeX; ++x)
			{
				if (plane.sizeX <= x) break;
				operator()(x, y).plane[destChannel] = plane(x, y).plane[srcChannel];
			}
		}
	}

	
	inline void allocate(int szX, int szY)
	{
		if ((memoryOwner && (szX != sizeX || szY != sizeY)) || !memoryOwner)
		{
			int pitch = ColorType::pixelSize() * szX;
			void* memBlock = realloc(bmp, pitch * szY);
			ENFORCE(memBlock);
			init(memBlock, pitch, szX, szY);
			memoryOwner = true;
		}		
	}

	inline const ColorType& operator ()(int x, int y) const { return line_ptr(y)[x]; }
	inline ColorType& operator ()(int x, int y) { return line_ptr(y)[x]; }

	inline Line<ColorType> operator ()(int y) const { return Line<ColorType>( (void*)line_ptr(y), sizeX ); }

	inline ColorType* line_ptr(int y) { return ((ColorType*)((char*)bmp + pitchInBytes * y)); }
	inline const ColorType* line_ptr(int y) const { return ((ColorType*)((char*)bmp + pitchInBytes * y)); }


	inline ColorType sample(int x, int y, WrapMode wm) const {	return sampleNearest(x, y, wm);	}
	inline const ColorType& index(int x, int y, WrapMode wm) const 	{ return indexNearest(x, y, wm); }


	ColorType sampleNearest(float x, float y, WrapMode wm) const
	{
				if( wm == CLAMP )		return sampleNearestClamp(x, y);
		else	if( wm == REPEAT )		return sampleNearestRepeat(x, y);
		else /*	if( wm == MIRROR )*/	return sampleNearestMirror(x, y);
	}

	ColorType sampleNearestClamp(const float x, const float y) const
	{
		int ix = nm::clamp(Private::iround(x * sizeX), 0, sizeX-1);
		int iy = nm::clamp(Private::iround(y * sizeY), 0, sizeY-1);
		return line_ptr(iy)[ix];
	}

	ColorType sampleNearestRepeat(const float x, const float y) const
	{
		int ix = Private::iround(Private::frac(x) * sizeX);
		int iy = Private::iround(Private::frac(y) * sizeY);
		return line_ptr(iy)[ix];
	}

	ColorType sampleNearestMirror(const float x, const float y) const
	{
		int ix = Private::mirror(Private::iround(x * sizeX), sizeX);
		int iy = Private::mirror(Private::iround(y * sizeY), sizeY);
		return line_ptr(iy)[ix];
	}


	const ColorType& indexNearest(int x, int y, WrapMode wm) const
	{
		return indexNearestClamp(x, y);

// 			if( wm == CLAMP )			return indexNearestClamp(x, y);
// 		else	if( wm == REPEAT )		return indexNearestRepeat(x, y);
// 		else /*	if( wm == MIRROR )*/	return indexNearestMirror(x, y);
	}

	const ColorType& indexNearestClamp(int x, int y) const
	{
		int ix = nm::clamp(x , 0, sizeX-1);
		int iy = nm::clamp(y , 0, sizeY-1);
		return line_ptr(iy)[ix];
	}

// 	ColorType indexNearestRepeat(const float x, const float y) const
// 	{
// 		int ix = iround(frac(x) * sizeX);
// 		int iy = iround(frac(y) * sizeY);
// 		return line_ptr(iy)[ix];
// 	}
// 
// 	ColorType indexNearestMirror(const float x, const float y) const
// 	{
// 		int ix = mirror(iround(x * sizeX), sizeX);
// 		int iy = mirror(iround(y * sizeY), sizeY);
// 		return line_ptr(iy)[ix];
// 	}

	
	inline ColorType interpolate(float u, float v)	const
	{
		nm::index2 i((int)(sizeX * u), (int)(sizeY * v));
		
		if(i.x < 0)
		{
			int cx = abs(i.x) % sizeX;
			i.x = sizeX - cx;
		}

		if(i.x > sizeX - 1)
		{
			i.x %= sizeX;
		}

		if(i.y < 0)
		{
			int cy = abs(i.y) % sizeY;
			i.y = sizeY - cy;
		}

		if(i.y > sizeY - 1)
		{
			i.y %= sizeY;
		}

// 		x = Base::Clamp(x, 0, size.x - 1);
// 		y = Base::Clamp(y, 0, size.y - 1);
		return (*this)(i.x, i.y);
//		return (*this)[ nm::index2(u, v) ];
// 		float fx = u*size.x;
// 		int x = int(fx);
// 		float dx = fx - float(x);
// 		x = x % size.x;
// 		int x1 = x + 1;
// 		if (x1 >= int(size.x))  x1 = 0;
// 
// 		float fy = v*size.y;
// 		int y = int(fy);
// 		float dy = fy - float(y);
// 		y = y % size.y;
// 		int y1 = y + 1;
// 		if (y1 >= int(size.y))  y1 = 0;
// 
// 		return
// 			Color<float, TOp::nPlanes>(operator[](nm::index2(x ,y ))) * (1.0f-dx)*(1.0f-dy) +
// 			Color<float, TOp::nPlanes>(operator[](nm::index2(x1,y) )) * dx*(1.0f-dy) +
// 			Color<float, TOp::nPlanes>(operator[](nm::index2(x ,y1))) * (1.0f-dx)*dy +
// 			Color<float, TOp::nPlanes>(operator[](nm::index2(x1,y1))) * dx*dy;
	}

private:
	bool memoryOwner;	
};


/*
Растяжение с билинейной интерполяцией, небезопасно - нет анализа на границы изображения назначения
Поддерживает тайлинг
*/
/*
template<typename TfmtSrc, typename TfmtDest>
void stretch(const TfmtSrc& src, const Base::RectPS& rectSrc,
						 const TfmtDest& dest, const Base::RectPS& rectDest)
{	//TODO: оптимизация деления на умножения, не вычислять каждый раз х-координату
	float v = float(rectSrc.top()) / src.size.y;
	float dv = float(rectSrc.size.y) / src.size.y / rectDest.size.y;
	nm::index2 i;
	for(i.y=rectDest.top(); i.y<rectDest.bottom(); i.y++)
	{
		float u = float(rectSrc.left()) / src.size.x;
		float du = float(rectSrc.size.x) / src.size.x / rectDest.size.x;
		for(i.x = rectDest.left(); i.x < rectDest.right(); i.x++)
		{
			dest[i] = src.interpolate(u, v);
			u += du;
		}
		v += dv;
	}
}	*/

						/*
//небезопасный метод - не анализирует выход за границы, поддерживает тайлинг
template<typename TfmtSrc, typename TfmtDest>
void blit(const TfmtSrc& src, const Base::RectPS& rectSrc,
						 const TfmtDest& dest, const Base::RectPS& rectDest)
{
	//TODO: оптимизация через обращение к строкам
	nm::index2 s,d;
	s.y = rectSrc.top();
	for(d.y=rectDest.top(); d.y<rectDest.bottom(); d.y++, s.y++)
	{
		s.x = rectSrc.left();
		for(d.x = rectDest.left(); d.x < rectDest.right(); d.x++, s.x++)
			dest[d] = src[s];
	}
}



					  */
 #pragma pack(pop)

};	//art

