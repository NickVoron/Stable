#pragma once

#include "stuff/library.include.h"

namespace TextFile
{
	namespace CharSet
	{
		enum CS
		{
			UTF_8,		
			UTF_7,		
			UTF_16BE,	
			UTF_16LE,	
			UTF_32BE,	
			UTF_32LE,	
			UTF_EBCDIC,
			SCSU,		
			BOCU_1,
			UNKNOWN_CHARSET,
			CHARSETS_COUNT,
		};

		DECLARE_ENUM_STR_TABLE(CS);
	}

	const char* getCharsetName(CharSet::CS charSet);
	CharSet::CS charsetRecognize(const char* buffer);

	class TextFromFile
	{
	public:
		TextFromFile();
		TextFromFile(const char* fileName);
		~TextFromFile();
		void load(const char* fileName);
		void text(const char** pdata, int* size) const;
		std::string text() const;
	private:
		void clear();
		void defaults();

		CharSet::CS charset;
		char* buffer;
		unsigned int textStartShift;
		size_t bufferSize;
	};
}