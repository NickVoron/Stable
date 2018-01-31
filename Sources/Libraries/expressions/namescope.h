// Copyright (C) 2017-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <atomic>

#if !defined(USE_APPLE) && !defined(__APPLE__)
#include <optional>
#else
#include <experimental/optional>
namespace std
{
	template<class T>
	using optional = std::experimental::optional<T>;

	using nullopt_t = std::experimental::nullopt_t;
}
#endif

#include "stuff/library.include.h"

namespace Expressions
{
	
	
	
	
	
	

	enum class InsertMethod
	{
		INSERT,
		REPLACE,
		REPLACE_OR_INSERT,
		IGNORE_IF_EXIST
	};

	uint64_t newid();

	template<typename ExpressionType>
	class NameScope: public std::map<std::string, ExpressionType*>
	{
	public:		
		NameScope(const std::string& scopeName_, NameScope* parent_) : scopeName(scopeName_), parent(parent_) {	}

		void add(const std::string& name, ExpressionType* expr, InsertMethod method);
		bool exists(const std::string& name) const;
		ExpressionType* get(const std::string& name) const;
		ExpressionType* getByType(const std::string& type) const;
		void trace(const std::string& name, std::string& result) const;
		void trace_parents(std::string& result) const;

		void remove(const std::string& name);
		std::optional<std::string> name(ExpressionType* expr) const;

		std::string string() const
		{
			str::stringize result;
			for (auto& val : *this)
			{
				result("{ ", val.first, ": ", val.second->string(), " }");
			}
			return result.str();
		}

		std::string path() const
		{
			return parent ? (str::stringize(parent->path(), ".", scopeName)) : scopeName;
		}

					
		bool isClassMember(const ExpressionType* expr) const { return classMembers.count(expr); }

		uint64_t id = newid();
		std::string scopeName;
		std::set<const ExpressionType*> classMembers;

		NameScope * const parent = nullptr;
	};

	template<typename ExpressionType>
	void NameScope<ExpressionType>::add(const std::string& name, ExpressionType* expr, InsertMethod method)
	{
		auto found = this->find(name);
		ENFORCE_MSG(method != InsertMethod::INSERT || found == std::end(*this), "INSERT: scopenames alredy have property by name: " + name);
		ENFORCE_MSG(method != InsertMethod::REPLACE || found != std::end(*this), "REPLACE: scopenames not have property by name: " + name);

		if (method == InsertMethod::IGNORE_IF_EXIST && found != std::end(*this))
		{
			return;
		}

		(*this)[name] = expr;
	}

	template<typename ExpressionType>
	ExpressionType* NameScope<ExpressionType>::get(const std::string& name) const
	{
		auto found = this->find(name);
		ExpressionType* result = (found != std::end(*this)) ? found->second : nullptr;
		if (!result && parent)
		{
			result = parent->get(name);
		}

		return result;
	}

	template<typename ExpressionType>
	void NameScope<ExpressionType>::trace_parents(std::string& result) const
	{
		result += str::stringize(id, "(", scopeName, ")");
		if (parent)
		{
			result += ".";
			parent->trace_parents(result);
		}
	}

	template<typename ExpressionType>
	void NameScope<ExpressionType>::trace(const std::string& name, std::string& result) const
	{
		if(result.empty())
		{
			result += str::stringize(name, "<-");
		}

		result += str::stringize(id, "(", scopeName, ")");
		auto found = this->find(name);
		if(found != std::end(*this))
		{
			result += "=" + name;
		}
		else
		{
			if (parent)
			{
				result += ".";
				parent->trace(name, result);
			}
			else
			{
				result += "->NOT_FOUND";
			}
		}
	}

	template<typename ExpressionType>
	bool NameScope<ExpressionType>::exists(const std::string& name) const
	{
		return get(name)!=nullptr;
	}

	template<typename ExpressionType>
	ExpressionType* NameScope<ExpressionType>::getByType(const std::string& type) const
	{
		auto found = std::find_if(std::begin(*this), std::end(*this), [type](auto& iter)
		{
			return (iter.second) && (iter.second->typeName() == type);
		});

		ExpressionType* result = (found != std::end(*this)) ? found->second : nullptr;

		if (!result && parent)
		{
			result = parent->getByType(type);
		}

		return result;
	}

	template<typename ExpressionType>
	std::optional<std::string> NameScope<ExpressionType>::name(ExpressionType* expr) const
	{
		auto found = std::find_if(std::begin(*this), std::end(*this), [expr](auto& iter)
		{
			return iter.second == expr;
		});

		if (found != this->end())
		{
			return found->first;
		}

		return {};
	}

	template<typename ExpressionType>
	void NameScope<ExpressionType>::remove(const std::string& name)
	{
		auto iter = this->find(name);
		ENFORCE(iter != this->end());
		this->erase(iter);
	}  	
}
	




// Copyright (C) 2017-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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