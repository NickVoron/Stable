// Copyright (C) 2012-2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "math/library.include.h"
#include "utfstr/library.include.h"
#include "font/library.include.h"
#include "rectAtlasBuilder/library.include.h"

namespace unigui {
namespace font{

	struct Alphabet : public std::set<UTF32>{};

	class FontExport
	{
	public:
		FontExport();
		FontExport(const char* name, const char* srcName, int size);
		
		void init(const char* name, const char* srcName, int size);

		void addFilesToAnalyse(const std::list<std::string>& filesToAnalyse);
		
		void addString(const std::string& s);
		void addString(const std::wstring& s);
		void addString(const string_utf8& s);
		void addString(const string_utf16& s);
		void addString(const string_utf32& s);
		void addString(const char* s);
		void addString(const wchar_t* s);

		void addRange(int startCodePoint, int endCodePoint);
		void addCharacterCode(int charCode);

		void clear();
		void compile(Face& face, TextureAtlasComposer::AtlasImage& img, const char* name, const char* srcName, int size);

		static float stringWidth(const char* systemFontName, int size, const char* string);
		static float stringHeight(const char* systemFontName, int size, const char* string);
		static float fontAscender(const char* systemFontName, int size);

	private:
		typedef std::vector< std::pair<int, int> > RangesList;

		
		

		std::set<std::string> filesToAnalyse;
		

		Alphabet alphabet;
		
	};

}}



// Copyright (C) 2012-2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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