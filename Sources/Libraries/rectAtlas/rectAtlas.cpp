// Copyright (C) 2012-2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "rectAtlas.h"
#include <stdlib.h>
#include <memory.h>




RectAtlas::Bitmask::Bitmask():
data(0)
{
	
}

RectAtlas::Bitmask::~Bitmask()
{
	clear();
}

void RectAtlas::Bitmask::create(void* d, int sizeX, int sizeY)
{
	clear();

	size.x = sizeX;
	size.y = sizeY;

	unsigned int len = sizeX * sizeY * sizeof(Element);
	data = (unsigned char*)malloc(len);
	memcpy(data, d, len);
}

void RectAtlas::Bitmask::create(const Size& sz)
{
	clear();

	size = sz;

	unsigned int len = size.x * size.y * sizeof(Element);
	data = (unsigned char*)malloc(len);
	memset(data, 0, len);
}

unsigned int RectAtlas::Bitmask::memsize() const
{
	return size.x * size.y * sizeof(Element);
}

void RectAtlas::Bitmask::clear()
{
	if(data)
	{
		free(data);
	}

	size.x = 0;
	size.y = 0;
	data = 0;
}

bool RectAtlas::Bitmask::inside(int orgX, int orgY, int x, int y) const
{
	x -= orgX;
	y -= orgY;

	if(data && x >= 0  && x < size.x && y >= 0 && y < size.y)
	{
		unsigned char val = data[y * size.x + x];
		return val > 0;
	}

	return false;
}

RectAtlas::Rect::Rect():
orgX(0),
orgY(0),
minX(0),
minY(0),
maxX(0),
maxY(0)
{

}

RectAtlas::Rect::~Rect()
{

}

bool RectAtlas::Rect::inside(int oX, int oY, int x, int y) const
{
	return oX <= x && (oX + sizeX()) > x && oY <= y && (oY + sizeY()) > y;
}

float RectAtlas::Rect::sizeX() const { return maxX - minX; }
float RectAtlas::Rect::sizeY() const { return maxY - minY; }




RectAtlas::RectAtlas():cnt(0), rects(0), bitmasks(0)
{
	sz.x = 0;
	sz.y = 0;
	sourceSz.x = 0;
	sourceSz.y = 0;
}

RectAtlas::RectAtlas(int cnt, int sX, int sY)
{
	init(cnt, sX, sY);	
}

RectAtlas::~RectAtlas()
{
	clear();
}

void RectAtlas::alloc(int cnt_)
{
	cnt = cnt_;
	if(cnt > 0)
	{
		rects = new Rect[cnt];
		bitmasks = new Bitmask[cnt];
	}
}

void RectAtlas::init(int cnt_, int sX, int sY)
{
	clear();

	sz.x = sX;
	sz.y = sY;
	
	alloc(cnt_);
}

void RectAtlas::clear()
{
	if(rects)
	{
		delete [] rects;
		rects = 0;
	}

	if(bitmasks)
	{
		delete [] bitmasks;
		bitmasks = 0;
	}

	cnt = 0;
	sz.x = sz.y = 0;
	sourceSz.x = sourceSz.y = 0;
}

int RectAtlas::count() const
{
	return cnt;
}

RectAtlas::Bitmask& RectAtlas::bitmask(int idx)
{
	return bitmasks[idx];
}

const RectAtlas::Bitmask& RectAtlas::bitmask(int idx) const
{
	return bitmasks[idx];
}

RectAtlas::Rect& RectAtlas::absoluteRect(int idx)
{
	static Rect zeroRect;
	return idx >= 0 ? rects[idx] : zeroRect;	
}

const RectAtlas::Rect& RectAtlas::absoluteRect(int idx) const
{
	return ((RectAtlas*)this)->absoluteRect(idx);
}

bool RectAtlas::inside(int rectIdx, int orgX, int orgY, int x, int y, bool useBitmask) const
{
	return !useBitmask ? absoluteRect(rectIdx).inside(orgX, orgY, x, y) : bitmask(rectIdx).inside(orgX, orgY, x, y);
}

RectAtlas::Rect RectAtlas::relativeRect(int idx, bool halfPixelCorrection) const
{
	Rect rect = absoluteRect(idx);
	
	if(halfPixelCorrection)
	{
		rect.minX += 0.5f;
		rect.minY += 0.5f;
		rect.maxX -= 0.5f;
		rect.maxY -= 0.5f;
	}

	float invX = 1.0f / (float)sz.x;
	float invY = 1.0f / (float)sz.y;

	rect.orgX *= invX;
	rect.minX *= invX;
	rect.maxX *= invX;
	rect.minY *= invY;
	rect.maxY *= invY;
	rect.orgY *= invY;

	return rect;
}

const str::string32& RectAtlas::name(int index) const
{
	NamesTable::const_iterator it = namesTable.begin();
	NamesTable::const_iterator end = namesTable.end();

	for (; it != end; ++it)
	{
		if(it->second == index)
		{
			return it->first;
		}
	}

	static str::string32 notFound = "not found index";
	return notFound;
}

int RectAtlas::index(const str::string32& name) const
{
	NamesTable::const_iterator it = namesTable.find(name);
	return it == namesTable.end() ? -1 : it->second;
}

const RectAtlas::Rect& RectAtlas::absoluteRect(const str::string32& name) const
{
	return absoluteRect( index(name) );
}

RectAtlas::Rect RectAtlas::relativeRect(const str::string32& name, bool halfPixelCorrection) const
{
	return relativeRect( index(name), halfPixelCorrection );
}




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