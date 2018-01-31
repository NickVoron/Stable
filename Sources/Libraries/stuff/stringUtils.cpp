// Copyright (C) 2012-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "stringUtils.h"
#include <boost/filesystem.hpp>

namespace str
{
	std::ostream& operator<<(std::ostream& os, const stringize& str)
	{
		return os << str.str();
	}

	std::string msvs_out(const std::string& fileName, unsigned int line, const std::string& message)
	{
		return str::stringize(boost::filesystem::canonical(fileName).string(), "(", line, "):").str();
	}
}

namespace Base {

namespace StrUtils {

const size_t conversionBufferSize = 1024;
wchar_t wideConversionBuffer[ conversionBufferSize ];
char multiConversionBuffer[ conversionBufferSize ];


void toUpper(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

void toLower(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}


std::wstring toUnicode(const std::string& str)
{
	static const size_t maxLen = 4096;
	static wchar_t uname[maxLen];

	size_t size = str.size();
	if(size > maxLen) return std::wstring();
	
	::mbstowcs(uname, str.c_str(), size);
	uname[size] = 0;
	return std::wstring(uname);
}

std::string getLine(std::istream& stream)
{
	static char buffer[1024];
	stream.getline(buffer, 1024);
	return buffer;
}

std::wstring Convert( const std::string& s )
{
	size_t charsConverted = mbstowcs( wideConversionBuffer,	s.c_str(), conversionBufferSize );
	if ( s.size() != charsConverted )
		throw Base::Errors::ConvertError( s );

	return std::wstring(wideConversionBuffer);
}

std::string Convert( const std::wstring& s )
{
	size_t charsConverted = wcstombs( multiConversionBuffer, s.c_str(), conversionBufferSize );
	if ( s.size() != charsConverted )
		throw Base::Errors::ConvertError( "<cannot report unconverted>" );

	return std::string(multiConversionBuffer);
}

std::string sizeString(std::size_t sizeInBytes)
{	
	std::size_t		denom []	= { 1, 1024, 1024 * 1024, 1024 * 1024 * 1024 };
	const char*		name []		= {	" b", " kb", " mb", " gb" };

	std::size_t mode = 0;
	while (mode <= 3)
	{
		if(sizeInBytes < denom[mode++])
			break;
	}				  

	float val = (float) sizeInBytes / denom[mode];
	return str::stringize(val, name[mode]);
}






































}  

} //namespace Base



// Copyright (C) 2012-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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