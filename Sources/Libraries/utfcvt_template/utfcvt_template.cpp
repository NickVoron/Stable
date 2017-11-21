#include "utfcvt_template.h"


namespace UTF
{

	template<>
	ConversionResult Convert<UTF8, UTF16>(const UTF8** srcS, const UTF8* srcE, UTF16** trgS, UTF16* trgE, ConversionFlags flags)
	{
		return ConvertUTF8toUTF16(srcS, srcE, trgS, trgE, flags);
	}

	template<>
	ConversionResult Convert<UTF8, UTF32>(const UTF8** srcS, const UTF8* srcE, UTF32** trgS, UTF32* trgE, ConversionFlags flags)
	{
		return ConvertUTF8toUTF32(srcS, srcE, trgS, trgE, flags);
	}

	template<>
	ConversionResult Convert<UTF16, UTF8>(const UTF16** srcS, const UTF16* srcE, UTF8** trgS, UTF8* trgE, ConversionFlags flags)
	{
		return ConvertUTF16toUTF8(srcS, srcE, trgS, trgE, flags);
	}

	template<>
	ConversionResult Convert<UTF16, UTF32>(const UTF16** srcS, const UTF16* srcE, UTF32** trgS, UTF32* trgE, ConversionFlags flags)
	{
		return ConvertUTF16toUTF32(srcS, srcE, trgS, trgE, flags);
	}

	template<>
	ConversionResult Convert<UTF32, UTF8>(const UTF32** srcS, const UTF32* srcE, UTF8** trgS, UTF8* trgE, ConversionFlags flags)
	{
		return ConvertUTF32toUTF8(srcS, srcE, trgS, trgE, flags);
	}

	template<>
	ConversionResult Convert<UTF32, UTF16>(const UTF32** srcS, const UTF32* srcE, UTF16** trgS, UTF16* trgE, ConversionFlags flags)
	{
		return ConvertUTF32toUTF16(srcS, srcE, trgS, trgE, flags);
	}


	template<>
	ConversionResult Convert<UTF8, UTF8>(const UTF8** srcS, const UTF8* srcE, UTF8** trgS, UTF8* trgE, ConversionFlags flags)
	{
		const UTF8* source = *srcS;
		UTF8* target = *trgS;
		while (source < srcE && target < trgE)
		{
			*target = *source;
			++source;
			++target;
		}

		return conversionOK;
	}

	template<>
	ConversionResult Convert<UTF16, UTF16>(const UTF16** srcS, const UTF16* srcE, UTF16** trgS, UTF16* trgE, ConversionFlags flags)
	{
		const UTF16* source = *srcS;
		UTF16* target = *trgS;
		while (source < srcE && target < trgE)
		{
			*target = *source;
			++source;
			++target;
		}

		return conversionOK;
	}

	template<>
	ConversionResult Convert<UTF32, UTF32>(const UTF32** srcS, const UTF32* srcE, UTF32** trgS, UTF32* trgE, ConversionFlags flags)
	{
		const UTF32* source = *srcS;
		UTF32* target = *trgS;
		while (source < srcE && target < trgE)
		{
			*target = *source;
			++source;
			++target;
		}

		return conversionOK;
	}

}//