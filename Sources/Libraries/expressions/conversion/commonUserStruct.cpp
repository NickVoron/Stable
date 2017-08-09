// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "commonUserStruct.h"

namespace Expressions {
namespace CommonUserStructures {

const ConstExprList& extractConstList(const Expression& expr)
{
	const Struct* structDef = expr.cast<Struct>();
	ENFORCE(structDef);
	return structDef->params;
}


void Index2Converter::convert(const Expression& expr, nm::index2& client)
{
	const ConstExprList& vl = extractConstList(expr);

	ENFORCE(vl.size() == 2);
	convertVar(*vl[0], client.x);
	convertVar(*vl[1], client.y);
}

void Index3Converter::convert(const Expression& expr, nm::index3& client)
{
	const ConstExprList& vl = extractConstList(expr);

	ENFORCE(vl.size() == 3);
	convertVar(*vl[0], client.x);
	convertVar(*vl[1], client.y);
	convertVar(*vl[2], client.z);
}


void Vector2Converter::convert(const Expression& expr, Vector2& client)
{
	const ConstExprList& vl = extractConstList(expr);

	ENFORCE(vl.size() == 2);
	convertVar(*vl[0], client.x);
	convertVar(*vl[1], client.y);
}

void Vector3Converter::convert(const Expression& expr, Vector3& client)
{
	const ConstExprList& vl = extractConstList(expr);

	ENFORCE(vl.size() == 3);
	convertVar(*vl[0], client.x);
	convertVar(*vl[1], client.y);
	convertVar(*vl[2], client.z);
}

void Vector4Converter::convert(const Expression& expr, Vector4& client)
{
	const ConstExprList& vl = extractConstList(expr);

	ENFORCE(vl.size() == 4);
	convertVar(*vl[0], client.x);
	convertVar(*vl[1], client.y);
	convertVar(*vl[2], client.z);
	convertVar(*vl[3], client.w);
}

void Point2Converter::convert(const Expression& expr, unigui::Point2& client)
{
	const ConstExprList& vl = extractConstList(expr);

	ENFORCE(vl.size() == 2);
	convertVar(*vl[0], client.x);
	convertVar(*vl[1], client.y);
}


void RectConverter::convert(const Expression& expr, unigui::Rect& client)
{
	const ConstExprList& vl = extractConstList(expr);

	ENFORCE(vl.size() == 4);
	convertVar(*vl[0], client.pos.x);
	convertVar(*vl[1], client.pos.y);
	convertVar(*vl[2], client.size.x);
	convertVar(*vl[3], client.size.y);
}
							 
}
}

namespace Expressions
{
	using namespace CommonUserStructures;

	template<> void RegisterExpressionConverter<nm::index2>() { UserStructsConvertersLib::addInst<Index2Converter>(); }
	template<> void RegisterExpressionConverter<nm::index3>() { UserStructsConvertersLib::addInst<Index3Converter>(); }
	template<> void RegisterExpressionConverter<Vector2>() { UserStructsConvertersLib::addInst<Vector2Converter>(); }
	template<> void RegisterExpressionConverter<Vector3>() { UserStructsConvertersLib::addInst<Vector3Converter>(); }
	template<> void RegisterExpressionConverter<Vector4>() { UserStructsConvertersLib::addInst<Vector4Converter>(); }

	template<> void RegisterExpressionConverter<unigui::Point2>() { UserStructsConvertersLib::addInst<Point2Converter>(); }
	template<> void RegisterExpressionConverter<unigui::Rect>() { UserStructsConvertersLib::addInst<RectConverter>(); }

	template<> void RegisterExpressionConverter<Range<int>>() { UserStructsConvertersLib::addInst<IntRangeConverter>(); }
	template<> void RegisterExpressionConverter<Range<float>>() { UserStructsConvertersLib::addInst<FloatRangeConverter>(); }
	template<> void RegisterExpressionConverter<Range<double>>() { UserStructsConvertersLib::addInst<DoubleRangeConverter>(); }

	template<> void RegisterExpressionConverter<RangedValue<int>>() { UserStructsConvertersLib::addInst<IntRangedValueConverter>(); }
	template<> void RegisterExpressionConverter<RangedValue<float>>() { UserStructsConvertersLib::addInst<FloatRangedValueConverter>(); }
	template<> void RegisterExpressionConverter<RangedValue<double>>() { UserStructsConvertersLib::addInst<DoubleRangedValueConverter>(); }

	template<> void RegisterExpressionConverter<image::Color<image::RGBA32F>>() { UserStructsConvertersLib::addInst<RGBA32F_ColorValueConverter>(); }

	template<> void RegisterExpressionConverter<UserStruct::StringList>() { UserStructsConvertersLib::addInst<StringListConverter>(); }
	template<> void RegisterExpressionConverter<UserStruct::IntList>() { UserStructsConvertersLib::addInst<IntListConverter>(); }
	template<> void RegisterExpressionConverter<UserStruct::FloatList>() { UserStructsConvertersLib::addInst<FloatListConverter>(); }
	template<> void RegisterExpressionConverter<UserStruct::Vector3List>() { UserStructsConvertersLib::addInst<Vector3ListConverter>(); }
	template<> void RegisterExpressionConverter<UserStruct::Index2List>() { UserStructsConvertersLib::addInst<Index2ListConverter>(); }

	template<> void RegisterExpressionConverter<ConstExprList>() {  }
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