// Copyright (C) 2012-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "cfile.h"
#include "errors.h"

#include "hash/library.include.h"

namespace stream
{

template<bool crcCheck = true> struct crcRead;
template<bool crcCheck = true> struct crcWrite;

template<> 
struct crcRead<true>
{
	static unsigned long read(cfile& file)
	{
		unsigned long readedSumm = -1;
		file.fread(&readedSumm, sizeof(readedSumm) );
		return readedSumm;
	}

	static unsigned long calc(const void* buf, std::size_t len)
	{
		return crc::CRC32(buf, len);		
	}

	static void readAndCheck(const void* buf, std::size_t len, cfile& file)
	{
		if(read(file) != calc(buf, len)) 
			throw crc_check_failed();
	}

	static int entrySize(){ return sizeof(unsigned long);}
};

template<> 
struct crcRead<false>
{
	static void readAndCheck(const void* buf, std::size_t len, cfile& file){}
	static int entrySize(){ return 0;}
};

template<> 
struct crcWrite<true>
{
	static void write(const void* buf, std::size_t len, cfile& file)
	{
		unsigned long crcSumm = crc::CRC32(buf, len);
		file.fwrite(&crcSumm, sizeof(unsigned long) );
	}
		
};

template<> 
struct crcWrite<false>
{
	static void write(const void* buf, std::size_t len, cfile& file)	{}
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