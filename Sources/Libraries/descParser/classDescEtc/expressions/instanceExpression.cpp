// Copyright (C) 2016-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "instanceExpression.h"
#include "../unroller.h"


namespace ObjectParser
{

InstanceDefinitionExpression::InstanceDefinitionExpression(const std::string& type_, const std::string& name_, const PropertyAssignmentList& params_, bool noinst):
	type(type_),
	name(name_),
	params(params_),
	noinstance(noinst)
{
	
}

InstanceDefinitionExpression::InstanceDefinitionExpression(const std::string& type_, const std::string& name_):InstanceDefinitionExpression(type_, name_, PropertyAssignmentList(), false)
{
}

std::string InstanceDefinitionExpression::string() const
{
	auto value = str::stringize("InstanceDefinitionExpression type: ", type, ", name: ", name).str();

	if (arrayData)
	{
		value += str::stringize(", arrayData:", arrayData->string()).str();
	}

	return value;
}




const Expressions::Expression* InstanceDefinitionExpression::evaluated(const Expressions::ScopeNames& parentScopenames, boost::any* userData) const
{
	ENFORCE_MSG(userData, "Unroller == 0");
	Unroller* unroller = boost::any_cast<Unroller*>(*userData);
	ENFORCE_MSG(unroller, "Unroller == 0");
	return unroller->unrollInstance(this, const_cast<Expressions::ScopeNames&>(parentScopenames));
}

Expressions::References InstanceDefinitionExpression::references() const
{
	Expressions::References resultRefs;

	const Expressions::References& propertiesRefs = params.references();
	resultRefs.insert(resultRefs.begin(), propertiesRefs.begin(), propertiesRefs.end());

	if (arrayData)
	{
		const Expressions::References& arrayDataRefs = arrayData->references();
		resultRefs.insert(resultRefs.begin(), arrayDataRefs.begin(), arrayDataRefs.end());
	}

	return resultRefs;
}

}



// Copyright (C) 2016-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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