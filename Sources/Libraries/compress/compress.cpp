#include "compress.h"

zlib::~zlib(){ mem::deallocate(out); }

void zlib::pack(void* data, unsigned int dataLen, int compressionLevel)
{
	mem::allocate(out, dataLen * 102/100+12);
//	Base::Timer t;
	ENFORCE( compress2( (Bytef*)out.data, (uLongf *)&out.len, (const Bytef*)data, dataLen, compressionLevel) == Z_OK );
// 	float dt = t.getDT();
// 	LOG_MSG( "time: " << dt );
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