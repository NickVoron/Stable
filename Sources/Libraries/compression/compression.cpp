#include "compression.h"

#include "stuff/library.include.h"

#include "stuff/enforce.h"

#include "zlib/library.include.h"

#include "lz4/lz4.h"
#include "lz4/lz4hc.h"
#include "lz4/xxhash.h"

#include "lzma/LzmaLib.h"

#include "bcm/bcm.h"

namespace compression
{
	int lzma_cq(int sourceSize) { return sourceSize * 3 + 5; }
	int lzma_c(const unsigned char* source, unsigned char* dest, int sourceSize)
	{
		size_t outPropsSize = LZMA_PROPS_SIZE;
 		size_t destLen = lzma_cq(sourceSize);

		LzmaCompress(&dest[LZMA_PROPS_SIZE], &destLen, source, sourceSize,
			&dest[0], &outPropsSize, 
			9,      
			1 << 24,  
			3,        
			0,        
			2,        
			32,        
			1 
			);

		return destLen;
	}

	int lzma_dc(const unsigned char* source, unsigned char* dest, int compressedSize, int originalSize)
	{				
		size_t destLen1 = -1;
		SizeT srcSize = compressedSize;

		auto res = LzmaUncompress(dest, &destLen1, &source[LZMA_PROPS_SIZE], &srcSize, &source[0], LZMA_PROPS_SIZE);

		return destLen1;
	}	  	


	int bcm_cq(int sourceSize) { return impl_bcm_cq(sourceSize); }
	int bcm_c(const unsigned char* source, unsigned char* dest, int sourceSize)
	{
		return impl_bcm_c(source, dest, sourceSize);
	}

	int bcm_dc(const unsigned char* source, unsigned char* dest, int compressedSize, int originalSize)
	{
		return impl_bcm_dc(source, dest, compressedSize, originalSize);
	}

   

	int lz4_cq(int sourceSize) { return LZ4_compressBound(sourceSize); }
	int lz4_c(const unsigned char* source, unsigned char* dest, int sourceSize) {	return LZ4_compress((const char*)source, (char*) dest, sourceSize); 	}
	int lz4_dc(const unsigned char* source, unsigned char* dest, int compressedSize, int originalSize)
	{	  
		auto resultSz = LZ4_decompress_fast((char*) source, (char*) dest, originalSize);
		ENFORCE(resultSz == compressedSize);
		return originalSize;
	}



	
	int lz4hc_cq(int sourceSize) { return LZ4_compressBound(sourceSize); }
	int lz4hc_c(const unsigned char* source, unsigned char* dest, int sourceSize) { return LZ4_compressHC((char*) source, (char*) dest, sourceSize); }
	int lz4hc_dc(const unsigned char* source, unsigned char* dest, int compressedSize, int originalSize)
	{
		ENFORCE(LZ4_decompress_fast((char*) source, (char*) dest, originalSize) == compressedSize);
		return originalSize;
	}



	
	int ppmd_cq(int sourceSize) { return 0; }
	
	int ppmd_c(const unsigned char* source, unsigned char* dest, int sourceSize)
	{
		
		return 0;
	}

	int ppmd_dc(const unsigned char* source, unsigned char* dest, int compressedSize, int originalSize)
	{
		return 0;
	}


	
	int zlib_cq(int sourceSize) 
	{ 
		__int64 sz = sourceSize;
		sz *= 102;
		sz /= 100;
		return sz + 12;
	}

	int zlib_c(const unsigned char* source, unsigned char* dest, int sourceSize)
	{
		uLongf destLen = zlib_cq(sourceSize);
		compress2((Bytef*) dest, (uLongf *) &destLen, (const Bytef*) source, sourceSize, Z_BEST_COMPRESSION);

		return destLen;
	}

	int zlib_dc(const unsigned char* source, unsigned char* dest, int compressedSize, int originalSize)
	{
		uLongf destLen = originalSize;
		int res = uncompress((Bytef*) dest, (uLongf *) &destLen, (const Bytef*) source, compressedSize);
		ENFORCE(res == Z_OK);
		return destLen;
	}

}