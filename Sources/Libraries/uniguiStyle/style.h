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