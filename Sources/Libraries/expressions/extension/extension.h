// Copyright (C) 2014-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include "../proxy.h"
#include <string>



namespace Expressions
{
	
	class VariableRequster
	{
	public:

		virtual ~VariableRequster(){}

		virtual Proxy* addProxy() = 0;
		virtual Proxy* addProxy(const std::string& targetName)=0;
		virtual Proxy* addProxy(const std::string& targetName, int index)=0;
		virtual Proxy::PathElement* addProxyArrayPath(Expression* expr)=0;
		virtual Proxy::PathElement* addProxyComponentsPath(const std::string& name)=0;
		virtual Proxy::PathElement* addProxyPropertyPath(const std::string& name)=0;

	};


	class Extension
	{
	public:
		virtual ~Extension(){}

		virtual std::string name() = 0;
		virtual void parse(const std::string& extensionText, VariableRequster* requster)=0;
	};


	template<const char* name_>
	class NamedExtension: public Extension
	{
	public:
		virtual std::string name() { return name_; }
		
	};


}//



// Copyright (C) 2014-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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