// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "entityList.h"
#include "class.h"
#include "property.h"


EntitiesList::EntitiesList()
{
	
}

EntitiesList::~EntitiesList()
{
	clear();
	executionList.destroy();
}

void EntitiesList::activate(bool active)
{
	for (auto& entity : *this)
	{
		entity.activate(active);
	}
}

Entity* EntitiesList::at(std::size_t index) const
{
	return pool.getEntity(index);
}

Entity& EntitiesList::add()
{
	Entity* e = pool.newEntity(); 
	e->parent = this;
	push_back(*e);
	return *e;
}

Entity* EntitiesList::create(const std::string& className)
{
	const Class& cls = classes.find(className);
	return &cls.createInstance(*this);
}

Entity* EntitiesList::create(std::size_t classIndex)
{
	return &classes.classes[classIndex]->createInstance(*this);
}

void EntitiesList::save(stream::ostream& os) const
{	
	classes.save(os);
	saveObjects(os);
}

void EntitiesList::load(stream::istream& is)
{
	clear();			  
	classes.load(is, executionList);
	loadObjects(is, false, std::vector<ComponentExternalLink>());
}

void EntitiesList::execute()
{
	executionList.execute();
}

void EntitiesList::saveObjects(stream::ostream& os) const
{
	os << size();
	for (auto& entity : *this)
	{
		os << entity.getClass().getIndex();
		entity.save(os);
	}
}

void EntitiesList::loadObjects(stream::istream& is, bool activate, const std::vector<ComponentExternalLink>& externalLinks)
{
	loadObjects(is, activate, externalLinks, [](Entity&) {});
}

void EntitiesList::solveLoadList(const LoadList& loadList, const std::vector<ComponentExternalLink>& externalLinks)
{
	for(auto& info : loadLinks)
	{
		if(info.data.objectIndex)
		{
			auto objidx = *info.data.objectIndex;
			if (objidx < loadList.size())
			{
				ENFORCE_LESS(objidx, loadList.size());
				Entity* obj = loadList[objidx];
				ENFORCE_EQUAL(info.data.componentIndices.size(), info.links.size());

				for (std::size_t idx = 0; idx < info.data.componentIndices.size(); ++idx)
				{
					*info.links[idx] = &obj->getComponent(info.data.componentIndices[idx]);
				}
			}
		}
		else
		{
			
		}
	}
}

bool EntitiesList::findComponent(ComponentBase* component, std::optional<std::size_t>& objectIndex, std::size_t& componentIndex) const
{
	const Entity* object = nullptr;
	if (component)
	{
		std::size_t i = 0;
		for (auto& entity : *this)
		{
			if (entity.componentIndex(component, componentIndex))
			{
				objectIndex = i;
				object = &entity;
				break;
			}
			++i;
		}


	}	

	return objectIndex.operator bool();
}


void EntitiesList::clearObjects()
{
	activate(false);
	Base::IntrusiveList<Entity>::clear();
	pool.clear();
}

void EntitiesList::clear()
{
	clearObjects();	
	classes.clear();
}

void EntitiesList::debugOutput() const
{
	classes.debugOutput();
	LOG_INFO(debugstr());
}

std::string EntitiesList::debugstr() const
{

	std::map<const Class*, int > sorted;
	for (auto& entity : *this)
	{
		++sorted[&entity.getClass()];
	}

	str::spaced result("entities: {");
	for (auto& v : sorted)
	{
		result(*v.first, " objects count: ", v.second);
	}
	result("}");

	return result.str();
}




EntitiesList& EntityArena::create()
{
	emplace_back(new EntitiesList());
	return *back();
}

void EntityArena::execute()
{
	for (auto& list : *this)
	{
		list->execute();
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