#include "utfstr.h"
#include "utfcvt_template/library.include.h"

namespace
{
	template<class UTF_T>
	int utflen_impl(const UTF_T* str)
	{
		const UTF_T* i = str;
		while(*i != 0) 
			++i;

		return (i - str) - 1;		
	}

	int utflen(const UTF8* str) { return utflen_impl(str); }
	int utflen(const UTF16* str){ return utflen_impl(str); }
	int utflen(const UTF32* str){ return utflen_impl(str); }

	template<class UTF_T0, class UTF_T1, int maxLen>
	void convert(const UTF_T0* source, str::string_t<UTF_T1, maxLen>& dest)
	{
		static UTF_T1 data[maxLen];
		UTF_T1* d = data;
		int endCursor = utflen(source) + 1;
		UTF::Convert(&source, source + endCursor, &d, d + endCursor, lenientConversion);
		data[endCursor] = 0;
		dest = data;
	}

	template<class UTF_T1, int maxLen>
	void convert(const char* source, str::string_t<UTF_T1, maxLen>& dest)
	{
		static UTF8 data[maxLen];
		for (int i = 0; i < maxLen; ++i)
		{
			data[i] = source[i];

			if(source[i] == 0)
				break;
		}

		convert(data, dest);
	}

	template<class UTF_T1, int maxLen>
	void convert(const wchar_t* source, str::string_t<UTF_T1, maxLen>& dest)
	{
		static UTF16 data[maxLen];
		for (int i = 0; i < maxLen; ++i)
		{
			data[i] = source[i];

			if(source[i] == 0)
				break;
		}

		convert(data, dest);
	}
}




string_utf8::string_utf8(const UTF16* str) { convert(str, *this); }
string_utf8::string_utf8(const UTF32* str) { convert(str, *this); }
string_utf8::string_utf8(const char* str)		{ convert(str, *this); }
string_utf8::string_utf8(const wchar_t* str)	{ convert(str, *this); }
string_utf8::string_utf8(const std::string& str) { convert(str.c_str(), *this);}
string_utf8::string_utf8(const std::wstring& str){ convert(str.c_str(), *this);}
string_utf8::string_utf8(const string_utf16& str){ convert(str.c_str(), *this);}
string_utf8::string_utf8(const string_utf32& str){ convert(str.c_str(), *this);}

string_utf16::string_utf16(const UTF8* str)	{ convert(str, *this); }
string_utf16::string_utf16(const UTF32* str)	{ convert(str, *this); }
string_utf16::string_utf16(const char* str)			{ convert(str, *this); }
string_utf16::string_utf16(const wchar_t* str)		{ convert(str, *this); }
string_utf16::string_utf16(const std::string& str)	{ convert(str.c_str(), *this);}
string_utf16::string_utf16(const std::wstring& str)	{ convert(str.c_str(), *this);}
string_utf16::string_utf16(const string_utf8& str)	{ convert(str.c_str(), *this);}
string_utf16::string_utf16(const string_utf32& str)	{ convert(str.c_str(), *this);}

string_utf32::string_utf32(const UTF8* str)	{ convert(str, *this); }
string_utf32::string_utf32(const UTF16* str)	{ convert(str, *this); }
string_utf32::string_utf32(const char* str)			{ convert(str, *this); }
string_utf32::string_utf32(const wchar_t* str)		{ convert(str, *this); }
string_utf32::string_utf32(const std::string& str)	{ convert(str.c_str(), *this);}
string_utf32::string_utf32(const std::wstring& str)	{ convert(str.c_str(), *this);}
string_utf32::string_utf32(const string_utf8& str)	{ convert(str.c_str(), *this);}
string_utf32::string_utf32(const string_utf16& str)	{ convert(str.c_str(), *this);}
