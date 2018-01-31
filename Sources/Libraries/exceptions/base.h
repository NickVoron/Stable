// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <iostream>
#include <fstream>
#include <ostream>
#include <stdexcept>

namespace Base {
	
namespace Errors {

class Unknown : public std::runtime_error {
public:
	virtual ~Unknown() throw() {}
	Unknown() : std::runtime_error( "unknown error" ), errorCode("unknown error"){}
	Unknown( const std::string& code ) : std::runtime_error( code.c_str()), errorCode(code){} 
	const std::string& GetMessageText() const { return errorCode; }
protected:
	std::string errorCode;
};


class Simple : public Unknown {
public:
	Simple( const std::string& code ):Unknown(code) {}
	virtual ~Simple() throw() {}
};


std::ostream& operator<<( std::ostream& s, const Base::Errors::Unknown& error );

}
}






// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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