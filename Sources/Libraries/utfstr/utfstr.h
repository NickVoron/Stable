// Copyright (C) 2012-2015 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <string>
#include "strings/library.include.h"
#include "utfcvt/library.include.h"

struct string_utf8;
struct string_utf16;
struct string_utf32;

int utflen(const UTF8* str);
int utflen(const UTF16* str);
int utflen(const UTF32* str);

struct string_utf8 : public str::string_t<UTF8, 256>
{
	string_utf8(const UTF16* str);	
	string_utf8(const UTF32* str);
	string_utf8(const char* str);
	string_utf8(const wchar_t* str);
	string_utf8(const std::string& str);
	string_utf8(const std::wstring& str);
	string_utf8(const string_utf16& str);
	string_utf8(const string_utf32& str);

};

struct string_utf16 : public str::string_t<UTF16, 256>
{
	string_utf16(const UTF8* str);	
	string_utf16(const UTF32* str);
	string_utf16(const char* str);
	string_utf16(const wchar_t* str);
	string_utf16(const std::string& str);
	string_utf16(const std::wstring& str);
	string_utf16(const string_utf8& str);
	string_utf16(const string_utf32& str);
};

struct string_utf32 : public str::string_t<UTF32, 256>
{
	string_utf32(const UTF8* str);	
	string_utf32(const UTF16* str);
	string_utf32(const char* str);
	string_utf32(const wchar_t* str);
	string_utf32(const std::string& str);
	string_utf32(const std::wstring& str);
	string_utf32(const string_utf8& str);
	string_utf32(const string_utf16& str);
};



// Copyright (C) 2012-2015 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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