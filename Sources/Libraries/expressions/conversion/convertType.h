// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "strings/library.include.h"
#include "stuff/library.include.h"

#include "../expression.h"
#include "../array.h"
#include "../holder.h"
#include "../const.h"

namespace Expressions
{
	template<class T, bool is_arithmetic = std::is_arithmetic<typename std::decay<T>::type>::value || std::is_enum<T>::value>
	struct ConstExpression;

	template<class T>
	struct ConstExpression<T, true>
	{
		static Expression* convert(T value) { return Expressions::template add<Const<T>>(value); }
	};

	template<bool is_arithmetic>
	struct ConstExpression<std::string, is_arithmetic>
	{
		static Expression* convert(std::string value) { return Expressions::template add< Const<std::string> >(value); }
	};

	template<bool is_arithmetic>
	struct ConstExpression<str::string64, is_arithmetic>
	{
		static Expression* convert(str::string64 value) { return Expressions::template add<Const<std::string>>(value.c_str()); }
	};

	template<class T>
	Expression* convertType(T value)
	{
		return ConstExpression<T>::convert(value);
	}

	Expression* convertType(Expression*);
	Expression* convertType(const Vector2& val);
	Expression* convertType(const Vector3& val);
	Expression* convertType(const Vector4& val);
	Expression* convertType(const nm::index2& val);
	Expression* convertType(const nm::index3& val);


	Expression* convertType(const UserStruct::Vector3List& val);
	Expression* convertType(const UserStruct::Index2List& val);
	Expression* convertType(const UserStruct::StringList& val);

	template<class T>
	Expression* convertArray(const T& val)
	{
		int count = (int)val.size();
		ConstExprList list;
		list.resize(count);
		for(int i = 0; i < count; ++i)
		{
			list[i] = convertType(val[i]);
		}

		return Expressions::template add<Array>(list);
	}
}








// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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