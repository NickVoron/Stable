// Copyright (C) 2012-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "createFromDesc.h"
#include "stuff/library.include.h"

namespace unigui 
{
namespace font
{		
	namespace
	{
		typedef unsigned int DWORD;
		typedef unsigned char BYTE;

		DWORD hexToDword(const std::string & hex)
		{
			std::string preparedString = hex;
			if(preparedString[1] == 'x')
			{
				preparedString = preparedString.substr(2, preparedString.size() - 2 );
			}

			int size = preparedString.size();
			int c = 8 - size;
			if(c > 0)
			{
				for (int i = 0; i < c; ++i)
				{
					preparedString = "0" + preparedString;
				}
				hexToDword(preparedString);
			}

			DWORD v = 0;
			size = static_cast<int>(hex.size());
			DWORD h = 1;
			for(int b = size - 1; b >= 0; --b)
			{
				BYTE x = 0;
				if('0' <= hex[b] && hex[b] <= '9') x = hex[b] - '0';
				else {
					const char l = tolower(hex[b]);
					if('a' <= l && l <= 'f') x = l + 10 - 'a';
				}
				v += x * h;
				h <<= 4;
			}
			return v;
		}
	}

	void createFromXMLDesc(const char *descFileName, FontExport& font)
	{
		
	}
}
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