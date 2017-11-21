#include "style.h"
#include "uniguidraw/library.include.h"

namespace unigui{
namespace style{


	void ComplexImage::clear()
	{
		atlas = 0;
	}

	void ComplexImage::draw(const unigui::Rect& rect, const unigui::Color& color)
	{
		if (atlas)
		{
			RectAtlasGAPI* a = atlas->GetResource();
			if (a)
			{
				const RectAtlas& atl = a->atlas;

				for (unsigned int c = 0; c < names.size(); ++c)
				{
					for (unsigned int r = 0; r < names[c].size(); ++r)
					{
						table.addData[c][r] = atl.index(names[c][r]);
					}
				}

				unigui::initIndexedTableFromAtlas(table, atl);
				table.build(rect);
				unigui::draw::texrect(table, atlas, color);
			}
		}		
	}

	void ComplexImage::save(stream::ostream& os) const
	{
		os << atlasName << table << names;
	}

	void ComplexImage::load(stream::istream& is)
	{
		clear();
		is >> atlasName >> table >> names;
		Resources::load(atlas, atlasName.c_str());
	}

	stream::istream& operator>>(stream::istream& is, ComplexImage& img) { img.load(is); return is; }
	stream::ostream& operator<<(stream::ostream& os, const ComplexImage& img) { img.save(os); return os; }

//
//
//
void Images::save(stream::ostream& os) const
{
	os << (const DataType&) *this;
}

void Images::load(stream::istream& is)
{
	is >> (DataType&) *this;
}


//
//
//
void Style::clear()
{
	images.clear();
}

void Style::save(stream::ostream& os) const
{
	images.save(os);
}

void Style::load(stream::istream& is)
{
	images.load(is);
}

ComplexImage* Style::image(const str::string32& name)
{
	auto it = images.find(name);
	return it != images.end() ? &it->second : 0;
}

}
}