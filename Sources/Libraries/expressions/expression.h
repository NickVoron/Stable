// Copyright (C) 2014-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <vector>
#include <string>
#include <boost/any.hpp>

#include "containers/library.include.h"

namespace Expressions
{
	struct PropertyPath;
	struct ArrayPath;
	struct ScopeNames;
	
	class Proxy;
	typedef std::vector<const Proxy*> References;

	class Expression
	{
	public:
		Expression() {}
		Expression(const Expression&) = delete;
		const Expression& operator=(const Expression&) = delete;

		virtual ~Expression() {}
	
		template <class T> const T* cast() const	{ return dynamic_cast<const T*>(this); }
		template <class T> T* cast() { return dynamic_cast<T*>(this); }
		
		virtual const Expression* evaluated(const ScopeNames& environment, boost::any* userData = 0) const	{ return this; }
		virtual std::string string() const											{ return "unknown Expression"; }
		virtual bool elementary() const												{ return false; }
		virtual bool boolean() const												{ return false; }
		virtual std::string typeName() const										{ return "unknown operand type"; }
		virtual References references() const										{ return References(); }
		
		virtual const Expression* child(const PropertyPath* path) const { ENFORCE(false); return 0; }
		virtual const Expression* child(const ArrayPath* path) const { ENFORCE(false); return 0; }
	};
	
	template<class ValueType> std::string stringize(ValueType value);
	template<> inline std::string stringize<std::string>(std::string value) { return value; };
	template<> inline std::string stringize<const Expressions::Expression*>(const Expressions::Expression* value) { return value ? value->string() : "#nullptr"; };
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