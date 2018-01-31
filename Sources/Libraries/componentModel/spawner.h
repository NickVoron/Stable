// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "prototypesHolder.h"
#include "entityList.h"

class SystemSpawner
{
public:
	template<class Configurator, class... ConfiguratorFunctions>
	void spawn(EntitiesList& parent, Configurator&& cfg, ConfiguratorFunctions&&... functions);

	void spawn(EntitiesList& parent);

	void prototype(stream::dmemstream* is);

	void resetCounter();

	std::size_t totalCounter = 0;
	std::size_t afterResetCounter = 0;
	int limit = -1;
	int remaining = -1;

private:
	stream::dmemstream* is = nullptr;
};

template<class Configurator, class... ConfiguratorFunctions>
void SystemSpawner::spawn(EntitiesList& entities, Configurator&& configurator, ConfiguratorFunctions&&... functions)
{
	if ((remaining != 0) && is)
	{
		stream::cycle_idmemstream cis(*is);

		std::vector<ComponentExternalLink> links;
		cis >> links;

		entities.loadObjects(cis, true, links, [&configurator, &functions...](auto& entity)
		{
			configurator(entity, functions...);
		});

		if (remaining > 0)
		{
			--remaining;
		}

		++totalCounter;
		++afterResetCounter;
	}
}



// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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