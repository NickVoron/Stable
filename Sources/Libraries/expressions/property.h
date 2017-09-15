// Copyright (C) 2014-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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
	
	struct Properties : public std::unordered_map<std::string, const Expressions::Expression*>, public Base::NonCopyable
	{
		void add(const std::string& name, const Expressions::Expression* value);
		const Expression* get(const std::string& name) const;

		std::string string() const;
	};
	
	
	
	struct PropertiesStruct : public Properties, public StructBase
	{
	public:
		PropertiesStruct(const std::string& typeName);

		virtual EvaluationUnit* evaluated(const EvaluatedScope& environment, boost::any* userData = 0) const override;

		virtual std::string string() const override;
		virtual References references() const override;
	};


	
	
	struct EvalProperties : public std::unordered_map<std::string, Expressions::EvaluationUnit*>, public Base::NonCopyable
	{
		void add(const std::string& name, Expressions::EvaluationUnit* value);
		const EvaluationUnit* get(const std::string& name) const;
		std::string string() const;

		Properties unEvaluatedPropertyies;
	};


	
	struct EvalPropertiesStruct : public EvalProperties, public StructBase
	{
	public:
		EvalPropertiesStruct(const std::string& typeName);
		virtual std::string string() const override;
		virtual EvaluateState evaluateStep(const Expressions::EvaluatedScope& parentScopename, boost::any* userData = 0) override;

		virtual const EvaluationUnit* child(const PropertyPath* path) const override;
	};


}




// Copyright (C) 2014-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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