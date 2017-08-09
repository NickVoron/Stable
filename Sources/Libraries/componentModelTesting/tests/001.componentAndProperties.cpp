// Copyright (C) 2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "001.componentAndProperties.h"
#include "utils.h"
#include "quietConfigurator.h"
#include "utils.h"
#include "componentLinkModelConfigurator.h"

namespace ComponentModelTesting
{
	using namespace Expressions;
	using namespace ObjectParser;

	ComponentAndProperties::ComponentAndProperties()
	{
		std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/001.componentsAndProperties.desc");

		
		ObjectParser::Compiler comp(path.c_str());

		testClassesCount(comp.result, 2);

		const ObjectParser::ClassTable& table = comp.result.classes();
		
		
		ObjectParser::ClassDesc* mainClass = table.get("Main");
		ENFORCE_MSG(mainClass, "No 'Main' Class");
		{
			const InstanceDefinitionExpression* objectInstance = findInstance(mainClass, "object");
			ENFORCE_MSG(objectInstance, "No instance 'object' in 'Main' class");
			ENFORCE_MSG(objectInstance->params.size() == 3, "The number of parameters 'main.object' instance, does not match");

			ENFORCE_MSG(objectInstance->params[0]->propertyName == "pos", "The first param 'main.object' instance, does not 'pos'");
			ENFORCE_MSG(objectInstance->params[1]->propertyName == "vel", "The first param 'main.object' instance, does not 'vel'");
		}

		
		ObjectParser::ClassDesc* objectClass = table.get("Object");
		ENFORCE_MSG(objectClass, "No 'Object' class");
		{
			const Expressions::Expression* velExpr = objectClass->properties().get("vel");
			ENFORCE_MSG(velExpr, "No property 'vel' in class 'Object'");

			const Expressions::Struct* velStruct = velExpr->cast<Expressions::Struct>();
			ENFORCE_MSG(velStruct, "Property 'vel' is no type Struct");
			ENFORCE_MSG(velStruct->typeName() == "Vector3", "Struct 'vel' is no type Vector3");

			const Component* positionComponent = testComponent(objectClass, "position");
			testType(positionComponent, "Position");

			const Component* linearMoverComponent = testComponent(objectClass, "linearMover");
			testType(linearMoverComponent, "LinearMover");

			const Component* drawerComponent = testComponent(objectClass, "drawer");
			testType(drawerComponent, "SphereDrawer");
		}

		
		ComponentLinkModelConfigurator debugConfigurator;
		Expressions::ScopeNames worldScopename = unroll(comp.result.classes(), debugConfigurator, "Main", "main");

		
		auto unrolledMainInstace = testInstance(worldScopename, "main");

		
		const Expressions::Expression* unrolledObjectExpr = unrolledMainInstace->get("object");
		ENFORCE_MSG(unrolledObjectExpr, "No 'object' expression in main instance");
		const ObjectParser::InstanceHandle* unrolledObjectInstace = unrolledObjectExpr->cast<const ObjectParser::InstanceHandle>();
		ENFORCE_MSG(unrolledObjectInstace, "'object' expression in not ObjectParser::InstanceDefinitionExpression");
		const ComponentHandle* position = testComponent(unrolledObjectInstace, 0, "position", "Position");
		{
			const Expressions::Expression* positionExpr = position->get("position");
			ENFORCE_MSG(positionExpr, "'No param 'position' in component");
			const Struct* postionStruct = positionExpr->cast<Struct>();
			ENFORCE_MSG(postionStruct, "'Param 'position' in no Struct type");

			ENFORCE_MSG(postionStruct->typeName() == "Vector3", "Param 'position' is no type 'Vector3'");
			const Expression* posX = postionStruct->params[0];

			const Expressions::Const<float>* posXFloat = posX->cast<Const<float>>();
			ENFORCE(posXFloat->value == -1);
		}

		const ComponentHandle* linearMover = testComponent(unrolledObjectInstace, 0, "linearMover", "LinearMover");
		{
			const Expressions::Expression* velExpr = linearMover->get("velocity");
			ENFORCE_MSG(velExpr, "'No param 'velocity' in component");
			const Struct* velStruct = velExpr->cast<Struct>();
			ENFORCE_MSG(velStruct, "'Param 'velocity' in no Struct type");

			ENFORCE_MSG(velStruct->typeName() == "Vector3", "Param 'velocity' is no type 'Vector3'");
			const Expression* velX = velStruct->params[0];

			const Expressions::Const<float>* velXFloat = velX->cast<Const<float>>();
			ENFORCE(velXFloat->value == 10);
		}

		const ComponentHandle* drawer = testComponent(unrolledObjectInstace, 0, "drawer", "SphereDrawer");
		{
			const Expressions::Expression* colorExpr = drawer->get("color");
			ENFORCE_MSG(colorExpr, "'No param 'color' in component");
			const Struct* colorStruct = colorExpr->cast<Struct>();
			ENFORCE_MSG(colorStruct, "'Param 'color' in no Struct type");

			ENFORCE_MSG(colorStruct->typeName() == "Color", "Param 'color' is no type 'Color'");
			const Expression* colorX = colorStruct->params[0];

			const Expressions::Const<int>* velXFloat = colorX->cast<Const<int>>();
			ENFORCE(velXFloat->value == 1);
		}

		test(worldScopename, "main.three", Vector3(3, 3, 3));
		test(worldScopename, "main.object.acceleration", Vector3(3, 3, 3));		
		test(worldScopename, "main.object.derivativeFromVel", Vector3(20, 20, 20));
		
		const Expression* position2derivativeFromVel = get(worldScopename, "main.object.position2.position");
		test(position2derivativeFromVel, Vector3(20, 20, 20));
	}

}//



// Copyright (C) 2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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