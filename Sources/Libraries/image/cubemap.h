// Copyright (C) 2012-2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "plane.h"

#pragma pack(push,1)

namespace image
{

enum CUBE_FACE { POSITIVE_X = 0, NEGATIVE_X, POSITIVE_Y, NEGATIVE_Y, POSITIVE_Z, NEGATIVE_Z, FACE_PLANES_COUNT };

template<typename ColorT> 
struct Cubemap
{
	typedef ColorT ColorType;

	ColorType* planes[FACE_PLANES_COUNT];
	unsigned int size;
	unsigned int pitchInBytes;

	inline Cubemap() { defaults(); }






	inline ~Cubemap()
	{
		clear();
	}

	inline void defaults()
	{
		mem::memzero(planes);
		pitchInBytes = 0;
		size = 0;
		memoryOwner = false;
	}

	bool empty() const { return size == 0; }

	void clear()
	{
		if (memoryOwner)
		{
			for (int i = POSITIVE_X; i < FACE_PLANES_COUNT; ++i)
			{
				free(planes[i]);
			} 			
		}

		defaults();
	}

	inline void init(void* ptr[6], unsigned int pitch, unsigned int sz)
	{
		size = sz;
		pitchInBytes = pitch;
		mem::memcpy(planes, ptr, sizeof(planes));
	}

	inline void allocate(int sz)
	{
		int pitch = ColorType::pixelSize() * sz; 
		void* memBlocks[6];
		for (int i = POSITIVE_X; i < FACE_PLANES_COUNT; ++i)
		{
			memBlocks[i] = realloc(planes[i], pitch * sz);
			ENFORCE(memBlocks[i]);
		}
		
		init(memBlocks, pitch, sz);
		memoryOwner = true;
	}

	inline void copy(CUBE_FACE face, const Plane<ColorType>& plane)
	{
		ENFORCE(face >= POSITIVE_X && face < FACE_PLANES_COUNT);
		ENFORCE(plane.sizeX == plane.sizeY);

		if (size == 0)
		{
			allocate(plane.sizeX);
		}

		ENFORCE(plane.sizeX == size);
		ENFORCE(plane.pitchInBytes == pitchInBytes);

		mem::memcpy(planes[face], plane.bmp, plane.memory());
	}

	const Plane<ColorType> operator ()(CUBE_FACE f) const {	return (*(Cubemap*)this)(f); }
	Plane<ColorType> operator ()(CUBE_FACE f) { return Plane<ColorType>(planes[f], size, size); }
	const ColorType& operator ()(const Vector3& dir) const	{ return (*(Cubemap*)this)(dir);	}

	ColorType& operator ()(const Vector3& dir)
	{
		Vector2 uv;
		CUBE_FACE f;
		Vector3 a(fabs(dir.x), fabs(dir.y), fabs(dir.z));

		if(a.x >= a.y && a.x >= a.z)
		{
			uv.x = 0.5f - 0.5f*dir.z/dir.x;
			uv.y = 0.5f - 0.5f*dir.y/a.x;
			f = dir.x > 0.0f ? POSITIVE_X : NEGATIVE_X;
		}
		else
			if(a.y > a.x && a.y > a.z)
			{
				uv.x = 0.5f + 0.5f*dir.x/a.y;
				uv.y = 0.5f + 0.5f*dir.z/dir.y;
				f = dir.y > 0.0f ? POSITIVE_Y : NEGATIVE_Y;
			}
			else
			{
				uv.x = 0.5f + 0.5f*dir.x/dir.z;
				uv.y = 0.5f - 0.5f*dir.y/a.z;
				f = dir.z > 0.0f ? POSITIVE_Z : NEGATIVE_Z;
			}

		return planes[f][int(uv.y*float(size))*size + int(uv.x*float(size))];
	}

private:
	bool memoryOwner = false;
};

#pragma pack(pop)

};



// Copyright (C) 2012-2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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