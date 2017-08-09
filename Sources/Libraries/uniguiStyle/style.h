// Copyright (C) 2014-2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "unigui/library.include.h"
#include "resourceAtlas/library.include.h"


namespace unigui{
namespace style{

	struct ComplexImage 
	{
		void clear();
		void draw(const unigui::Rect& rect, const unigui::Color& color);
		void save(stream::ostream& os) const;
		void load(stream::istream& is);

		Resources::Atlas* atlas;
		str::string32 atlasName;
		IndexedSizeableTable table;
		std::vector<std::vector<str::string32>> names;
	};

	struct Images : public std::map<str::string32, ComplexImage>
	{
		typedef std::map<str::string32, ComplexImage> DataType;

		void save(stream::ostream& os) const;
		void load(stream::istream& is);
	};

	struct Style 
	{
		void clear();
		void save(stream::ostream& os) const;
		void load(stream::istream& is);

		ComplexImage* image(const str::string32& name);

		Images images;
	};

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