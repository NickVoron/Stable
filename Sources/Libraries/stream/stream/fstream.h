// Copyright (C) 2012-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "bufstream.h"
#include "bufcrc.h"

#include "../dataop/dataop.h"
#include "stuff/stringUtils.h"

namespace stream
{



struct FormatHeader
{
	char o1; 
	char o2;
	bool crc;

	template<template<class> class O> static char tag();

	template<template<class> class O1, template<class> class O2, bool CRC>	
	static FormatHeader build() { FormatHeader fh = {tag<O1>(), tag<O2>(), CRC}; return fh;	}

	bool operator==(const FormatHeader& fh) const { return o1 == fh.o1 && o2 == fh.o2 && crc == fh.crc;	}
	bool operator!=(const FormatHeader& fh) const {	return !(*this == fh); }

	std::string toString() const { return str::stringize("(", optostr(o1), ", ", optostr(o2), ", ", crc ? "crc = true" : "crc = false", ")" ); }

	static const char* optostr(char o)
	{
		switch (o)
		{
			case 0: return "null";
			case 1: return "zlib";
			case 2: return "rijndael";
		}

		return "unknown_operator";
	}
};

template<> inline char FormatHeader::tag<dataop::null>()		{return 0;}
template<> inline char FormatHeader::tag<dataop::zlib>()		{return 1;}
template<> inline char FormatHeader::tag<dataop::rijndael>()	{return 2;}


template<bool read>
void readAndCheckStreamHeader(cfile& file, const FormatHeader& header);

template<bool write>
void writeStreamHeader(cfile& file, const FormatHeader& header);


class basic_ifstream : public istream
{
public:
	basic_ifstream();
	basic_ifstream(const char* fileName);
	virtual ~basic_ifstream();

 	virtual std::size_t read(void* buf, std::size_t size) override;

	virtual std::size_t size() const override;

	virtual std::size_t left() const override;
	virtual void skip(std::size_t bytes) override;
	virtual void rollback(std::size_t bytes) override;
	virtual bool good() const override;

	void open(const char* fileName);
	void close();
	
	const FormatHeader& getHeader() const;

protected:
	
	virtual void readData(mem::mem_desc& readed, std::size_t& cursor) = 0;
	virtual std::size_t getBufferLen() = 0;

	std::size_t readFullBuffer(void* buf, std::size_t size);

	void defaults();

	cfile file;
	mem::mem_desc data;
	std::size_t cursor;
	FormatHeader header;
};


class basic_ofstream : public ostream
{
public:
	basic_ofstream();
	basic_ofstream(const char* fileName);
	virtual ~basic_ofstream();

	virtual void write(const void* buf, std::size_t size) override;
	virtual std::size_t size() const override;

	void open(const char* fileName);
	bool good() const;

	virtual void flush() = 0;

	void close();

	const FormatHeader& getHeader() const;

protected:

	cfile file;
	FormatHeader header;

	virtual void writeData(const void* buf, std::size_t size) = 0;

	template<class MemBuf>
	void writeToBuffer(const void* buf, std::size_t size, MemBuf& buffer)
	{
		auto bufLen = buffer.getLength();
		if(size > bufLen)
		{	
			char* bufPtr = (char*)buf;
			
			std::size_t count = size / bufLen;
			std::size_t tail  = size % bufLen;

			for(std::size_t i = 0; i < count; ++i)
			{
				writeData(bufPtr, bufLen);
				bufPtr += bufLen;
			}

			if(tail > 0)
			{
				writeData(bufPtr, tail);
			}
		}
		else if( !buffer.canContain(size) )
		{
			flush();
			buffer.insert(buf, size);
		}
		else
		{
			buffer.insert(buf, size);
		}
	}
};




template<
template <class> class O1 = dataop::null,
template <class> class O2 = dataop::null,
bool writeHeader = false, 
bool crcCheck = false,
int bufferSize = 4096*4>
class ifstream;

template<
template <class> class O1 = dataop::null, 
template <class> class O2 = dataop::null,
bool writeHeader = false, 
bool crcCheck = false,
int bufferSize = 4096*4>
class ofstream;


template<template <class> class O1, template <class> class O2, bool readHeader, bool crcCheck, int bufferSize >
class ifstream : public basic_ifstream, public streambuf<bufferSize, O1, O2>
{
public:
	ifstream(){header = FormatHeader::build<O1, O2, crcCheck>();}
	ifstream(const char* fileName) : basic_ifstream(fileName)
	{
		if(readHeader)
		{
			header = FormatHeader::build<O1, O2, crcCheck>();
			readAndCheckStreamHeader<true>(file, header);	
		}
	}

	virtual ~ifstream() {}

protected:
	virtual void readData(mem::mem_desc& readed, std::size_t& cursor) override
	{
		if(cursor == readed.len)
		{
			this->buffer.clear();
						
			file.fread(&this->uplBeg, sizeof(this->uplBeg) );
			if(this->uplBeg > 0)
			{
				file.fread(&this->upl, sizeof(unsigned int) * this->uplBeg );
			}
			
			mem::mem_desc tempBuf = this->getData();
			tempBuf.len = streambuf<bufferSize, O1, O2>::OutBuffer::getLength();
			std::size_t packedSize = 0;
			file.fread( &packedSize, sizeof(packedSize) );

			if(tempBuf.len < packedSize) 
			{
				throw std::runtime_error(str::stringize("ifstream::readData(): not enough space into internal buffer. buffer size = ", tempBuf.len, " needed size = ", packedSize) );
			}
			
			file.fread( tempBuf.data, packedSize );
			tempBuf.len = packedSize;

			mem::mem_desc unpacked = this->unprocessData(tempBuf);
			readed = unpacked;			
			cursor = 0;
			this->uplBeg = 0;
			crcRead<crcCheck>::readAndCheck(unpacked.data, unpacked.len, file);
		}
	}

	virtual std::size_t getBufferLen() override
	{
		return this->buffer.getLength();
	}
};


template<int bufferSize, bool readHeader, bool crcCheck>
class ifstream<dataop::null, dataop::null, readHeader, crcCheck, bufferSize>
	:public basic_ifstream, public streambuf<bufferSize, dataop::null, dataop::null>
{
public:
	ifstream(){header = FormatHeader::build<dataop::null, dataop::null, crcCheck>();}
	ifstream(const char* fileName) : basic_ifstream(fileName)
	{
		if(readHeader)
		{
			header = FormatHeader::build<dataop::null, dataop::null, crcCheck>();
			readAndCheckStreamHeader<true>(file, header);	
		}
	}

	virtual ~ifstream() {}

private:
	virtual void readData(mem::mem_desc& readed, std::size_t& cursor) override
	{
		if(cursor == readed.len)
		{
			auto leftBytes = left() - crcRead<crcCheck>::entrySize();
			auto toRead = leftBytes < this->buffer.getLength() ? leftBytes : this->buffer.getLength();
			this->buffer.clear();
			void* dataPtr = this->buffer.getEndPtr();
			file.fread(dataPtr, toRead );
			this->buffer.setSize( toRead );
			readed = this->buffer.getDataDesc();
		   	cursor = 0;

			crcRead<crcCheck>::readAndCheck(readed.data, readed.len, file);
		}
	}

	virtual std::size_t getBufferLen() override
	{
		return streambuf<bufferSize, dataop::null, dataop::null>::buffer.getLength();
	}
};



template<template <class> class O1, template <class> class O2, bool writeHeader, bool crcCheck, int bufferSize>
class ofstream : public streambuf<bufferSize, O1, O2>, public basic_ofstream
{
public:
	
	ofstream()
	{

	}

	ofstream(const char* fileName):basic_ofstream(fileName)
	{
		if(writeHeader)
		{
			header = FormatHeader::build<O1, O2, crcCheck>();
			writeStreamHeader<true>(file, header);
		}
	}

	virtual ~ofstream(){close();}

	virtual void flush() override
	{
		mem::mem_desc bufferDesc = this->buffer.getDataDesc();
		mem::mem_desc md = this->processData(bufferDesc);

		file.fwrite(&this->uplBeg, sizeof(this->uplBeg));

		if(this->uplBeg > 0)
			file.fwrite(this->upl, sizeof(unsigned int) * this->uplBeg);
		
		file.fwrite(&md.len, sizeof(md.len) );
		file.fwrite(md.data, md.len);

		this->buffer.clear();
		this->uplBeg = 0;

		crcWrite<crcCheck>::write(bufferDesc.data, bufferDesc.len, file);
		file.fflush();
	}

protected:
	virtual void writeData(const void* buf, std::size_t size) override
	{
		writeToBuffer(buf, size, streambuf<bufferSize, O1, O2>::buffer);
	}
};


template<int bufferSize, bool writeHeader, bool crcCheck>
class ofstream<dataop::null, dataop::null, writeHeader, crcCheck, bufferSize> : public streambuf<bufferSize, dataop::null, dataop::null>, public basic_ofstream
{
public:
	ofstream()
	{
	}

	ofstream(const char* fileName):basic_ofstream(fileName)
	{
		if(writeHeader)
		{
			header = FormatHeader::build<dataop::null, dataop::null, crcCheck>();
			writeStreamHeader<true>(file, header);
		}
	}

	virtual ~ofstream(){close();}

	virtual void flush() override
	{
		mem::mem_desc bufferDesc = streambuf<bufferSize, dataop::null, dataop::null>::buffer.getDataDesc();
		if(bufferDesc.len > 0)
		{
			file.fwrite(bufferDesc.data, bufferDesc.len);
			streambuf<bufferSize, dataop::null, dataop::null>::buffer.clear();

			crcWrite<crcCheck>::write(bufferDesc.data, bufferDesc.len, file);
		}		
	}

protected:

	virtual void writeData(const void* buf, std::size_t size)  override
	{
		writeToBuffer(buf, size, streambuf<bufferSize, dataop::null, dataop::null>::buffer);
	}
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