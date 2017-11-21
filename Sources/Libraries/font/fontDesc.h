#pragma once

#include <string>
#include <list>

namespace unigui
{
	namespace font
	{

		struct Outline
		{
			int color;
			int lineSize;
		};

		struct FontDesc
		{
		public:
			FontDesc();
			~FontDesc();

			int size;
			std::string name;
			std::string file;
			bool bold;
			bool italic;
			bool underline;

			std::list<int> indexes;

			Outline central;
			Outline inner;
			Outline outer;
		};

	}//
}//