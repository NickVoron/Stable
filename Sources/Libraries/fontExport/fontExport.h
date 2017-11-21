#pragma once

#include "math/library.include.h"
#include "utfstr/library.include.h"
#include "font/library.include.h"
#include "rectAtlasBuilder/library.include.h"

namespace unigui {
namespace font{

	struct Alphabet : public std::set<UTF32>{};

	class FontExport
	{
	public:
		FontExport();
		FontExport(const char* name, const char* srcName, int size);
		
		void init(const char* name, const char* srcName, int size);

		void addFilesToAnalyse(const std::list<std::string>& filesToAnalyse);
		
		void addString(const std::string& s);
		void addString(const std::wstring& s);
		void addString(const string_utf8& s);
		void addString(const string_utf16& s);
		void addString(const string_utf32& s);
		void addString(const char* s);
		void addString(const wchar_t* s);

		void addRange(int startCodePoint, int endCodePoint);
		void addCharacterCode(int charCode);

		void clear();
		void compile(Face& face, TextureAtlasComposer::AtlasImage& img, const char* name, const char* srcName, int size);

		static float stringWidth(const char* systemFontName, int size, const char* string);
		static float stringHeight(const char* systemFontName, int size, const char* string);
		static float fontAscender(const char* systemFontName, int size);

	private:
		typedef std::vector< std::pair<int, int> > RangesList;

		//void createAlphabet(Alphabet& alphabet);
		//void flushAllToUTF32();

		std::set<std::string> filesToAnalyse;
		//std::list<string_utf32> utf32strs;

		Alphabet alphabet;
		//RangesList ranges;
	};

}}