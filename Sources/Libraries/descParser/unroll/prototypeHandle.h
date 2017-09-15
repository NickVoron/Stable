// Copyright (C) 2017 Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include "expressions/library.include.h"
#include "prototypeHandle.h"
#include "../classDescEtc/expressions/instanceExpression.h"


namespace ObjectParser
{
	class Unroller;

	struct PrototypeHandle : public Expressions::EvaluationUnit, public Expressions::EvaluatedScope
	{
		PrototypeHandle(const Expressions::EvaluatedScope& parentScope_, const InstanceDefinitionExpression& prototype_);
		
		const Expressions::EvaluatedScope& parentScope;
		const InstanceDefinitionExpression& prototype;
		PropertyAssignmentList unrolledParams;

		virtual Expressions::EvaluateState evaluateStep(const Expressions::EvaluatedScope& parentScopename, boost::any* userData = 0) override;

		Expressions::EvaluationUnit*  unroll(Unroller* unroller);

		virtual const Expressions::EvaluationUnit* child(const Expressions::PropertyPath* path) const override { return get(path->name); }
	};


}



// Copyright (C) 2017 Denis Netakhin <denis.netahin@yandex.ru>
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