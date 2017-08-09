// Copyright (C) 2012-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "memory.h"
#include <stdlib.h>
#include <string.h>

namespace mem
{
	
	void memset(void* dst, char val, std::size_t len)
	{
		::memset(dst, val, len);
	}

	
	void memcpy(void* dst, const void* src, std::size_t len)
	{	
		::memcpy(dst, src, len);
	}	

	void mem_desc::copy(const void* src, std::size_t len)
	{
		allocate(*this, len);
		::memcpy(data, src, len);
	}

	void mem_desc::copy(const mem_desc& src)
	{
		copy(src.data, src.len);
	}

	bool mem_desc::includeAddress(void* d) const
	{
		ptrdiff_t idata = (char*)d - (char*) data;
		
		return (idata >= 0) && (idata < len);
	}

	void mem_desc::memset(char val)
	{
		::memset(data, val, len);
	}

	
	void allocate(mem_desc& md, std::size_t size)
	{
		md.data = md.data ? realloc(md.data, size) : malloc(size);
		md.len = size;
	}

	
	void deallocate(mem_desc& md)
	{
		md.len = 0;
		free(md.data);
		md.data = nullptr;
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