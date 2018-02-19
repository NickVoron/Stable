// Copyright (C) 2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <stack>

#include "apply.h"

namespace str
{
	struct delimeter
	{
		delimeter() {}

		template<class T>
		delimeter(const T& d)
		{
			std::stringstream ds;
			ds << d;
			str = ds.str();
		}

		std::string str;
	};

	struct comma : public delimeter		{	comma() : delimeter(", ") {}	};
	struct space : public delimeter		{	space() : delimeter(" ") {}		};
	struct nodelim : public delimeter	{	nodelim() : delimeter("") {}	};

	struct stringize;
	template<class ValueType> 
	struct StrAppend
	{
		static std::size_t append(stringize& s, const ValueType& value);
	};

	struct stringize
	{
	template<class ValueType> friend struct StrAppend;
	template<class Container> friend std::size_t append_container(stringize& s, const Container& values);
	public:
		template<class... T>
		explicit stringize(T&&... values)
		{
			operator()(std::forward<T>(values)...);
		}

		operator std::string () const { return str(); }

		std::string str() const		{ return ss.str(); }
		const char* c_str() const	{ str_ = ss.str(); return str_.c_str(); }

		template<class... T>
		auto& operator()(T&&... values)
		{
			auto a = { std::size_t(0), (append(std::forward<T>(values)), std::size_t(0))... };
			return a.size(), *this;
		}

		template<class ValueType>
		std::size_t append(const ValueType& value)
		{
			return StrAppend<ValueType>::append(*this, value);
		}

		auto append(const delimeter& d)
		{
			delimeters.push(d);
			newdelim = true;
			return count;
		}

		auto append(const comma& d) { return append((const delimeter&) d); }
		auto append(const space& d) { return append((const delimeter&) d); }
		auto append(const nodelim& d) 
		{ 
			if (!delimeters.empty())
			{
				delimeters.pop();
			}

			newdelim = true;
			
			return 0; 
		}

		template<class ValueType>
		stringize& operator<<(const ValueType& value)
		{
			append(value);
			return *this;
		}

	private:
		bool newdelim = false;
		std::size_t count = 0;
		std::stack<delimeter> delimeters;
		mutable std::string str_;
		std::stringstream ss;
	};

	struct spaced : public stringize
	{
		template<class... T>
		spaced(T&&... values) : stringize(space(), std::forward<T>(values)...)
		{
		}
	};

	struct pointed : public stringize
	{
		template<class... T>
		pointed(T&&... values) : stringize(delimeter("."), std::forward<T>(values)...)
		{
		}
	};

	template< typename T >	std::string	convert(const T& val) { return str::stringize(val);	}
	template< typename T > void convert(const T& val, std::string& res) { res = str::stringize(val); }

	template< typename T>
	T& convert(const std::string& str, T& res)
	{
		std::stringstream ss;
		ss << str;
		ss >> res;
		return res;
	}

	template< typename T>
	T convert(const std::string& str)
	{
		T res;
		convert(str, res);
		return res;
	}

	template<class StringType>	const char* c_str(StringType& strvalue) { return strvalue; }

	template<>	inline const char* c_str<const std::string>(const std::string& strvalue) { return strvalue.c_str(); }
	template<>	inline const char* c_str<std::string>(std::string& strvalue) { return strvalue.c_str(); }

	template<class ValueType>
	std::size_t StrAppend<ValueType>::append(stringize& s, const ValueType& value)
	{
		if (!s.delimeters.empty() && !s.delimeters.top().str.empty() && (s.count > 0) && !s.newdelim)
		{
			s.ss << s.delimeters.top().str;
		}

		s.ss << value;
		++s.count;
		s.newdelim = false;

		return s.count;
	}

	template<class Value0, class Value1>
	struct StrAppend<std::pair<Value0, Value1>>
	{
		static std::size_t append(stringize& s, const std::pair<Value0, Value1>& value)
		{
			std::size_t result = 0;
			s("{ ", str::comma());
			result += s.append(value.first);
			result += s.append(value.second);
			s(str::nodelim(), "}");

			return result;
		}
	};

	template<class... Values>
	struct StrAppend<std::tuple<Values...>>
	{
		static std::size_t append(stringize& s, const std::tuple<Values...>& value)
		{
			s("{ ", str::comma());
			stl::for_each(value, [&s](auto& val) { s.append(val); });
			s(str::nodelim(), "}");

			return sizeof... (Values);
		}
	};


	template<class Container>
	std::size_t append_container(stringize& s, const Container& values)
	{
		std::size_t result = 0;
		s.newdelim = true;
		s("{ ", str::comma());
		for (auto& value : values)
		{
			result += s.append(value);
		}
		s(str::nodelim(), " }");

		return result;
	}

	template<class ValueType, class Allocator> struct StrAppend<std::vector<ValueType, Allocator>>
	{
		static std::size_t append(stringize& s, const std::vector<ValueType>& values) { return append_container(s, values); }
	};
	
	template<class ValueType, class Allocator> struct StrAppend<std::list<ValueType, Allocator>>
	{
		static std::size_t append(stringize& s, const std::list<ValueType>& values) { return append_container(s, values); }
	};

	template<class ValueType, class Allocator> struct StrAppend<std::deque<ValueType, Allocator>>
	{
		static std::size_t append(stringize& s, const std::deque<ValueType>& values) { return append_container(s, values); }
	};

	template<class ValueType, class Compare, class Allocator> struct StrAppend<std::set<ValueType, Compare, Allocator>>
	{
		static std::size_t append(stringize& s, const std::set<ValueType, Compare, Allocator>& values) { return append_container(s, values); }
	};

	template<class Key, class Mapped, class Compare, class Allocator> struct StrAppend<std::map<Key, Mapped, Compare, Allocator>>
	{
		static std::size_t append(stringize& s, const std::map<Key, Mapped, Compare, Allocator>& values) { return append_container(s, values); }
	};

	template<class Value, class Hasher, class Equal, class Allocator> struct StrAppend<std::unordered_set<Value, Hasher, Equal, Allocator>>
	{
		static std::size_t append(stringize& s, const std::unordered_set<Value, Hasher, Equal, Allocator>& values) { return append_container(s, values); }
	};

	template<class Key, class Mapped, class Hasher, class Equal, class Allocator> struct StrAppend<std::unordered_map<Key, Mapped, Hasher, Equal, Allocator>>
	{
		static std::size_t append(stringize& s, const std::unordered_map<Key, Mapped, Hasher, Equal, Allocator>& values) { return append_container(s, values); }
	};

	std::ostream& operator<<(std::ostream& os, const stringize& str);

	std::string msvs_out(const std::string& fileName, unsigned int line, const std::string& message);
}




// Copyright (C) 2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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