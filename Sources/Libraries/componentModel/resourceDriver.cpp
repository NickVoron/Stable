// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "resourceDriver.h"
#include "factory.h"
#include "class.h"
															  
void ComponentResource::init(const ComponentBase& component)
{
	table = ComponentsFactory::resourceTable(component);
}

bool ComponentResource::empty() const
{
	return !table;
}

void ComponentResource::dump()
{
	if(!empty())
	{
		
	}
	else
	{
		LOG_WARNING("ClassResource::dump: try to dump empty Resource");
	}
}

void ClassResources::create(const Class& cls)
{
	parent = &cls;
	auto cc = cls.getComponentsCount();
	if(cc > 0)
	{
		Entity entity;
		cls.createInstance(entity);

		init(cc); 
		zero();

		for (std::size_t i = 0; i < cc; ++i)
		{
			data[i].init(entity.getComponent(i));
		}
	}
}












void ClassResources::dump() const
{
	LOG_ERROR(parent->name() << ":")
		for (std::size_t i = 0; i < size(); ++i)
		{
			if(!data[i].empty())
			{
				LOG_ERROR(logs::tabs(1) << ComponentsFactory::className( parent->getComponentClassIndex(i) ) << "(" << parent->getComponentName(i) << ")" );
				data[i].dump();
			}
		}
	LOG_ERROR(logs::repeat('-', 20));
}




void edit(ComponentResource& componentResource)
{
	if(!componentResource.empty())
	{

	}
	else
	{
		LOG_WARNING("ClassResource::dump: try to edit empty Resource");
	}
}

void edit(ClassResources& classResources)
{
	for (std::size_t i = 0; i < classResources.size(); ++i)
	{
		edit(classResources[i]);
	}
}





// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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