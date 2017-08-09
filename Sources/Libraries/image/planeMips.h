// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "plane.h"
namespace image
{

template<class ColorT> 
class PlaneMips
{
	typedef Plane<ColorT> Image;

	std::vector< Image > mips;
public:

	PlaneMips(size_t n)
	{
		mips.resize(n);
	}

	void assign(size_t m, const Plane<ColorT>& im)
	{
		mips[m] = im;
	}

	Plane<ColorT>& operator [](size_t m)
	{
		return mips[m];
	}
	
	size_t numMips() const
	{
		return mips.size();
	}

	void generateMips()
	{
		for(size_t m=1; m < mips.size(); ++m)
		{
			Plane<ColorT>& src = mips[m-1];
			Plane<ColorT>& dest = mips[m];

			for(int y=0; y < dest.sizeY; ++y)
			{
				for(int x=0; x < dest.sizeX; ++x)
				{
					ColorT c0 = src(x*2 + 0, y*2 + 0);
					ColorT c1 = src(x*2 + 0, y*2 + 1);
					ColorT c2 = src(x*2 + 1, y*2 + 0);
					ColorT c3 = src(x*2 + 1, y*2 + 1);

					ColorT c = (c0 + c1 + c2 + c3)*0.25f;
					dest(x, y) = c;
				}
			}
		}
	}

};

};	




// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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