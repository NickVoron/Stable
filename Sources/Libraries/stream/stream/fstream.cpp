// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "fstream.h"

#include "stuff/enforce.h"

namespace stream
{

	template<>
	void readAndCheckStreamHeader<false>(cfile& file, const FormatHeader& header){}
	template<>
	void writeStreamHeader<false>(cfile& file, const FormatHeader& header){}


	template<>
	void readAndCheckStreamHeader<true>(cfile& file, const FormatHeader& header)
	{
		FormatHeader fileHeader;
		file.fread(&fileHeader, sizeof(FormatHeader));

		if(fileHeader != header)
			throw incompatible_file_header();
	}

	template<>
	void writeStreamHeader<true>(cfile& file, const FormatHeader& header)
	{
		file.fwrite( &header, sizeof(FormatHeader) );
	}

	
	
	
	basic_ifstream::basic_ifstream()
	{
		defaults();
	}

	basic_ifstream::basic_ifstream(const char* fileName)
	{ 
		open(fileName); 
	}

	basic_ifstream::~basic_ifstream() 
	{ 
		close(); 
	}

	std::size_t basic_ifstream::read(void* buf, std::size_t size)
	{
		std::size_t totalReaded = 0;
		if (size > 0)
		{
			auto bufLen = getBufferLen();
			auto usize = size;
			while (bufLen <= usize)
			{
				auto readed = readFullBuffer(buf, bufLen);
				usize -= readed;
				buf = (char*) buf + readed;
				totalReaded += readed;
			}

			if (usize > 0)
			{
				totalReaded += readFullBuffer(buf, usize);
			}
		}
		return totalReaded;
	}

	std::size_t basic_ifstream::size() const
	{
		std::size_t s = file.fsize();
		return s > cursor ? s : cursor;
	}

	std::size_t basic_ifstream::left() const
	{
		std::size_t inBuffer = data.len - cursor;
		return file.fleft() + inBuffer;
	}

	void basic_ifstream::skip(std::size_t bytes)
	{	
		file.fskip(bytes); 
	}

	void basic_ifstream::rollback(std::size_t bytes)
	{
		INCOMPLETE;
	}

	bool basic_ifstream::good() const
	{
		bool file_good = file.fgood();
		return file_good || (!file_good && cursor < data.len);
	}

	void basic_ifstream::open(const char* fileName)	
	{ 
		defaults(); 
		file.fopen(fileName, true);
	}

	void basic_ifstream::close() 
	{ 
		defaults(); 
		file.fclose(); 
	}

	const FormatHeader& basic_ifstream::getHeader() const{return header;}

	std::size_t basic_ifstream::readFullBuffer(void* buf, std::size_t size)
	{
		readData(data, cursor);

		if (cursor > data.len)
		{
			throw cursor_out_of_bound(cursor, data.len);
		}															

		auto rSize = size;
		if(rSize > data.len - cursor)
		{	   
			rSize = data.len - cursor;
		}

		mem::memcpy(buf, (char*)data.data + cursor, rSize);
		cursor += rSize;
		if(cursor > data.len) cursor = data.len;

		if(rSize < size)
		{
			rSize += readFullBuffer((char*)buf + rSize, size - rSize);
		}

		return rSize;
	}

	void basic_ifstream::defaults()
	{
		cursor = 0;
		data.data = 0;
		data.len = 0;
	}

	
	
	
	basic_ofstream::basic_ofstream(){}
	basic_ofstream::basic_ofstream(const char* fileName){ open(fileName); }
	basic_ofstream::~basic_ofstream() {}

	void basic_ofstream::write(const void* buf, std::size_t size)
	{
		writeData(buf, size);
	}

	std::size_t basic_ofstream::size() const
	{
		return file.fsize();
	}

	void basic_ofstream::open(const char* fileName)	
	{ 
		file.fopen(fileName, false);
	}

	bool basic_ofstream::good() const
	{ 
		return file.fgood();
	}

	void basic_ofstream::close() 
	{ 
		flush();
		file.fclose(); 
	}

	const FormatHeader& basic_ofstream::getHeader() const
	{
		return header;
	}

}





// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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