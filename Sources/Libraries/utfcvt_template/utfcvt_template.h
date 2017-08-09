// Copyright (C) 2015 Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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



// Copyright (C) 2015 Denis Netakhin <denis.netahin@yandex.ru>
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