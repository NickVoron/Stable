#include <math.h>

#include "fontExport.h"
#include "stuff/library.include.h"
#include "unigui/library.include.h"

#include "imageFiltering/library.include.h"
#include "freetype/library.include.h" 
#include "distanceProcessing/library.include.h"

#include <algorithm>

namespace unigui 
{
namespace font
{

	FontExport::FontExport(){}
	
	void FontExport::clear()
	{
		//ranges.clear();	
		filesToAnalyse.clear();	
		alphabet.clear();	
	}

	void FontExport::addFilesToAnalyse(const std::list<std::string>& files)
	{
		for(const std::string& f : files)
		{
			filesToAnalyse.insert(f);
		}
	}

	void FontExport::addCharacterCode(int charCode)
	{
		addRange(charCode, charCode);
	}

	void FontExport::addRange(int startCodePoint, int endCodePoint)
	{
		if(startCodePoint > endCodePoint)
			std::swap(startCodePoint, endCodePoint);

		for (int i = startCodePoint; i <= endCodePoint; ++i)
		{
			alphabet.insert(i);			
		}
	}

	void FontExport::addString(const std::string& s)	{ addString( string_utf32(s.c_str()) );	}
	void FontExport::addString(const std::wstring& s)	{ addString( string_utf32(s.c_str()) );	}
	void FontExport::addString(const string_utf8& s)	{ addString( string_utf32(s));			}
	void FontExport::addString(const string_utf16& s)	{ addString( string_utf32(s));			}
	void FontExport::addString(const char* s)			{ addString( string_utf32(s));			}
	void FontExport::addString(const wchar_t* s)		{ addString( string_utf32(s));			}

	void FontExport::addString(const string_utf32& s)
	{ 
		int size = s.size();
		for(int i = 0; i < size; ++i )
		{
			alphabet.insert( s[i] );
		}
	}

// 	void FontExport::addUTF8(const std::string& s){	utf8strs.push_back(s);}
// 	void FontExport::addUTF16(const std::wstring& s){ utf16strs.push_back(s);}
// 	void FontExport::addUTF32(const StringUTF32& s){ utf32strs.push_back(s);}

/*
	namespace
	{
		void growIfNeed(StringUTF32& buffer, size_t len)
		{
			if(buffer.size() < len )
				buffer.resize(len);
		}

		void growIfNeed(StringUTF32& buffer, const std::string& u8)
		{
			growIfNeed(buffer, u8.size() * 8);
		}

		void growIfNeed(StringUTF32& buffer, const std::wstring& u16)
		{
			growIfNeed(buffer, u16.size() * 4);
		}

		template<class StrType>
		void checkConversionResult(ConversionResult res, const StrType& s)
		{
			std::string err;
			switch(res)
			{
			case targetExhausted: err = "target string is exhaused"; break;
			case sourceExhausted: err = "source string is exhaused"; break;
			case sourceIllegal: err = "source string is illegal"; break;
			}

			if(!err.empty())
			{
				throw Base::Errors::Simple(__FUNCTION__" " + err);
			}
		}
	}


	void FontExport::flushAllToUTF32()
	{
		StringUTF32 buffer;

		foreach(const std::string& u8, utf8strs)
		{
			growIfNeed(buffer, u8);

			const UTF8* srcBegin = (const UTF8*)&u8[0];
			const UTF8* srcEnd = srcBegin + u8.size();
			UTF32* dstBegin = &buffer[0];
			UTF32* dstEnd = dstBegin + buffer.size();

			ConversionResult res;

			res = ConvertUTF8toUTF32(&srcBegin, srcEnd, &dstBegin, dstEnd, lenientConversion );
			checkConversionResult(res, u8);

			StringUTF32::iterator end = std::find(buffer.begin(), buffer.end(), 0);
			addUTF32( StringUTF32(buffer.begin(), end) );
		}

		foreach(const std::wstring& u16, utf16strs)
		{
			growIfNeed(buffer, u16);

			const UTF16* srcBegin = (const UTF16*)&u16[0];
			const UTF16* srcEnd = srcBegin + u16.size();
			UTF32* dstBegin = &buffer[0];
			UTF32* dstEnd = dstBegin + buffer.size();

			ConversionResult res;
			res = ConvertUTF16toUTF32(&srcBegin, srcEnd, &dstBegin, dstEnd, lenientConversion );
			checkConversionResult(res, u16);

			StringUTF32::iterator end = std::find(buffer.begin(), buffer.end(), 0);
			addUTF32( StringUTF32(buffer.begin(), end) );
		}
	}


	void FontExport::createAlphabet(Alphabet& alphabet)
	{
		foreach(const StringUTF32& u32, utf32strs)
		{
			foreach(int charCode, u32)
			{
				alphabet.insert(charCode);
			}
		}

		foreach(const RangesList::value_type& range, ranges)
		{
			int v0 = range.first;
			int v1 = range.second;
			if(v0 > v1)
				std::swap(v0, v1);

			for (int i = v0; i <= v1; ++i)
			{
				alphabet.insert(i);			
			}
		}
	}
 */
	int sizeDenominator = 64;	


	void FontExport::compile(Face& face, TextureAtlasComposer::AtlasImage& img, const char* name, const char* srcName, int size)
	{
		face.name = name;
		face.systemName = srcName;
		face.size = size;

		FT_Library    library;
		FT_Face       faceFT;

		FT_GlyphSlot  slot;
		FT_Error      error;

		error = FT_Init_FreeType(&library); /* initialize library */
		ENFORCE(error == 0);

		error = FT_New_Face(library, face.systemName.c_str(), 0, &faceFT); /* create face object */
		ENFORCE(error == 0);

		/* use 50pt at 100dpi */
		error = FT_Set_Char_Size(faceFT, face.size * sizeDenominator, 0, 0, 0); /* set character size */
		ENFORCE(error == 0);

		slot = faceFT->glyph;
		face.height = faceFT->size->metrics.height / sizeDenominator;
		face.characters.resize(alphabet.size());

		std::map<int, int> codeToIdx;

		TextureAtlasComposer atlasBuilder;
		int charIdx = 0;
		for(UTF32 charCode : alphabet)
		{
			/* load glyph image into the slot (erase previous one) */
			error = FT_Load_Char(faceFT, charCode, FT_LOAD_RENDER);
			if (error)
				continue;                 /* ignore errors */


			FT_Bitmap& bmp = slot->bitmap;

			image::Plane<image::Color<image::R32F>> distPlane;

			if (bmp.buffer)
			{
				
				image::Plane<image::Color<image::R8>> resPlane;
				image::Plane<image::Color<image::R8>> resPlaneResized;
				
				image::Color<image::R8> clr;
				clr.zero();

				resPlane.init(bmp.buffer, bmp.pitch, bmp.width, bmp.rows);

				resPlaneResized.allocate(resPlane.sizeX * 2, resPlane.sizeY * 2);
				resPlaneResized.fill(clr);

				image::copy_rect(resPlane, resPlaneResized, unigui::Rect(0, 0, resPlane.sizeX, resPlane.sizeY), unigui::Rect(resPlane.sizeX / 2, resPlane.sizeY / 2, resPlane.sizeX, resPlane.sizeY));

				distPlane.copy(resPlane);
 				//distanceProcessing::processTexture(resPlaneResized, distPlane);
 				//image::freeimage::save(distPlane, (Base::StrUtils::toString(charCode) + ".png").c_str());
			}			
						
			atlasBuilder.addSourceRectMem(str::stringize(charCode), charCode, distPlane.bmp, image::R32F, distPlane.sizeX, distPlane.sizeY, distPlane.pitchInBytes);
			//atlasBuilder.addSourceRectMem(Base::StrUtils::toString(charCode), charCode, bmp.buffer, image::R8, bmp.width, bmp.rows, bmp.pitch);

			codeToIdx[charIdx] = charCode;
			face.codeTable[charCode] = charIdx;

			CharacterDesc& charDesc = face.characters[charIdx++];

			bool use_kerning = FT_HAS_KERNING(faceFT) != 0;
			if (use_kerning)
			{
				charDesc.kerning.reserve(128);
				FT_UInt glyphIdx = FT_Get_Char_Index(faceFT, charCode);
				for(UTF32 nextSymbol : alphabet)
				{
					FT_UInt nextIdx = FT_Get_Char_Index(faceFT, nextSymbol);
					FT_Vector  delta;
					FT_Get_Kerning(faceFT, glyphIdx, nextIdx, FT_KERNING_DEFAULT, &delta);
					if (delta.x > 0.0f)
					{
						Kerning kern = { nextSymbol, delta.x / sizeDenominator };
						charDesc.kerning.push_back(kern);
					}
				}
			}

			charDesc.charCode = charCode;
			charDesc.metric.width = slot->metrics.width / sizeDenominator;
			charDesc.metric.height = slot->metrics.height / sizeDenominator;
			charDesc.metric.horiAdvance = slot->metrics.horiAdvance / sizeDenominator;
			charDesc.metric.horiBearingX = slot->metrics.horiBearingX / sizeDenominator;
			charDesc.metric.horiBearingY = slot->metrics.horiBearingY / sizeDenominator;
			charDesc.metric.vertAdvance = slot->metrics.vertAdvance / sizeDenominator;
			charDesc.metric.vertBearingX = slot->metrics.vertBearingX / sizeDenominator;
			charDesc.metric.vertBearingY = slot->metrics.vertBearingY / sizeDenominator;
		}

		FT_Done_Face(faceFT);
		FT_Done_FreeType(library);

		RectAtlas atlas;
		atlasBuilder.build(atlas, img);
		for(const auto& p : codeToIdx)
		{
			CharacterDesc& charDesc = face.characters[p.first];
			const RectAtlas::Rect& r = atlas.absoluteRect( str::stringize(p.second).c_str());
			charDesc.blackBody.pos.x = r.minX + 0;
			charDesc.blackBody.pos.y = r.minY;
			charDesc.blackBody.size.x = r.sizeX();
			charDesc.blackBody.size.y = r.sizeY();
		}
	}


	float FontExport::stringWidth(const char* systemFontName, int size, const char* string)
	{
		FT_Library    library;
		FT_Face       faceFT;

		FT_GlyphSlot  slot;
		FT_Error      error;

		error = FT_Init_FreeType(&library); /* initialize library */
		ENFORCE(error == 0);

		error = FT_New_Face(library, systemFontName, 0, &faceFT); /* create face object */
		ENFORCE(error == 0);

		/* use 50pt at 100dpi */
		error = FT_Set_Char_Size(faceFT, size * sizeDenominator, 0, 0, 0); /* set character size */
		ENFORCE(error == 0);

		slot = faceFT->glyph;

		int result = 0;
		while(*string)
		{
			/* load glyph image into the slot (erase previous one) */
			error = FT_Load_Char(faceFT, *string, FT_LOAD_RENDER);
			if (error)
				continue;                 /* ignore errors */
			result += slot->metrics.horiAdvance / sizeDenominator;
			++string;
		}

		FT_Done_Face(faceFT);
		FT_Done_FreeType(library);

		return result;
	}

	float FontExport::stringHeight(const char* systemFontName, int size, const char* string)
	{
		FT_Library    library;
		FT_Face       faceFT;

		FT_GlyphSlot  slot;
		FT_Error      error;

		error = FT_Init_FreeType(&library); /* initialize library */
		ENFORCE(error == 0);

		error = FT_New_Face(library, systemFontName, 0, &faceFT); /* create face object */
		ENFORCE(error == 0);

		/* use 50pt at 100dpi */
		error = FT_Set_Char_Size(faceFT, size * sizeDenominator, 0, 0, 0); /* set character size */
		ENFORCE(error == 0);

		slot = faceFT->glyph;

		int maxY = 0;
		int minY = 0;
		int maxH = 0;

		while (*string)
		{
			/* load glyph image into the slot (erase previous one) */
			error = FT_Load_Char(faceFT, *string, FT_LOAD_RENDER);
			if (error)
				continue;                 /* ignore errors */

			int height = slot->metrics.height / sizeDenominator;
			int bearing = slot->metrics.horiBearingY / sizeDenominator;
			maxY = std::max(maxY, bearing);
			minY = std::min(minY, bearing - height);
			maxH = std::max(maxH, height);
			++string;
		}

		FT_Done_Face(faceFT);
		FT_Done_FreeType(library);

		return maxH;
		return maxY - minY;
	}

	float FontExport::fontAscender(const char* systemFontName, int size)
	{
		FT_Library    library;
		FT_Face       faceFT;

		FT_GlyphSlot  slot;
		FT_Error      error;

		error = FT_Init_FreeType(&library); /* initialize library */
		ENFORCE(error == 0);

		error = FT_New_Face(library, systemFontName, 0, &faceFT); /* create face object */
		ENFORCE(error == 0);

		
		float result = (float)(faceFT->ascender * size) / faceFT->units_per_EM;
		
		FT_Done_Face(faceFT);
		FT_Done_FreeType(library);

		return result;
	}

}//
}//