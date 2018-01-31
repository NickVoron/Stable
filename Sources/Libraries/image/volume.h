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

template<typename ColorT> 
struct Volume
{
	typedef ColorT ColorType;

	ColorType* bmp;
	unsigned int slicePitch, rowPitch;
	int sizeX, sizeY, sizeZ;

	inline Volume():bmp(0), slicePitch(0), rowPitch(0), sizeX(0), sizeY(0), sizeZ(0){}
	inline Volume(void* ptr, unsigned int slicePitch, unsigned int rowPitch, int sX, int sY, int sZ){ init(ptr, slicePitch, rowPitch, sX, sY, sZ); }

	inline void init(void* ptr, unsigned int _slicePitch, unsigned int _rowPitch, int sX, int sY, int sZ)
	{
		bmp = (ColorType*)ptr;
		slicePitch = _slicePitch;
		rowPitch = _rowPitch;
		sizeX = sX;
		sizeY = sY;
		sizeZ = sZ;
	}

	inline Plane<ColorType> operator ()(int z)
	{
		return Plane<ColorType>(&bmp[z*slicePitch/sizeof(bmp[0])], rowPitch, sizeX, sizeY);
	}

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