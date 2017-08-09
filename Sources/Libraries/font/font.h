// Copyright (C) 2012-2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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

		
		
		void align(TextAlign align, const math::Rect<float>& rect, int spacesCount, const UTF32* b, const UTF32* e, GlyphInfo* beg, GlyphInfo* end) const;
		
		
		
		
	};
	

	template<class StrType>
	void Face::glyphs(const StrType& str, GlyphsQuery& query) const
	{
		string_utf32 u32(str);
		getGlyphs(&u32[0], &u32[u32.size()], query.params.rect, query.params.firstLineShift, query.params.wordWrap, query.params.align, query.result.beg, query.result.end);
	}



	
	
	
	
	
	
	
	
	
	
	
	
	
	


	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

	stream::ostream& operator<<(stream::ostream& s, const CharacterDesc& face);
	stream::istream& operator>>(stream::istream& s, CharacterDesc& face);
	stream::ostream& operator<<(stream::ostream& s, const Face& face);
	stream::istream& operator>>(stream::istream& s, Face& face);

}}





// Copyright (C) 2012-2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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