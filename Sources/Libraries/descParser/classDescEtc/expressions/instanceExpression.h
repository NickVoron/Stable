// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include "stuff/dependencyGraph.h"
#include "../propertyAssignment.h"
#include <memory>

namespace ObjectParser
{	 
	
	
	

	class ClassDesc;
	struct ClassTable;

	class InstanceDefinitionExpression : public Expressions::Expression
	{
	public:
		typedef std::map<std::string, Expressions::EvaluationUnit*> ImmediatelyParams;

		InstanceDefinitionExpression(const std::string& type, const std::string& name, const PropertyAssignmentList& params, bool noinst);
		InstanceDefinitionExpression(const std::string& type, const std::string& name);
		
		virtual std::string string() const override;
		virtual Expressions::EvaluationUnit* evaluated(const Expressions::EvaluatedScope& parentScopenames) const override;
		virtual Expressions::References references() const override;
				
		const std::string name;
		const std::string type;
		PropertyAssignmentList params;
		const bool noinstance = false;

		InstanceDefinitionExpression* instance() const;

		
		Expressions::Expression* arrayData = nullptr;
	private:
	};

	Expressions::EvaluationUnit* evaluateOnce(const Expressions::EvaluatedScope& parentScopenames, const ClassTable& classes, const std::string& type, const std::string& name, const PropertyAssignmentList& params);
	void fillScopenamesFromClass(const ClassTable& classes, const ClassDesc& classDesc, const PropertyAssignmentList& params, Expressions::ExpressionScope& result);


}//



// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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