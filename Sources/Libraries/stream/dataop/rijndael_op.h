// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "data_op.h"
#include "rijndael/library.include.h"

namespace dataop
{
	template<class D>
	struct rijndael : public D
	{
		enum {OUTPUT_SIZE = D::OUTPUT_SIZE + 16};

		rijndael() {	mem::memset(key, 0, 32); }

		
		mem::mem_desc processData( const mem::mem_desc& d )
		{
			mem::mem_desc out = D::getOutBuf(d, d.len);
	
			if(out.len > 0)
			{
				rin.init(Rijndael::CBC, Rijndael::Encrypt, key, Rijndael::Key32Bytes);
				try
				{
					int len = rin.padEncrypt((const unsigned char*)d.data, d.len, (unsigned char*)out.data);
					if(len < 0) operationError<encryption_error>("Rijndael encryption error");
					out.len = len;
				}
				catch(...)
				{
					operationError<encryption_error>("Rijndael encryption error");
				}
		   	}

			D::upl[D::uplBeg++] = out.len;

			return D::processData(out);
		}

		
		mem::mem_desc unprocessData( const mem::mem_desc& d )
		{
			mem::mem_desc dest = D::unprocessData(d);

			mem::mem_desc out = getOutBuf(dest, D::upl[--D::uplBeg]);

			if(out.len > 0)
			{
				rin.init(Rijndael::CBC, Rijndael::Decrypt, key, Rijndael::Key32Bytes);
				try
				{
					int len = rin.padDecrypt((unsigned char*)dest.data, out.len, (unsigned char*)out.data);
					if(len < 0)	operationError<decryption_error>("Rijndael decryption error");
					out.len = len;
				}
				catch(...)
				{
					operationError<decryption_error>("Rijndael decryption error");
				}
			}

			return out;
		}

		void setKey(const char* k)
		{
			mem::memset(key, 0, 32);
			mem::memcpy(key, k, sizeof(unsigned char) * 32 );
		}
		 
	private:
		unsigned char key[32];
		Rijndael rin;   
	};	

}





// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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