// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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

	inline int memory() const { return pitchInBytes * sizeY; }
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
		else 	return sampleNearestMirror(x, y);
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




	}

	const ColorType& indexNearestClamp(int x, int y) const
	{
		int ix = nm::clamp(x , 0, sizeX-1);
		int iy = nm::clamp(y , 0, sizeY-1);
		return line_ptr(iy)[ix];
	}















	
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



		return (*this)(i.x, i.y);




















	}

private:
	bool memoryOwner;	
};





						
 #pragma pack(pop)

};	





// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions 
// of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.