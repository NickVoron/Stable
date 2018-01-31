// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include "expression.h"
#include "holder.h"
#include "struct.h"
#include <unordered_map>

namespace Expressions
{
	template<class PropertyType>
	struct Properties : public std::unordered_map<std::string, const PropertyType*>
	{
		void add(const std::string& name, const PropertyType* value)
		{
			this->emplace(name, value);
		}

		const PropertyType* get(const std::string& name) const
		{
			auto it = this->find(name);
			return it != this->end() ? it->second : nullptr;
		}
	};
	

	
	struct PropertiesStruct : public Expression, public Properties<Expression>
	{
	public:
		PropertiesStruct(const std::string& typeName);
		virtual EvaluationUnit* evaluated(EvaluatedScope& environment) const override;

		std::string structType;
	};

	
	struct PropertiesStructUnit : public EvaluationUnit
	{
	public:
		PropertiesStructUnit(Expressions::EvaluatedScope& parent, const PropertiesStruct& prototype);
		virtual std::string typeName() const override;
		virtual std::string string() const override;
		void add(const std::string& name, EvaluationUnit* value);
		const EvaluationUnit* get(const std::string& name) const;

		const PropertiesStruct& prototype;
	};
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