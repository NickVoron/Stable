 #include "font.h"

#include "stuff/library.include.h"

namespace unigui {
namespace font {
 	namespace SystemSymbols
	{
		int NULL_SYMBOL				= 0x0000;
		int EOL						= 0x000A;
		int CARRIAGE_RETURN			= 0x000D;
		int SPACE					= 0x0020;
		int CHARACTER_TABULATION	= 0x0009;
	}

//
stream::ostream& operator<<(stream::ostream& s, const Face& face)
{
	s << face.name << face.systemName << face.codeTable << face.height;

	int count = (int)face.characters.size();
	s << count;
	for (int i = 0; i < count; ++i)
	{
		s << face.characters[i];
	}

	return s;
}

stream::istream& operator>>(stream::istream& s, Face& face)
{
	s >> face.name >> face.systemName >> face.codeTable >> face.height;

	int count = 0;
	s >> count;
	face.characters.resize(count);
	for (int i = 0; i < count; ++i)
	{
		s >> face.characters[i];
	}

	return s;
}

stream::ostream& operator<<(stream::ostream& s, const CharacterDesc& charDesc)
{
	return s << charDesc.charCode << charDesc.metric << charDesc.blackBody << charDesc.kerning;
}

stream::istream& operator>>(stream::istream& s, CharacterDesc& charDesc)
{
	return s >> charDesc.charCode >> charDesc.metric >> charDesc.blackBody >> charDesc.kerning;
}

// stream::ostream& operator<<(stream::ostream& s, const FontDesc& desc)
// {
// 	s << desc.name << desc.sysFontName << desc.textureFileName << desc.fontSize;
// 
// 	int size = (int)desc.styles.size();
// 	s << size;
// 	for (int i = 0; i < size; ++i)
// 	{
// 		s << desc.styles[i];		
// 	}
// 
// 	return s;
// }
// 
// stream::istream& operator>>(stream::istream& s, FontDesc& desc)
// {
// 	s >> desc.name >> desc.sysFontName >> desc.textureFileName >> desc.fontSize;
// 
// 	int size; s >> size;
// 	desc.styles.resize(size);
// 	for (int i = 0; i < size; ++i)
// 	{
// 		s >> desc.styles[i];		
// 	}
// 
// 	return s;
// }
// 
// const Style& FontDesc::getStyle(const std::string& str)
// {
// 	foreach(const Style& style, styles)
// 	{
// 		if(style.name == str) return style;
// 	}
// 
// 	throw Base::Errors::Simple("FontDesc::getStyle() : not found style : font name = " + name + " style name = " + str);
// }
// 
// void Strings::countSymbols()
// {
// 	symbolsCount = 0;
// 	foreach(const std::wstring& str, data)
// 	{
// 		symbolsCount += str.size();
// 	}
// }

int CharacterDesc::getKerning(UTF32 nextCharCode) const
{
	for(const Kerning& kern : kerning)
	{
		if(kern.charCode == nextCharCode)
			return kern.kerning;
	}

	return 0;
}

Face::Face()
{
	mem::memzero(codeTable);
}

const CharacterDesc& Face::getCharDesc(UTF32 charCode) const
{					   /*
	CodeTranslationTable::const_iterator it = codeTable.find(unicodeValue);
	if( it == codeTable.end() )
	{
		return characters[ codeTable.begin()->second ];
		std::string codeStr = Base::StrUtils::toString(unicodeValue);
		throw Base::Errors::Simple("Style::getCharDesc(): symbol with code [" + codeStr + "] not in compiled range in font");
	}

	const CharacterDesc& charDesc = characters[ it->second ];
	return  charDesc;*/
	return characters[ codeTable[charCode] ];
}

void Face::getGlyphs(const UTF32* b, const UTF32* e, const Rect& inRect, int firstLineShift, bool wordWrap, unsigned int align_, GlyphInfo*& beg, GlyphInfo*& end) const
{
	Rect rect = inRect;
	rect.normalize();

	if( !prepareGlyphsBuffer(b, e, beg, end) ) return;

	WordsVector::iterator words_beg, words_end;
	splitStringOnWords(b, e, words_beg, words_end);

	float baseLine = rect.pos.y + height;

	CharRect charRect;
	charRect.pos = CharRect::PointType(rect.pos.x + firstLineShift, baseLine);

	bool needFillSpacesList = (align_ & TAL_JUSTIFY) == TAL_JUSTIFY;

	int currentStringSpacesCount = 0;
	std::vector<int> spacesCount;
	std::vector<GlyphInfo*> glyphStringsEnds;
	std::vector<UTF32*> logicStringsEnds;

	GlyphInfo* glyphIt = beg;
	for (; words_beg != words_end; ++words_beg)
	{
		WordsVector::iterator curWord_beg = words_beg;
		WordsVector::iterator curWord_end = words_beg;
		++curWord_end;

		const UTF32* cbeg = *curWord_beg;
		const UTF32* cend = *curWord_end;

		int wordWidth = stringWidth(cbeg, cend);
		IntPoint2 wordSize;
		getStringSize(cbeg, cend, wordSize);

		//GUI2::RenderManager::get().drawLineRect( IntRect(charRect.pos, wordSize), GUI2::Color::WHITE );

 		if(wordWrap && charRect.pos.x + wordWidth >= rect.right() && charRect.pos.x != rect.pos.x)
 		{
 			charRect.pos.x = rect.pos.x;
 			baseLine += height;
 			glyphStringsEnds.push_back(glyphIt);
 
 			spacesCount.push_back(currentStringSpacesCount);
 			currentStringSpacesCount = 0;
 		}

		for(;cbeg != cend; ++cbeg)
		{
			int currentKerning = 0;
			const CharacterDesc& charDesc = getCharDesc( *cbeg );
			const UTF32* next = cbeg + 1;
			if(next != cend)
			{
				 currentKerning = charDesc.getKerning(*next);
			}

			bool isSpace = isSpaceChar(charDesc.charCode);
			if(needFillSpacesList && isSpace)
			{
				++currentStringSpacesCount;
			}

			if(wordWrap && charRect.pos.x + charRect.size.x > rect.right())
			{
				charRect.pos.x = rect.pos.x;
				baseLine += height;
				glyphStringsEnds.push_back(glyphIt);

				spacesCount.push_back(currentStringSpacesCount);
				currentStringSpacesCount = isSpace ? 1 : 0;
			}

			//GUI2::RenderManager::get().drawLine(IntPoint2(rect.left(), baseLine), IntPoint2(rect.right(), baseLine), GUI2::Color::WHITE );

			const GlyphMetric& metric = charDesc.metric;

			charRect.pos.x += metric.horiBearingX;
			charRect.size.x = metric.width;
			charRect.size.y = metric.height;

			charRect.toBottom(baseLine);
			charRect.pos.y += (metric.height - metric.horiBearingY);

			GlyphInfo& glyphInfo = *glyphIt++;
			glyphInfo.rect = charRect;
			glyphInfo.texRect = charDesc.blackBody;
			glyphInfo.charCode = charDesc.charCode;

			charRect.pos.x += metric.horiAdvance - metric.horiBearingX + currentKerning;
		}
	}
	  
	//text align
 	{
 		TextAlign textAlign = TAL_NOT_DEFINED;
 
 		if((align_ & TAL_RIGHT) == TAL_RIGHT) textAlign = TAL_RIGHT;
 		else if((align_ & TAL_HORIZONTAL_CENTER) == TAL_HORIZONTAL_CENTER) textAlign = TAL_HORIZONTAL_CENTER;
 		else if((align_ & TAL_JUSTIFY) == TAL_JUSTIFY) textAlign = TAL_JUSTIFY;
 
 
 		GlyphInfo* glyphStrBeg = beg;
		const UTF32* logicStrBeg = b;
 		const UTF32* logicStrEnd = e;
 
 		if(!logicStringsEnds.empty())
 		{
 			logicStrEnd = *logicStringsEnds.begin();
 		}
 
 		std::vector<int>::iterator scit = spacesCount.begin();
 		std::vector<int>::iterator scend = spacesCount.end();
 		for(GlyphInfo* strEnd : glyphStringsEnds)
 		{
 			int sc = (scit != scend) ? *scit : 0;
 			align(textAlign, rect, sc, logicStrBeg, logicStrEnd, glyphStrBeg, strEnd);
 			glyphStrBeg = strEnd;
 			logicStrBeg = logicStrEnd;
 			++logicStrEnd;
 			++scit;
 		}
 
 		align(textAlign, rect, 0, logicStrBeg, logicStrEnd, glyphStrBeg, end);
 
 		if((align_ & TAL_VERTICAL_CENTER) == TAL_VERTICAL_CENTER)
 		{
			IntPoint2 size;
			getStringSize(b, e, size);
			height = size.y;
 			align(TAL_VERTICAL_CENTER, rect, 0, logicStrBeg, logicStrEnd, beg, end);
 		}
 	}
}

void Face::align(TextAlign align, const Rect& rect, int spacesCount, const UTF32* b, const UTF32* e, GlyphInfo* beg, GlyphInfo* end) const
{
	if(align == TAL_RIGHT)
	{
		GlyphInfo* it = end - 1;
		GlyphInfo* rend = beg - 1;

		int diff = rect.right() - it->rect.right();
		for (; it != rend; --it)
		{
			it->rect.pos.x += diff;
		}
	}
	else if(align == TAL_HORIZONTAL_CENTER)
	{
		GlyphInfo* it = end - 1;
		GlyphInfo* rend = beg - 1;

		int diff = (rect.right() - it->rect.right()) / 2;
		for (; it != rend; --it)
		{
			it->rect.pos.x += diff;
		}		
	}
	else if(align == TAL_VERTICAL_CENTER)
	{
		GlyphInfo* it = end - 1;
		GlyphInfo* rend = beg - 1;

		int diff = (rect.bottom() - it->rect.bottom()) / 2;
		for (; it != rend; --it)
		{
			it->rect.pos.y += diff;
		}	
	}
	else if(align == TAL_JUSTIFY)
	{
		GlyphInfo* it = beg;
		GlyphInfo* endns = end - 1;
		const UTF32* logicIt = b;
	
		int diff = rect.right() - (end - 1)->rect.right();

		int borderSpacesWidth = 0;
		for (; it != end; ++it)
		{
			if( !isSpaceChar(it->charCode) )
			{
				break;
			}
			else
			{
				borderSpacesWidth += it->rect.size.x;
				--spacesCount;
			}
		}

		int leadingSpacesWidth = borderSpacesWidth;

		for (; endns != it - 1; --endns)
		{
			if( !isSpaceChar(endns->charCode) )
			{
				break;
			}
			else
			{
				borderSpacesWidth += endns->rect.size.x;
				--spacesCount;
			}
		}

		++endns;

		int totalSpace = diff + borderSpacesWidth;
		int widthPerSpace = (spacesCount > 0) ? totalSpace / spacesCount : 0;
		int leftSpace = (spacesCount > 0) ? totalSpace - widthPerSpace * spacesCount : 0;
		int additionalPos = 0;
		for (; it != endns; ++it)
		{
			if( !isSpaceChar(it->charCode) )
			{
				it->rect.pos.x += additionalPos - leadingSpacesWidth;
				//additionalPos += it->rect.size.x;
			}
			else
			{
				int add = leftSpace-- > 0 ? 1 : 0;

				additionalPos += widthPerSpace + add;
 				it->rect.pos.x += additionalPos - leadingSpacesWidth;
 				it->rect.size.x += widthPerSpace;
			}
			++logicIt;			
		}	
	}
}

int Face::stringWidth(const char* str) const
{
	return stringWidth( string_utf32(str) );
}

// int Face::stringWidth(const std::string& str) const
// {
// 	return stringWidth( string_utf32(str) );
// }
// 
// int Face::stringWidth(const std::wstring& str) const
// {
// 	return stringWidth( string_utf32(str) ); 
// }

int Face::stringWidth(const string_utf32& str) const
{
	return stringWidth(&str[0], &str[str.size()]);
}

int Face::stringWidth(const UTF32* b, const UTF32* e) const
{
	int width = 0;
	const UTF32* it = b;
	for (; it != e; ++it)
	{
		const CharacterDesc& charDesc = getCharDesc(*it);
		const GlyphMetric& metric = charDesc.metric;

		width += metric.horiAdvance;
	}

	return width;
}

int Face::stringHeight(const char* str) const { return stringHeight( string_utf32(str) ); }
int Face::stringHeight(const string_utf32& str) const { return stringHeight(&str[0], &str[str.size()]); }
int Face::stringHeight(const UTF32* b, const UTF32* e) const
{
	int height = 0;
	const UTF32* it = b;
	for(;it != e; ++it)
	{
		const CharacterDesc& charDesc = getCharDesc(*it);
		const GlyphMetric& metric = charDesc.metric;

		height = std::max(height, metric.height);
	}

	return height;
}

void Face::getStringSize(const UTF32* b, const UTF32* e, IntPoint2& size) const
{
	size.x = size.y = 0;
	const UTF32* it = b;
	for(;it != e; ++it)
	{
		const CharacterDesc& charDesc = getCharDesc(*it);
		const GlyphMetric& metric = charDesc.metric;

		size.x += metric.horiAdvance;
		size.y = std::max(size.y, metric.height);
	}
}

void Face::splitStringOnWords(const UTF32* b, const UTF32* e, WordsVector::iterator& beg, WordsVector::iterator& end) const
{
	prepareWordsBuffer(b, e, beg, end);

	*beg = b;
	end = beg + 1;
	*end = e;

	const UTF32* it = b;
	for (; it != e; ++it)
	{
		if( isSplitChar(*it) )
		{
			*end = it + 1;
			++end;
			*end = e;
		}
	}
}

bool Face::isSplitChar(int charCode)
{
	return charCode == SystemSymbols::SPACE;
}

bool Face::isSpaceChar(int charCode)
{
	return charCode == SystemSymbols::SPACE;
}

void Face::prepareWordsBuffer(const UTF32* b, const UTF32* e, WordsVector::iterator& beg, WordsVector::iterator& end) const
{
	ptrdiff_t diff = std::distance(b, e);
	if(diff <= 0) 
	{
		beg = end; 
		return;
	}

	static WordsVector wordsBuffer;
	wordsBuffer.resize( diff + 1 );

	beg = wordsBuffer.begin();
	end = wordsBuffer.end();
}

bool Face::prepareGlyphsBuffer(const UTF32* b, const UTF32* e, GlyphInfo*& beg, GlyphInfo*& end) const
{
	ptrdiff_t diff = std::distance(b, e);
	if(diff <= 0) 
	{
		beg = end; 
		return false;
	}

	static GlyphsList glyphsBuffer;
	glyphsBuffer.resize(diff);

	beg = &glyphsBuffer[0];
	end = &glyphsBuffer[diff - 1] + 1;

	return true;
}

// int Style::getKerning(const CharacterDesc& char1, const CharacterDesc& char2) const
// {
// 	size_t i = char1.kerningIdx;
// 	size_t end = i + char1.numKerningPairs;
// 	
// 	unsigned short c2 = char2.symbol;
// 	for (; i < end; ++i)
// 	{
// 		if( kerning[i].character == c2 )	return kerning[i].kerning;
// 	}
// 	
// 	return 0;
// }
// 
// std::wstring Style::validateString(const std::wstring& str)	const
// {	   /*
// 	if( codeTable.empty() ) 
// 	{
// 		throw Base::Errors::Simple("Style::validateString(): font code table is empty");
// 	}
// 
// 	std::wstring res;
// 	res.reserve(str.size() );
// 	foreach(wchar_t c, str)
// 	{
// 		CodeTranslationTable::const_iterator it = codeTable.find(c);
// 		if( it == codeTable.end() ) c = codeTable.begin()->first;
// 		res.push_back(c);				
// 	}*/
// 
// 	unsigned int i = 0;
// 	std::wstring res = str;
// 	foreach(wchar_t c, res)
// 	{
// 		res[i++] = codeTable[c];
// 	}
// 
// 	return res;
// 	
// }
// 
// StringProcessor::GlyphsList StringProcessor::tempGlyphs;
// 
// void StringProcessor::reserveBuffer(int count)
// {
// 	if( tempGlyphs.size() < count ) tempGlyphs.resize( count );
// }
// 
// void StringProcessor::getGlyphs(const Style& style, const Strings& text, GlyphsList::iterator& beg, GlyphsList::iterator& end)
// {
// 	reserveBuffer(text.symbolsCount);
// 
// 	beg = tempGlyphs.begin();
// 	end = beg;
// 
// 	size_t stringCount = text.data.size();
// 	int glyphIdx = 0; 
// 
// 	for (size_t strIdx = 0; strIdx < stringCount; ++strIdx)
// 	{
// 		const std::wstring& str = text.data[strIdx];
// 
// 		int s = str.size() - 1;
// 		int yShift = style.metric.height * strIdx;
// 
// 		if(s >= 0) tempGlyphs[glyphIdx].rect.pos.x = 0;
// 
// 		for (int i = 0; i < s; ++i)
// 		{
// 			const CharacterDesc& c = style.getCharDesc( str[i] );
// 			const CharacterDesc& c2 = style.getCharDesc( str[i + 1] );
// 			GlyphInfo& gi = tempGlyphs[glyphIdx];
// 
// 			int kerning = style.getKerning(c, c2);
// 
// 			gi.texRect = c.texRect;
// 			gi.rect.pos.y = c.rect.pos.y + yShift;
// 			gi.rect.size = c.rect.size;
// 			
// 			tempGlyphs[glyphIdx + 1].rect.pos.x = gi.rect.pos.x + c.b + c.c + kerning + c2.a;
// 			++end;
// 			++glyphIdx;
// 		}
// 
// 		if(s >= 0)
// 		{
// 			const CharacterDesc& c = style.getCharDesc( str[s] );
// 			GlyphInfo& gi = tempGlyphs[glyphIdx];
// 
// 			gi.texRect = c.texRect;
// 			gi.rect.pos.y = c.rect.pos.y + yShift;
// 			gi.rect.size = c.rect.size;
// 			++end;
// 			++glyphIdx;
// 		}
// 	}
// }
// 
// void StringProcessor::getGlyphs(const Style& style, const std::wstring& str, GlyphsList::iterator& beg, GlyphsList::iterator& end )
// {
// 	Strings strs;
// 	strs.data.push_back(str);
// 	strs.countSymbols();
// 	getGlyphs(style, strs, beg, end );
// }
// 
// void StringProcessor::loadUnicodeTextFromFile(const char* fileName, Strings& text)
// {
// 	if(!Base::FileUtils::FileExists(fileName))
// 	{
// 		throw std::runtime_error( (std::string(__FUNCTION__) + " file not found: " + fileName).c_str() );
// 	}
// 
// 	FILE *fl = fopen(fileName , "rb");
// 
// 	fseek(fl, 0, SEEK_END);
// 	int nSyms = ftell(fl);
// 	fseek(fl, 0, SEEK_SET);
// 	char *sym = new char[nSyms + 2];
// 	fread(sym, nSyms, sizeof *sym, fl);
// 	fclose(fl);
// 
// 	sym[nSyms] = sym[nSyms + 1] = 0;
// 
// 	wchar_t* str = (wchar_t*)(sym + 2);
// 	int len = nSyms / 2;
// 	text.data.reserve(len / 20);
// 
// 	int strCounter = 0;
// 	text.data.resize(1);
// 	for (int i = 0; i < len; ++i)
// 	{
// 		wchar_t c = str[i];
// 
// 		if(c == SystemSymbols::EOL)
// 		{
// 			++strCounter;
// 			text.data.push_back( std::wstring() );
// 			continue;
// 		}
// 
// 		if(c == SystemSymbols::CHARACTER_TABULATION) c = SystemSymbols::SPACE;
// 		if(c == SystemSymbols::CARRIAGE_RETURN) continue;
// 		if(c == SystemSymbols::NULL_SYMBOL) break;
// 
// 		text.data[strCounter].push_back(c);
// 	}
// 
// 	text.symbolsCount = len;
// 
// 	delete [] sym;
// 
// }


}}