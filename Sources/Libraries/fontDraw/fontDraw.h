#pragma once

#include "font/library.include.h"
//#include "uniguidraw/library.include.h"
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
	material.endStrIndex = (int)str::length(str);
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
