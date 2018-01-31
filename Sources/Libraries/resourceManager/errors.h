// Copyright (C) 2012-2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once


namespace Resources {
namespace Errors {

class Unknown : public Base::Errors::Unknown {
public:
	Unknown() {}
	Unknown( const std::string& e ) : Base::Errors::Unknown( "resource error: " + e ) {}
};

class Unnamed : public Unknown {
public:
	Unnamed() : Unknown("has no name") {}
};

class NameRedefinition : public Unknown {
public:
	NameRedefinition( const std::string& newName, const std::string& oldName ) : Unknown(str::stringize("trying to change resource name from", std::quoted(oldName), " to ",std::quoted(newName) )) {}
};

class NotFound : public Unknown {
public:
	NotFound( const std::string& fileName ) : Unknown(str::stringize("file not found: ", std::quoted( fileName )) ) {}
};

class NotCompiled : public Unknown {
public:
	NotCompiled( const std::string& fileName, const std::string& code  ) : Unknown(str::stringize("failed to compile ", std::quoted(fileName), " reason: ", code )) {}
};

}
}



// Copyright (C) 2012-2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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