// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "componentModelConfigurator.h"
#include "unroller.h"

namespace ObjectParser
{


const Expressions::Expression* PrototypeHandle::evaluated(const Expressions::ScopeNames& parentScopenames, boost::any* userData) const 
{
	ENFORCE_MSG(userData, "Unroller == 0");
	Unroller* unroller = boost::any_cast<Unroller*>(*userData);
	ENFORCE_MSG(unroller, "Unroller == 0");

	PropertyAssignmentList params;
	for (auto& param : *this)
	{
		params.emplace_back(new PropertyAssignment(param.first, param.second));
	}

	InstanceDefinitionExpression instanceDefinition(prototype.type, prototype.name, params, false);
	return unroller->unrollInstance(&instanceDefinition, const_cast<Expressions::ScopeNames&>(parentScopenames));
}


PrototypeHandle* ComponentModelConfigurator::beginCreatePrototype(const InstanceDefinitionExpression& prototype)
{
	return Expressions::add<PrototypeHandle>(prototype);
}

void ComponentModelConfigurator::endCreatePrototype(PrototypeHandle* handle)
{


}


}



// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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