// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "base.h"
#include <string>

namespace Base {
namespace Errors {
namespace File {



class Unknown : public Base::Errors::Unknown {
public:
	virtual ~Unknown() throw (){}
	Unknown( const std::string& userFile )
		: fileName( userFile )
		{
			Base::Errors::Unknown::errorCode = "unknown file error in ";
			AddFileNameTo( Base::Errors::Unknown::errorCode );
		}
protected:
	void AddFileNameTo( std::string& target ) { std::string s = "\"" + fileName + "\""; target += s;}
	std::string fileName;
};

class NotFound : public Base::Errors::File::Unknown {
public:
	
	NotFound( const std::string& userFile )
		: Base::Errors::File::Unknown( userFile )
		{
			Base::Errors::Unknown::errorCode = "file ";
			AddFileNameTo( Base::Errors::Unknown::errorCode );
			Base::Errors::Unknown::errorCode += " not found";
		}

	virtual ~NotFound() throw (){}

};


class Read : public Base::Errors::File::Unknown {
public:
	Read( const std::string& userFile, const std::string& errorTag )
		: Base::Errors::File::Unknown( userFile )
		{
			Base::Errors::Unknown::errorCode = "failed to read ";
			Base::Errors::Unknown::errorCode += errorTag;
			Base::Errors::Unknown::errorCode += " from";
			AddFileNameTo( Base::Errors::Unknown::errorCode );
		}

	virtual ~Read() throw (){}
};


} 
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