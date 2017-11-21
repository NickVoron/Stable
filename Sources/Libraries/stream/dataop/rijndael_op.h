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

		//
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

		//
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

