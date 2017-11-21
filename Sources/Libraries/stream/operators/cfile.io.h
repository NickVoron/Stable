#pragma once

#include "../stream/stream.h"
#include "../stream/cfile.h"

#include <iostream>

namespace stream
{

// std::wstring
inline ostream& operator <<(ostream& stream, cfile& file)
{
	static const std::size_t BUF_SIZE = 4096;
	char buf[BUF_SIZE];
	std::size_t size = file.fsize();
	while(size > 0)
	{
		std::size_t sizeToRead = size < BUF_SIZE ? size : BUF_SIZE;
		file.fread(buf, sizeToRead);
		stream.write(buf, sizeToRead);
		size -= sizeToRead;
	}

	return stream;
}

inline istream& operator >>(istream& stream, cfile& file)
{
	const std::size_t BUF_SIZE = 4096;
	char buf[BUF_SIZE];
	auto left = stream.left();
			   
	while(stream.good())
	{
		std::size_t sizeToRead = left < BUF_SIZE ? left : BUF_SIZE;
		std::size_t readed = stream.read(buf, sizeToRead);
		file.fwrite(buf, readed);
		left = stream.left();
	}

	return stream;
}

}