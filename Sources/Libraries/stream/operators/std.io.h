#pragma once

#include <stdlib.h>

namespace stream
{

// std::string
inline ostream& operator <<(ostream& stream, const char* s)
{
	int size = int( strlen(s) ); 
	stream << size;
	stream.write(s, size);
	return stream;
}

// std::wstring
inline ostream& operator <<(ostream& stream, const wchar_t* s)
{
	int size = int( wcslen(s)); 
	stream << size;
	stream.write(s, size * sizeof(wchar_t));
	return stream;
}


}