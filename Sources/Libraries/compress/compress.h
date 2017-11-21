#pragma once

#include "zlib/library.include.h"
#include "memory/library.include.h"
#include "stream/library.include.h"

struct zlib
{
	~zlib();

	void pack(void* data, unsigned int dataLen, int compressionLevel = Z_BEST_COMPRESSION);
	void pack(const mem::mem_desc& data, int compressionLevel = Z_BEST_COMPRESSION);
	void save(stream::ostream& os) const;

	void saveDeflate(stream::ostream& os) const;

	void save(const char* fileName) const;
	void saveDeflate(const char* fileName) const;

	mem::mem_desc out;
};

void zlib_it(const mem::mem_desc& data, const char* fileName);
void deflate_it(const mem::mem_desc& data, const char* fileName);

void lz4_it(const mem::mem_desc& data, const char* fileName);
