// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "unroller.h"
#include "instanceHandle.h"

#include "expressions/library.include.h"


namespace ObjectParser
{

Unroller::Unroller(ClassTable& classes_, EntitiesList& entities_):
	classes(classes_),
	entities(entities_)
{

}


Expressions::EvaluationUnit* Unroller::unrollInstance(const InstanceDefinitionExpression& instanceDefinition, Expressions::EvaluatedScope& namescope)
{
	InstanceHandle::unroller = this;
	Expressions::EvaluationUnit* unit = Expressions::unrollExpression(instanceDefinition, namescope, false);
	return unit->cast<InstanceHandle>();
}

std::unique_ptr<Expressions::EvaluatedScope> unroll(ClassTable& classTable, const InstanceDefinitionExpression& instance)
{
	EntitiesList entities;
	return std::move(unroll(classTable, entities, instance, nullptr));
}

std::unique_ptr<Expressions::EvaluatedScope> unroll(ClassTable& classTable, const std::string& mainInstanceClassName, const std::string& mainInstanceName)
{
	EntitiesList entities;
	return std::move(unroll(classTable, entities, mainInstanceClassName, mainInstanceName));
}

std::unique_ptr<Expressions::EvaluatedScope> unroll(ClassTable& classTable, EntitiesList& target, const InstanceDefinitionExpression& instance, Expressions::EvaluatedScope* parent)
{
	auto mainClass = classTable.get(instance.type);
	ENFORCE_POINTER(mainClass);

	Unroller unroller(classTable, target);
	std::unique_ptr<Expressions::EvaluatedScope> world(new Expressions::EvaluatedScope("world", parent));
	world->add("main", unroller.unrollInstance(instance, *world), Expressions::InsertMethod::INSERT);
	return std::move(world);
}

std::unique_ptr<Expressions::EvaluatedScope> unroll(ClassTable& classTable, EntitiesList& target, const std::string& mainInstanceClassName, const std::string& mainInstanceName)
{
	auto definition = Expressions::add<InstanceDefinitionExpression>(mainInstanceClassName, mainInstanceName);
	return std::move(unroll(classTable, target, *definition, nullptr));
}


}//



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