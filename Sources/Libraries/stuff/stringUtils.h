// Copyright (C) 2012-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <string>
#include <sstream>

#include "exceptions/library.include.h"
#include "strings/library.include.h"

namespace Base {

namespace Errors {
		struct ConvertError : public Base::Errors::Unknown {
			ConvertError( const std::string& str ) { Unknown::errorCode = "Cannot convert <" + str + ">"; }
			virtual ~ConvertError() throw() {}
		};
		struct ModuleFailed : public Base::Errors::Unknown {
			ModuleFailed( const std::string& str ) { Unknown::errorCode = "Base module failed: " + str; }
			virtual ~ModuleFailed() throw() {}
		};
	} 

namespace StrUtils {

	
	std::string getLine(std::istream& stream);

	
	std::wstring Convert( const std::string& s );
	std::string	 Convert( const std::wstring& s );

	
	template< typename T>
	T fromString( const std::string& str )
	{
		T res;
		std::stringstream ss;
		ss << str;
		ss >> res;
		return res;
	}

	
	void toUpper(std::string& str);
	void toLower(std::string& str);

	
	std::wstring toUnicode(const std::string& str);

	
	std::string sizeString(size_t sizeInBytes);

	
	

} 
}





// Copyright (C) 2012-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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