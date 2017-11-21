#include "stringUtils.h"

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

// все символы в верхний/нижний регистр
void toUpper(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

void toLower(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

//
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

//
//DWORD hexToDword(const std::string & hex)
//{
//	std::string preparedString = hex;
//	if(preparedString[1] == 'x')
//	{
//		preparedString = preparedString.substr(2, preparedString.size() - 2 );
//	}
//
//	int size = preparedString.size();
//	int c = 8 - size;
//	if(c > 0)
//	{
//		for (int i = 0; i < c; ++i)
//		{
//			preparedString = "0" + preparedString;
//		}
//		hexToDword(preparedString);
//	}
//
//	DWORD v = 0;
//	size = static_cast<int>(hex.size());
//	DWORD h = 1;
//	for(int b = size - 1; b >= 0; --b)
//	{
//		BYTE x = 0;
//		if('0' <= hex[b] && hex[b] <= '9') x = hex[b] - '0';
//		else {
//			const char l = tolower(hex[b]);
//			if('a' <= l && l <= 'f') x = l + 10 - 'a';
//		}
//		v += x * h;
//		h <<= 4;
//	}
//	return v;
//}

}  // namespace StrUtils

} //namespace Base