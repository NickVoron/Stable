// Copyright (C) 2014-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include <string>
#include <map>

namespace Expressions
{
	class Expression;

	
	
	
	
	
	

	struct ScopeNames : public std::map<std::string, const Expressions::Expression*>
	{
		enum Method
		{
			INSERT,
			REPLACE,
			REPLACE_OR_INSERT,
			IGNORE_IF_EXIST			
		};

		ScopeNames():scopeName(unnamed) {};
		ScopeNames(const std::string& scopeName_) : scopeName(scopeName_){}
		~ScopeNames() {}

		void add(const std::string& name, const Expressions::Expression* expr, Method method);
		bool exists(const std::string& name) const;
		const Expressions::Expression* get(const std::string& name) const;
		const Expressions::Expression* getByType(const std::string& type) const;

		void copyFrom(const ScopeNames& proto);

		static const std::string unnamed;
		std::string scopeName;		
	};
}
	



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