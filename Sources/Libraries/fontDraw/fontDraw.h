// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "font/library.include.h"

#include "multiapiUtlis/library.include.h"

namespace Resources{ class Font; }

namespace unigui {
namespace font{

struct FontMaterial 
{
	Color color;
};

struct FontMaterialEntry
{
	int endStrIndex;
	FontMaterial material;
};

class FontDriver
{
public:
	~FontDriver();
	
	template<class StrType>	void print(const StrType& str, const Rect& rect, bool stroked = false) const;
	template<class StrType>	void print(const StrType& str, const Rect& rect, const Color& c, bool stroked = false) const;
	template<class StrType>	void print(const StrType& str, const Rect& rect, const FontMaterialEntry* materials, int materialsCount, bool stroked = false) const;

	void save(stream::ostream& s);
	void load(stream::istream& s);

	void clear();

	Face face;
	multiapi::Texture texture;
	Resources::Font* parent;
private:
	void print(const Face::GlyphInfo* beg, const Face::GlyphInfo* end, const FontMaterialEntry* materials, int materialsCount, bool stroked) const;

};

namespace str
{
	template<class StrType>	
	std::size_t length(StrType str);

	template<>	inline std::size_t length<const char*>			(const char* str)			{ return strlen(str); }
	template<>	inline std::size_t length<const wchar_t*>		(const wchar_t* str)		{ return wcslen(str); }
	template<>	inline std::size_t length<const std::string&>	(const std::string& str)	{ return str.length(); }
	template<>	inline std::size_t length<const std::wstring&>	(const std::wstring& str)	{ return str.length(); }
}

template<class StrType>	
void FontDriver::print(const StrType& str, const Rect& rect, bool stroked) const
{
	print(str, rect, color(1.0f, 1.0f, 1.0f, 1.0f), stroked);
}

template<class StrType>	
void FontDriver::print(const StrType& str, const Rect& rect, const Color& c, bool stroked) const
{
	FontMaterialEntry material;
	material.endStrIndex = str::length(str);
	material.material.color = c;

	print(str, rect, &material, 1, stroked);
}

template<class StrType>	
void FontDriver::print(const StrType& str, const Rect& rect, const FontMaterialEntry* materials, int materialsCount, bool stroked) const
{   	
	Face::GlyphsQuery query;
	query.params.rect = rect;
	query.params.firstLineShift = 0;
	query.params.wordWrap = false;
	query.params.align = TAL_LEFT | TAL_VERTICAL_CENTER;

	face.glyphs(str, query);
	print(query.result.beg, query.result.end, materials, materialsCount, stroked);
}

}}




// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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