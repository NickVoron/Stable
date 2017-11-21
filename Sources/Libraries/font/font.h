#pragma once

#include <vector>
#include <array>

#include "utfstr/library.include.h"
#include "stream/library.include.h"
#include "unigui/library.include.h"

namespace unigui{
namespace font{

	namespace SystemSymbols
	{
		extern int NULL_SYMBOL;
		extern int EOL;
		extern int CARRIAGE_RETURN;
		extern int SPACE;
		extern int CHARACTER_TABULATION;
	}

	typedef math::Point2<int> IntPoint2;
	typedef math::Point2<float> FloatPoint2;
	typedef math::Rect<int> IntRect;
	typedef math::Rect<float> FloatRect;
	typedef math::Rect<float> CharRect;

	struct GlyphMetric
	{
		int width;
		int height;

		int horiBearingX;
		int horiBearingY;
		int horiAdvance;

		int vertBearingX;
		int vertBearingY;
		int vertAdvance;
	};

	struct Kerning
	{
		UTF32 charCode;
		int kerning;
	};
	typedef std::vector<Kerning> KerningList;

	struct CharacterDesc
	{
		UTF32 charCode;
		GlyphMetric metric;
		CharRect blackBody;
		KerningList kerning;
		int getKerning(UTF32 nextCharCode) const;
	};
	typedef std::vector<CharacterDesc> CharactersList;

	enum TextAlign
	{
		TAL_NOT_DEFINED = 0,
		TAL_LEFT				= 0x00001,
		TAL_RIGHT				= 0x00010,
		TAL_JUSTIFY				= 0x00100,
		TAL_HORIZONTAL_CENTER	= 0x01000,
		TAL_VERTICAL_CENTER		= 0x10000,
	};


	struct Face
	{
		struct GlyphInfo
		{
			int charCode;
			CharRect rect;
			CharRect texRect;
		};

		struct GlyphsQuery
		{
			struct Result
			{
				GlyphInfo* beg; 
				GlyphInfo* end;
			};

			struct Params 
			{
				Rect rect;
				int firstLineShift;
				bool wordWrap;
				unsigned int align;
			};

			Params params;
			Result result;			
		};

		struct CodeTable 
		{
			unsigned short data[65536];
		};

		typedef std::vector<GlyphInfo> GlyphsList;

		Face();

		std::string name;
		std::string systemName;
		int size;
		mutable int height;

		std::array<unsigned short, 65536> codeTable;
		CharactersList characters;

		const CharacterDesc& getCharDesc(UTF32 charCode) const;

		template<class StrType>
		void glyphs(const StrType& str, GlyphsQuery& query) const;
		
		int stringWidth(const char* str) const;
		int stringWidth(const string_utf8& str) const;
		int stringWidth(const string_utf16& str) const;
		int stringWidth(const string_utf32& str) const;
		int stringWidth(const UTF32* b, const UTF32* e) const;

		int stringHeight(const char* str) const;
		int stringHeight(const string_utf8& str) const;
		int stringHeight(const string_utf16& str) const;
		int stringHeight(const string_utf32& str) const;
		int stringHeight(const UTF32* b, const UTF32* e) const;

		void getStringSize(const UTF32* b, const UTF32* e, IntPoint2& size) const;

	private:

		void getGlyphs(const UTF32* b, const UTF32* e, const Rect& rect, int firstLineShift, bool wordWrap, unsigned int align, GlyphInfo*& beg, GlyphInfo*& end) const;

		typedef std::vector<const UTF32*> WordsVector; 
		bool prepareGlyphsBuffer(const UTF32* b, const UTF32* e, GlyphInfo*& beg, GlyphInfo*& end) const;
		void prepareWordsBuffer(const UTF32* b, const UTF32* e, WordsVector::iterator& beg, WordsVector::iterator& end) const;
		void splitStringOnWords(const UTF32* b, const UTF32* e, WordsVector::iterator& beg, WordsVector::iterator& end) const;
		static bool isSplitChar(int charCode);
		static bool isSpaceChar(int charCode);

		//	void getGlyphs(const StringUTF32::iterator& b, const StringUTF32::iterator& e, GlyphsList::iterator& beg, GlyphsList::iterator& end) const;
		//	void measureGlyphs(const StringUTF32::iterator& b, const StringUTF32::iterator& e, GlyphsList::iterator& beg, GlyphsList::iterator& end) const;
		void align(TextAlign align, const math::Rect<float>& rect, int spacesCount, const UTF32* b, const UTF32* e, GlyphInfo* beg, GlyphInfo* end) const;
		//int getKerning(const CharacterDesc& char1, const CharacterDesc& char2) const;
		//
		//вырезает из строки все невалидные символы заменяя их на служебный
		//std::wstring validateString(const std::wstring& str) const;
	};
	//typedef std::vector<Style> StylesList;

	template<class StrType>
	void Face::glyphs(const StrType& str, GlyphsQuery& query) const
	{
		string_utf32 u32(str);
		getGlyphs(&u32[0], &u32[u32.size()], query.params.rect, query.params.firstLineShift, query.params.wordWrap, query.params.align, query.result.beg, query.result.end);
	}



	// struct FontDesc
	// {
	// 	typedef stream::ofstream<> DefaultOStream;
	// 	typedef stream::ifstream<> DefaultIStream;
	// 
	// 	std::string name;
	// 	std::string sysFontName;
	// 	std::string textureFileName;
	// 	int fontSize;
	// 	
	// 	StylesList styles;
	// 
	// 	const Style& getStyle(const std::string& str);
	// };


	//
	// struct Strings
	// {
	// 	typedef std::vector<std::wstring> DataList;
	// 	DataList data;
	// 
	// 	void countSymbols();
	// 	unsigned int symbolsCount;
	// };
	// 
	// class StringProcessor
	// {
	// public:
	// 	
	// 	struct GlyphInfo
	// 	{
	// 		CharRect rect;
	// 		CharRect texRect;
	// 	};
	// 
	// 	typedef std::vector<GlyphInfo> GlyphsList;
	// 	
	// 	static void getGlyphs(const Style& style, const Strings& text, GlyphsList::iterator& beg, GlyphsList::iterator& end);
	// 	static void getGlyphs(const Style& style, const std::wstring& str, GlyphsList::iterator& beg, GlyphsList::iterator& end);
	// 	
	// 	
	// 	static void loadUnicodeTextFromFile(const char* fileName, Strings& text);
	// 
	// private:
	// 
	// 	static void reserveBuffer(int count);
	// 	static GlyphsList tempGlyphs;
	// };

	stream::ostream& operator<<(stream::ostream& s, const CharacterDesc& face);
	stream::istream& operator>>(stream::istream& s, CharacterDesc& face);
	stream::ostream& operator<<(stream::ostream& s, const Face& face);
	stream::istream& operator>>(stream::istream& s, Face& face);

}}

