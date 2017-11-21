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