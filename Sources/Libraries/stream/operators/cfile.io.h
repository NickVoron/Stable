// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "../stream/stream.h"
#include "../stream/cfile.h"

#include <iostream>

namespace stream
{


inline ostream& operator <<(ostream& stream, cfile& file)
{
	static const std::size_t BUF_SIZE = 4096;
	char buf[BUF_SIZE];
	std::size_t size = file.fsize();
	while(size > 0)
	{
		std::size_t sizeToRead = size < BUF_SIZE ? size : BUF_SIZE;
		file.fread(buf, sizeToRead);
		stream.write(buf, sizeToRead);
		size -= sizeToRead;
	}

	return stream;
}

inline istream& operator >>(istream& stream, cfile& file)
{
	const std::size_t BUF_SIZE = 4096;
	char buf[BUF_SIZE];
	auto left = stream.left();
			   
	while(stream.good())
	{
		std::size_t sizeToRead = left < BUF_SIZE ? left : BUF_SIZE;
		std::size_t readed = stream.read(buf, sizeToRead);
		file.fwrite(buf, readed);
		left = stream.left();
	}

	return stream;
}

}



// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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