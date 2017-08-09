// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "convertType.h"
#include "../struct.h"
#include "../holder.h"


namespace Expressions
{
	Expression* convertType(Expression* val)		{ return  val; }

	Expression* convertType(const Vector2& value)	{ return Expressions::template add<Struct>("Vector2", ConstExprList(convertType(value.x), convertType(value.y)));	}
	Expression* convertType(const Vector3& value)	{ return Expressions::template add<Struct>("Vector3", ConstExprList(convertType(value.x), convertType(value.y), convertType(value.z)));	}
	Expression* convertType(const Vector4& value)	{ return Expressions::template add<Struct>("Vector4", ConstExprList(convertType(value.x), convertType(value.y), convertType(value.z), convertType(value.w)));	}

	Expression* convertType(const nm::index2& value)	{ return Expressions::template add<Struct>("Index2", ConstExprList(convertType(value.x), convertType(value.y))); }
	Expression* convertType(const nm::index3& value)	{ return Expressions::template add<Struct>("Index3", ConstExprList(convertType(value.x), convertType(value.y), convertType(value.z))); }

	Expression* convertType(const UserStruct::Vector3List& val) { return convertArray(val); }
	Expression* convertType(const UserStruct::Index2List& val)	{ return convertArray(val); }
	Expression* convertType(const UserStruct::StringList& val) { return convertArray(val); }

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