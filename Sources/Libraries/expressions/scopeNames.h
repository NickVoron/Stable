// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include <string>
#include <unordered_map>
#include "expression.h"


namespace Expressions
{
	class EvaluationUnit;

	
	
	
	
	
	

	enum class InsertMethod
	{
		INSERT,
		REPLACE,
		REPLACE_OR_INSERT,
		IGNORE_IF_EXIST
	};

	uint64_t newid();

	template<typename ExpressionType>
	class ScopeNames: public std::map<std::string, ExpressionType*>
	{
	public:		
		ScopeNames() :scopeName("unnamed") {};
		ScopeNames(const std::string& scopeName_) : scopeName(scopeName_) {}
		~ScopeNames() {}

		void add(const std::string& name, ExpressionType* expr, InsertMethod method, bool classMember = false);
		bool exists(const std::string& name) const;
		ExpressionType* get(const std::string& name) const;
		ExpressionType* getByType(const std::string& type) const;

		void copyFrom(const ScopeNames& proto);

		void remove(const std::string& name);

		uint64_t id = newid();

		std::string scopeName;		
		boost::any userData;

		void setParent(const ScopeNames<ExpressionType>* parent_) 
		{ 
			parent = parent_; 
			userData = parent->userData;
		}

		bool isClassMember(const ExpressionType* expr) const { return classMembers.count(expr); }

		std::vector<std::string> fields() const
		{
			std::vector<std::string> result;
			for (auto& member : *this)
			{
				result.push_back(member.first);
			}
			return result;
		}

	protected:
		std::set<const ExpressionType*> classMembers;

	private:
		const ScopeNames* parent = 0;
	};

	template<typename ExpressionType>
	void ScopeNames<ExpressionType>::add(const std::string& name, ExpressionType* expr, InsertMethod method, bool isClassMember)
	{
		auto& found = find(name);
		ENFORCE_MSG(method != InsertMethod::INSERT || found == end(), __FUNCTION__ "INSERT: scopenames alredy have property by name: " + name);
		ENFORCE_MSG(method != InsertMethod::REPLACE || found != end(), __FUNCTION__ "REPLACE: scopenames not have property by name: " + name);

		if (method == InsertMethod::IGNORE_IF_EXIST && found != end())
		{
			return;
		}

		(*this)[name] = expr;
		
		if (isClassMember)
		{
			classMembers.emplace(expr);
		}

	}

	template<typename ExpressionType>
	ExpressionType* ScopeNames<ExpressionType>::get(const std::string& name) const
	{
		auto found = find(name);
		ExpressionType* result = (found != end()) ? found->second : nullptr;
		if (!result && parent)
		{
			result = parent->get(name);
		}

		return result;

	}

	template<typename ExpressionType>
	bool ScopeNames<ExpressionType>::exists(const std::string& name) const
	{
		return get(name)!=nullptr;
	}

	template<typename ExpressionType>
	ExpressionType* ScopeNames<ExpressionType>::getByType(const std::string& type) const
	{
		auto found = std::find_if(begin(), end(), [type](auto& iter)
		{
			return iter.second->typeName() == type;
		});

		ExpressionType* result = (found != end()) ? found->second : nullptr;

		if (!result && parent)
		{
			result = parent->getByType(type);
		}

		return result;
	}

	template<typename ExpressionType>
	void ScopeNames<ExpressionType>::copyFrom(const ScopeNames& proto)
	{
		(map&)(*this) = (map&)proto;
	}

	template<typename ExpressionType>
	void ScopeNames<ExpressionType>::remove(const std::string& name)
	{
		auto iter = find(name);
		ENFORCE(iter != end())

			erase(iter);
	}

	
	typedef ScopeNames<const Expression> ExpressionScope;
	typedef ScopeNames<EvaluationUnit> EvaluatedScope;


	
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