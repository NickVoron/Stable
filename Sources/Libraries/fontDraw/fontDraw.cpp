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