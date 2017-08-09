// Copyright (C) 2012-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "data_op.h"
#include "zlib/library.include.h"

namespace dataop
{
	template<class D>
	struct zlib : public D
	{
		enum {OUTPUT_SIZE = D::OUTPUT_SIZE * 102/100+12};
  
		zlib()
		{
			compressionLevel = Z_BEST_SPEED;
		}

		
		mem::mem_desc processData( const mem::mem_desc& d )
		{
			mem::mem_desc input = d;
			mem::mem_desc out = D::getOutBuf(d, d.len);

			if(d.len)
			{
				out.len = OUTPUT_SIZE;
				int res = compress2( (Bytef*)out.data, (uLongf *)&out.len, (const Bytef*)input.data, input.len, compressionLevel);
				if(res!=Z_OK) operationError<compression_error>("zlib compression error");
			}

			D::upl[D::uplBeg++] = d.len;

			return D::processData( out );
		}

		
		mem::mem_desc unprocessData( const mem::mem_desc& d )
		{
			auto& tupl = D::upl;
			auto idx = D::uplBeg;

		   	mem::mem_desc input = D::unprocessData(d);
		   	mem::mem_desc out	= D::getOutBuf(input, tupl[idx - 1]);
			--D::uplBeg;

			
			if(out.len)
			{
				auto res = uncompress((Bytef*)out.data, (uLongf *)&out.len, (const Bytef*)input.data, input.len);
				if(res!=Z_OK) operationError<decompression_error>("zlib decompression error");
			}

			return out;
		}

		void setCompLevel(int l)
		{
			compressionLevel = l;
		}

	private:
		int compressionLevel;
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