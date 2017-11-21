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

		//
		mem::mem_desc processData( const mem::mem_desc& d )
		{
			mem::mem_desc input = d;
			mem::mem_desc out = D::getOutBuf(d, d.len);

			if(d.len)
			{
				out.len = OUTPUT_SIZE;
				int res = compress2( (Bytef*)out.data, (uLongf *)&out.len, (const Bytef*)input.data, static_cast<uLong>(input.len), compressionLevel);
				if(res!=Z_OK) operationError<compression_error>("zlib compression error");
			}

			D::upl[D::uplBeg++] = d.len;

			return D::processData( out );
		}

		//
		mem::mem_desc unprocessData( const mem::mem_desc& d )
		{
			auto& tupl = D::upl;
			auto idx = D::uplBeg;

		   	mem::mem_desc input = D::unprocessData(d);
		   	mem::mem_desc out	= D::getOutBuf(input, tupl[idx - 1]);
			--D::uplBeg;

			
			if(out.len)
			{
				uLongf outlen = static_cast<decltype(outlen)>(out.len);
				auto res = uncompress((Bytef*)out.data, (uLongf*)&outlen, (const Bytef*)input.data, static_cast<uLong>(input.len));
				out.len = static_cast<decltype(out.len)>(outlen);
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
