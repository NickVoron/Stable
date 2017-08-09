// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "spawner.h"
#include "class.h"
#include "classesLib.h"
#include "entityList.h"


void SystemSpawner::prototype(stream::dmemstream* is_)
{
	ENFORCE(is_)
	is = is_;
}

Entity* SystemSpawner::spawn(Entity& parent, ConfiguratorBase* configurator)
{
	Entity* entity = nullptr;

	if((remaining != 0) && is)
	{
		auto& entities = parent.getParent();
		stream::cycle_idmemstream cis(*is);
		
		entities.loadObjects(cis, true, [&parent, configurator](Entity* entity)
		{
			if (configurator)
			{
				configurator->configure(*entity);
			}
		});
		

		if (remaining > 0)
		{
			--remaining;
		}

		++totalCounter;
		++afterResetCounter;
	}
	
	return entity;
}

Entity* SystemSpawner::spawn(Entity& parent)
{
	LOG_MSG("SystemSpawner::spawn");
	return spawn(parent, nullptr);
}

void SystemSpawner::resetCounter()
{
	remaining = limit;
	afterResetCounter = 0;
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