// Copyright (C) 2014-2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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




void Images::save(stream::ostream& os) const
{
	os << (const DataType&) *this;
}

void Images::load(stream::istream& is)
{
	is >> (DataType&) *this;
}





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



// Copyright (C) 2014-2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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