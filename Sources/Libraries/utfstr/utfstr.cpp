// Copyright (C) 2012-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "utfstr.h"
#include "utfcvt_template/library.include.h"

namespace
{
	template<class UTF_T>
	auto utflen_impl(const UTF_T* str)
	{
		const UTF_T* i = str;
		while(*i != 0) 
			++i;

		return (i - str) - 1;		
	}

	auto utflen(const UTF8* str) { return utflen_impl(str); }
	auto utflen(const UTF16* str){ return utflen_impl(str); }
	auto utflen(const UTF32* str){ return utflen_impl(str); }

	template<class UTF_T0, class UTF_T1, int maxLen>
	void convert(const UTF_T0* source, str::string_t<UTF_T1, maxLen>& dest)
	{
		static UTF_T1 data[maxLen];
		UTF_T1* d = data;
		auto endCursor = utflen(source) + 1;
		UTF::Convert(&source, source + endCursor, &d, d + endCursor, lenientConversion);
		data[endCursor] = 0;
		dest = data;
	}
    
    template<class UTF_T1, int maxLen>
    void convert_0(const char* source, str::string_t<UTF_T1, maxLen>& dest)
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




string_utf8::string_utf8(const UTF16* str)			{ }
string_utf8::string_utf8(const UTF32* str)			{ }
string_utf8::string_utf8(const char* str)			{ }
string_utf8::string_utf8(const wchar_t* str)		{ }
string_utf8::string_utf8(const std::string& str)	{ }
string_utf8::string_utf8(const std::wstring& str)	{ }
string_utf8::string_utf8(const string_utf16& str)	{ }
string_utf8::string_utf8(const string_utf32& str)	{ }
													  
string_utf16::string_utf16(const UTF8* str)			{ }
string_utf16::string_utf16(const UTF32* str)		{ }
string_utf16::string_utf16(const char* str)			{ }
string_utf16::string_utf16(const wchar_t* str)		{ }
string_utf16::string_utf16(const std::string& str)	{ }
string_utf16::string_utf16(const std::wstring& str)	{ }
string_utf16::string_utf16(const string_utf8& str)	{ }
string_utf16::string_utf16(const string_utf32& str)	{ }
													  
string_utf32::string_utf32(const UTF8* str)			{ }
string_utf32::string_utf32(const UTF16* str)		{ }
string_utf32::string_utf32(const char* str)			{ }
string_utf32::string_utf32(const wchar_t* str)		{ }
string_utf32::string_utf32(const std::string& str)	{ }
string_utf32::string_utf32(const std::wstring& str)	{ }
string_utf32::string_utf32(const string_utf8& str)	{ }
string_utf32::string_utf32(const string_utf16& str)	{ }




// Copyright (C) 2012-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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