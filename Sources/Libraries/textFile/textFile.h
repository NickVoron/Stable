// Copyright (C) 2012-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "stuff/library.include.h"

namespace TextFile
{
	namespace CharSet
	{
		enum CS
		{
			UTF_8,		
			UTF_7,		
			UTF_16BE,	
			UTF_16LE,	
			UTF_32BE,	
			UTF_32LE,	
			UTF_EBCDIC,
			SCSU,		
			BOCU_1,
			UNKNOWN_CHARSET,
			CHARSETS_COUNT,
		};

		DECLARE_ENUM_STR_TABLE(CS);
	}

	const char* getCharsetName(CharSet::CS charSet);
	CharSet::CS charsetRecognize(const char* buffer);

	class TextFromFile
	{
	public:
		TextFromFile();
		TextFromFile(const char* fileName);
		~TextFromFile();
		void load(const char* fileName);
		void text(const char** pdata, int* size) const;
		std::string text() const;
	private:
		void clear();
		void defaults();

		CharSet::CS charset;
		char* buffer;
		unsigned int textStartShift;
		size_t bufferSize;
	};
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