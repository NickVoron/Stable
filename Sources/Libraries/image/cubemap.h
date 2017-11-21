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
// 	inline Cubemap(void* ptr[6], unsigned int sz)
// 	{
// 		defaults()
// 		init(ptr, sz);
// 	}

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