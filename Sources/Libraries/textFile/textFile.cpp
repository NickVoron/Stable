// Copyright (C) 2012-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "textFile.h"

namespace TextFile
{
	namespace CharSet
	{
		BEGIN_ENUM_STR_TABLE(CS)
		{UTF_8,			"UTF-8"},
		{UTF_7,			"UTF-7"},
		{UTF_16BE,		"UTF-16BE"},
		{UTF_16LE,		"UTF-16LE"},
		{UTF_32BE,		"UTF-32BE"},
		{UTF_32LE,		"UTF-32LE"},
		{UTF_EBCDIC,	"UTF-EBCDIC"},
		{SCSU,			"SCSU"},
		{BOCU_1,		"BOCU-1"},
		{UNKNOWN_CHARSET,   "not Unicode charset"},
		END_ENUM_STR_TABLE(CS);
	}

	namespace 
	{
		bool isUTF8(const char* input) { return input[0] == 0xEF && input[1] == 0xBB && input[2] == 0xBF;}

		bool isUTF7(const char* input) 
		{
			bool common = (input[0] == 0x2B) && (input[1] == 0x2F) && (input[2] == 0x76);

			char c = input[3];
			return common && ( c == 0x38 || c == 0x39 || c == 0x2B || c == 0x2F);
		}

		bool isUTF_EBCDIC(const char* input) { return input[0] == 0xDD && input[1] == 0x73 && input[2] == 0x66 && input[3] == 0x73;}
		bool isSCSU(const char* input)		 { return input[0] == 0x0E && input[1] == 0xFE && input[2] == 0xFF;}
		bool isBOCU_1(const char* input)	 { return input[0] == 0xFB && input[1] == 0xEE && input[2] == 0x28;}

		bool isUTF16BE(const char* input) { return input[0] == 0xFE && input[1] == 0xFF;}
		bool isUTF16LE(const char* input) { return input[0] == 0xFF && input[1] == 0xFE;}

		bool isUTF32BE(const char* input) { return input[0] == 0x00 && input[1] == 0x00 && input[2] == 0xFE && input[3] == 0xFF;}
		bool isUTF32LE(const char* input) {	return input[0] == 0xFF && input[1] == 0xFE && input[2] == 0x00 && input[3] == 0x00;}
	}

	const char* getCharsetName(CharSet::CS charSet)
	{
		using namespace CharSet;
		return CONVERT_ENUM_TO_STR(CS, charSet);
	}

	CharSet::CS recognizeCharset(const char* input)
	{
		using namespace CharSet;
		CS res = UNKNOWN_CHARSET;

		if( isUTF8(input) )		res = UTF_8;
		else if( isUTF16BE(input) )		res = UTF_16BE;
		else if( isUTF16LE(input) )		res = UTF_16LE;
		else if( isUTF32BE(input) )		res = UTF_32BE;
		else if( isUTF32LE(input) )		res = UTF_32LE;
		else if( isUTF7(input) )		res = UTF_7;
		else if( isUTF_EBCDIC(input) )	res = UTF_EBCDIC;
		else if( isSCSU(input) )		res = SCSU;
		else if( isBOCU_1(input) )		res = BOCU_1;

		return res;
	}

	int getBytesCountOfFEFF(CharSet::CS charSet)
	{
		using namespace CharSet;
		switch(charSet)
		{
		case UTF_8:		 return 3;
		case UTF_16BE:	 return 2;
		case UTF_16LE:	 return 2;
		case UTF_32BE:	 return 4;
		case UTF_32LE:	 return 4;
		case UTF_7:		 return 0;
		case UTF_EBCDIC: return 4;
		case SCSU:		 return 3;
		case BOCU_1:	 return 0;
		default:		 return 0;
		};
	}

	
	
	
	TextFromFile::TextFromFile()
	{
		defaults();
	}

	TextFromFile::TextFromFile(const char* fileName)
	{
		defaults();
		load(fileName);
	}

	TextFromFile::~TextFromFile()
	{
		clear();
	}

	void TextFromFile::load(const char* fileName)
	{
		clear();

		stream::cfile f(fileName, true, true);
		if (f.fgood())
		{
			bufferSize = f.fsize();
			buffer = (char*) malloc(bufferSize + 1);
			f.fread(buffer, bufferSize);
			buffer[bufferSize] = 0;
			f.fclose();
			charset = recognizeCharset(buffer);
			textStartShift = getBytesCountOfFEFF(charset);
		}			
	}

	void TextFromFile::text(const char** pdata, int* size) const
	{
		if(pdata)
		{
			*pdata = buffer + textStartShift;
		}

		if (size)
		{
			*size = bufferSize - textStartShift;
		}
	}

	std::string TextFromFile::text() const
	{
		return std::string(buffer + textStartShift);
	}

	void TextFromFile::clear()
	{
		if(buffer)
		{
			free(buffer);
		}

		defaults();
	}

	void TextFromFile::defaults()
	{
		charset = CharSet::UNKNOWN_CHARSET;
		buffer = nullptr;
		textStartShift = 0;
		bufferSize = 0;
	}

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