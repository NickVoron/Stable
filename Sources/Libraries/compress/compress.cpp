// Copyright (C) 2012-2013 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "compress.h"

zlib::~zlib(){ mem::deallocate(out); }

void zlib::pack(void* data, unsigned int dataLen, int compressionLevel)
{
	mem::allocate(out, dataLen * 102/100+12);

	ENFORCE( compress2( (Bytef*)out.data, (uLongf *)&out.len, (const Bytef*)data, dataLen, compressionLevel) == Z_OK );


}

void zlib::pack(const mem::mem_desc& data, int compressionLevel)	{ pack(data.data, data.len, compressionLevel); }
void zlib::save(stream::ostream& os) const { os.write(out.data, out.len); }

void zlib::saveDeflate(stream::ostream& os) const
{
	char start = 2;
	char adler = 4;
	char* data = (char*)out.data + start;
	os.write(data, out.len - (start + adler));
}

void zlib::save(const char* fileName) const { stream::ofstream<> os(fileName); save(os); }
void zlib::saveDeflate(const char* fileName) const { stream::ofstream<> os(fileName);	saveDeflate(os); }


void zlib_it(const mem::mem_desc& data, const char* fileName)
{
	zlib zl; zl.pack(data);
	zl.save(fileName);
}

void deflate_it(const mem::mem_desc& data, const char* fileName)
{
	zlib zl; zl.pack(data);
	zl.saveDeflate(fileName);

	LOG_MSG( fileName <<" : " << "unpaked size = " << data.len << "packed size = " << zl.out.len);
}



// Copyright (C) 2012-2013 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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