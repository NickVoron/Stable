// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "025.nestedSpawners.h"
#include "utils.h"
#include "quietConfigurator.h"
#include "componentLinkModelConfigurator.h"

#include "componentModel/library.include.h"
#include "commonComponents/library.include.h"

namespace
{
	std::string result;
}

namespace ComponentModelTesting
{

class ComponentS1 : public UpdatableComponentAutoLink<ComponentS1, ConfigurableSpawner<Position>>
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(ComponentS1);

	ComponentS1() { result += "1"; }
	void update() { spawn([](Position& component) { 	}); }

};

class ComponentS2 : public UpdatableComponentAutoLink<ComponentS2, ConfigurableSpawner<ComponentS1>>
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(ComponentS2);

	ComponentS2() { result += "2"; }
	void update() { spawn([](ComponentS1& component) {}); }

};

class ComponentS3 : public UpdatableComponentAutoLink<ComponentS3, ConfigurableSpawner<ComponentS2>>
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(ComponentS3);

	ComponentS3() { result += "3"; }
	void update() { spawn([](ComponentS2& component) {}); }

};

class ComponentS4 : public UpdatableComponentAutoLink<ComponentS4, ConfigurableSpawner<ComponentS3>>
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(ComponentS4);

	ComponentS4() { result += "4"; }
	void update() { spawn([](ComponentS3& component) { }); }

};

class ComponentS5 : public UpdatableComponentAutoLink<ComponentS5, ConfigurableSpawner<ComponentS4>>
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(ComponentS5);

	ComponentS5() { result += "5"; }
	void update() { spawn([](ComponentS4& component) { }); }


};
									 

NestedSpawners::NestedSpawners()
{
	try
	{



		ComponentsCore::components<ComponentS1, ComponentS2, ComponentS3, ComponentS4, ComponentS5>();
		result.clear();
		std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/025.nestedSpawners.desc");
		ComponentModel::descriptionTest(
			path, 10,
			[](auto&) { ENFORCE_EQUAL(result, "54321"); result.clear(); }, 
			[](auto&) { ENFORCE_EQUAL(result, "4321"); },
			[](auto&, auto&) {}
		);
	}
	catch (const std::exception& e)
	{
		LOG_ERROR(e);
	}
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