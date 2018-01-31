// Copyright (C) 2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "025.nestedSpawners.h"
#include "utils.h"


#include "componentModel/library.include.h"
#include "commonComponents/library.include.h"

#ifdef ENABLE_TEST
namespace ComponentModelTesting
{

class ComponentS1 : public UpdatableComponentAutoLink<ComponentS1, ConfigurableSpawner<Position>>
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(ComponentS1);

	void update() 
	{ 
		spawn([](Position& component)
		{ 
			LOG_EXPRESSION(typeid(component).name()) 
		}); 
	}
};

class ComponentS2 : public UpdatableComponentAutoLink<ComponentS2, ConfigurableSpawner<ComponentS1>>
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(ComponentS2);

	void update() 
	{ 
		spawn([](ComponentS1& component)
		{
			LOG_EXPRESSION(typeid(component).name());
		}); 
	}
};

class ComponentS3 : public UpdatableComponentAutoLink<ComponentS3, ConfigurableSpawner<ComponentS2>>
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(ComponentS3);

	void update() 
	{ 
		spawn([](ComponentS2& component)
		{
			LOG_EXPRESSION(typeid(component).name());
		}); 
	}
};

class ComponentS4 : public UpdatableComponentAutoLink<ComponentS4, ConfigurableSpawner<ComponentS3>>
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(ComponentS4);

	void update() 
	{ 
		spawn([](ComponentS3& component)
		{
			LOG_EXPRESSION(typeid(component).name());
		}); 
	}
};

class ComponentS5 : public UpdatableComponentAutoLink<ComponentS5, ConfigurableSpawner<ComponentS4>>
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(ComponentS5);

	void update() 
	{ 
		spawn([](ComponentS4& component)
		{
			LOG_EXPRESSION(typeid(component).name());
		}); 
	}
};


	


NestedSpawners::NestedSpawners()
{
	ComponentModel::components<ComponentS1,	ComponentS2, ComponentS3, ComponentS4, ComponentS5>();

	std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/025.nestedSpawners.desc");
	ObjectParser::Compiler comp(path.c_str());

	auto worldScopename = unroll(comp.result.classes(), "Main", "main");

	try
	{
		ComponentModel::descriptionTest(path.c_str());
	}
	catch (const std::exception& e)
	{
		LOG_ERROR(e);
	}
}

}
#endif



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