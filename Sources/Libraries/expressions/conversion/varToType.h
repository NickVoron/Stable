// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "Loki/library.include.h"
#include "defaultLogs/library.include.h"
#include "variantType/library.include.h"
#include "strings/library.include.h"

#include "stuff/callDispatcher.h"

#include "../typeTraits.h"
#include "../const.h"
#include "../struct.h"
#include "../property.h"


namespace Expressions
{
	template<class T0, class T1>
	struct BoolSafeConverter
	{
		static void convert(const T0& src, T1& dst)
		{
			dst = (T1) src;
		}
	};

	template<class T>
	struct BoolSafeConverter<T, bool>
	{
		static void convert(const T& src, bool& dst)
		{
			dst = (src != T(0));
		}
	};

	template<class T0, class T1, bool signed_t0 = std::is_signed<T0>::value, bool signed_t1 = std::is_signed<T1>::value> 
	struct IntegralSafeConverter;
	
	template<class T0, class T1> 
	struct IntegralSafeConverter<T0, T1, true, true>
	{
		static void convert(const T0& src, T1& dst)
		{
			BoolSafeConverter<T0, T1>::convert(src, dst);
		}
	};

	template<class T0, class T1>
	struct IntegralSafeConverter<T0, T1, false, false>
	{
		static void convert(const T0& src, T1& dst)
		{
			BoolSafeConverter<T0, T1>::convert(src, dst);
		}
	};

	template<class T0, class T1>
	struct IntegralSafeConverter<T0, T1, true, false>
	{
		static void convert(const T0& src, T1& dst)
		{
			BoolSafeConverter<T0, T1>::convert(src, dst);
		}
	};

	template<class T0, class T1>
	struct IntegralSafeConverter<T0, T1, false, true>
	{
		static void convert(const T0& src, T1& dst)
		{
			BoolSafeConverter<T0, T1>::convert(src, dst);
		}
	};

	template<class T0, class T1, bool integral_t0 = std::is_integral<T0>::value, bool integral_t1 = std::is_integral<T1>::value>
	struct SafeConverter;

	template<class T0, class T1> struct SafeConverter<T0, T1, true, true>
	{
		static void convert(const T0& src, T1& dst) { IntegralSafeConverter<T0, T1>::convert(src, dst);	}
	};

	template<class T0, class T1> struct SafeConverter<T0, T1, false, true>
	{
		static void convert(const T0& src, T1& dst)
		{
			BoolSafeConverter<T0, T1>::convert(src, dst);
		}
	};

	template<class T0, class T1> struct SafeConverter<T0, T1, true, false>
	{
		static void convert(const T0& src, T1& dst)
		{
			BoolSafeConverter<T0, T1>::convert(src, dst);
		}
	};

	template<class T0, class T1> struct SafeConverter<T0, T1, false, false>
	{
		static void convert(const T0& src, T1& dst)
		{
			BoolSafeConverter<T0, T1>::convert(src, dst);
		}
	};

	template<class T0, class T1, bool is_same = std::is_same<T0, T1>::value>
	struct SafeAssigner;

	template<class T0, class T1>
	struct SafeAssigner<T0, T1, true>
	{
		static void assign(const T0& src, T1& dst)
		{
			BoolSafeConverter<T0, T1>::convert(src, dst);
		}
	};

	template<class T0, class T1>
	struct SafeAssigner<T0, T1, false>
	{
		static void assign(const T0& src, T1& dst)
		{
			SafeConverter<T0, T1>::convert(src, dst);
		}
	};

	template<class T0, class T1>
	void safe_assign(const T0& src, T1& dst)
	{
		return SafeAssigner<T0, T1>::assign(src, dst);
	}

	template<class ConstType, class TargetType>
	bool checkedConvert(const Expression& expr, TargetType& value, bool& converted)
	{
		if (!converted)
		{
			if (auto res = expr.cast<Const<ConstType>>())
			{
				safe_assign(res->value, value);
				converted = true;
			}
		}
		
		return converted;
	}


	template<class US>
	struct UserStructConverter
	{
		typedef US UserStruct;
		virtual bool validate(const Expression& expr) const { return true; }
		virtual void convert(const Expression& expr, UserStruct& client) = 0;
	};

	template<class US>
	struct UserStructConverter_NameCheck : public UserStructConverter<US>
	{
		virtual const char* structName() const = 0;
		virtual bool validate(const Expression& expr) const
		{
			if (auto structDef = expr.cast<StructBase>())
			{
				return structDef->typeName() == structName();
			}
			
			return false;
		}
	};

	
	
	
	class UserStructsConvertersLib : public TemplateCallDispatcher<UserStructConverter>{};

	
	template<class T, bool isExpression = boost::is_base_of<Expression, typename std::remove_pointer<T>::type>::value> struct ConverterExpr;

	template<class T>
	struct ConverterExpr<T, true>
	{
		static bool convert(const Expression& expr, T& value)
		{
			value = dynamic_cast<T>((Expression*) &expr);

			if (!value)
			{
				value = (T)expr.cast<typename std::remove_pointer<T>::type>();
			}
						
			return value != 0;
		}
	};

	template<class T>
	struct ConverterExpr<T, false>
	{
		static bool convert(const Expression& expr, T& value)
		{
			value = 0;
			LOG_ERROR("can't convert: " << typeid(T).name());
			return false;
		}
	};

	template<class T, bool isPointer = std::is_pointer<T>::value> struct ConverterPointer;

	template<class T>
	struct ConverterPointer<T, true>
	{
		static bool convert(const Expression& expr, T& value)
		{
			return ConverterExpr<T>::convert(expr, value);
		}
	};

	template<class T> 
	void RegisterExpressionConverter();

	template<class T>
	struct ConverterPointer<T, false>
	{
		static bool convert(const Expression& expr, T& value)
		{
			RegisterExpressionConverter<T>();
			UserStructConverter<T>* conv = UserStructsConvertersLib::ptr<T>();
			ENFORCE(conv);
			if (conv)
			{
				bool valid = conv->validate(expr);
				if (valid)
				{
					conv->convert(expr, value);
				}
				else
				{
					LOG_ERROR("can't convert expression: " << expr.string() << " to type: " <<  typeid(T).name());
				}

				return valid;
			}
			else
			{
				LOG_ERROR("converter for type: " << typeid(T).name() << " not registered");
			}

			return false;
		}
	};

	
	template<class V, class... T>
	struct ChainConverter
	{
		static bool convert(const Expression& expr, V& value)
		{
			bool converted = false;
			auto tpl = std::make_tuple(checkedConvert<T>(expr, value, converted)...);
			return converted;
		}
	};

	template<class... T>
	struct ChainConverter<std::string, T...>
	{
		static bool convert(const Expression& expr, std::string& value)
		{
			value = expr.string();
			return true;
		}
	};

	template<class... T, class V>
	bool convert_chain(const Expression& expr, V& value)
	{
		return ChainConverter<V, T...>::convert(expr, value);
	}
	
	template<class T, bool isAtomic = (Variant::IsAtomicType<T>::value && !std::is_pointer<T>::value)> struct Converter;

	template<class T, bool isEnum = std::is_enum<typename std::decay<T>::type>::value>
	struct EnumSelector;

	template<class T>
	struct EnumSelector<T, false>
	{
		static bool convert(const Expression& expr, T& value)
		{
			return convert_chain<
				T,
				char,
				short,
				int,
				long,
				long long,
				unsigned char,
				unsigned short,
				unsigned int,
				unsigned long,
				unsigned long long,
				double,
				float
			>(expr, value);
		}
	};

	template<class T>
	struct EnumSelector<T, true>
	{
		static bool convert(const Expression& expr, T& value)
		{
			return convert_chain<
				T,
				char,
				short,
				int,
				long,
				long long,
				unsigned char,
				unsigned short,
				unsigned int,
				unsigned long,
				unsigned long long
			>(expr, value);
		}
	};

	template<class T> 
	struct Converter<T, true>
	{
		static bool convert(const Expression& expr, T& value)
		{
			return EnumSelector<T>::convert(expr, value);
		}
	};

	template<class T> 
	struct Converter<T, false>
	{
		static bool convert(const Expression& expr, T& value)
		{
			return ConverterPointer<T>::convert(expr, value);
		}
	};

	template<class T>
	bool convertVar(const Expression& var, T& value)
	{
		return Converter<T>::convert(var, value);
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