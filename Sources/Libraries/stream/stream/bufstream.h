// Copyright (C) 2012-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "stream.h"
#include "../dataop/zlib_op.h"
#include "../dataop/rijndael_op.h"

namespace stream
{


template<int bufferSize, template <class> class O1, template <class> class O2>
class streambuf 
	: 
	  public dataop::data_operator<mem::membuf<bufferSize>, O1, O2>::Op
{
protected:
	streambuf(){ this->setBuffers(b1, b2); }
	virtual ~streambuf(){};

	mem::membuf<bufferSize> buffer;

private:
	typedef typename dataop::data_operator<mem::membuf<bufferSize>, O1, O2>::OutputBuffer OB;
	OB b1, b2;
};


template<int bufferSize>
class streambuf<bufferSize, dataop::null, dataop::null> 
{
protected:
	virtual ~streambuf(){};
	mem::membuf<bufferSize> buffer;
};


}




// Copyright (C) 2012-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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