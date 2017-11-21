#pragma once

namespace compression
{	
	int lzma_cq(int sourceSize);
	int bcm_cq(int sourceSize);
	int lz4_cq(int sourceSize);
	int lz4hc_cq(int sourceSize);
	int ppmd_cq(int sourceSize);
	int zlib_cq(int sourceSize);

	int lzma_c(const unsigned char* source, unsigned char* dest, int sourceSize);
	int bcm_c(const unsigned char* source, unsigned char* dest, int sourceSize);
	int lz4_c(const unsigned char* source, unsigned char* dest, int sourceSize);
	int lz4hc_c(const unsigned char* source, unsigned char* dest, int sourceSize);
	int ppmd_c(const unsigned char* source, unsigned char* dest, int sourceSize);
	int zlib_c(const unsigned char* source, unsigned char* dest, int sourceSize);

	int lzma_dc(const unsigned char* source, unsigned char* dest, int compressedSize, int originalSize);
	int bcm_dc(const unsigned char* source, unsigned char* dest, int compressedSize, int originalSize);
	int lz4_dc(const unsigned char* source, unsigned char* dest, int compressedSize, int originalSize);
	int lz4hc_dc(const unsigned char* source, unsigned char* dest, int compressedSize, int originalSize);
	int ppmd_dc(const unsigned char* source, unsigned char* dest, int compressedSize, int originalSize);
	int zlib_dc(const unsigned char* source, unsigned char* dest, int compressedSize, int originalSize);
}