// Copyright (C) 2012-2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <string>
#include <map>
#include "strings/library.include.h"

class RectAtlas 
{
friend class TextureAtlasComposer;
public:

#pragma pack(push,1)
	struct Size
	{
		Size():x(0),y(0){}
		int x;
		int y;
	};

	struct Bitmask 
	{

		typedef unsigned char Element;

		Bitmask();
		~Bitmask();

		void create(const Size& size);
		void create(void* data, int sizeX, int sizeY);
		void clear();

		unsigned int memsize() const;

		bool inside(int orgX, int orgY, int x, int y) const;

		unsigned char* data;		
		Size size;		
	};

	struct Rect
	{
		Rect();
		~Rect();

		bool inside(int orgX, int orgY, int x, int y) const;
		float sizeX() const;
		float sizeY() const;

		float orgX;
		float orgY;
		float minX;
		float minY;
		float maxX;
		float maxY;
	};
#pragma pack(pop)

	

	RectAtlas();
	RectAtlas(int cnt, int sizeX, int sizeY);

	~RectAtlas();

	void init(int count, int sizeX, int sizeY);
	void clear();

	Bitmask& bitmask(int idx);
	const Bitmask& bitmask(int idx) const;

	Rect& absoluteRect(int idx);
	const Rect& absoluteRect(int idx) const;

	bool inside(int rectIdx, int orgX, int orgY, int x, int y, bool useBitmask) const;

	Rect relativeRect(int idx, bool halfPixelCorrection) const;
	int count() const;

	const Size& size() const { return sz; } 
	const Size& sourceSize() const { return sourceSz; } 

	int index(const str::string32& name) const;
	const str::string32& name(int index) const;
	const Rect& absoluteRect(const str::string32& name) const;
	Rect relativeRect(const str::string32& name, bool halfPixelCorrection) const;


protected:
	void alloc(int count);
	int cnt;
	Rect* rects;
	Bitmask* bitmasks;

	typedef std::map<str::string32, int> NamesTable;
	NamesTable namesTable;
	
	Size sz;
	Size sourceSz;
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