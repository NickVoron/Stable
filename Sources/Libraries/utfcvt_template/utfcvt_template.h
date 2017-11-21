#pragma once
#include "utfcvt/library.include.h"


namespace UTF 
{

	template<class UTF_T0, class UTF_T1>
	ConversionResult Convert(const UTF_T0** srcS, const UTF_T0* srcE, UTF_T1** trgS, UTF_T1* trgE, ConversionFlags flags)
	{
	 	return conversionOK;	
	}

	template<>
	ConversionResult Convert<UTF8, UTF16>(const UTF8** srcS, const UTF8* srcE, UTF16** trgS, UTF16* trgE, ConversionFlags flags);
	
	template<>
	ConversionResult Convert<UTF8, UTF32>(const UTF8** srcS, const UTF8* srcE, UTF32** trgS, UTF32* trgE, ConversionFlags flags);

	template<>
	ConversionResult Convert<UTF16, UTF8>(const UTF16** srcS, const UTF16* srcE, UTF8** trgS, UTF8* trgE, ConversionFlags flags);

	template<>
	ConversionResult Convert<UTF16, UTF32>(const UTF16** srcS, const UTF16* srcE, UTF32** trgS, UTF32* trgE, ConversionFlags flags);

	template<>
	ConversionResult Convert<UTF32, UTF8>(const UTF32** srcS, const UTF32* srcE, UTF8** trgS, UTF8* trgE, ConversionFlags flags);

	template<>
	ConversionResult Convert<UTF32, UTF16>(const UTF32** srcS, const UTF32* srcE, UTF16** trgS, UTF16* trgE, ConversionFlags flags);

	template<>
	ConversionResult Convert<UTF8, UTF8>(const UTF8** srcS, const UTF8* srcE, UTF8** trgS, UTF8* trgE, ConversionFlags flags);

	template<>
	ConversionResult Convert<UTF16, UTF16>(const UTF16** srcS, const UTF16* srcE, UTF16** trgS, UTF16* trgE, ConversionFlags flags);

	template<>
	ConversionResult Convert<UTF32, UTF32>(const UTF32** srcS, const UTF32* srcE, UTF32** trgS, UTF32* trgE, ConversionFlags flags);


}//