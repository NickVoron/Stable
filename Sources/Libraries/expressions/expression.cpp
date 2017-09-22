// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "expression.h"
#include "holder.h"
#include "reference.h"
#include "scopeNames.h"

namespace Expressions
{
	EvaluatedScope EvaluationUnit::commonParent;

	EvaluateState merge(EvaluateState first, EvaluateState second)
	{
		if (first == Complete && second == Complete)			return Complete;
		if (first == Complete && second == Reject)			return Reject;
		if (first == Complete && second == Impossible)		return Impossible;

		if (first == Reject && second == Complete)			return Reject;
		if (first == Reject && second == Reject)			return Reject;
		if (first == Reject && second == Impossible)		return Reject;

		if (first == Impossible && second == Complete)		return Impossible;
		if (first == Impossible && second == Reject)		return Reject;
		if (first == Impossible && second == Impossible)	return Impossible;

		ENFORCE(false);
		return Impossible;
	}

	
	std::ostream& operator<<(std::ostream& os, const EvaluatedScope& ctx)
	{
		return os;
	}

	
	bool References::canResolveReference(const EvaluatedScope& scopename)
	{
		for (const Reference* ref : *this)
		{
			if (!ref->canResolveReference(scopename))
			{
				return false;
			}
		}

		return true;
	}

	
	EvaluationUnit::EvaluationUnit(const EvaluatedScope& parent_) : parent(parent_.id) {}

	References EvaluationUnit::references() const
	{ 
		ENFORCE_MSG(false, "No one references cant be in EvaluationUtin based class;")
		return References();
	}

	EvaluationUnit* EvaluationUnit::evaluated(const EvaluatedScope& parentScopename) const
	{
		ENFORCE_MSG(false, "Already evaluated;")
		return (EvaluationUnit*)this;
	}
	
	EvaluateState EvaluationUnit::evaluateStep(const EvaluatedScope& parentScopename)
	{
		return Complete;
	}

	bool EvaluationUnit::isParent(const EvaluatedScope& scope) 
	{ 
		return parent == scope.id; 
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