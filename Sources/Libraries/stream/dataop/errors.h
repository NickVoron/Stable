// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <stdexcept>

namespace dataop
{

class encryption_error : public std::runtime_error
{  
public:
	encryption_error(const char* err):std::runtime_error(err){} 
	virtual ~encryption_error() throw() {}
};

class decryption_error : public std::runtime_error
{  
public:
	decryption_error(const char* err):std::runtime_error(err){} 
	virtual ~decryption_error() throw() {}
};

class compression_error : public std::runtime_error
{  
public:
	compression_error(const char* err):std::runtime_error(err){} 
	virtual ~compression_error() throw() {}
};

class decompression_error : public std::runtime_error
{  
public:
	decompression_error(const char* err):std::runtime_error(err){} 
	virtual ~decompression_error() throw() {}
};

class buffer_error : public std::runtime_error
{  
public:
	buffer_error(const char* err):std::runtime_error(err){} 
	virtual ~buffer_error() throw() {}
};

template<class E>
void operationError(const char* err)
{
	throw E(err);
}

}




// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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