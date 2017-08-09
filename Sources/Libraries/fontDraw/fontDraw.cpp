// Copyright (C) 2012-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "fontDraw.h"

#include "uniguidraw/library.include.h"

namespace unigui 
{
namespace font
{

FontDriver::~FontDriver()
{
	clear();
}

void FontDriver::clear()
{
	texture.clear();
}

void FontDriver::save(stream::ostream& s)
{
	s << face;
	texture.save(s);
}

void FontDriver::load(stream::istream& s)
{
	clear();
	s >> face;
	texture.load(s);
}

void FontDriver::print(const Face::GlyphInfo* beg, const Face::GlyphInfo* end, const FontMaterialEntry* materials, int materialsCount, bool stroked) const
{
	int idx = 0;
	FontMaterialEntry defaultEntry = { end - beg, {color(1.0f, 1.0f, 1.0f, 1.0f)} };
	const FontMaterialEntry* entry = (materialsCount > 0) ? materials : &defaultEntry;

	for (; beg != end; ++beg)
	{
		unigui::draw::character(beg->rect, beg->texRect, parent, entry->material.color, stroked);
		++idx;	

		if(idx >= entry->endStrIndex)
		{
 			++materials;
			--materialsCount;
 			entry = (materialsCount > 0) ? materials : &defaultEntry;
		}
	}	

}

}
}



// Copyright (C) 2012-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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