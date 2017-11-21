#pragma once

#include "cfile.h"
#include "errors.h"

#include "hash/library.include.h"

namespace stream
{

template<bool crcCheck = true> struct crcRead;
template<bool crcCheck = true> struct crcWrite;

template<> 
struct crcRead<true>
{
	static unsigned long read(cfile& file)
	{
		unsigned long readedSumm = -1;
		file.fread(&readedSumm, sizeof(readedSumm) );
		return readedSumm;
	}

	static unsigned long calc(const void* buf, std::size_t len)
	{
		return crc::CRC32(buf, len);		
	}

	static void readAndCheck(const void* buf, std::size_t len, cfile& file)
	{
		if(read(file) != calc(buf, len)) 
			throw crc_check_failed();
	}

	static int entrySize(){ return sizeof(unsigned long);}
};

template<> 
struct crcRead<false>
{
	static void readAndCheck(const void* buf, std::size_t len, cfile& file){}
	static int entrySize(){ return 0;}
};

template<> 
struct crcWrite<true>
{
	static void write(const void* buf, std::size_t len, cfile& file)
	{
		unsigned long crcSumm = crc::CRC32(buf, len);
		file.fwrite(&crcSumm, sizeof(unsigned long) );
	}
		
};

template<> 
struct crcWrite<false>
{
	static void write(const void* buf, std::size_t len, cfile& file)	{}
};

}
