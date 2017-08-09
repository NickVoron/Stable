// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "image/library.include.h"
#include "unigui/library.include.h"
#include "stuff/userStruct.h"

#include "../expression.h"
#include "../const.h"
#include "../array.h"

#include "varToType.h"



namespace Expressions 
{
	namespace CommonUserStructures 
	{
		const ConstExprList& extractConstList(const Expression& expr);
	
		
		
		
		struct Vector2Converter : public UserStructConverter_NameCheck<Vector2> { virtual const char* structName() const { return "Vector2"; } void convert(const Expression& expr, Vector2& client); };
		struct Vector3Converter : public UserStructConverter_NameCheck<Vector3> { virtual const char* structName() const { return "Vector3"; } void convert(const Expression& expr, Vector3& client); };
		struct Vector4Converter : public UserStructConverter_NameCheck<Vector4> { virtual const char* structName() const { return "Vector4"; } void convert(const Expression& expr, Vector4& client); };

		struct Index2Converter : public UserStructConverter_NameCheck<nm::index2> { virtual const char* structName() const { return "Index2"; } void convert(const Expression& expr, nm::index2& client); };
		struct Index3Converter : public UserStructConverter_NameCheck<nm::index3> { virtual const char* structName() const { return "Index3"; } void convert(const Expression& expr, nm::index3& client); };

		struct Point2Converter : public UserStructConverter_NameCheck<unigui::Point2> { virtual const char* structName() const { return "Point2"; } void convert(const Expression& expr, unigui::Point2& client); };
		struct RectConverter : public UserStructConverter_NameCheck<unigui::Rect> { virtual const char* structName() const { return "Rect"; } void convert(const Expression& expr, unigui::Rect& client); };

		
		
		
		template<class T>
		struct RangeConverter : public UserStructConverter_NameCheck< Range<T> >
		{
			virtual const char* structName() const { return "Range"; }

			void convert(const Expression& expr, Range<T>& client)
			{
				const ConstExprList& vl = extractConstList(expr);

				ENFORCE(vl.size() == 2);
				convertVar(*vl[0], client.minValue);
				convertVar(*vl[1], client.maxValue);
			}
		};

		struct IntRangeConverter : public RangeConverter<int>	{};
		struct FloatRangeConverter : public RangeConverter<float>	{};
		struct DoubleRangeConverter : public RangeConverter<double>	{};

		template<class T>
		struct RangedValueConverter : public UserStructConverter_NameCheck< RangedValue<T> >
		{
			virtual const char* structName() const { return "RangedValue"; }

			void convert(const Expression& expr, RangedValue<T>& client)
			{
				const ConstExprList& vl = extractConstList(expr);

				ENFORCE(vl.size() == 3);
				T value, minV, maxV;
				convertVar(*vl[0], value);
				convertVar(*vl[1], minV);
				convertVar(*vl[2], maxV);

				client = RangedValue<T>(minV, maxV, value);
			}
		};

		template<image::Format format>
		struct ColorConverter : public UserStructConverter_NameCheck< image::Color<format> >
		{
			virtual const char* structName() const { return "Color"; }

			void convert(const Expression& expr, image::Color<format>& client)
			{
				const ConstExprList& vl = extractConstList(expr);

				ENFORCE(vl.size() == 3);

				float r, g, b;
				convertVar(*vl[0], r);
				convertVar(*vl[1], g);
				convertVar(*vl[2], b);

				image::rgb(r, g, b, client);
			}
		};

		struct IntRangedValueConverter : public RangedValueConverter<int>{};
		struct FloatRangedValueConverter : public RangedValueConverter<float>{};
		struct DoubleRangedValueConverter : public RangedValueConverter<double>{};

		struct RGBA32F_ColorValueConverter : public ColorConverter<image::RGBA32F>{};

		
		
		
		template<class ListType>
		struct ListStructureConverter : public UserStructConverter<ListType>
		{
			void convert(const Expression& expr, ListType& client)
			{
				const ConstExprList& vl = extractConstList(expr);

				auto count = vl.size();
				if (count > 0)
				{
					client.resize(count);
					for (std::size_t i = 0; i < count; ++i)
					{
						convertVar(*vl[i], client[i]);
					}
				}
				else
				{
					client.clear();
				}
			}
		};

		template<class ListType>
		struct ListArrayConverter : public UserStructConverter<ListType>
		{
			void convert(const Expression& expr, ListType& client)
			{
				const Array* arrayExpr = expr.cast<Array>();
				if (arrayExpr)
				{
					auto count = arrayExpr->count();
					client.resize(count);
					for (std::size_t i = 0; i < count; ++i)
					{
						convertVar(*arrayExpr->element(i), client[i]);
					}
				}
				else
				{
					client.clear();
				}
			}
		};

		
		
		
		struct StringListConverter : public ListArrayConverter<UserStruct::StringList>	{};
		struct IntListConverter : public ListArrayConverter<UserStruct::IntList>	{};
		struct FloatListConverter : public ListArrayConverter<UserStruct::FloatList>	{};
		struct Vector3ListConverter : public ListArrayConverter<UserStruct::Vector3List>{};
		struct Index2ListConverter : public ListArrayConverter<UserStruct::Index2List>{};
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