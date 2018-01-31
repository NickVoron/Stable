// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "defaultLogs/library.include.h"
#include "variantType/library.include.h"
#include "strings/library.include.h"

#include "../typeTraits.h"
#include "../const.h"
#include "../array.h"
#include "../struct.h"
#include "../property.h"
#include "common/type_traits.h"

namespace Expressions
{
	template<class T>
	bool convertVar(const EvaluationUnit& var, T& value);

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
	struct CheckedConverter
	{
		static bool convert(const EvaluationUnit& unit, TargetType& value)
		{
			if (auto res = unit.cast<Const<ConstType>>())
			{
				safe_assign(res->value, value);
				return true;
			}
			return false;
		}
	};

	template<class ConstType>
	struct CheckedConverter<ConstType, boost::any>
	{
		static bool convert(const EvaluationUnit& unit, boost::any& value)
		{
			try
			{
				if(typeid(ConstType*) == value.type())
				{
					if (auto val = boost::any_cast<ConstType*>(value))
					{
						convertVar(unit, *val);
					}
				}				
				else
				{
					return false;
				}
				return true;
			}
			catch (const std::exception&)
			{

			}

			return false;
		}
	};
	
	template<class ConstType, class TargetType>
	bool checkedConvert(const EvaluationUnit& unit, TargetType& value, bool& converted)
	{
		if (!converted)
		{
			converted = CheckedConverter<ConstType, TargetType>::convert(unit, value);
		}
		
		return converted;
	}

	
	template<class T, bool isEvaluationUnit = boost::is_base_of<EvaluationUnit, typename std::remove_pointer<T>::type>::value> 
	struct ConverterExpr;

	template<class T>
	struct ConverterExpr<T, true>
	{
		static bool convert(const EvaluationUnit& unit, T& value)
		{
			value = dynamic_cast<T>((EvaluationUnit*) &unit);

			if (!value)
			{
				value = (T)unit.cast<typename std::remove_pointer<T>::type>();
			}
						
			return value != 0;
		}
	};

	template<class T>
	struct ConverterExpr<T, false>
	{
		static bool convert(const EvaluationUnit& unit, T& value)
		{
			value = 0;
			LOG_ERROR("can't convert: " << typeid(T).name());
			return false;
		}
	};

	template<class T, bool isPointer = std::is_pointer<T>::value> 
	struct ConverterPointer;

	template<class T>
	struct ConverterPointer<T, true>
	{
		static bool convert(const EvaluationUnit& unit, T& value)
		{
			return ConverterExpr<T>::convert(unit, value);
		}
	};

	extern multimethods2::Table2<bool> multimethodsConversionTable;

	template<class T>
	struct ConverterPointer<T, false>
	{
		static bool convert(const EvaluationUnit& unit, T& value)
		{
			auto type = mirror::type(value);

			
			if (auto structure = unit.cast<EvalStruct>())
			{
				if (structure->typeName() == type.name()) 
				{
					auto& params = structure->params;
					if (!params.empty())
					{
						if (params.size() == 1)
						{
							if (auto unittype = params[0]->reflectedType())
							{
								unittype->get(value);
								return true;
							}
						}
						else if (type.size() == params.size()) 
						{
							bool success = true;
							for (std::size_t i = 0; i < params.size(); ++i)
							{
								boost::any ref;
								type.member(i, value, ref);
								success &= convertVar(*params[i], ref);
							}			
							return success;
						}
					}
				}
			}	

			if (bool result = multimethodsConversionTable.exec((EvaluationUnit&) unit, type))
			{
				value = type.val();
				return true;
			}			
			
			return false;
		}
	};
	
	template<class V, class... T>
	struct ChainConverter
	{
		static bool convert(const EvaluationUnit& unit, V& value)
		{
			bool converted = false;
			auto tpl = std::make_tuple(checkedConvert<T>(unit, value, converted)...);
			return converted;
		}
	};

	template<class... T>
	struct ChainConverter<std::string, T...>
	{
		static bool convert(const EvaluationUnit& unit, std::string& value)
		{
			value = unit.string();
			return true;
		}
	};

	template<class CT, std::size_t size, class... T>
	struct ChainConverter<str::string_t<CT, size>, T...>
	{
		static bool convert(const EvaluationUnit& unit, str::string_t<CT, size>& value)
		{
			value = unit.string().c_str();
			return true;
		}
	};

	template<class... T, class V>
	bool convert_chain(const EvaluationUnit& unit, V& value)
	{
		return ChainConverter<V, T...>::convert(unit, value);
	}
	
	template<class T, bool isAtomic = (Variant::IsAtomicType<T>::value && !std::is_pointer<T>::value)> 
	struct Converter;

	template<class T, bool isEnum = std::is_enum<typename std::decay<T>::type>::value>
	struct EnumSelector;

	template<class T>
	struct EnumSelector<T, false>
	{
		static bool convert(const EvaluationUnit& unit, T& value)
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
			>(unit, value);
		}
	};

	template<class T>
	struct EnumSelector<T, true>
	{
		static bool convert(const EvaluationUnit& unit, T& value)
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
			>(unit, value);
		}
	};

	template<class T, bool isContainer = stl::IsContainer<T>::value>
	struct UserTypeConverter;

	template<class T>
	struct UserTypeConverter<T, true>
	{
		static bool convert(const EvaluationUnit& unit, T& value)
		{
			auto type = mirror::type(value);
			if (!multimethodsConversionTable.exists((EvaluationUnit&)unit, type))
			{
				if (auto array = unit.cast<ArrayContainer>())
				{
					auto count = array->count();
					for (std::size_t i = 0; i < count; ++i)
					{
						typename T::value_type val;
						if (convertVar(*array->element(i), val))
						{
							value.push_back(val);
						}
					}
					return true;
				}
			}
			else
			{
				return ConverterPointer<T>::convert(unit, value);
			}			

			return false;
		}
	};

	template<class T>
	struct UserTypeConverter<T, false>
	{
		static bool convert(const EvaluationUnit& unit, T& value)
		{
			return ConverterPointer<T>::convert(unit, value);
		}
	};

	template<class T> 
	struct Converter<T, true>
	{
		static bool convert(const EvaluationUnit& unit, T& value)
		{
			return EnumSelector<T>::convert(unit, value);
		}
	};

	template<class T> 
	struct Converter<T, false>
	{
		static bool convert(const EvaluationUnit& unit, T& value)
		{
			return UserTypeConverter<T>::convert(unit, value);
		}
	};

	template<>
	struct Converter<boost::any, false>
	{
		static bool convert(const EvaluationUnit& unit, boost::any& value)
		{
			return EnumSelector<boost::any>::convert(unit, value);
		}
	};

	template<class T>
	bool convertVar(const EvaluationUnit& var, T& value)
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